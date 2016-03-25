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

static int Input_ChoiceNew(lua_State *L)
    {
    int arg = 1;
    int x, y, w, h;
    const char* label;
    x = luaL_checkinteger(L, arg++);
    y = luaL_checkinteger(L, arg++);
    w = luaL_checkinteger(L, arg++);
    h = luaL_checkinteger(L, arg++);
    label = luaL_optstring(L, arg, NULL);
    Fl_Input_Choice *p = new Fl_Input_Choice(x, y, w, h, NULL);
    if(label) p->copy_label(label);
    DBG_CREATE(Input_Choice, p);
    newuserdata(L, p, MT_Input_Choice);
    /* bind encapsulated objects to Lua userdata: */    
    Fl_Menu_Button *button = p->menubutton();
    newuserdata(L, button, MT_Menu_Button);
    lua_pop(L, 1);
    Fl_Input *input = p->input();
    newuserdata(L, input, MT_Input);
    lua_pop(L, 1);
    return 1;
    }

WIDGET_DELETE(Input_Choice, Input_ChoiceDelete) //@@ or GROUP_ ??

static int Input_Choicemenubutton(lua_State *L)
    {
    Fl_Input_Choice *p = check_Input_Choice(L, 1);
    push_Menu_Button(L, p->menubutton());
    return 1;
    }

static int Input_Choiceinput(lua_State *L)
    {
    Fl_Input_Choice *p = check_Input_Choice(L, 1);
    push_Input(L, p->input());
    return 1;
    }

#if 0
static int Input_Choicemenu(lua_State *L) //@@
  //const Fl_Menu_Item *menu() { return (menu_->menu()); }
  //void menu(const Fl_Menu_Item *m) { menu_->menu(m); }
    {
    Fl_Input_Choice *p = check_Input_Choice(L, 1);
    push_Input(L, p->input());
    return 1;
    }
#endif


GETSET_ENUM(Input_Choice, Boxtype, down_box, Input_Choicedown_box)
GETSET_INTTYPE(Input_Choice, Color, textcolor, Input_Choicetextcolor)
GETSET_INTTYPE(Input_Choice, Font, textfont, Input_Choicetextfont)
GETSET_INTTYPE(Input_Choice, Fontsize, textsize, Input_Choicetextsize)
VOID_INT4(Input_Choice, resize, Input_Choiceresize)

BOOLEAN_VOID(Input_Choice, changed, Input_Choicechanged)
VOID_VOID(Input_Choice, clear_changed, Input_Choiceclear_changed)
VOID_VOID(Input_Choice, set_changed, Input_Choiceset_changed)

static int Input_Choicevalue(lua_State *L)
    {
    Fl_Input_Choice *p = check_Input_Choice(L, 1);
    if(lua_isnone(L, 2))
        {
        const char *val = p->value();
        if(!val) return 0;
        lua_pushstring(L, val);
        return 1;
        }
    int type = lua_type(L, 2);
    if(type == LUA_TNUMBER)
        p->value(luaL_checkinteger(L, 2));
    else
        p->value(luaL_checkstring(L, 2));
    return 0;
    }

static int Input_Choiceadd(lua_State *L)
    {
    Fl_Input_Choice *p = check_Input_Choice(L, 1);
    p->add(luaL_checkstring(L, 2));
    return 0;
    }

static int Input_Choiceclear(lua_State *L)
    {
    Fl_Input_Choice *p = check_Input_Choice(L, 1);
    Fl_Menu_ *menu= p->menubutton();
    menuclear(L, menu);
    p->clear();
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Input_ChoiceMethods[] = 
    {
        { "add", Input_Choiceadd },
        { "changed", Input_Choicechanged },
        { "clear", Input_Choiceclear },
        { "clear_changed", Input_Choiceclear_changed },
        { "down_box", Input_Choicedown_box },
        { "input", Input_Choiceinput },
        { "menubutton", Input_Choicemenubutton },
        { "resize", Input_Choiceresize },
        { "set_changed", Input_Choiceset_changed },
        { "textcolor", Input_Choicetextcolor },
        { "textfont", Input_Choicetextfont },
        { "textsize", Input_Choicetextsize },
        { "value", Input_Choicevalue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Input_ChoiceMetaMethods[] = 
    {
        { "__gc",  Input_ChoiceDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Input_ChoiceFunctions[] = 
    {
        { "input_choice",  Input_ChoiceNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Input_Choice(lua_State *L)
    {
    udata_define(L, MT_Input_Choice, Input_ChoiceMethods, Input_ChoiceMetaMethods);
    udata_inherit(L, MT_Input_Choice, MT_Group);
    luaL_setfuncs(L, Input_ChoiceFunctions, 0);
    }

