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

static int FcRef = LUA_NOREF;
static lua_State *FcState = NULL;
static void FcCallback(const char *filename)
    {
#define L FcState 
    if(pushvalue(L, FcRef) != LUA_TFUNCTION)
        { unexpected(L); return; }
    if(filename)
        lua_pushstring(L, filename);
    else
        lua_pushnil(L);
    if(lua_pcall(L, 1, 0, 0) != LUA_OK)
        { lua_error(L); return; }
#undef L
    }

static int File_chooser_callback(lua_State *L)
    {
    if(FcRef != LUA_NOREF)
        {
        fl_file_chooser_callback(NULL);
        unreference(L, FcRef);
        FcState = 0;
        }
    if(lua_isnoneornil(L, 1))
        return 0;
    if(!lua_isfunction(L, 1))
        return luaL_argerror(L, 1, "function expected");
    reference(L, FcRef, 1);
    FcState = L;
    fl_file_chooser_callback(FcCallback);
    return 0;
    }

static int File_chooser(lua_State *L)
    {
    const char *message = luaL_checkstring(L, 1);
    const char *pattern = luaL_optstring(L, 2, NULL);
    const char *fname = luaL_optstring(L, 3, NULL);
    int relative = optboolean(L, 4, 0);
    char *c = fl_file_chooser(message, pattern, fname, relative);
    if(!c)
        return 0;
    lua_pushstring(L, c);
    return 1;
    }

static int FcOkRef = LUA_NOREF;
static int File_chooser_ok_label(lua_State *L)
    {
    /* need to save it on the Lua registry because FLTK does not strdup it */
    fl_file_chooser_ok_label(fl_ok);
    if(FcOkRef != LUA_NOREF)
        unreference(L, FcOkRef);
    if(lua_isnoneornil(L, 1))
        return 0;
    const char *label = luaL_checkstring(L, 1);
    /* save it on the Lua registry */
    reference(L, FcOkRef, 1);
    fl_file_chooser_ok_label(label);
    return 0;
    }
    
/*------------------------------------------------------------------------------*
 | Common dialogs                                                               |
 *------------------------------------------------------------------------------*/

static int Alert(lua_State *L)
    {
    const char* s = luaL_checkstring(L, 1);
    fl_alert("%s", s);
    return 0;
    }

static int Beep(lua_State *L)
    {
    fl_beep(check_Beep(L, 1));
    return 0;
    }


int flcommon_Choice(lua_State *L)
    {
    const char* s = luaL_checkstring(L, 1);
    const char* b0 = luaL_checkstring(L, 2);
    const char* b1 = luaL_optstring(L, 3, NULL);
    const char* b2 = luaL_optstring(L, 4, NULL);
    lua_pushinteger(L, fl_choice("%s", b0, b1, b2, s));
    return 1;
    }

static int Color_Chooser_f(lua_State *L)
/* r, g, b = fl.color_chooser(name, r, g, b, [mode]) */
    {
    const char *name = luaL_checkstring(L, 1);
    double r = luaL_checknumber(L, 2);
    double g = luaL_checknumber(L, 3);
    double b = luaL_checknumber(L, 4);
    int mode = check_Color_ChooserMode(L, 5);
    if(!fl_color_chooser(name, r, g, b, mode))
        return 0;
    lua_pushnumber(L, r);
    lua_pushnumber(L, g);
    lua_pushnumber(L, b);
    return 3;
    }
    
static int Color_Chooser_b(lua_State *L)
/* r, g, b = fl.color_chooser(name, r, g, b, [mode]) */
    {
    const char *name = luaL_checkstring(L, 1);
    uchar r = luaL_checknumber(L, 2);
    uchar g = luaL_checknumber(L, 3);
    uchar b = luaL_checknumber(L, 4);
    int mode = check_Color_ChooserMode(L, 5);
    if(!fl_color_chooser(name, r, g, b, mode))
        return 0;
    lua_pushinteger(L, r);
    lua_pushinteger(L, g);
    lua_pushinteger(L, b);
    return 3;
    }
    
static int Dir_chooser(lua_State *L)
    {
    const char *message = luaL_checkstring(L, 1);
    const char *fname = luaL_checkstring(L, 2);
    int relative = optboolean(L, 3, 0);
    char *c = fl_dir_chooser(message, fname, relative);
    if(!c)
        return 0;
    lua_pushstring(L, c);
    return 1;
    }


int flcommon_Input(lua_State *L)
    {
    const char *message = luaL_checkstring(L, 1);
    const char *defstr = luaL_optstring(L, 2, NULL);
    const char *c = fl_input("%s", defstr, message);
    if(!c)
        return 0;
    lua_pushstring(L, c);
    return 1;
    }

static int Password(lua_State *L)
    {
    const char *message = luaL_checkstring(L, 1);
    const char *defstr = luaL_optstring(L, 2, NULL);
    const char *c = fl_password("%s", defstr, message);
    if(!c)
        return 0;
    lua_pushstring(L, c);
    return 1;
    }


static int Message(lua_State *L)
    {
    const char* s = luaL_checkstring(L, 1);
    fl_message("%s", s);
    return 0;
    }


FUNC_GETSET_BOOLEAN(fl_message_hotspot, Message_hotspot)

static int Message_title(lua_State *L)
    {
    const char* title = luaL_checkstring(L, 1);
    fl_message_title(title);
    return 0;
    }


static int Message_title_default(lua_State *L)
    {
    const char* title = luaL_checkstring(L, 1);
    fl_message_title_default(title);
    return 0;
    }

#if 0
//  Fl_Widget *w = fl_message_icon(); //@@
static int Message_icon(lua_State *L)
    {
    return luaL_error(L, "this function is not available (yet)");
    // The problem here is that the widget (a Fl_Box) in not bound to
    // a userdata, as it was not created by MoonFLTK.
    // Shall we bound it here and track it with a Fl_Widget_Tracker?
    Fl_Widget *w = fl_message_icon();
    if(!w) return 0;
    push_Widget(L, w);
    return 1;
    }
#endif


static const struct luaL_Reg Functions[] = 
    {
        { "alert", Alert },
//      { "ask", Ask }, fl_ask() is deprecated, use fl_choice() instead
        { "beep", Beep },
        { "color_chooser_b", Color_Chooser_b },
        { "color_chooser_f", Color_Chooser_f },
        { "dir_chooser", Dir_chooser },
        { "file_chooser", File_chooser }, //@@ accorpare con Fl_File_Chooser
        { "file_chooser_callback", File_chooser_callback },
        { "file_chooser_ok_label", File_chooser_ok_label },
        { "message", Message },
//      { "message_icon", Message_icon },
        { "message_hotspot", Message_hotspot },
        { "message_title", Message_title },
        { "message_title_default", Message_title_default },
        { "password", Password },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_FlCommon(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

