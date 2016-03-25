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

GROUP_NEW(Color_Chooser, Color_ChooserNew)
GROUP_DELETE(Color_Chooser, Color_ChooserDelete)

DOUBLE_VOID(Color_Chooser, hue, Color_Chooserhue)
DOUBLE_VOID(Color_Chooser, saturation, Color_Choosersaturation)
DOUBLE_VOID(Color_Chooser, value, Color_Chooservalue)
DOUBLE_VOID(Color_Chooser, r, Color_Chooserr)
DOUBLE_VOID(Color_Chooser, g, Color_Chooserg)
DOUBLE_VOID(Color_Chooser, b, Color_Chooserb)

#define F(method, Func) \
static int Func(lua_State *L)                           \
    {                                                   \
    Fl_Color_Chooser *p = check_Color_Chooser(L, 1);    \
    double rh = luaL_checknumber(L, 2);                 \
    double sg = luaL_checknumber(L, 2);                 \
    double vb = luaL_checknumber(L, 2);                 \
    lua_pushboolean(L, p->method(rh, sg, vb));          \
    return 1;                                           \
    }

F(hsv, Color_Chooserhsv)
F(rgb, Color_Chooserrgb)

#undef F

static int Color_Choosermode(lua_State *L)
    {
    Fl_Color_Chooser *p = check_Color_Chooser(L, 1);
    if(lua_isnone(L, 2))
        return push_Color_ChooserMode(L, p->mode());
    p->mode(check_Color_ChooserMode(L, 2));
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Color_ChooserMethods[] = 
    {
        { "b", Color_Chooserb },
        { "g", Color_Chooserg },
        { "hue", Color_Chooserhue },
        { "hsv", Color_Chooserhsv },
        { "mode", Color_Choosermode },
        { "r", Color_Chooserr },
        { "rgb", Color_Chooserrgb },
        { "saturation", Color_Choosersaturation },
        { "value", Color_Chooservalue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Color_ChooserMetaMethods[] = 
    {
        { "__gc",  Color_ChooserDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Color_ChooserFunctions[] = 
    {
        { "color_chooser",  Color_ChooserNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Color_Chooser(lua_State *L)
    {
    udata_define(L, MT_Color_Chooser, Color_ChooserMethods, Color_ChooserMetaMethods);
    udata_inherit(L, MT_Color_Chooser, MT_Group);
    luaL_setfuncs(L, Color_ChooserFunctions, 0);
    }

