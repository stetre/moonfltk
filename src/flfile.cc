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
 | Filename and URI utilities                                                   |
 *------------------------------------------------------------------------------*/

#define BUFLEN FL_PATH_MAX /* all path buffers should use this length */
#define MAXLEN  (BUFLEN-1)

static const char* Checklstring(lua_State *L, int arg, size_t *len)
    {
    const char *s = luaL_checklstring(L, arg, len);
    if(*len > MAXLEN)
        luaL_argerror(L, arg, "too large");
    return s;
    }


static int Decode_uri(lua_State *L)
    {
    char buf[BUFLEN];
    size_t len;
    const char *uri = Checklstring(L, 1, &len);
    strncpy(buf, uri, len);
    buf[len] = '\0';
    fl_decode_uri(buf);
    lua_pushstring(L, buf);
    return 1;   
    }

static int Open_uri(lua_State *L)
    {
    char buf[BUFLEN];
    buf[0] = '\0';
    const char *uri = luaL_checkstring(L, 1);
    lua_pushboolean(L, fl_open_uri(uri, buf, BUFLEN));
    lua_pushstring(L, buf);
    return 2;
    }

static int Filename_absolute(lua_State *L)
    {
    char buf[BUFLEN];
    size_t len;
    const char *from = Checklstring(L, 1, &len);
    if(fl_filename_absolute(buf, BUFLEN, from) == 0)
        lua_pushvalue(L, 1); /* no change */
    else
        lua_pushstring(L, buf);
    return 1;
    }

static int Filename_relative(lua_State *L)
    {
    char buf[BUFLEN];
    size_t len;
    const char *from = Checklstring(L, 1, &len);
    if(fl_filename_relative(buf, BUFLEN, from) == 0)
        lua_pushvalue(L, 1); /* no change */
    else
        lua_pushstring(L, buf);
    return 1;
    }


static int Filename_expand(lua_State *L)
    {
    char buf[BUFLEN];
    size_t len;
    const char *from = Checklstring(L, 1, &len);
    if(fl_filename_expand(buf, BUFLEN, from) == 0)
        lua_pushvalue(L, 1); /* no change */
    else
        lua_pushstring(L, buf);
    return 1;
    }

static int Filename_ext(lua_State *L)
    {
    const char *name = luaL_checkstring(L, 1);
    const char *ext = fl_filename_ext(name);
    if(!ext) return 0;
    lua_pushstring(L, ext);
    return 1;
    }

static int Filename_isdir(lua_State *L)
    {
    const char *name = luaL_checkstring(L, 1);
    lua_pushboolean(L, fl_filename_isdir(name));
    return 1;
    }


static int Filename_match(lua_State *L)
    {
    const char *name = luaL_checkstring(L, 1);
    const char *pattern = luaL_checkstring(L, 2);
    lua_pushboolean(L, fl_filename_match(name, pattern));
    return 1;
    }

static int Filename_name(lua_State *L)
    {
    const char *name = luaL_checkstring(L, 1);
    lua_pushstring(L, fl_filename_name(name));
    return 1;
    }

static int Filename_setext(lua_State *L)
    {
    char buf[BUFLEN];
    size_t len;
    const char *name = Checklstring(L, 1, &len);
    const char *ext = luaL_checkstring(L, 2);
    strncpy(buf, name, len);
    buf[len] = '\0';
    fl_filename_setext(buf, BUFLEN, ext);
    lua_pushstring(L, buf);
    return 1;   
    }


static int Filename_list(lua_State *L)
    {
    struct dirent **list;
    Fl_File_Sort_F *s = fl_numericsort;
    const char *dir = luaL_checkstring(L, 1);
    if(!lua_isnone(L, 2))
        s = check_File_Sort_F(L, 2);
    int n = fl_filename_list(dir, &list, s);
    if(n < 0)
        return 0;
    for(int i = 0; i < n; i++)
        lua_pushstring(L, list[i]->d_name);
    fl_filename_free_list(&list, n);
    return n;
    }



static const struct luaL_Reg Functions[] = 
    {
        { "decode_uri", Decode_uri },
        { "filename_absolute", Filename_absolute },
        { "filename_expand", Filename_expand },
        { "filename_ext", Filename_ext },
        { "filename_isdir", Filename_isdir },
        { "filename_list", Filename_list },
        { "filename_match", Filename_match },
        { "filename_name", Filename_name },
        { "filename_relative", Filename_relative },
        { "filename_setext", Filename_setext },
        { "open_uri", Open_uri },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_FlFile(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

