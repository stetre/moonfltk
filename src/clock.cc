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
 | Clock_Output                                                                 |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Clock_Output, Clock_OutputNew)
WIDGET_DELETE(Clock_Output, Clock_OutputDelete)

static int Clock_Outputvalue(lua_State *L)
    {
    Fl_Clock_Output *clock = check_Clock_Output(L, 1);
    int arg = 2;
    if(lua_isnone(L, arg))
        {
        lua_pushinteger(L, clock->value());
        return 1;
        }
    ulong h = luaL_checkinteger(L, arg++);
    if(lua_isnone(L, arg))
        {
        clock->value(h);
        return 0;
        }
    ulong m = luaL_checkinteger(L, arg++);
    ulong s = luaL_checkinteger(L, arg++);
    clock->value(h, m, s);
    return 0;
    }

INT_VOID(Clock_Output, hour, Clock_Outputhour)
INT_VOID(Clock_Output, minute, Clock_Outputminute)
INT_VOID(Clock_Output, second, Clock_Outputsecond)

static const struct luaL_Reg Clock_OutputMethods[] = 
    {
        { "hour", Clock_Outputhour },
        { "minute", Clock_Outputminute },
        { "second", Clock_Outputsecond },
        { "value", Clock_Outputvalue  },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Clock_OutputMetaMethods[] = 
    {
        { "__gc",  Clock_OutputDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Clock                                                                    |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Clock, ClockNew)
WIDGET_DELETE(Clock, ClockDelete)

#define ClockMethods NULL

static const struct luaL_Reg ClockMetaMethods[] = 
    {
        { "__gc",  ClockDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Round_Clock                                                                  |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Round_Clock, Round_ClockNew)
WIDGET_DELETE(Round_Clock, Round_ClockDelete)

#define Round_ClockMethods NULL

static const struct luaL_Reg Round_ClockMetaMethods[] = 
    {
        { "__gc",  Round_ClockDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "clock_output",  Clock_OutputNew },
        { "clock",  ClockNew },
        { "round_clock",  Round_ClockNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Clock(lua_State *L)
    {
    udata_define(L, MT_Clock_Output, Clock_OutputMethods, Clock_OutputMetaMethods);
    udata_inherit(L, MT_Clock_Output, MT_Box);

    udata_define(L, MT_Clock, ClockMethods, ClockMetaMethods);
    udata_inherit(L, MT_Clock, MT_Clock_Output);

    udata_define(L, MT_Round_Clock, Round_ClockMethods, Round_ClockMetaMethods);
    udata_inherit(L, MT_Round_Clock, MT_Clock);

    luaL_setfuncs(L, Functions, 0);
    }

