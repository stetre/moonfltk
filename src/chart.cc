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
 | Fl_Chart                                                                     |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Chart, ChartNew)
WIDGET_DELETE(Chart, ChartDelete)
WIDGET_TYPE(Chart, Charttype)
VOID_VOID(Chart, clear, ChartClear) 
GETSET_ENUM(Chart, Font, textfont, ChartTextfont)
GETSET_INTTYPE(Chart, Fontsize, textsize, ChartTextsize)
GETSET_INTTYPE(Chart, Color, textcolor, ChartTextcolor)
GETSET_INT(Chart, maxsize, ChartMaxsize)
GETSET_BOOLEAN(Chart, autosize, ChartAutosize)

static int ChartAdd(lua_State *L)
    {
    Fl_Chart *p = check_Chart(L, 1);
    double val = luaL_checknumber(L, 2);
    const char *str = luaL_optstring(L, 3, NULL);
    unsigned col = 0;
    if(!lua_isnoneornil(L, 4))
        col = check_Color(L, 4);
    p->add(val, str, col);
    return 0;
    }

#define F(method, Func)                     \
static int Func(lua_State *L)               \
    {                                       \
    Fl_Chart *p = check_Chart(L, 1);        \
    int ind = luaL_checkinteger(L, 2);      \
    double val = luaL_checknumber(L, 3);            \
    const char *str = luaL_optstring(L, 4, NULL);   \
    unsigned col = 0;                       \
    if(!lua_isnoneornil(L, 5))              \
        col = check_Color(L, 5);            \
    p->method(ind, val, str, col);          \
    return 0;                               \
    }

F(insert, ChartInsert)
F(replace, ChartReplace)

#undef F

static int ChartBounds(lua_State *L)
/* a, b = bounds(),
 * bounds(a, b)
 */
    {
    Fl_Chart *p = check_Chart(L, 1);
    if(lua_isnone(L, 2))
        {
        double a, b;
        p->bounds(&a, &b);
        lua_pushnumber(L, a);
        lua_pushnumber(L, b);
        return 2;
        }
    p->bounds(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
    return 0;
    }

static int ChartSize(lua_State *L)
    {
    Fl_Chart *p = check_Chart(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushinteger(L, p->size());
        return 1;
        }
    p->size(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
    }

static const struct luaL_Reg ChartMethods[] = 
    {
        { "add", ChartAdd },
        { "autosize", ChartAutosize },
        { "bounds", ChartBounds },
        { "clear", ChartClear },
        { "insert", ChartInsert },
        { "maxsize", ChartMaxsize },
        { "replace", ChartReplace },
        { "size", ChartSize },
        { "textcolor", ChartTextcolor },
        { "textfont", ChartTextfont },
        { "textsize", ChartTextsize },
        { "type", Charttype  },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg ChartMetaMethods[] = 
    {
        { "__gc",  ChartDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "chart",  ChartNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Chart(lua_State *L)
    {
    udata_define(L, MT_Chart, ChartMethods, ChartMetaMethods);
    udata_inherit(L, MT_Chart, MT_Widget);
    luaL_setfuncs(L, Functions, 0);
    }

