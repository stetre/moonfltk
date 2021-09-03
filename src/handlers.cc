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


/*------------------------------------------------------------------------------*
 | Timeout                                                                      |
 *------------------------------------------------------------------------------*/

/* The problem: FLTK allows registering multiple timeout callbacks, but it
 * identifies them by the callback address, so we cannot use a single C
 * callback as entry point for multiple Lua callbacks.
 *
 * @@TODO: is this still true?
 * 
 *  Fortunately, we know how to create a timer wheel in Lua (see LunaSDL), so
 *  we can live with a single timeout callback.
 */
static int TmrRef = LUA_NOREF;
static int Repeat = 0;
static double Seconds = 0;
#define Reset() do {                            \
    if(TmrRef != LUA_NOREF)                     \
        {                                       \
        Fl::remove_timeout(TmrHandler);         \
        unreference(L, TmrRef);                 \
        }                                       \
    Repeat = 0;                                 \
} while (0)

static void TmrHandler(void *data)
    {
    (void)data;
    lua_State* L = main_lua_state;
    if (!L)
        return;
    if(pushvalue(L, TmrRef) != LUA_TFUNCTION)
        { unexpected(L); return; }
    /* restart or reset the timer before executing the callback
     * (the callback code may reset, or set a new handler)
     */
    if(Repeat)
        Fl::repeat_timeout(Seconds, TmrHandler, (void*)0);
    else
        Reset();
    if(lua_pcall(L, 0, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }

static int HasTimeout(lua_State *L)
    {
    //lua_pushboolean(L, Fl::has_timeout(TmrHandler));
    lua_pushboolean(L, TmrRef != LUA_NOREF);
    return 1;
    }

static int SetTimeout(lua_State *L)
    {
    /* unregister the current callback (if any) */
    Reset();
    if(lua_isnoneornil(L, 1))
        return 0;
    /* register the new callback */
    Seconds = luaL_checknumber(L, 1);
    if(Seconds < 0 )
        return luaL_argerror(L, 1, "negative timeout");
    Repeat = checkboolean(L, 2);
    if(!lua_isfunction(L, 3))
        return luaL_argerror(L, 3, "function expected");
    reference(L, TmrRef, 3);
    Fl::add_timeout(Seconds, TmrHandler, (void*)0);
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Check                                                                        |
 *------------------------------------------------------------------------------*/

static int CheckRef = LUA_NOREF;
static void CheckHandler(void *data)
    {
    (void)data;
    lua_State* L = main_lua_state;
    if (!L)
        return;
    if(pushvalue(L, CheckRef) != LUA_TFUNCTION)
        { unexpected(L); return; }
    if(lua_pcall(L, 0, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }

static int Has_check(lua_State *L)
    {
    //lua_pushboolean(L, CheckRef != LUA_NOREF);
    lua_pushboolean(L, Fl::has_check(CheckHandler));
    /* this returns true if the callback exists
     * and has not yet been called */
    return 1;
    }

static int Set_check(lua_State *L)
    {
    /* unregister the current callback (if any) */
    if(CheckRef != LUA_NOREF)
        {
        Fl::remove_check(CheckHandler);
        unreference(L, CheckRef);
        }
    if(lua_isnoneornil(L, 1)) return 0;
    /* register the new callback */
    if(!lua_isfunction(L, 1))
        return luaL_argerror(L, 1, "function expected");
    reference(L, CheckRef, 1);
    Fl::add_check(CheckHandler, (void*)0);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Idle handler                                                                 |
 *------------------------------------------------------------------------------*/

static int IdleRef = LUA_NOREF;
static void IdleHandler(void *data)
    {
    (void)data;
    lua_State* L = main_lua_state;
    if (!L)
        return;
    if(pushvalue(L, IdleRef) != LUA_TFUNCTION)
        { unexpected(L); return; }
    if(lua_pcall(L, 0, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }

static int Has_idle(lua_State *L)
    {
    lua_pushboolean(L, IdleRef != LUA_NOREF);
    return 1;
    }

static int Set_idle(lua_State *L)
    {
    /* unregister the current callback (if any) */
    if(IdleRef != LUA_NOREF)
        {
        Fl::remove_idle(IdleHandler);
        unreference(L, IdleRef);
        }
    if(lua_isnoneornil(L, 1)) return 0;
    /* register the new callback */
    if(!lua_isfunction(L, 1))
        return luaL_argerror(L, 1, "function expected");
    reference(L, IdleRef, 1);
    Fl::add_idle(IdleHandler, (void*)0);
    return 0;
    }

#include <stdio.h>
#include <errno.h>
static int Fileno(lua_State *L) /* POSIX */
/* Returns the integer file descriptor for a Lua io library file handler */
    {
    luaL_Stream* lstream = (luaL_Stream*)luaL_checkudata(L, 1, LUA_FILEHANDLE);
    int fd = fileno(lstream->f);
    if(fd==-1)
        {
        int errnum = errno;
        lua_pushnil(L);
        lua_pushstring(L, strerror(errnum));
        return 2;
        }
    lua_pushinteger(L, fd);
    return 1;
    }

/*------------------------------------------------------------------------------*
 | Fd                                                                           |
 *------------------------------------------------------------------------------*/

static int FdRef = LUA_NOREF;
static void FdHandler(FL_SOCKET fd, void *data)
    {
    (void)data;
    lua_State* L = main_lua_state;
    if (!L)
        return;
    if(pushvalue(L, FdRef) != LUA_TFUNCTION)
//      { unexpected(L); return; } 
        { luaL_error(L, "missing fd handler"); }
    lua_pushinteger(L, fd);
    if(lua_pcall(L, 1, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }

static int Has_fd_handler(lua_State *L)
    {
    lua_pushboolean(L, FdRef != LUA_NOREF);
    return 1;
    }

static int Set_fd_handler(lua_State *L)
    {
    /* unregister the current callback (if any) */
    if(FdRef != LUA_NOREF)
        unreference(L, FdRef);
    if(lua_isnoneornil(L, 1)) return 0;
    /* register the new callback */
    if(!lua_isfunction(L, 1))
        return luaL_argerror(L, 1, "function expected");
    reference(L, FdRef, 1);
    return 0;
    }

static int Add_fd(lua_State *L)
    {
    FL_SOCKET fd = luaL_checkinteger(L, 1);
    int when = check_WhenFd(L, 2);
    Fl::add_fd(fd, when, FdHandler, (void*)0);  
    return 0;
    }

static int Remove_fd(lua_State *L)
    {
    FL_SOCKET fd = luaL_checkinteger(L, 1);
    Fl::remove_fd(fd);  
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Fatal, error, warning                                                        |
 *------------------------------------------------------------------------------*/

#if(0) /*@@ warning, error, fatal */
  /** \addtogroup group_comdlg
    @{ */
  static void (*Fl::warning)(const char*, ...);
  static void (*Fl::error)(const char*, ...);
  static void (*Fl::fatal)(const char*, ...);
#endif



/*------------------------------------------------------------------------------*
 |                                                                              |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "has_timeout",  HasTimeout },
        { "set_timeout",  SetTimeout },
        { "set_check", Set_check },
        { "has_check", Has_check },
        { "set_idle", Set_idle },
        { "has_idle", Has_idle },
        { "has_fd_handler", Has_fd_handler },
        { "set_fd_handler", Set_fd_handler },
        { "add_fd", Add_fd },
        { "remove_fd", Remove_fd },
        { "fileno", Fileno },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Handlers(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }


