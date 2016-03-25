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
 | Fl_Slider                                                                    |
 *------------------------------------------------------------------------------*/

/* This constructor is redundant:
 * Fl_Slider(uchar type,int X,int Y,int W,int H, const char *L); */
WIDGET_NEW(Slider, SliderNew)
WIDGET_DELETE(Slider, SliderDelete)
WIDGET_TYPE(Slider, Slidertype)
GETSET_ENUM(Slider, Boxtype, slider, Sliderslider)
GETSET_DOUBLE(Slider, slider_size, Sliderslider_size)
VOID_DOUBLE_DOUBLE(Slider, bounds, Sliderbounds)

static int Sliderscrollvalue(lua_State *L)
    {
    Fl_Slider *p = check_Slider(L, 1);
    int pos = luaL_checkinteger(L, 2);
    int size = luaL_checkinteger(L, 3);
    int first = luaL_checkinteger(L, 4);
    int total = luaL_checkinteger(L, 5);
    lua_pushnumber(L, p->scrollvalue(pos, size, first, total));
    return 1;
    }

static const struct luaL_Reg SliderMethods[] = 
    {
        { "bounds", Sliderbounds },
        { "scrollvalue", Sliderscrollvalue },
        { "slider", Sliderslider },
        { "slider_size", Sliderslider_size },
        { "type", Slidertype },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg SliderMetaMethods[] = 
    {
        { "__gc",  SliderDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Fill_Slider                                                                  |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Fill_Slider, Fill_SliderNew)
WIDGET_DELETE(Fill_Slider, Fill_SliderDelete)

#define Fill_SliderMethods NULL

static const struct luaL_Reg Fill_SliderMetaMethods[] = 
    {
        { "__gc",  Fill_SliderDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Hor_Fill_Slider                                                              |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Hor_Fill_Slider, Hor_Fill_SliderNew)
WIDGET_DELETE(Hor_Fill_Slider, Hor_Fill_SliderDelete)

#define Hor_Fill_SliderMethods NULL

static const struct luaL_Reg Hor_Fill_SliderMetaMethods[] = 
    {
        { "__gc",  Hor_Fill_SliderDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Hor_Nice_Slider                                                              |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Hor_Nice_Slider, Hor_Nice_SliderNew)
WIDGET_DELETE(Hor_Nice_Slider, Hor_Nice_SliderDelete)

#define Hor_Nice_SliderMethods NULL

static const struct luaL_Reg Hor_Nice_SliderMetaMethods[] = 
    {
        { "__gc",  Hor_Nice_SliderDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Hor_Slider                                                                   |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Hor_Slider, Hor_SliderNew)
WIDGET_DELETE(Hor_Slider, Hor_SliderDelete)

#define Hor_SliderMethods NULL

static const struct luaL_Reg Hor_SliderMetaMethods[] = 
    {
        { "__gc",  Hor_SliderDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Nice_Slider                                                                  |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Nice_Slider, Nice_SliderNew)
WIDGET_DELETE(Nice_Slider, Nice_SliderDelete)

#define Nice_SliderMethods NULL

static const struct luaL_Reg Nice_SliderMetaMethods[] = 
    {
        { "__gc",  Nice_SliderDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Scrollbar                                                                    |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Scrollbar, ScrollbarNew)
WIDGET_DELETE(Scrollbar, ScrollbarDelete)
GETSET_INT(Scrollbar, linesize, Scrollbarlinesize)
WIDGET_TYPE(Scrollbar, Scrollbartype)

static int Scrollbarvalue(lua_State *L)
    {
    Fl_Scrollbar *p = check_Scrollbar(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushinteger(L, p->value());
        return 1;
        }
    int pos = luaL_checkinteger(L, 2);
    if(lua_isnone(L, 3))
        {
        lua_pushboolean(L, p->value(pos));
        return 1;
        }
    int size = luaL_checkinteger(L, 3);
    int first = luaL_checkinteger(L, 4);
    int total = luaL_checkinteger(L, 5);
    lua_pushboolean(L, p->value(pos, size, first, total));
    return 1;
    }

static const struct luaL_Reg ScrollbarMethods[] = 
    {
        { "type", Scrollbartype },
        { "linesize", Scrollbarlinesize },
        { "value", Scrollbarvalue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg ScrollbarMetaMethods[] = 
    {
        { "__gc",  ScrollbarDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Value_Slider                                                                 |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Value_Slider, Value_SliderNew)
WIDGET_DELETE(Value_Slider, Value_SliderDelete)
GETSET_ENUM(Value_Slider, Font, textfont, Value_Slidertextfont)
GETSET_INTTYPE(Value_Slider, Fontsize, textsize, Value_Slidertextsize)
GETSET_INTTYPE(Value_Slider, Color, textcolor, Value_Slidertextcolor)

static const struct luaL_Reg Value_SliderMethods[] = 
    {
        { "textfont", Value_Slidertextfont },
        { "textsize", Value_Slidertextsize },
        { "textcolor", Value_Slidertextcolor },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Value_SliderMetaMethods[] = 
    {
        { "__gc",  Value_SliderDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Hor_Value_Slider                                                             |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Hor_Value_Slider, Hor_Value_SliderNew)
WIDGET_DELETE(Hor_Value_Slider, Hor_Value_SliderDelete)

#define Hor_Value_SliderMethods NULL

static const struct luaL_Reg Hor_Value_SliderMetaMethods[] = 
    {
        { "__gc",  Hor_Value_SliderDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "slider",  SliderNew },
        { "fill_slider",  Fill_SliderNew },
        { "hor_fill_slider",  Hor_Fill_SliderNew },
        { "hor_nice_slider",  Hor_Nice_SliderNew },
        { "hor_slider",  Hor_SliderNew },
        { "nice_slider",  Nice_SliderNew },
        { "scrollbar",  ScrollbarNew },
        { "value_slider",  Value_SliderNew },
        { "hor_value_slider",  Hor_Value_SliderNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Slider(lua_State *L)
    {
    udata_define(L, MT_Slider, SliderMethods, SliderMetaMethods);
    udata_inherit(L, MT_Slider, MT_Valuator);

    udata_define(L, MT_Fill_Slider, Fill_SliderMethods, Fill_SliderMetaMethods);
    udata_inherit(L, MT_Fill_Slider, MT_Slider);

    udata_define(L, MT_Hor_Fill_Slider, Hor_Fill_SliderMethods, Hor_Fill_SliderMetaMethods);
    udata_inherit(L, MT_Hor_Fill_Slider, MT_Slider);

    udata_define(L, MT_Hor_Nice_Slider, Hor_Nice_SliderMethods, Hor_Nice_SliderMetaMethods);
    udata_inherit(L, MT_Hor_Nice_Slider, MT_Slider);

    udata_define(L, MT_Hor_Slider, Hor_SliderMethods, Hor_SliderMetaMethods);
    udata_inherit(L, MT_Hor_Slider, MT_Slider);

    udata_define(L, MT_Nice_Slider, Nice_SliderMethods, Nice_SliderMetaMethods);
    udata_inherit(L, MT_Nice_Slider, MT_Slider);

    udata_define(L, MT_Scrollbar, ScrollbarMethods, ScrollbarMetaMethods);
    udata_inherit(L, MT_Scrollbar, MT_Slider);

    udata_define(L, MT_Value_Slider, Value_SliderMethods, Value_SliderMetaMethods);
    udata_inherit(L, MT_Value_Slider, MT_Slider);

    udata_define(L, MT_Hor_Value_Slider, Hor_Value_SliderMethods, Hor_Value_SliderMetaMethods);
    udata_inherit(L, MT_Hor_Value_Slider, MT_Value_Slider);

    luaL_setfuncs(L, Functions, 0);
    }


