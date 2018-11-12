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

#ifdef BEGIN_FLTK_INCLUDE
    BEGIN_FLTK_INCLUDE // {
#endif

#include <FL/fl_show_colormap.H>
#if defined(LINUX) && FL_MAJOR_VERSION >= 1 && FL_MINOR_VERSION >= 4
    #include <FL/platform.H>
#endif

#ifdef END_FLTK_INCLUDE
    END_FLTK_INCLUDE   // }
#endif

/*----------------------------------------------------------------------*
 | Fl_Color                                                             |
 *----------------------------------------------------------------------*/

Fl_Color check_Color(lua_State *L, int arg)
    { return luaL_checkinteger(L, arg); }

int push_Color(lua_State *L, Fl_Color color)
    {
    lua_pushinteger(L, (unsigned int)color);
    return 1;
    }

Fl_Color check_ColorIndex(lua_State *L, int arg)
    {
    Fl_Color index = luaL_checkinteger(L, arg);
    if((index & 0x0ff) != index)
        return luaL_argerror(L, arg, "out of range");
    return index;
    }

/*------------------------------------------------------------------------------*
 | Color functions                                                              |
 *------------------------------------------------------------------------------*/


static int Inactive(lua_State *L)
    {
    return push_Color(L, fl_inactive(check_Color(L, 1)));
    }

static int Contrast(lua_State *L)
    {
    return push_Color(L, fl_contrast(check_Color(L, 1), check_Color(L, 2)));
    }

static int Color_average(lua_State *L)
    {
    return push_Color(L, 
        fl_color_average(check_Color(L, 1), check_Color(L, 2), luaL_checknumber(L, 3)));
    }

static int Lighter(lua_State *L)
    {
    return push_Color(L, fl_lighter(check_Color(L, 1)));
    }

static int Darker(lua_State *L)
    {
    return push_Color(L, fl_darker(check_Color(L, 1)));
    }

static int Color(lua_State *L)
    {
    if(lua_isnone(L, 1))
        return push_Color(L, fl_color());
    if(lua_isnone(L, 2))
        fl_color(check_Color(L, 1));
    else
        fl_color(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
    }

static int Show_colormap(lua_State *L)
    {
    return push_Color(L, fl_show_colormap(check_Color(L, 1)));
    }

#ifdef LINUX
static int Xpixel(lua_State *L)
    {
    ulong n;
    if(lua_isnone(L, 2))
        n = fl_xpixel(check_ColorIndex(L, 1));
    else
        n = fl_xpixel(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    lua_pushinteger(L, n);
    return 1;
    }
#endif

static int Free_color(lua_State *L)
    {
#ifdef MACOSX
    (void)L; /* @@ missing symbol, it seems */
#else
    Fl_Color index = check_ColorIndex(L, 1);
    int overlay = optboolean(L, 2, 0);
    Fl::free_color(index, overlay);
#endif
    return 0;
    }

static int Get_color(lua_State *L)
/* rgbi, r, g, b = get_color(index) */
    {
    uchar r, g, b;
    Fl_Color index = check_ColorIndex(L, 1);
    push_Color(L, Fl::get_color(index));
    Fl::get_color(index, r, g, b);
    lua_pushinteger(L, r);
    lua_pushinteger(L, g);
    lua_pushinteger(L, b);
    return 4;
    }

static int Set_color(lua_State *L)
    {
    Fl_Color index = check_ColorIndex(L, 1);
    if(lua_isnone(L, 3))
        Fl::set_color(index, check_ColorIndex(L, 2));
    else
        Fl::set_color(index, 
            luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4)); 
    return 0;
    }


static int Rgb_color(lua_State *L)
    {
    if(lua_isnone(L, 2))
        push_Color(L, fl_rgb_color(luaL_checkinteger(L, 1)));
    else
        push_Color(L, fl_rgb_color(
                luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3)));
    return 1;
    }

static int Gray_ramp(lua_State *L)
    {
    push_Color(L, fl_gray_ramp(luaL_checkinteger(L, 1)));
    return 1;
    }

static int Color_cube(lua_State *L)
    {
    push_Color(L, fl_color_cube(
            luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3)));
    return 1;
    }

FUNC_VOID_VOID(Fl::get_system_colors, Get_system_colors)

FUNC_VOID_VOID(Fl::own_colormap, Own_colormap)
FUNC_VOID_INT3(Fl::foreground, Foreground)
FUNC_VOID_INT3(Fl::background, Background)
FUNC_VOID_INT3(Fl::background2, Background2)


#define F(func, Func)                                       \
static int Func(lua_State *L)                               \
    {                                                       \
    double in1 = luaL_checknumber(L, 1);                    \
    double in2 = luaL_checknumber(L, 2);                    \
    double in3 = luaL_checknumber(L, 3);                    \
    double out1, out2, out3;                                \
    Fl_Color_Chooser::func(in1, in2, in3, out1, out2, out3);    \
    lua_pushnumber(L, out1);        \
    lua_pushnumber(L, out2);        \
    lua_pushnumber(L, out3);        \
    return 3;                       \
    }

F(hsv2rgb, Hsv2rgb)
F(rgb2hsv, Rgb2hsv)

#undef F


static const struct luaL_Reg Functions[] = 
    {
        { "background", Background },
        { "background2", Background2 },
        { "color", Color },
        { "color_average", Color_average },
        { "color_cube", Color_cube },
        { "contrast", Contrast },
        { "darker", Darker },
        { "free_color", Free_color },
        { "foreground", Foreground },
        { "get_color", Get_color },
        { "get_system_colors", Get_system_colors },
        { "gray_ramp", Gray_ramp },
        { "inactive", Inactive },
        { "lighter", Lighter },
        { "own_colormap", Own_colormap },
        { "rgb_color", Rgb_color },
        { "set_color", Set_color },
        { "show_colormap", Show_colormap },
#ifdef LINUX
        { "xpixel", Xpixel },
#endif
        /* From Fl_Color_Chooser: */
        { "hsv2rgb",  Hsv2rgb },
        { "rgb2hsv",  Rgb2hsv },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_FlColor(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

