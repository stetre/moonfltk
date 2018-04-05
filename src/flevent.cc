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

/*----------------------------------------------------------------------*
 | Handlers                                                             |
 *----------------------------------------------------------------------*/

/* Event handler -------------------------------------------------------*/

static int EvRef = LUA_NOREF;
static lua_State *EvState = NULL;
static int EventHandler(int event)
    {
#define L EvState
    if(pushvalue(L, EvRef) != LUA_TFUNCTION)
        return unexpected(L);
    push_Event(L, event);
    if(lua_pcall(L, 1, 1, 0) != LUA_OK)
        return unexpected(L);
    return checkboolean(L, -1);
#undef L
    }

static int Handler(lua_State *L) /* only 1 handler (multiplex it in Lua) */
    {
    if(EvRef != LUA_NOREF)
        {
        Fl::remove_handler(EventHandler);
        unreference(L, EvRef);
        EvState = NULL;
        }
    if(lua_isnoneornil(L, 1))
        return 0;
    if(!lua_isfunction(L, 1))
        return luaL_argerror(L, 1, "function expected");
    reference(L, EvRef, 1);
    EvState = L;
    Fl::add_handler(EventHandler);
    return 0;
    }


#if 0
/* System event handler ------------------------------------------------*/

static int SysRef = LUA_NOREF;
static lua_State *SysState = NULL;
static int SystemHandler(void *event, void *data)
    {
#define L SysState
    (void)data;
    if(pushvalue(L, SysRef) != LUA_TFUNCTION)
        return unexpected(L);
//  push_Event(L, event); @@ how to pass an XEvent ?!
    if(lua_pcall(L, 1, 1, 0) != LUA_OK)
        return unexpected(L);
    return checkboolean(L, -1);
#undef L
    }

static int System_handler(lua_State *L) /* only 1 handler (multiplex it in Lua) */
    {
    if(SysRef != LUA_NOREF)
        {
        Fl::remove_system_handler(SystemHandler);
        unreference(L, SysRef);
        SysState = NULL;
        }
    if(lua_isnoneornil(L, 1))
        return 0;
    if(!lua_isfunction(L, 1))
        return luaL_argerror(L, 1, "function expected");
    reference(L, SysRef, 1);
    SysState = L;
    Fl::add_system_handler(SystemHandler, NULL);
    return 0;
    }
#endif

/* Event dispatch ------------------------------------------------------*/


//typedef int (*Fl_Event_Dispatch)(int event, Fl_Window *w);

//static void   Fl::event_dispatch(Fl_Event_Dispatch d)
//static Fl_Event_Dispatch  Fl::event_dispatch()
static int Event_dispatch(lua_State *L) //@@ 
    {
    (void)L;
    return 0;
    }



/*----------------------------------------------------------------------*
 | Functions                                                            |
 *----------------------------------------------------------------------*/

// FUNC_GETSET_POINTER(Widget, Fl::focus, Focus)
static int Focus(lua_State *L)
    {
    if(lua_isnone(L, 1))
        {
        Fl_Widget *v = Fl::focus();
        if(!v) return 0;
        // Focus widget could be one belonging to FTLK unknown to 
        // lua (e.g. scaling factor display for FLTK 1.4), therefore
        // push_...IfValid pushes nil for unknown widgets.
        return push_WidgetIfValid(L, v); 
        }
    Fl_Widget *v = check_Widget(L, 1);
    Fl::focus(v);
    return 0;
    }
    
FUNC_GETSET_POINTER(Widget, Fl::belowmouse, Belowmouse)

#if 0
static int Compose(lua_State *L)
    {
    int del;
    lua_pushboolean(L, Fl::compose(del));
    lua_pushinteger(L, del);
    return 2;
    }

FUNC_VOID_VOID(Fl::compose_reset, Compose_reset)
#endif

FUNC_VOID_VOID(Fl::disable_im, Disable_im)
FUNC_VOID_VOID(Fl::enable_im, Enable_im)

static int Event(lua_State *L)
    {
    int e = Fl::event();
    if(!e) return 0;
    push_Event(L, e);
    return 1;
    }

FUNC_BOOLEAN_VOID(Fl::event_alt, Event_alt)

static int Event_button(lua_State *L)
    {
    int b = Fl::event_button();
    if(!b) return 0;
    push_Mouse_Button(L, b);
    return 1;
    }

FUNC_BOOLEAN_VOID(Fl::event_button1, Event_button1)
FUNC_BOOLEAN_VOID(Fl::event_button2, Event_button2)
FUNC_BOOLEAN_VOID(Fl::event_button3, Event_button3)

static int Event_buttons(lua_State *L) //@@ tutti e 7 o solo i primi 3?
    {
    int val = Fl::event_buttons();
    for(int n = 1; n < 4; n++)
    lua_pushboolean(L, val & FL_BUTTON(n));
    return 3;
    }

FUNC_GETSET_INT(Fl::event_clicks, Event_clicks)

static int Event_clipboard(lua_State *L)
    {
    const char *t = Fl::event_clipboard_type();
    if(!t)
        return 0;
    if(strcmp(t,Fl::clipboard_image) != 0)
        return 0;
    Fl_Image *p = (Fl_Image*)Fl::event_clipboard();
    if(!p)
        return 0;
    DBG_CREATE(Image, p);
    newuserdata(L, p, MT_Image);
    return 1;
    }

static int Event_clipboard_type(lua_State *L)
    {
    const char *t = Fl::event_clipboard_type();
    DBG("clipboard type = '%s'\n", t ? t : "none");
    if(!t) return 0;
    lua_pushstring(L, t);
    return 1;
    }

FUNC_BOOLEAN_VOID(Fl::event_command, Event_command)
FUNC_BOOLEAN_VOID(Fl::event_ctrl, Event_ctrl)

FUNC_INT_VOID(Fl::event_dx, Event_dx)
FUNC_INT_VOID(Fl::event_dy, Event_dy)

FUNC_GETSET_POINTER(Widget, Fl::pushed, Pushed) 
#if 0
//@@ uhm.... what if the pushed widget was not created by MoonFLTK?
//static Fl_Widget * Fl::pushed()
//static void Fl::pushed(Fl_Widget *)
#endif

static int Event_text(lua_State *L)
    {
    const char *t = Fl::event_text();
    if(!t)
        return 0;
    lua_pushstring(L, t);
    return 1;
    }

static int Event_key(lua_State *L)
    {
    if(lua_isnone(L, 1))
        {
        int key =  Fl::event_key();
        if(key==0)
            return 0;
        push_Key(L, key);
        return 1;
        }
    int key = check_Key(L, 1);
    lua_pushboolean(L, Fl::event_key(key));
    return 1;
    }

static int Event_inside(lua_State *L)
    {
    int rc;
    if(lua_type(L, 1) == LUA_TNUMBER)
        {
        int x = luaL_checkinteger(L, 1);
        int y = luaL_checkinteger(L, 2);
        int w = luaL_checkinteger(L, 3);
        int h = luaL_checkinteger(L, 4);
        rc = Fl::event_inside(x, y, w, h);
        }
    else
        rc = Fl::event_inside(check_Widget(L, 1));
    lua_pushboolean(L, rc);
    return 1;
    }

FUNC_GETSET_BOOLEAN(Fl::event_is_click, Event_is_click)

FUNC_INT_VOID(Fl::event_length, Event_length)

FUNC_BOOLEAN_VOID(Fl::event_shift, Event_shift)

static int Event_state(lua_State *L)
    {
    if(lua_isnone(L, 1))
        return push_Modifiers(L, Fl::event_state());
    lua_pushboolean(L, Fl::event_state(check_Modifiers(L, 1)));
    return 1;
    }

FUNC_INT_VOID(Fl::event_x, Event_x)
FUNC_INT_VOID(Fl::event_x_root, Event_x_root)
FUNC_INT_VOID(Fl::event_y, Event_y)
FUNC_INT_VOID(Fl::event_y_root, Event_y_root)

#if 0
//static int Fl::event_original_key() //@@
static int Event_original_key(lua_State *L) 
    {
    (void)L;
    return 0;
    }
#endif

static int Get_key(lua_State *L)
    {
    int key = check_Key(L, 1);
    lua_pushboolean(L, Fl::get_key(key));
    return 1;
    }

static int Get_mouse(lua_State *L)
    {
    int x, y;
    Fl::get_mouse(x, y);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    return 2;
    }

#if 0
//static int    Fl::handle(int, Fl_Window *)
static int Handle(lua_State *L) //@@ 
    {
    (void)L;
    return 0;
    }

//static int    Fl::handle_(int, Fl_Window *)
static int Handle_(lua_State *L) //@@ 
    {
    (void)L;
    return 0;
    }
#endif

static int Test_shortcut(lua_State *L)
    {
    int sc = check_Shortcut(L, 1);
    lua_pushboolean(L, Fl::test_shortcut(sc));
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/


static const struct luaL_Reg Functions[] = 
    {
        { "belowmouse", Belowmouse },
//      { "compose", Compose },
//      { "compose_reset", Compose_reset },
        { "disable_im", Disable_im },
        { "enable_im", Enable_im },
        { "event", Event },
        { "event_alt", Event_alt },
        { "event_button", Event_button },
        { "event_button1", Event_button1 },
        { "event_button2", Event_button2 },
        { "event_button3", Event_button3 },
        { "event_buttons", Event_buttons },
        { "event_clicks", Event_clicks },
        { "event_clipboard", Event_clipboard },
        { "event_clipboard_type", Event_clipboard_type },
        { "event_command", Event_command },
        { "event_ctrl", Event_ctrl },
        { "event_dx", Event_dx },
        { "event_dy", Event_dy },
        { "event_inside", Event_inside },
        { "event_is_click", Event_is_click },
        { "event_key", Event_key },
        { "event_length", Event_length },
//      { "event_original_key", Event_original_key },
        { "event_shift", Event_shift },
        { "event_state", Event_state },
        { "event_text", Event_text },
        { "event_x", Event_x },
        { "event_x_root", Event_x_root },
        { "event_y", Event_y },
        { "event_y_root", Event_y_root },
        { "focus", Focus },
        { "get_key", Get_key },
        { "get_mouse", Get_mouse },
//      { "handle", Handle },
//      { "handle_", Handle_ },
        { "pushed", Pushed },
        { "test_shortcut", Test_shortcut },
/* handlers */
        { "handler", Handler },
//      { "system_handler", System_handler },
        { "event_dispatch", Event_dispatch },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_FlEvent(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

