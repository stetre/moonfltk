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

WINDOW_NEW(Window, WindowNew)
WINDOW_DELETE(Window, WindowDelete)

#if 0
static int ToString(lua_State *L)
    { 
    lua_pushfstring(L, "this is a window");
    return 1;
    }
#endif


GETSET_BOOLEAN(Window, border, Windowborder)
VOID_VOID(Window, clear_border, Windowclear_border)
VOID_VOID(Window, set_override, Windowset_override)
BOOLEAN_VOID(Window, override, Windowoverride)

VOID_VOID(Window, set_modal, Windowset_modal)
BOOLEAN_VOID(Window, modal, Windowmodal)
VOID_VOID(Window, set_non_modal, Windowset_non_modal)
BOOLEAN_VOID(Window, non_modal, Windownon_modal)
VOID_VOID(Window, clear_modal_states, Windowclear_modal_states)

VOID_VOID(Window, set_menu_window, Windowset_menu_window)
BOOLEAN_VOID(Window, menu_window, Windowmenu_window)
VOID_VOID(Window, set_tooltip_window, Windowset_tooltip_window)
BOOLEAN_VOID(Window, tooltip_window, Windowtooltip_window)
VOID_VOID(Window, wait_for_expose, Windowwait_for_expose)

WINDOW_SHOW(Window, Windowshow)

BOOLEAN_VOID(Window, shown , Windowshown )

static int Windowhotspot(lua_State *L)
/* hotspot(x, y, offscreen)
 * hotspot(widget, offscreen)
 */
    {
    int x, y, offscreen;
    int arg = 1;
    Fl_Widget *widget = NULL;
    Fl_Window *window = check_Window(L, arg++);
    if(!lua_isinteger(L, arg))
        {
        widget = check_Widget(L, arg++);
        x = y = 0; /* just to avoid warnings ... */
        }
    else
        {
        x = luaL_checkinteger(L, arg++);
        y = luaL_checkinteger(L, arg++);
        }
    offscreen = luaL_optinteger(L, arg++, 0);
    if(widget)
        window->hotspot(widget, offscreen);
    else
        window->hotspot(x, y, offscreen);
    return 0;
    }

static int Windowsize_range(lua_State *L)
    {
    int arg = 1;
    Fl_Window *window = check_Window(L, arg++);
    int minw = luaL_checkinteger(L, arg++);
    int minh = luaL_checkinteger(L, arg++); 
    int maxw = luaL_optinteger(L, arg++, 0);
    int maxh = luaL_optinteger(L, arg++, 0);
    int dw = luaL_optinteger(L, arg++, 0);
    int dh = luaL_optinteger(L, arg++, 0);
    int aspect = optboolean(L, arg++, 0);
    window->size_range(minw, minh, maxw, maxh, dw, dh, aspect);
    return 0;
    }

INT_VOID(Window, decorated_w, Windowdecorated_w)
INT_VOID(Window, decorated_h, Windowdecorated_h)

VOID_VOID(Window, fullscreen, Windowfullscreen)
BOOLEAN_VOID(Window, fullscreen_active, Windowfullscreen_active)

static int Windowfullscreen_off(lua_State *L)
    {
    int arg = 1;
    Fl_Window *window = check_Window(L, arg++);
    if(lua_isnone(L, arg))
        {
        window->fullscreen_off();
        return 0;
        }
    int x = luaL_checkinteger(L, arg++);
    int y = luaL_checkinteger(L, arg++);
    int w = luaL_checkinteger(L, arg++);
    int h = luaL_checkinteger(L, arg++);
    window->fullscreen_off(x, y, w, h);
    return 0;
    }

static int Windowfullscreen_screens(lua_State *L)
    {
    int arg = 1;
    Fl_Window *window = check_Window(L, arg++);
    int top = luaL_checkinteger(L, arg++);
    int bottom = luaL_checkinteger(L, arg++);
    int left = luaL_checkinteger(L, arg++);
    int right = luaL_checkinteger(L, arg++);
    window->fullscreen_screens(top, bottom, left, right);
    return 0;
    }


VOID_VOID(Window, iconize, Windowiconize )

static int Windowiconlabel(lua_State *L)
    {
    Fl_Window *window = check_Window(L, 1);
    if(lua_isnone(L, 2))
        {
        const char *label = window->iconlabel();
        if(!label)
            return 0;
        lua_pushstring(L, label);
        return 1;
        }
    const char *label = luaL_checkstring(L, 2);
    window->iconlabel(label); //@@ is it copied or not?
    return 0;
    }

static int Windowcursor(lua_State *L)
    {
    int arg = 1;
    Fl_Window *window = check_Window(L, arg++);
    if(lua_type(L, arg)==LUA_TSTRING)
        {
        Fl_Cursor cursor = check_Cursor(L, arg++);
        window->cursor(cursor);
        return 0;
        }
    Fl_RGB_Image *image = check_RGB_Image(L, arg++);
    int hotx = luaL_checkinteger(L, arg++);
    int hoty = luaL_checkinteger(L, arg++);
    window->cursor(image, hotx, hoty);
    return 0;
    }

static int Windowdefault_cursor(lua_State *L)
    {
    Fl_Window *window = check_Window(L, 1);
    Fl_Cursor cursor = check_Cursor(L, 2);
    window->default_cursor(cursor);
    return 0;
    }

static int Windowxclass(lua_State *L)
    {
    Fl_Window *window = check_Window(L, 1);
    const char *c;
    if(lua_isnone(L, 2))
        {
        c = window->xclass();
        if(!c) return 0;
        lua_pushstring(L, c);
        return 1;
        }
    c = luaL_checkstring(L, 2);
    window->xclass(c);
    return 0;
    }

static int Windowshape(lua_State *L)
    {
    Fl_Window *window = check_Window(L, 1);
    const Fl_Image *image = check_Image(L, 2);
    window->shape(image);
    return 0;
    }

static int Default_xclass(lua_State *L)
    {
    const char *c;
    if(lua_isnone(L, 1))
        {
        c = Fl_Window::default_xclass();
        if(!c) return 0;
        lua_pushstring(L, c);
        return 1;
        }
    c = luaL_checkstring(L, 1);
    Fl_Window::default_xclass(c);
    return 0;
    }

static int Windowicon(lua_State *L)
    {
    Fl_Window *window = check_Window(L, 1);
    const Fl_RGB_Image *image = check_RGB_Image(L, 2);
    window->icon(image);
    return 0;
    }


WIDGET_LABEL(Window, Windowlabel)

VOID_VOID(Window, make_current, Windowmake_current)

INT_VOID(Window, x_root, Windowx_root)
INT_VOID(Window, y_root, Windowy_root)

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg WindowMethods[] = 
    {
        { "border", Windowborder },
        { "clear_border", Windowclear_border },
        { "clear_modal_states", Windowclear_modal_states },
        { "cursor", Windowcursor },
        { "decorated_h", Windowdecorated_h },
        { "decorated_w", Windowdecorated_w },
        { "default_cursor", Windowdefault_cursor },
        { "fullscreen", Windowfullscreen  },
        { "fullscreen_active", Windowfullscreen_active },
        { "fullscreen_off", Windowfullscreen_off },
        { "fullscreen_screens", Windowfullscreen_screens },
        { "hotspot", Windowhotspot },
        { "icon", Windowicon },
        { "iconize", Windowiconize  },
        { "iconlabel", Windowiconlabel },
        { "label", Windowlabel },
        { "make_current", Windowmake_current },
        { "menu_window", Windowmenu_window },
        { "modal", Windowmodal },
        { "non_modal", Windownon_modal },
        { "override", Windowoverride },
        { "set_menu_window", Windowset_menu_window  },
        { "set_modal", Windowset_modal },
        { "set_non_modal", Windowset_non_modal },
        { "set_override", Windowset_override },
        { "set_tooltip_window", Windowset_tooltip_window },
        { "shape", Windowshape },
        { "show", Windowshow },
        { "shown", Windowshown },
        { "size_range", Windowsize_range },
        { "tooltip_window", Windowtooltip_window },
        { "wait_for_expose", Windowwait_for_expose },
        { "x_root", Windowx_root },
        { "y_root", Windowy_root },
        { "xclass", Windowxclass },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg WindowMetaMethods[] = 
    {
        { "__gc",  WindowDelete },
//      { "__tostring",  ToString },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Double_Window                                                                |
 *------------------------------------------------------------------------------*/

WINDOW_NEW(Double_Window, Double_WindowNew)
WINDOW_DELETE(Double_Window, Double_WindowDelete)

VOID_VOID(Double_Window, flush, Double_Windowflush)
WINDOW_SHOW(Double_Window, Double_Windowshow)

static const struct luaL_Reg Double_WindowMethods[] = 
    {
        { "flush", Double_Windowflush },
        { "show", Double_Windowshow },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Double_WindowMetaMethods[] = 
    {
        { "__gc",  Double_WindowDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Overlay_Window (abstract class)                                           |
 *------------------------------------------------------------------------------*/

WINDOW_SHOW(Overlay_Window, Overlay_Windowshow)
VOID_VOID(Overlay_Window, flush, Overlay_Windowflush)
VOID_VOID(Overlay_Window, hide, Overlay_Windowhide)
VOID_INT4(Overlay_Window, resize, Overlay_Windowresize)
VOID_VOID(Overlay_Window, redraw_overlay, Overlay_Windowredraw_overlay)
BOOLEAN_VOID(Overlay_Window, can_do_overlay, Overlay_Windowcan_do_overlay)

static const struct luaL_Reg Overlay_WindowMethods[] = 
    {
        { "can_do_overlay", Overlay_Windowcan_do_overlay },
        { "flush", Overlay_Windowflush },
        { "hide", Overlay_Windowhide },
        { "redraw_overlay", Overlay_Windowredraw_overlay },
        { "resize", Overlay_Windowresize },
        { "show", Overlay_Windowshow },
        { NULL, NULL } /* sentinel */
    };

#define Overlay_WindowMetaMethods NULL

/*------------------------------------------------------------------------------*
 | Single_Window                                                                |
 *------------------------------------------------------------------------------*/

WINDOW_NEW(Single_Window, Single_WindowNew)
WINDOW_DELETE(Single_Window, Single_WindowDelete)

#define Single_WindowMethods NULL

static const struct luaL_Reg Single_WindowMetaMethods[] = 
    {
        { "__gc",  Single_WindowDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Menu_Window                                                                  |
 *------------------------------------------------------------------------------*/

WINDOW_NEW(Menu_Window, Menu_WindowNew)
WINDOW_DELETE(Menu_Window, Menu_WindowDelete)
VOID_VOID(Menu_Window, flush, Menu_WindowFlush)
VOID_VOID(Menu_Window, erase, Menu_WindowErase)
VOID_VOID(Menu_Window, set_overlay, Menu_WindowSet_overlay)
VOID_VOID(Menu_Window, clear_overlay, Menu_WindowClear_overlay)
BOOLEAN_VOID(Menu_Window, overlay, Menu_WindowOverlay)

static const struct luaL_Reg Menu_WindowMethods[] = 
    {
        { "clear_overlay", Menu_WindowClear_overlay },
        { "erase", Menu_WindowErase },
        { "flush", Menu_WindowFlush },
        { "overlay", Menu_WindowOverlay },
        { "set_overlay", Menu_WindowSet_overlay },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Menu_WindowMetaMethods[] = 
    {
        { "__gc",  Menu_WindowDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Gl_Window                                                                    |
 *------------------------------------------------------------------------------*/

#if USE_GL

#if 0 /* this class must be subclassed in order to use it */
WINDOW_NEW(Gl_Window, Gl_WindowNew)
WINDOW_DELETE(Gl_Window, Gl_WindowDelete)
#endif
/*@@ void* context() const {return context_;}
 *   void context(void*, int destroy_flag = 0);
 */

GETSET_BOOLEAN(Gl_Window, valid, Gl_Windowvalid)
GETSET_BOOLEAN(Gl_Window, context_valid, Gl_Windowcontext_valid) //@@ la set ha senso?
VOID_VOID(Gl_Window, invalidate, Gl_Windowinvalidate)

WINDOW_SHOW(Gl_Window, Gl_Windowshow)

VOID_VOID(Gl_Window, flush, Gl_Windowflush)
VOID_VOID(Gl_Window, hide, Gl_Windowhide)
VOID_INT4(Gl_Window, resize, Gl_Windowresize)
VOID_VOID(Gl_Window, make_current, Gl_Windowmake_current)
VOID_VOID(Gl_Window, swap_buffers, Gl_Windowswap_buffers)
//VOID_VOID(Gl_Window, ortho, Gl_Windowortho)
BOOLEAN_VOID(Gl_Window, can_do_overlay, Gl_Windowcan_do_overlay)
VOID_VOID(Gl_Window, redraw_overlay, Gl_Windowredraw_overlay)
VOID_VOID(Gl_Window, hide_overlay, Gl_Windowhide_overlay)
VOID_VOID(Gl_Window, make_overlay_current, Gl_Windowmake_overlay_current)

static int Gl_Windowcan_do(lua_State *L)
    {
    Fl_Gl_Window *p = check_Gl_Window(L, 1);
    if(lua_isnone(L, 2))
        lua_pushboolean(L, p->can_do());
    else
        lua_pushboolean(L, p->can_do(check_Mode(L, 2)));
    return 1;
    }

static int Gl_Windowmode(lua_State *L)
    {
    Fl_Gl_Window *p = check_Gl_Window(L, 1);
    if(lua_isnone(L, 2))
        return push_Mode(L, p->mode());
    lua_pushboolean(L, p->mode(check_Mode(L, 2)));
    return 1;
    }

static const struct luaL_Reg Gl_WindowMethods[] = 
    {
        { "can_do", Gl_Windowcan_do },
        { "can_do_overlay", Gl_Windowcan_do_overlay },
        { "context_valid", Gl_Windowcontext_valid },
        { "hide", Gl_Windowhide },
        { "hide_overlay", Gl_Windowhide_overlay },
        { "invalidate", Gl_Windowinvalidate },
        { "flush", Gl_Windowflush },
        { "make_current", Gl_Windowmake_current },
        { "make_overlay_current", Gl_Windowmake_overlay_current },
        { "mode", Gl_Windowmode },
//      { "ortho", Gl_Windowortho }, NO: only modern OpenGL
        { "redraw_overlay", Gl_Windowredraw_overlay },
        { "resize", Gl_Windowresize },
        { "show", Gl_Windowshow },
        { "swap_buffers", Gl_Windowswap_buffers },
        { "valid", Gl_Windowvalid },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Gl_WindowMetaMethods[] = 
    {
//      { "__gc",  Gl_WindowDelete },
        { NULL, NULL } /* sentinel */
    };

#endif /* USE_GL */


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/


static const struct luaL_Reg Functions[] = 
    {
        { "window",  WindowNew },
        { "default_xclass", Default_xclass }, /*@@DOC */
        { "double_window",  Double_WindowNew },
//      { "overlay_window",   }, abstract class: use overlay_window_subclass()
        { "single_window",  Single_WindowNew },
        { "menu_window",  Menu_WindowNew },
#if USE_GL
//      { "gl_window",  Gl_WindowNew },
#endif
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Window(lua_State *L)
    {
    udata_define(L, MT_Window, WindowMethods, WindowMetaMethods);
    udata_inherit(L, MT_Window, MT_Group);

    udata_define(L, MT_Double_Window, Double_WindowMethods, Double_WindowMetaMethods);
    udata_inherit(L, MT_Double_Window, MT_Window);

    udata_define(L, MT_Overlay_Window, Overlay_WindowMethods, Overlay_WindowMetaMethods);
    udata_inherit(L, MT_Overlay_Window, MT_Double_Window);

    udata_define(L, MT_Single_Window, Single_WindowMethods, Single_WindowMetaMethods);
    udata_inherit(L, MT_Single_Window, MT_Window);

    udata_define(L, MT_Menu_Window, Menu_WindowMethods, Menu_WindowMetaMethods);
    udata_inherit(L, MT_Menu_Window, MT_Single_Window);

#if USE_GL
    udata_define(L, MT_Gl_Window, Gl_WindowMethods, Gl_WindowMetaMethods);
    udata_inherit(L, MT_Gl_Window, MT_Window);
#endif

    luaL_setfuncs(L, Functions, 0);
    }


#if(0)
// NOT EXPOSED Fl_Window METHODS @@
// -------------------------------------------------------------------------
static void default_icon(const Fl_RGB_Image*);
static void default_icons(const Fl_RGB_Image*[], int);
void icons(const Fl_RGB_Image*[], int);
static void default_callback(Fl_Window*, void* v);
#endif
