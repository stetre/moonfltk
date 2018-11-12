/* The MIT License (MIT)
 *
 * Copyright (c) 2016 Stefano Trettel
 *
 * Software repository: MoonFLTK, https://github.com/stetre/moonfltk
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "internal.h"

int noprintf(const char *fmt, ...) 
    { (void)fmt; return 0; }

int notavailable(lua_State *L, ...) 
    { NOT_AVAILABLE; }


void printstack(lua_State *L, const char* fmat, ...)
/* prints the Lua stack */
    {
    va_list args;
    int i,r,t;
    int top = lua_gettop(L); /* stack top level */
    printf("###Lua-stack: state=%p top=[%d] ", (void*)L, top);
    if(fmat)
        {
        va_start(args,fmat);
        vprintf(fmat,args);
        va_end(args);
        }
    printf("\n");

    for(i=top;i>=1;i--)
        {
        r=i-top-1; /* relative index */
        t = lua_type(L,i);
        printf("[%d][%d] ",i,r);
        switch(t)
            {
            case LUA_TSTRING: printf("'%s'",lua_tostring(L,i)); break;
            case LUA_TBOOLEAN: printf(lua_toboolean(L,i) ? "true" : "false"); break;
            case LUA_TNUMBER: printf("%g", lua_tonumber(L,i));  break;
            default:
                    printf("%s", lua_typename(L,t));
            }
        printf("\n");
        }
    printf("\n");
    }


/*------------------------------------------------------------------------------*
 | Time utilities (POSIX)                                                       |
 *------------------------------------------------------------------------------*/

#if _POSIX_C_SOURCE >= 199309L
#include<time.h>
double moonfltk_now(void)
    {
    struct timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC,&ts)!=0)
        { printf("clock_gettime error\n"); return -1; }
    return ts.tv_sec + ts.tv_nsec*1.0e-9;
    }
#else
#include<sys/time.h>
double moonfltk_now(void)
    {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0)
        { printf("gettimeofday error\n"); return -1; }
    return tv.tv_sec + tv.tv_usec*1.0e-6;
    }
#endif

double moonfltk_tstosec(const struct timespec *ts)
    {
    return ts->tv_sec*1.0+ts->tv_nsec*1.0e-9;
    }

void moonfltk_sectots(struct timespec *ts, double seconds)
    {
    ts->tv_sec=(time_t)seconds;
    ts->tv_nsec=(long)((seconds-((double)ts->tv_sec))*1.0e9);
    }

#ifdef MINGW
#include <windows.h>
#define SleepSec(s) Sleep((s)*1.0e3)
#else
#include <unistd.h>
#define SleepSec(s) usleep((useconds_t)((s)*1.0e6))
#endif

#if _POSIX_C_SOURCE >= 199309L
void moonfltk_sleep(double seconds)
    {
    struct timespec ts, ts1;
    struct timespec *req, *rem, *tmp;
    moonfltk_sectots(&ts, seconds);
    req = &ts;
    rem = &ts1; 
    while(1)
        {   
        if(nanosleep(req, rem) == 0) 
            return;
        tmp = req;
        req = rem;
        rem = tmp;
        }
    }
#else
void moonfltk_sleep(double seconds)
    {
	SleepSec(seconds);
    }
#endif


/*------------------------------------------------------------------------------*
 | userdata handling                                                            |
 *------------------------------------------------------------------------------*/

ud_t *newuserdata(lua_State *L, void *ptr, const char *mt)
    {
    ud_t *ud;
    ud = userdata(ptr);
    if(ud)
        {
        ud->refcnt++;
        udata_push(L, ptr);
        return ud;
        }
    ud = (ud_t*)udata_new(L, sizeof(ud_t), ptr, mt);
    memset(ud, 0, sizeof(ud_t));
    ud->refcnt = 1;
    ud->cbref = ud->argref = LUA_NOREF;
    ud->draw = ud->handle = ud->show = ud->hide = ud->resize = ud->draw_overlay = LUA_NOREF;
    ud->draw_cell = LUA_NOREF;
    ud->modify = ud->modify_arg = ud->predelete = ud->predelete_arg = LUA_NOREF;
    ud->style = LUA_NOREF;
    ud->style_table = NULL;
    ud->key_func = LUA_NOREF;
    ud->help_func = LUA_NOREF;
    ud->obj = ptr ? ptr : (void*)ud;
    return ud;
    }

int freeuserdata(lua_State *L, void *ptr)
    {
    ud_t *ud = userdata(ptr);
    if(!ud)
        return 0; /* already deleted */

    if(--ud->refcnt > 0)
        return 1; /* shared userdata */

    unreference(L, ud->cbref);
    unreference(L, ud->argref);
    unreference(L, ud->draw);
    unreference(L, ud->handle);
    unreference(L, ud->show);
    unreference(L, ud->hide);
    unreference(L, ud->resize);
    unreference(L, ud->draw_overlay);
    unreference(L, ud->draw_cell);
    unreference(L, ud->modify);
    unreference(L, ud->modify_arg);
    unreference(L, ud->predelete);
    unreference(L, ud->predelete_arg);
    unreference(L, ud->style);
    if(ud->style_table) free(ud->style_table);
    unreference(L, ud->key_func);
    unreference(L, ud->help_func);

    if(ud->bits) free(ud->bits);
    if(ud->xpm)
        freepixmap(ud->xpm, ud->nxpm);
    udata_free(L, ptr);
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Custom luaL_checkxxx() style functions                                       |
 *------------------------------------------------------------------------------*/

int checkoption_hint(lua_State *L, int arg, const char *def, const char *const lst[])
/* Variant of luaL_checkoption(), with an added hint in the error message */
    {
    const char *hint = NULL;
    const char *name = (def) ? luaL_optstring(L, arg, def) : luaL_checkstring(L, arg);
    int i;
    for (i=0; lst[i]; i++)
        if (strcmp(lst[i], name) == 0)  return i;

    if(lua_checkstack(L, i*2))
        {
        for(i=0; lst[i]; i++)
            {
            lua_pushfstring(L, "'%s'", lst[i]);
            lua_pushstring(L, "|");
            }
        i = i*2;
        if(i>0)
            {
            lua_pop(L, 1); /* the last separator */
            lua_concat(L, i-1);
            hint = lua_tostring(L, -1); 
            }
        }
    if(hint)
        return luaL_argerror(L, arg, lua_pushfstring(L, 
                    "invalid option '%s', valid options are: %s", name, hint));
    return luaL_argerror(L, arg, lua_pushfstring(L, "invalid option '%s'", name));
    }

/* Boolean ------------------------------------------------------------*/

int checkboolean(lua_State *L, int arg)
    {
    if(!lua_isboolean(L, arg))
        return luaL_argerror(L, arg, "boolean expected");
    return lua_toboolean(L, arg);
    }

int optboolean(lua_State *L, int arg, int d)
    {
    if(!lua_isboolean(L, arg))
        return d;
    return lua_toboolean(L, arg);
    }

/* ASCII Character ----------------------------------------------------*/

int testchar(lua_State *L, int arg, char *c_)
    {
    size_t len;
    const char *c = luaL_checklstring(L, arg, &len);
    if(len != 1) return 0;
    if(c_) *c_ = c[0];
    return 1;
    }

char checkascii(lua_State *L, int arg)
    {
    size_t len;
    const char *c = luaL_checklstring(L, arg, &len);
    if(len != 1)
        return luaL_argerror(L, arg, lua_pushfstring(L, "invalid character length %d", len));
    return c[0];
    }

int pushascii(lua_State *L, char c)
    {
    lua_pushlstring(L, &c, 1);
    return 1;
    }


/* 1-based index to 0-based ------------------------------------------*/

int checkindex(lua_State *L, int arg)
    {
    int val = luaL_checkinteger(L, arg);
    if(val < 1)
        return luaL_argerror(L, arg, "positive integer expected");
    return val - 1;
    }

int optindex(lua_State *L, int arg, int optval /* 0-based */)
    {
    int val = luaL_optinteger(L, arg, optval + 1);
    if(val < 1)
        return luaL_argerror(L, arg, "positive integer expected");
    return val - 1;
    }

void pushindex(lua_State *L, int val)
    { lua_pushinteger((L), (val) + 1); }

/* Integer subranges -------------------------------------------------*/

int checkpositiveinteger(lua_State *L, int arg)
    {
    int val = luaL_checkinteger(L, arg);
    if(val < 1)
        return luaL_argerror(L, arg, "positive integer expected");
    return val;
    }

/* Pixmap ------------------------------------------------------------*/


char** checkpixmap(lua_State *L, int arg, int *n)
/* expects {str1, str2, str3, ..., strN } on the Lua stack at arg
 */
    {
    /* check the table containing the image */
    if(!n) unexpected(L);
    if(!lua_istable(L, arg))
        { luaL_argerror(L, arg, "table expected"); return NULL; }
    int nxpm = luaL_len(L, arg);
    if(nxpm <= 0)
        { luaL_argerror(L, arg, "empty table"); return NULL; } 
    for(int i = 0; i < nxpm; i++)
        {
        if(lua_geti(L, arg, i+1) != LUA_TSTRING)
            { luaL_argerror(L, arg, "table is not an array of strings"); return NULL; }
        lua_pop(L, 1);
        }
    /* get the image */
    char **xpm = (char**)malloc(nxpm * sizeof(char*));
    if(!xpm)
        { memoryerror(L); return NULL; }
    for(int i = 0; i < nxpm; i++)
        {
        size_t len;
        lua_geti(L, 1, i+1);
        const char *data = luaL_checklstring(L, -1, &len);
        xpm[i] = (char*)malloc((len+1)*sizeof(char));
        memcpy(xpm[i], data, len*sizeof(char));
        xpm[i][len]=0;
        lua_pop(L, 1);
        }
    *n = nxpm;
    return xpm;
    }

void freepixmap(char **xpm, int nxpm)
    {
    for(int i = 0; i<nxpm; i++)
        free(xpm[i]);
    free(xpm);
    }


