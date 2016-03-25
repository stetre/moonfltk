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

/************************************************************************
 * Enums for the widget type() method                                   *
 ************************************************************************/

uchar check_WidgetRtti(lua_State *L, int arg)
    {
    return (uchar)luaL_argerror(L, arg, "type() not supported by this widget");
    }

int push_WidgetRtti(lua_State *L, uchar type)
    {
    (void)type;
    lua_pushstring(L, "undefined");
    return 1;
    }

uchar check_ButtonRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_NORMAL_BUTTON, "normal");
        CASE(FL_TOGGLE_BUTTON, "toggle");
        CASE(FL_RADIO_BUTTON, "radio");
        CASE(FL_HIDDEN_BUTTON, "hidden");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_ButtonRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_NORMAL_BUTTON, "normal");
        CASE(FL_TOGGLE_BUTTON, "toggle");
        CASE(FL_RADIO_BUTTON, "radio");
        CASE(FL_HIDDEN_BUTTON, "hidden");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


uchar check_PackRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(Fl_Pack::HORIZONTAL, "horizontal"); 
        CASE(Fl_Pack::VERTICAL, "vertical"); 
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_PackRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(Fl_Pack::HORIZONTAL, "horizontal"); 
        CASE(Fl_Pack::VERTICAL, "vertical"); 
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


uchar check_ScrollRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(0, "no scrollbar"); 
        CASE(Fl_Scroll::HORIZONTAL, "horizontal"); 
        CASE(Fl_Scroll::VERTICAL, "vertical"); 
        CASE(Fl_Scroll::BOTH, "both");
        CASE(Fl_Scroll::ALWAYS_ON, "always on");
        CASE(Fl_Scroll::HORIZONTAL_ALWAYS, "horizontal always"); 
        CASE(Fl_Scroll::VERTICAL_ALWAYS, "vertical always");
        CASE(Fl_Scroll::BOTH_ALWAYS, "both always");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_ScrollRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(0, "no scrollbar"); 
        CASE(Fl_Scroll::HORIZONTAL, "horizontal"); 
        CASE(Fl_Scroll::VERTICAL, "vertical"); 
        CASE(Fl_Scroll::BOTH, "both");
        CASE(Fl_Scroll::ALWAYS_ON, "always on");
        CASE(Fl_Scroll::HORIZONTAL_ALWAYS, "horizontal always"); 
        CASE(Fl_Scroll::VERTICAL_ALWAYS, "vertical always");
        CASE(Fl_Scroll::BOTH_ALWAYS, "both always");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


uchar check_SliderRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_VERT_SLIDER, "vertical");
        CASE(FL_HOR_SLIDER, "horizontal");
        CASE(FL_VERT_FILL_SLIDER, "vertical fill");
        CASE(FL_HOR_FILL_SLIDER, "horizontal fill");
        CASE(FL_VERT_NICE_SLIDER, "vertical nice");
        CASE(FL_HOR_NICE_SLIDER, "horizontal nice");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_SliderRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_VERT_SLIDER, "vertical");
        CASE(FL_HOR_SLIDER, "horizontal");
        CASE(FL_VERT_FILL_SLIDER, "vertical fill");
        CASE(FL_HOR_FILL_SLIDER, "horizontal fill");
        CASE(FL_VERT_NICE_SLIDER, "vertical nice");
        CASE(FL_HOR_NICE_SLIDER, "horizontal nice");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

uchar check_ScrollbarRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_HORIZONTAL, "horizontal");
        CASE(FL_VERTICAL, "vertical");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_ScrollbarRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_HORIZONTAL, "horizontal");
        CASE(FL_VERTICAL, "vertical");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


int check_Native_File_ChooserRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(Fl_Native_File_Chooser::BROWSE_FILE , "file");
        CASE(Fl_Native_File_Chooser::BROWSE_DIRECTORY, "directory");
        CASE(Fl_Native_File_Chooser::BROWSE_MULTI_FILE, "multi file");
        CASE(Fl_Native_File_Chooser::BROWSE_MULTI_DIRECTORY, "multi directory");
        CASE(Fl_Native_File_Chooser::BROWSE_SAVE_FILE, "save file");
        CASE(Fl_Native_File_Chooser::BROWSE_SAVE_DIRECTORY, "save directory");
#undef CASE
    return (int)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Native_File_ChooserRtti(lua_State *L, int val)
    {
    switch(val)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(Fl_Native_File_Chooser::BROWSE_FILE , "file");
        CASE(Fl_Native_File_Chooser::BROWSE_DIRECTORY, "directory");
        CASE(Fl_Native_File_Chooser::BROWSE_MULTI_FILE, "multi file");
        CASE(Fl_Native_File_Chooser::BROWSE_MULTI_DIRECTORY, "multi directory");
        CASE(Fl_Native_File_Chooser::BROWSE_SAVE_FILE, "save file");
        CASE(Fl_Native_File_Chooser::BROWSE_SAVE_DIRECTORY, "save directory");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

uchar check_Browser_Rtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_NORMAL_BROWSER, "normal");
        CASE(FL_SELECT_BROWSER, "select");
        CASE(FL_HOLD_BROWSER, "hold");
        CASE(FL_MULTI_BROWSER, "multi");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Browser_Rtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_NORMAL_BROWSER, "normal");
        CASE(FL_SELECT_BROWSER, "select");
        CASE(FL_HOLD_BROWSER, "hold");
        CASE(FL_MULTI_BROWSER, "multi");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

uchar check_Menu_ButtonRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(Fl_Menu_Button::POPUP1, "popup1");
        CASE(Fl_Menu_Button::POPUP2, "popup2");
        CASE(Fl_Menu_Button::POPUP12, "popup12");
        CASE(Fl_Menu_Button::POPUP3, "popup3");
        CASE(Fl_Menu_Button::POPUP13, "popup13");
        CASE(Fl_Menu_Button::POPUP23, "popup23");
        CASE(Fl_Menu_Button::POPUP123, "popup123");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Menu_ButtonRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(Fl_Menu_Button::POPUP1, "popup1");
        CASE(Fl_Menu_Button::POPUP2, "popup2");
        CASE(Fl_Menu_Button::POPUP12, "popup12");
        CASE(Fl_Menu_Button::POPUP3, "popup3");
        CASE(Fl_Menu_Button::POPUP13, "popup13");
        CASE(Fl_Menu_Button::POPUP23, "popup23");
        CASE(Fl_Menu_Button::POPUP123, "popup123");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


uchar check_SpinnerRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_FLOAT_INPUT, "float");
        CASE(FL_INT_INPUT, "int");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }


uchar check_InputRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_NORMAL_INPUT, "normal");
        CASE(FL_FLOAT_INPUT, "float");
        CASE(FL_INT_INPUT, "int");
        CASE(FL_HIDDEN_INPUT, "hidden");
        CASE(FL_MULTILINE_INPUT, "multiline");
        CASE(FL_SECRET_INPUT, "secret");
//      CASE(FL_INPUT_TYPE, ""); mask
//      CASE(FL_INPUT_READONLY, ""); flag
//      CASE(FL_NORMAL_OUTPUT, "normal output");
//      CASE(FL_MULTILINE_OUTPUT, "multiline output");
//      CASE(FL_INPUT_WRAP, ""); flag
//      CASE(FL_MULTILINE_INPUT_WRAP, "multiline input wrap");
//      CASE(FL_MULTILINE_OUTPUT_WRAP, "multiline output wrap");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_InputRtti(lua_State *L, int val_)
    {
    int val = val_ & FL_INPUT_TYPE;
    switch(val)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_NORMAL_INPUT, "normal");
        CASE(FL_FLOAT_INPUT, "float");
        CASE(FL_INT_INPUT, "int");
        CASE(FL_HIDDEN_INPUT, "hidden");
        CASE(FL_MULTILINE_INPUT, "multiline");
        CASE(FL_SECRET_INPUT, "secret");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

uchar check_CounterRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_NORMAL_COUNTER, "normal");
        CASE(FL_SIMPLE_COUNTER, "simple");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_CounterRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_NORMAL_COUNTER, "normal");
        CASE(FL_SIMPLE_COUNTER, "simple");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

uchar check_DialRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_NORMAL_DIAL, "normal");
        CASE(FL_LINE_DIAL, "line");
        CASE(FL_FILL_DIAL, "fill");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_DialRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_NORMAL_DIAL, "normal");
        CASE(FL_LINE_DIAL, "line");
        CASE(FL_FILL_DIAL, "fill");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

uchar check_RollerRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_HORIZONTAL, "horizontal");
        CASE(FL_VERTICAL, "vertical");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_RollerRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_HORIZONTAL, "horizontal");
        CASE(FL_VERTICAL, "vertical");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

Fl_Table_Row::TableRowSelectMode check_Table_RowRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(Fl_Table_Row::SELECT_NONE, "none");
        CASE(Fl_Table_Row::SELECT_SINGLE, "single");
        CASE(Fl_Table_Row::SELECT_MULTI, "multi");
#undef CASE
    return (Fl_Table_Row::TableRowSelectMode)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Table_RowRtti(lua_State *L, Fl_Table_Row::TableRowSelectMode val)
    {
    switch(val)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(Fl_Table_Row::SELECT_NONE, "none");
        CASE(Fl_Table_Row::SELECT_SINGLE, "single");
        CASE(Fl_Table_Row::SELECT_MULTI, "multi");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


uchar check_ChartRtti(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_BAR_CHART, "bar");
        CASE(FL_HORBAR_CHART, "horbar");
        CASE(FL_LINE_CHART, "line");
        CASE(FL_FILL_CHART, "fill");
        CASE(FL_SPIKE_CHART, "spike");
        CASE(FL_PIE_CHART, "pie");
        CASE(FL_SPECIALPIE_CHART, "specialpie");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_ChartRtti(lua_State *L, uchar type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_BAR_CHART, "bar");
        CASE(FL_HORBAR_CHART, "horbar");
        CASE(FL_LINE_CHART, "line");
        CASE(FL_FILL_CHART, "fill");
        CASE(FL_SPIKE_CHART, "spike");
        CASE(FL_PIE_CHART, "pie");
        CASE(FL_SPECIALPIE_CHART, "specialpie");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

// I hate C++
uchar check_TreeRtti(lua_State *L, int arg)
    { return (uchar)check_Tree_Select(L, arg); }
int push_TreeRtti(lua_State *L, uchar type)
    { return push_Tree_Select(L, (Fl_Tree_Select)type); }

