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

GROUP_NEW(Tabs, TabsNew)
GROUP_DELETE(Tabs, TabsDelete)

static int Tabsvalue(lua_State *L)
    {
    Fl_Tabs *p = check_Tabs(L, 1);
    if(lua_isnone(L, 2))
        {
        Fl_Widget *w = p->value();
        if(!w) return 0;
        push_Widget(L, w);
        return 1;
        }
    lua_pushboolean(L, p->value(check_Widget(L, 2)));
    return 1;
    }

static int Tabspush(lua_State *L)
    {
    Fl_Tabs *p = check_Tabs(L, 1);
    if(lua_isnone(L, 2))
        {
        Fl_Widget *w = p->push();
        if(!w) return 0;
        push_Widget(L, w);
        return 1;
        }
    lua_pushboolean(L, p->push(check_Widget(L, 2)));
    return 1;
    }

static int Tabswhich(lua_State *L)
    {
    Fl_Tabs *p = check_Tabs(L, 1);
    int event_x = check_Event(L, 2);
    int event_y = check_Event(L, 3);
    Fl_Widget *w = p->which(event_x, event_y);
    if(!w) return 0;
    push_Widget(L, w);
    return 1;
    }

static int Tabsclient_area(lua_State *L)
/* rx, ry, rw, rh = client_area([tabh]) */
    {
    int rx, ry, rw, rh;
    Fl_Tabs *p = check_Tabs(L, 1);
    int tabh = luaL_optinteger(L, 2, 0);
    p->client_area(rx, ry, rw, rh, tabh);
    lua_pushinteger(L, rx);
    lua_pushinteger(L, ry);
    lua_pushinteger(L, rw);
    lua_pushinteger(L, rh);
    return 4;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg TabsMethods[] = 
    {
        { "client_area", Tabsclient_area },
        { "push", Tabspush },
        { "value", Tabsvalue },
        { "which", Tabswhich },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg TabsMetaMethods[] = 
    {
        { "__gc",  TabsDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg TabsFunctions[] = 
    {
        { "tabs",  TabsNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Tabs(lua_State *L)
    {
    udata_define(L, MT_Tabs, TabsMethods, TabsMetaMethods);
    udata_inherit(L, MT_Tabs, MT_Group);
    luaL_setfuncs(L, TabsFunctions, 0);
    }

