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
 | Button                                                                       |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Button, ButtonNew)
WIDGET_DELETE(Button, ButtonDelete)
WIDGET_TYPE(Button, Buttontype)

static inline int Buttonvalue(lua_State *L)
    {
    Fl_Button *p = check_Button(L, 1);
    if(lua_isnone(L, 2))
        lua_pushboolean(L, p->value());
    else
        lua_pushboolean(L, p->value(checkboolean(L, 2)));
    return 1;
    }   

BOOLEAN_VOID(Button, set, Buttonset )
BOOLEAN_VOID(Button, clear, Buttonclear )
VOID_VOID(Button, setonly, Buttonsetonly)
GETSET_INT(Button, shortcut, Buttonshortcut)
GETSET_ENUM(Button, Boxtype, down_box, Buttondown_box)

static const struct luaL_Reg Methods[] = 
    {
        { "clear", Buttonclear },
        { "down_box", Buttondown_box },
        { "set", Buttonset },
        { "setonly", Buttonsetonly },
        { "shortcut", Buttonshortcut },
        { "value", Buttonvalue },
        { "type", Buttontype },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  ButtonDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Light_Button                                                                 |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Light_Button, Light_ButtonNew)
WIDGET_DELETE(Light_Button, Light_ButtonDelete)

#define Light_ButtonMethods NULL

static const struct luaL_Reg Light_ButtonMetaMethods[] = 
    {
        { "__gc",  Light_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Check_Button                                                                 |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Check_Button, Check_ButtonNew)
WIDGET_DELETE(Check_Button, Check_ButtonDelete)

#define Check_ButtonMethods NULL

static const struct luaL_Reg Check_ButtonMetaMethods[] = 
    {
        { "__gc",  Check_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Round_Button                                                                 |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Round_Button, Round_ButtonNew)
WIDGET_DELETE(Round_Button, Round_ButtonDelete)

#define Round_ButtonMethods NULL

static const struct luaL_Reg Round_ButtonMetaMethods[] = 
    {
        { "__gc",  Round_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Radio_Round_Button                                                           |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Radio_Round_Button, Radio_Round_ButtonNew)
WIDGET_DELETE(Radio_Round_Button, Radio_Round_ButtonDelete)

#define Radio_Round_ButtonMethods NULL

static const struct luaL_Reg Radio_Round_ButtonMetaMethods[] = 
    {
        { "__gc",  Radio_Round_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 |  Radio_Light_Button                                                          |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Radio_Light_Button, Radio_Light_ButtonNew)
WIDGET_DELETE(Radio_Light_Button, Radio_Light_ButtonDelete)

#define Radio_Light_ButtonMethods NULL

static const struct luaL_Reg Radio_Light_ButtonMetaMethods[] = 
    {
        { "__gc",  Radio_Light_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Radio_Button                                                                 |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Radio_Button, Radio_ButtonNew)
WIDGET_DELETE(Radio_Button, Radio_ButtonDelete)

#define Radio_ButtonMethods NULL

static const struct luaL_Reg Radio_ButtonMetaMethods[] = 
    {
        { "__gc",  Radio_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Repeat_Button                                                                |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Repeat_Button, Repeat_ButtonNew)
WIDGET_DELETE(Repeat_Button, Repeat_ButtonDelete)

VOID_VOID(Repeat_Button, deactivate, Repeat_ButtonDeactivate)

static const struct luaL_Reg Repeat_ButtonMethods[] = 
    {
        { "deactivate",  Repeat_ButtonDeactivate },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Repeat_ButtonMetaMethods[] = 
    {
        { "__gc",  Repeat_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Return_Button                                                                |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Return_Button, Return_ButtonNew)
WIDGET_DELETE(Return_Button, Return_ButtonDelete)

#define Return_ButtonMethods NULL

static const struct luaL_Reg Return_ButtonMetaMethods[] = 
    {
        { "__gc",  Return_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Toggle_Button                                                                |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Toggle_Button, Toggle_ButtonNew)
WIDGET_DELETE(Toggle_Button, Toggle_ButtonDelete)

#define Toggle_ButtonMethods NULL

static const struct luaL_Reg Toggle_ButtonMetaMethods[] = 
    {
        { "__gc",  Toggle_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "button",  ButtonNew },
        { "light_button",  Light_ButtonNew }, /* aka Toggle_Light_Button */
        { "check_button",  Check_ButtonNew },
        { "radio_light_button",  Radio_Light_ButtonNew },
        { "round_button",  Round_ButtonNew }, /* aka Toggle_Round_Button */
        { "radio_round_button", Radio_Round_ButtonNew },
        { "radio_button",  Radio_ButtonNew },
        { "repeat_button",  Repeat_ButtonNew },
        { "return_button",  Return_ButtonNew },
        { "toggle_button",  Toggle_ButtonNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Button(lua_State *L)
    {
    udata_define(L, MT_Button, Methods, MetaMethods);
    udata_inherit(L, MT_Button, MT_Widget);

    udata_define(L, MT_Light_Button, Light_ButtonMethods, Light_ButtonMetaMethods);
    udata_inherit(L, MT_Light_Button, MT_Button);

    udata_define(L, MT_Check_Button, Check_ButtonMethods, Check_ButtonMetaMethods);
    udata_inherit(L, MT_Check_Button, MT_Light_Button);

    udata_define(L,MT_Radio_Light_Button,Radio_Light_ButtonMethods,Radio_Light_ButtonMetaMethods);
    udata_inherit(L, MT_Radio_Light_Button, MT_Light_Button);

    udata_define(L, MT_Round_Button, Round_ButtonMethods, Round_ButtonMetaMethods);
    udata_inherit(L, MT_Round_Button, MT_Light_Button);

    udata_define(L,MT_Radio_Round_Button,Radio_Round_ButtonMethods,Radio_Round_ButtonMetaMethods);
    udata_inherit(L, MT_Radio_Round_Button, MT_Round_Button);

    udata_define(L, MT_Radio_Button, Radio_ButtonMethods, Radio_ButtonMetaMethods);
    udata_inherit(L, MT_Radio_Button, MT_Button);

    udata_define(L, MT_Repeat_Button, Repeat_ButtonMethods, Repeat_ButtonMetaMethods);
    udata_inherit(L, MT_Repeat_Button, MT_Button);

    udata_define(L, MT_Return_Button, Return_ButtonMethods, Return_ButtonMetaMethods);
    udata_inherit(L, MT_Return_Button, MT_Button);

    udata_define(L, MT_Toggle_Button, Toggle_ButtonMethods, Toggle_ButtonMetaMethods);
    udata_inherit(L, MT_Toggle_Button, MT_Button);

    luaL_setfuncs(L, Functions, 0);
    }

