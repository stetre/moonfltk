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

GROUP_NEW(Text_Editor, Text_EditorNew)
GROUP_DELETE(Text_Editor, Text_EditorDelete)

#define F(func)                                     \
static int Text_Editor##func(lua_State *L)          \
    {                                               \
    int c = lua_isnil(L,1) ? 0 : check_Key(L, 1);   \
    Fl_Text_Editor *e = check_Text_Editor(L, 2);    \
    int rc  = Fl_Text_Editor::func(c, e);           \
    lua_pushboolean(L, rc);                         \
    return 1;                                       \
}

F(kf_default)
F(kf_ignore)
F(kf_backspace)
F(kf_enter)
F(kf_move)
F(kf_shift_move)
F(kf_ctrl_move)
F(kf_c_s_move)
F(kf_meta_move)
F(kf_m_s_move)
F(kf_home)
F(kf_end)
F(kf_left)
F(kf_up)
F(kf_right)
F(kf_down)
F(kf_page_up)
F(kf_page_down)
F(kf_insert)
F(kf_delete)
F(kf_copy)
F(kf_cut)
F(kf_paste)
F(kf_select_all)
F(kf_undo)

#undef F

GETSET_BOOLEAN(Text_Editor, insert_mode, Text_Editorinsert_mode)

/*------------------------------------------------------------------------------*
 | Key bindings                                                                 |
 *------------------------------------------------------------------------------*/

static int KeyCallback(int key, Fl_Text_Editor* p)
    {
    ud_t *ud = userdata(p);
    lua_State *L = main_lua_state;
    if(!ud || !L)
        return 0;
    if(pushvalue(L, ud->key_func) != LUA_TFUNCTION)
        return unexpected(L);
    push_Text_Editor(L, p);
    push_Key(L, key);
    push_Modifiers(L, Fl::event_state());
    if(lua_pcall(L, 3, 1, 0) != LUA_OK)
        return unexpected(L);
    return checkboolean(L, -1); 
    }

static int Text_Editorkey_callback(lua_State *L)
/* key_callback(func)
 * executed as boolean = func(editor, key, modifiers)
 * a single callback per editor, handles all the key/modifiers registered
 * with key_bind() and must return a boolean
 */
    {
    Fl_Text_Editor *p = check_Text_Editor(L, 1);
    ud_t *ud = userdata(p);
    if(!lua_isfunction(L, 2))
        return luaL_argerror(L, 2, "function expected");
    unreference(L, ud->key_func);
    reference(L, ud->key_func, 2);
    return 0;
    }

static int Text_Editorkey_bind(lua_State *L)
/* key_bind(key, modifiers) 
 */
    {
    Fl_Text_Editor *p = check_Text_Editor(L, 1);
    ud_t *ud = userdata(p);
    int key = check_Key(L, 2);
    int state = check_Modifiers(L, 3);
    if(ud->key_func == LUA_NOREF)
        return luaL_error(L, "missing key_callback() call");
    Fl_Text_Editor::Key_Func f = p->bound_key_function(key, state);
    if(f == KeyCallback)
        return 0; /* nothing to do */
    if(f)
        p->remove_key_binding(key, state);
    p->add_key_binding(key, state, KeyCallback);
    return 0;
    }

static int Text_Editorkey_unbind(lua_State *L)
/* key_unbind(key, modifiers) */
    {
    Fl_Text_Editor *p = check_Text_Editor(L, 1);
    int key = check_Key(L, 2);
    int state = check_Modifiers(L, 3);
    Fl_Text_Editor::Key_Func f = p->bound_key_function(key, state);
    if(f)
        p->remove_key_binding(key, state);
    return 0;
    }

static int Text_Editorremove_all_key_bindings(lua_State *L)
    {
    Fl_Text_Editor *p = check_Text_Editor(L, 1);
    p->remove_all_key_bindings();
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Text_EditorMethods[] = 
    {
        { "insert_mode", Text_Editorinsert_mode },
        { "key_callback", Text_Editorkey_callback },
        { "key_bind", Text_Editorkey_bind },
        { "key_unbind", Text_Editorkey_unbind },
        { "remove_all_key_bindings", Text_Editorremove_all_key_bindings },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Text_EditorMetaMethods[] = 
    {
        { "__gc",  Text_EditorDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Text_EditorFunctions[] = 
    {
        { "text_editor",  Text_EditorNew },
#define F_REG(func) { #func , Text_Editor##func },
        F_REG(kf_default)
        F_REG(kf_ignore)
        F_REG(kf_backspace)
        F_REG(kf_enter)
        F_REG(kf_move)
        F_REG(kf_shift_move)
        F_REG(kf_ctrl_move)
        F_REG(kf_c_s_move)
        F_REG(kf_meta_move)
        F_REG(kf_m_s_move)
        F_REG(kf_home)
        F_REG(kf_end)
        F_REG(kf_left)
        F_REG(kf_up)
        F_REG(kf_right)
        F_REG(kf_down)
        F_REG(kf_page_up)
        F_REG(kf_page_down)
        F_REG(kf_insert)
        F_REG(kf_delete)
        F_REG(kf_copy)
        F_REG(kf_cut)
        F_REG(kf_paste)
        F_REG(kf_select_all)
        F_REG(kf_undo)
#undef F_REG
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Text_Editor(lua_State *L)
    {
    udata_define(L, MT_Text_Editor, Text_EditorMethods, Text_EditorMetaMethods);
    udata_inherit(L, MT_Text_Editor, MT_Text_Display);
    luaL_setfuncs(L, Text_EditorFunctions, 0);
    }

