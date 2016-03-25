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
 | Fl_Input_ (abstract class)                                                   |
 *------------------------------------------------------------------------------*/

GETSET_ENUM(Input_, InputRtti, input_type, Input_input_type)
GETSET_BOOLEAN(Input_, readonly, Input_readonly)
GETSET_BOOLEAN(Input_, wrap, Input_wrap)
VOID_INT4(Input_, resize, Input_resize)
GETSET_ENUM(Input_, Font, textfont, Input_textfont)
GETSET_INTTYPE(Input_, Fontsize, textsize, Input_textsize)
GETSET_INTTYPE(Input_, Color, textcolor, Input_textcolor)
GETSET_INTTYPE(Input_, Color, cursor_color, Input_cursor_color)
GETSET_BOOLEAN(Input_, tab_nav, Input_tab_nav)
GETSET_INT(Input_, shortcut, Input_shortcut)
BOOLEAN_VOID(Input_, undo, Input_undo)
BOOLEAN_VOID(Input_, copy_cuts, Input_copy_cuts)
BOOLEAN_INT(Input_, copy, Input_copy)
GETSET_INT(Input_, maximum_size, Input_maximum_size)
INT_INT(Input_, index, Input_index)

static int Input_insert(lua_State *L)
    {
    size_t len;
    Fl_Input_ *input = check_Input_(L, 1);
    const char* t = luaL_checklstring(L, 2, &len);
    lua_pushboolean(L, input->insert(t, (int)len));
    return 1;
    }

static int Input_cut(lua_State *L)
    {
    Fl_Input_ *input = check_Input_(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushboolean(L, input->cut());
        return 1;
        }
    int a = luaL_checkinteger(L, 2);
    if(lua_isnone(L, 3))
        {
        lua_pushboolean(L, input->cut(a));
        return 1;
        }
    int b = luaL_checkinteger(L, 3);
    lua_pushboolean(L, input->cut(a, b));
    return 1;
    }

static int Input_replace(lua_State *L)
    {
    size_t len;
    Fl_Input_ *input = check_Input_(L, 1);
    int b = luaL_checkinteger(L, 2);
    int e = luaL_checkinteger(L, 3);
    const char* t = luaL_checklstring(L, 4, &len);
    lua_pushboolean(L, input->replace(b, e, t, (int)len));
    return 1;
    }

static int Input_mark(lua_State *L)
    {
    Fl_Input_ *input = check_Input_(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushinteger(L, input->mark());
        return 1;
        }
    int m = luaL_checkinteger(L, 2);
    lua_pushboolean(L, input->mark(m));
    return 1;
    }

static int Input_position(lua_State *L)
    {
    Fl_Input_ *input = check_Input_(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushinteger(L, input->position());
        return 1;
        }
    int p = luaL_checkinteger(L, 2);
    if(lua_isnone(L, 3))
        {
        lua_pushboolean(L, input->position(p));
        return 1;
        }
    int m = luaL_checkinteger(L, 3);
    lua_pushboolean(L, input->position(p, m));
    return 1;
    }


static int Input_size(lua_State *L)
    {
    Fl_Input_ *input = check_Input_(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushinteger(L, input->size());
        return 1;
        }
    /* Fl_Widget::size() */
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    input->size(w,h);
    return 0;
    }


static int Input_value(lua_State *L)
    {
    size_t len;
    const char *str;
    Fl_Input_ *input = check_Input_(L, 1);
    if(lua_isnone(L, 2))
        {
        str = input->value();
        if(!str) return 0;
        lua_pushstring(L, str); /* note that this makes a copy */
        return 1;
        }
    str = luaL_optlstring(L, 2, "", &len);  
    lua_pushboolean(L, input->value(str, (int)len));
    return 1;
    }


static const struct luaL_Reg Input_Methods[] = 
    {
        { "copy", Input_copy },
        { "copy_cuts", Input_copy_cuts },
        { "cursor_color", Input_cursor_color },
        { "cut", Input_cut },
        { "index", Input_index },
        { "input_type", Input_input_type },
        { "insert", Input_insert },
        { "mark", Input_mark },
        { "maximum_size", Input_maximum_size },
        { "position", Input_position },
        { "readonly", Input_readonly },
        { "replace", Input_replace },
        { "resize", Input_resize },
        { "shortcut", Input_shortcut },
        { "size", Input_size },
        { "tab_nav", Input_tab_nav },
        { "textfont", Input_textfont },
        { "textsize", Input_textsize },
        { "textcolor", Input_textcolor },
        { "undo", Input_undo },
        { "value", Input_value },
        { "wrap", Input_wrap },
        { NULL, NULL } /* sentinel */
    };

#define Input_MetaMethods NULL

#if(0)
// NON EXPOSED Fl_Input_ METHODS
int static_value(const char*);
int static_value(const char*, int);
#endif 



/*------------------------------------------------------------------------------*
 | Fl_Input                                                                     |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Input, InputNew)

int flcommon_Input(lua_State *L);
static int Input(lua_State *L)
    { return (lua_type(L, 1) == LUA_TSTRING) ? flcommon_Input(L) : InputNew(L); }

WIDGET_DELETE(Input, InputDelete)

#define InputMethods NULL

static const struct luaL_Reg InputMetaMethods[] = 
    {
        { "__gc",  InputDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | File_Input                                                                   |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(File_Input, File_InputNew)
WIDGET_DELETE(File_Input, File_InputDelete)
GETSET_ENUM(File_Input, Boxtype, down_box, File_Inputdown_box)
GETSET_INTTYPE(File_Input, Color, errorcolor, File_Inputerrorcolor)

static int File_Inputvalue(lua_State *L)
    {
    size_t len;
    const char *str;
    Fl_File_Input *input = check_File_Input(L, 1);
    if(lua_isnone(L, 2))
        {
        str = input->value();
        if(!str) return 0;
        lua_pushstring(L, str); /* note that this makes a copy */
        return 1;
        }
    str = luaL_optlstring(L, 2, "", &len);  
    lua_pushboolean(L, input->value(str, (int)len));
    return 1;
    }

static const struct luaL_Reg File_InputMethods[] = 
    {
        { "down_box", File_Inputdown_box },
        { "errorcolor", File_Inputerrorcolor },
        { "value", File_Inputvalue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg File_InputMetaMethods[] = 
    {
        { "__gc",  File_InputDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Float_Input                                                               |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Float_Input, Float_InputNew)
WIDGET_DELETE(Float_Input, Float_InputDelete)

#define Float_InputMethods NULL

static const struct luaL_Reg Float_InputMetaMethods[] = 
    {
        { "__gc",  Float_InputDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Int_Input                                                                 |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Int_Input, Int_InputNew)
WIDGET_DELETE(Int_Input, Int_InputDelete)

#define Int_InputMethods NULL

static const struct luaL_Reg Int_InputMetaMethods[] = 
    {
        { "__gc",  Int_InputDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Multiline_Input                                                           |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Multiline_Input, Multiline_InputNew)
WIDGET_DELETE(Multiline_Input, Multiline_InputDelete)

#define Multiline_InputMethods NULL

static const struct luaL_Reg Multiline_InputMetaMethods[] = 
    {
        { "__gc",  Multiline_InputDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Secret_Input                                                              |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Secret_Input, Secret_InputNew)
WIDGET_DELETE(Secret_Input, Secret_InputDelete)

#define Secret_InputMethods NULL

static const struct luaL_Reg Secret_InputMetaMethods[] = 
    {
        { "__gc",  Secret_InputDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Output                                                                    |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Output, OutputNew)
WIDGET_DELETE(Output, OutputDelete)

#define OutputMethods NULL

static const struct luaL_Reg OutputMetaMethods[] = 
    {
        { "__gc",  OutputDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Fl_Multiline_Output                                                          |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Multiline_Output, Multiline_OutputNew)
WIDGET_DELETE(Multiline_Output, Multiline_OutputDelete)

#define Multiline_OutputMethods NULL

static const struct luaL_Reg Multiline_OutputMetaMethods[] = 
    {
        { "__gc",  Multiline_OutputDelete },
        { NULL, NULL } /* sentinel */
    };



/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "input",  Input },
        { "file_input",  File_InputNew },
        { "float_input",  Float_InputNew },
        { "int_input",  Int_InputNew },
        { "multiline_input",  Multiline_InputNew },
        { "secret_input",  Secret_InputNew },
        { "output",  OutputNew },
        { "multiline_output",  Multiline_OutputNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Input(lua_State *L)
    {
    udata_define(L, MT_Input_, Input_Methods, Input_MetaMethods);
    udata_inherit(L, MT_Input_, MT_Widget);

    udata_define(L, MT_Input, InputMethods, InputMetaMethods);
    udata_inherit(L, MT_Input, MT_Input_);

    udata_define(L, MT_File_Input, File_InputMethods, File_InputMetaMethods);
    udata_inherit(L, MT_File_Input, MT_Input);

    udata_define(L, MT_Float_Input, Float_InputMethods, Float_InputMetaMethods);
    udata_inherit(L, MT_Float_Input, MT_Input);

    udata_define(L, MT_Int_Input, Int_InputMethods, Int_InputMetaMethods);
    udata_inherit(L, MT_Int_Input, MT_Input);

    udata_define(L, MT_Multiline_Input, Multiline_InputMethods, Multiline_InputMetaMethods);
    udata_inherit(L, MT_Multiline_Input, MT_Input);

    udata_define(L, MT_Secret_Input, Secret_InputMethods, Secret_InputMetaMethods);
    udata_inherit(L, MT_Secret_Input, MT_Input);

    udata_define(L, MT_Output, OutputMethods, OutputMetaMethods);
    udata_inherit(L, MT_Output, MT_Input);

    udata_define(L, MT_Multiline_Output, Multiline_OutputMethods, Multiline_OutputMetaMethods);
    udata_inherit(L, MT_Multiline_Output, MT_Output);

    luaL_setfuncs(L, Functions, 0);
    }


