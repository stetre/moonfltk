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
 | Screen functions                                                             |
 *------------------------------------------------------------------------------*/

FUNC_INT_VOID( Fl::x, Get_x)
FUNC_INT_VOID( Fl::y, Get_y)
FUNC_INT_VOID( Fl::w, Get_w)
FUNC_INT_VOID( Fl::h, Get_h)
FUNC_INT_VOID( Fl::screen_count, Screen_count)

static int checkscreen_(lua_State *L, int arg, int optional) 
#define checkscreen(L, arg) checkscreen_((L), (arg), 0)
#define optscreen(L, arg) checkscreen_((L), (arg), 1)
    {
    int n;
    if(optional)
        n = optindex(L, arg, 0);
    else
        n = checkindex(L, arg);
    if((n < 0) || (n >= Fl::screen_count()))
        return luaL_argerror(L, 1, "out of range");
    return n;
    }


static int Screen_dpi(lua_State *L)
    {
    float h, v;
    int n = optscreen(L, 1);
    Fl::screen_dpi(h, v, n);
    lua_pushnumber(L, h);
    lua_pushnumber(L, v);
    return 2;
    }

static int Screen_num(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    if(lua_isnone(L, 3))
        pushindex(L, Fl::screen_num(x, y));
    else
        {
        int w = luaL_checkinteger(L, 3);
        int h = luaL_checkinteger(L, 4);
        pushindex(L, Fl::screen_num(x, y, w, h));
        }
    return 1;
    }


static int Screen_xywh(lua_State *L)
/*
 * screen_xywh()
 * screen_xywh(n)
 * screen_xywh(mx, my)
 * screen_xywh(mx, my, mw, mh)
 * -> x, y, w, h
 */
    {
    int x, y, w, h;
    if(lua_isnone(L, 1))
        Fl::screen_xywh(x, y, w, h);
    else if(lua_isnone(L, 2))
        {
        int n = checkscreen(L, 1);
        Fl::screen_xywh(x, y, w, h, n);
        }
    else
        {
        int mx = luaL_checkinteger(L, 1);
        int my = luaL_checkinteger(L, 2);
        if(lua_isnone(L, 3))
            Fl::screen_xywh(x, y, w, h, mx, my);
        else
            {
            int mw = luaL_checkinteger(L, 3);
            int mh = luaL_checkinteger(L, 4);
            Fl::screen_xywh(x, y, w, h, mx, my, mw, mh);
            }
        }
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 4;
    }

static int Screen_work_area(lua_State *L)
/*
 * screen_work_area()
 * screen_work_area(n)
 * screen_work_area(mx, my)
 * -> x, y, w, h
 */
    {
    int x, y, w, h;
    if(lua_isnone(L, 1))
        Fl::screen_work_area(x, y, w, h);
    else if(lua_isnone(L, 2))
        {
        int n = checkscreen(L, 1);
        Fl::screen_work_area(x, y, w, h, n);
        }
    else
        {
        int mx = luaL_checkinteger(L, 1);
        int my = luaL_checkinteger(L, 2);
        Fl::screen_work_area(x, y, w, h, mx, my);
        }
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 4;
    }

static int Get_xywh(lua_State *L) /* NONFLTK */
    {
    lua_pushinteger(L, Fl::x());
    lua_pushinteger(L, Fl::y());
    lua_pushinteger(L, Fl::w());
    lua_pushinteger(L, Fl::h());
    return 4;
    }


static const struct luaL_Reg Functions[] = 
    {
        { "h", Get_h },
        { "screen_count", Screen_count },
        { "screen_dpi", Screen_dpi },
        { "screen_num", Screen_num },
        { "screen_work_area", Screen_work_area },
        { "screen_xywh", Screen_xywh },
        { "w", Get_w },
        { "x", Get_x },
        { "y", Get_y },
        { "xywh", Get_xywh },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_FlScreen(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

