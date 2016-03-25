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
 | Fl_Valuator (abstract class)                                                 |
 *------------------------------------------------------------------------------*/

VOID_DOUBLE_DOUBLE(Valuator, bounds, Valuatorbounds)
GETSET_DOUBLE(Valuator, minimum, Valuatorminimum)
GETSET_DOUBLE(Valuator, maximum, Valuatormaximum)
VOID_DOUBLE_DOUBLE(Valuator, range, Valuatorrange)
VOID_INT(Valuator, precision, Valuatorprecision)
DOUBLE_DOUBLE(Valuator, round, Valuatorround)
DOUBLE_DOUBLE(Valuator, clamp, Valuatorclamp)

static inline int Valuatorincrement(lua_State *L)
    {
    Fl_Valuator *p = check_Valuator(L, 1);
    lua_pushnumber(L,  p->increment(luaL_checknumber(L, 2), luaL_checkinteger(L, 3)));
    return 1;
    }   

static int Valuatorstep(lua_State *L)
    {
    Fl_Valuator *p = check_Valuator(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushnumber(L, p->step());
        return 1;
        }
    double a = luaL_checknumber(L, 2);
    if(lua_isnone(L, 3))
        {
        p->step(a);
        return 0;
        }
    int b = luaL_checkinteger(L, 3);
    p->step(a, b);
    return 0;
    }

static int Valuatorvalue(lua_State *L)
    {
    Fl_Valuator *p = check_Valuator(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushnumber(L, p->value());
        return 1;
        }
    double val = luaL_checknumber(L, 2);
    lua_pushboolean(L, p->value(val));
    return 1;
    }

//virtual int format(char*); @@
static int Valuatorformat(lua_State *L)
    {
    Fl_Valuator *p = check_Valuator(L, 1);
    char buf[128];
    size_t len = p->format(buf);
    lua_pushlstring(L, buf, len);
    return 1;
    }

static const struct luaL_Reg ValuatorMethods[] = 
    {
//      { "type", Valuatortype },
        { "bounds", Valuatorbounds },
        { "clamp", Valuatorclamp },
        { "format", Valuatorformat },
        { "increment", Valuatorincrement },
        { "maximum", Valuatormaximum },
        { "minimum", Valuatorminimum },
        { "precision", Valuatorprecision },
        { "range", Valuatorrange },
        { "round", Valuatorround },
        { "step", Valuatorstep },
        { "value", Valuatorvalue },
        { NULL, NULL } /* sentinel */
    };

#define ValuatorMetaMethods NULL

#if(0)
// NON EXPOSED Fl_Valuator METHODS
// --------------------------------------------------------------------------
// shared type() values for classes that work in both directions:
#define FL_VERTICAL     0 ///< The valuator can work vertically
#define FL_HORIZONTAL       1 ///< The valuator can work horizontally
// type()  (get/set) --> use the method of the subclass
#endif

/*------------------------------------------------------------------------------*
 | Adjuster                                                                     |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Adjuster, AdjusterNew)
WIDGET_DELETE(Adjuster, AdjusterDelete)
GETSET_BOOLEAN(Adjuster, soft, Adjustersoft)

static const struct luaL_Reg AdjusterMethods[] = 
    {
        { "soft", Adjustersoft },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg AdjusterMetaMethods[] = 
    {
        { "__gc",  AdjusterDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Counter                                                                      |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Counter, CounterNew)
WIDGET_DELETE(Counter, CounterDelete)
WIDGET_TYPE(Counter, Countertype)

GETSET_ENUM(Counter, Font, textfont, Countertextfont)
GETSET_INTTYPE(Counter, Fontsize, textsize, Countertextsize)
GETSET_INTTYPE(Counter, Color, textcolor, Countertextcolor)

static int Counterstep(lua_State *L)
    {
    Fl_Counter *p = check_Counter(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushnumber(L, p->step());
        return 1;
        }
    double a = luaL_checknumber(L, 2);
    if(lua_isnone(L, 3))
        {
        p->step(a);
        return 0;
        }
    double b = luaL_checknumber(L, 3);
    p->step(a, b);
    return 0;
    }

VOID_DOUBLE(Counter, lstep, Counterlstep)

static const struct luaL_Reg CounterMethods[] = 
    {
        { "lstep", Counterlstep },
        { "step", Counterstep },
        { "textcolor", Countertextcolor },
        { "textfont", Countertextfont },
        { "textsize", Countertextsize },
        { "type", Countertype },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg CounterMetaMethods[] = 
    {
        { "__gc",  CounterDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Simple_Counter                                                               |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Simple_Counter, Simple_CounterNew)
WIDGET_DELETE(Simple_Counter, Simple_CounterDelete)

#define Simple_CounterMethods NULL

static const struct luaL_Reg Simple_CounterMetaMethods[] = 
    {
        { "__gc",  Simple_CounterDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Dial                                                                         |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Dial, DialNew)
WIDGET_DELETE(Dial, DialDelete)
WIDGET_TYPE(Dial, Dialtype)
GETSET_INT(Dial, angle1, Dialangle1)
GETSET_INT(Dial, angle2, Dialangle2)
VOID_INT_INT(Dial, angles, Dialangles)

static const struct luaL_Reg DialMethods[] = 
    {
        { "angle1", Dialangle1 },
        { "angle2", Dialangle2 },
        { "angles", Dialangles },
        { "type", Dialtype },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg DialMetaMethods[] = 
    {
        { "__gc",  DialDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Fill_Dial                                                                    |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Fill_Dial, Fill_DialNew)
WIDGET_DELETE(Fill_Dial, Fill_DialDelete)

#define Fill_DialMethods NULL

static const struct luaL_Reg Fill_DialMetaMethods[] = 
    {
        { "__gc",  Fill_DialDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Line_Dial                                                                    |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Line_Dial, Line_DialNew)
WIDGET_DELETE(Line_Dial, Line_DialDelete)

#define Line_DialMethods NULL

static const struct luaL_Reg Line_DialMetaMethods[] = 
    {
        { "__gc",  Line_DialDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Roller                                                                       |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Roller, RollerNew)
WIDGET_DELETE(Roller, RollerDelete)
WIDGET_TYPE(Roller, Rollertype)

static const struct luaL_Reg RollerMethods[] = 
    {
        { "type", Rollertype },
        { NULL, NULL } /* sentinel */
    };


static const struct luaL_Reg RollerMetaMethods[] = 
    {
        { "__gc",  RollerDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Value_Input                                                                  |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Value_Input, Value_InputNew)
WIDGET_DELETE(Value_Input, Value_InputDelete)
GETSET_ENUM(Value_Input, Font, textfont, Value_Inputtextfont)
GETSET_INTTYPE(Value_Input, Fontsize, textsize, Value_Inputtextsize)
GETSET_INTTYPE(Value_Input, Color, textcolor, Value_Inputtextcolor)
GETSET_INTTYPE(Value_Input, Color, cursor_color, Value_Inputcursor_color)
GETSET_BOOLEAN(Value_Input, soft, Value_Inputsoft)
GETSET_INT(Value_Input, shortcut, Value_Inputshortcut)
VOID_INT4(Value_Input, resize, Value_Inputresize)

static const struct luaL_Reg Value_InputMethods[] = 
    {
        { "cursor_color", Value_Inputcursor_color },
        { "resize", Value_Inputresize },
        { "shortcut", Value_Inputshortcut },
        { "soft", Value_Inputsoft },
        { "textcolor", Value_Inputtextcolor },
        { "textfont", Value_Inputtextfont },
        { "textsize", Value_Inputtextsize },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Value_InputMetaMethods[] = 
    {
        { "__gc",  Value_InputDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Value_Output                                                                 |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Value_Output, Value_OutputNew)
WIDGET_DELETE(Value_Output, Value_OutputDelete)
GETSET_ENUM(Value_Output, Font, textfont, Value_Outputtextfont)
GETSET_INTTYPE(Value_Output, Fontsize, textsize, Value_Outputtextsize)
GETSET_INTTYPE(Value_Output, Color, textcolor, Value_Outputtextcolor)
GETSET_BOOLEAN(Value_Output, soft, Value_Outputsoft)

static const struct luaL_Reg Value_OutputMethods[] = 
    {
        { "soft", Value_Outputsoft },
        { "textcolor", Value_Outputtextcolor },
        { "textfont", Value_Outputtextfont },
        { "textsize", Value_Outputtextsize },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Value_OutputMetaMethods[] = 
    {
        { "__gc",  Value_OutputDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "adjuster",  AdjusterNew },
        { "counter",  CounterNew },
        { "simple_counter",  Simple_CounterNew },
        { "dial",  DialNew },
        { "fill_dial",  Fill_DialNew },
        { "line_dial",  Line_DialNew },
        { "roller",  RollerNew },
        { "value_input",  Value_InputNew },
        { "value_output",  Value_OutputNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Valuator(lua_State *L)
    {
    udata_define(L, MT_Valuator, ValuatorMethods, ValuatorMetaMethods);
    udata_inherit(L, MT_Valuator, MT_Widget);

    udata_define(L, MT_Adjuster, AdjusterMethods, AdjusterMetaMethods);
    udata_inherit(L, MT_Adjuster, MT_Valuator);

    udata_define(L, MT_Counter, CounterMethods, CounterMetaMethods);
    udata_inherit(L, MT_Counter, MT_Valuator);

    udata_define(L, MT_Simple_Counter, Simple_CounterMethods, Simple_CounterMetaMethods);
    udata_inherit(L, MT_Simple_Counter, MT_Counter);

    udata_define(L, MT_Dial, DialMethods, DialMetaMethods);
    udata_inherit(L, MT_Dial, MT_Valuator);

    udata_define(L, MT_Fill_Dial, Fill_DialMethods, Fill_DialMetaMethods);
    udata_inherit(L, MT_Fill_Dial, MT_Dial);

    udata_define(L, MT_Line_Dial, Line_DialMethods, Line_DialMetaMethods);
    udata_inherit(L, MT_Line_Dial, MT_Dial);

    udata_define(L, MT_Roller, RollerMethods, RollerMetaMethods);
    udata_inherit(L, MT_Roller, MT_Valuator);

    udata_define(L, MT_Value_Input, Value_InputMethods, Value_InputMetaMethods);
    udata_inherit(L, MT_Value_Input, MT_Valuator);

    udata_define(L, MT_Value_Output, Value_OutputMethods, Value_OutputMetaMethods);
    udata_inherit(L, MT_Value_Output, MT_Valuator);

    luaL_setfuncs(L, Functions, 0);
    }


