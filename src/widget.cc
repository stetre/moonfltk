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
 | Fl_Widget (abstract class)                                                   |
 *------------------------------------------------------------------------------*/

static void CommonCallback(Fl_Widget *p, void *not_used)
    {
    lua_State *L = main_lua_state;
    if (!L) 
        return;
    (void)not_used;
    int nargs = 1;
    ud_t *ud = userdata(p);
    if( p->callback() != CommonCallback )
        return;
    /* retrieve Lua callback and execute it */
    if(pushvalue(L, ud->cbref) != LUA_TFUNCTION)
        { unexpected(L); return; }
    push_Widget(L, p);
    if(ud->argref != LUA_NOREF)
        { pushvalue(L, ud->argref); nargs++; }
    if(lua_pcall(L, nargs, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }

#define SetUserData(L, ud, arg) do {                        \
    unreference((L), (ud)->argref);                         \
    if(!lua_isnoneornil((L), (arg)))                        \
        reference((L), (ud)->argref, (arg));                \
} while(0)

static int Widgetcallback(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    ud_t *ud = userdata(p);
    if(lua_isnone(L, 2)) /* push callback */
        {
        if(ud->cbref == LUA_NOREF)
            return 0;
        if(pushvalue(L, ud->cbref) != LUA_TFUNCTION)
            return unexpected(L);
        return 1;
        }
    unreference(L, ud->cbref); /* old callback */
    if(!lua_isfunction(L, 2))
        return luaL_argerror(L, 2, "function expected");
    reference(L, ud->cbref, 2); 
    SetUserData(L, ud, 3);
    p->callback(CommonCallback, (void*)NULL);
    return 0;
    }

#if 0
static int Widgetdefault_callback(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    ud_t *ud = userdata(p);
    SetUserData(L, ud, 2);
    p->default_callback(p, NULL);
    return 0;
    }
#endif

static int Widgetdo_callback(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    ud_t *ud = userdata(p);
    if(!lua_isnone(L, 2))
        SetUserData(L, ud, 2);
    p->do_callback(p);
    return 0;
    }

static int Widgetargument(lua_State *L) 
    {
    Fl_Widget *p = check_Widget(L, 1);
    ud_t *ud = userdata(p);
    if(lua_isnone(L, 2))
        {
        pushvalue(L, ud->argref);
        return 1;
        }
    SetUserData(L, ud, 2);
    return 0;
    }

WIDGET_TYPE(Widget, Widgettype)

INT_VOID(Widget, x, Widgetx)
INT_VOID(Widget, y, Widgety)
INT_VOID(Widget, w, Widgetw)
INT_VOID(Widget, h, Widgeth)

static int Widgetxywh(lua_State *L) /* NONFLTK */
    {
    Fl_Widget *p = check_Widget(L, 1);
    lua_pushinteger(L, p->x());
    lua_pushinteger(L, p->y());
    lua_pushinteger(L, p->w());
    lua_pushinteger(L, p->h());
    return 4;
    }

VOID_INT_INT(Widget, position, Widgetposition)
VOID_INT_INT(Widget, size, Widgetsize)
VOID_INT4(Widget, resize, Widgetresize)

WIDGET_LABEL(Widget, Widgetlabel)

static int MeasureLabel(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    int w, h;
    p->measure_label(w, h);
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 2;
    }

static int Widgettooltip(lua_State *L)
    {
    const char *text;
    Fl_Widget *p = check_Widget(L, 1);
    if(lua_isnone(L, 2)) /* get */  
        {
        text = p->tooltip();
        if (!text) return 0;
        lua_pushstring(L, text);
        return 1;
        }
    text = luaL_checkstring(L, 2);
/*  p->tooltip(text); */
    p->copy_tooltip(text);
    return 0;
    }


static int Widgetcolor(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    if(lua_isnone(L, 2)) /* get */  
        {
        push_Color(L, p->color());
        return 1;   
        }
    Fl_Color color = check_Color(L, 2);
    if(lua_isnone(L, 3))
        {
        p->color(color);
        return 0;
        }
    Fl_Color sel = check_Color(L, 3);
    p->color(color, sel);
    return 0;
    }

GETSET_INTTYPE(Widget, Color, selection_color, Widgetselection_color)
GETSET_ENUM(Widget, Boxtype, box, Widgetbox)

static int Widgetalign(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    if(lua_isnone(L, 2)) /* get */  
        return push_Align(L, p->align()); /* multiple return values */
    p->align(check_Align(L, 2));
    return 0;
    }

GETSET_ENUM(Widget, Labeltype, labeltype, Widgetlabeltype)
GETSET_INTTYPE(Widget, Color, labelcolor, Widgetlabelcolor)
GETSET_ENUM(Widget, Font, labelfont, Widgetlabelfont)
GETSET_INTTYPE(Widget, Fontsize, labelsize, Widgetlabelsize)

static int Widgetwhen(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    if(lua_isnone(L, 2)) /* get */  
        return push_When(L, p->when()); /* multiple return values */
    p->when(check_When(L, 2));
    return 0;
    }


GETSET_POINTER_OPT(Widget, Image, image, Widgetimage)
GETSET_POINTER_OPT(Widget, Image, deimage, Widgetdeimage)

VOID_VOID(Widget, draw, Widgetdraw)
BOOLEAN_EVENT(Widget, handle, Widgethandle)
VOID_VOID(Widget, redraw, Widgetredraw)
VOID_VOID(Widget, redraw_label, Widgetredraw_label)

BOOLEAN_VOID(Widget, visible, Widgetvisible)
BOOLEAN_VOID(Widget, visible_r, Widgetvisible_r)
VOID_VOID(Widget, show, Widgetshow)
VOID_VOID(Widget, hide, Widgethide)
VOID_VOID(Widget, set_visible, Widgetset_visible)
VOID_VOID(Widget, clear_visible, Widgetclear_visible)
BOOLEAN_VOID(Widget, active, Widgetactive)
BOOLEAN_VOID(Widget, active_r, Widgetactive_r)
VOID_VOID(Widget, activate, Widgetactivate)
VOID_VOID(Widget, deactivate, Widgetdeactivate)
BOOLEAN_VOID(Widget, output, Widgetoutput)
VOID_VOID(Widget, set_output, Widgetset_output)
VOID_VOID(Widget, clear_output, Widgetclear_output)
BOOLEAN_VOID(Widget, takesevents, Widgettakesevents)
BOOLEAN_VOID(Widget, changed, Widgetchanged)
VOID_VOID(Widget, set_changed, Widgetset_changed)
VOID_VOID(Widget, clear_changed, Widgetclear_changed)
VOID_VOID(Widget, clear_active, Widgetclear_active)
VOID_VOID(Widget, set_active, Widgetset_active)
BOOLEAN_VOID(Widget, take_focus, Widgettake_focus)
VOID_VOID(Widget, set_visible_focus, Widgetset_visible_focus)
VOID_VOID(Widget, clear_visible_focus, Widgetclear_visible_focus)
GETSET_BOOLEAN(Widget, visible_focus, Widgetvisible_focus)

//BOOLEAN_VOID(Widget, use_accents_menu, Widgetuse_accents_menu)

POINTER_VOID(Widget, Group, parent, Widgetparent)
POINTER_VOID(Widget, Window, window, Widgetwindow)
POINTER_VOID(Widget, Window, top_window, Widgettop_window)

static int Widgettop_window_offset(lua_State *L)
    {
    int xoff, yoff;
    Fl_Widget *p = check_Widget(L, 1);
    Fl_Window *window = p->top_window_offset(xoff, yoff);
    if(!window)
        return 0;
    push_Window(L, window);
    lua_pushinteger(L, xoff);
    lua_pushinteger(L, yoff);
    return 3;
    }


static int Widgetcontains(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    Fl_Widget *w = check_Widget(L, 2);
    lua_pushboolean(L, p->contains(w));
    return 1;
    }


static int Widgetinside(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    Fl_Widget *w = check_Widget(L, 2);
    lua_pushboolean(L, p->inside(w));
    return 1;
    }

static int Widgetdamage(lua_State *L)
/* bit1, ... = damage()   
 * damage(bit1, bit2, ...)
 * damage(x, y, w, h, bit1, bit2, ...)
 */
    {
    Fl_Widget *p = check_Widget(L, 1);
    if(lua_isnone(L, 2))
        return push_Damage(L, (Fl_Damage)p->damage());
    if(lua_type(L, 2) == LUA_TNUMBER)
        {
        int x = luaL_checkinteger(L, 2);
        int y = luaL_checkinteger(L, 3);
        int w = luaL_checkinteger(L, 4);
        int h = luaL_checkinteger(L, 5);
        Fl_Damage c = check_Damage(L, 6);
        p->damage(c, x, y, w, h);
        }
    else
        {
        Fl_Damage c = check_Damage(L, 2);
        p->damage(c);
        }
    return 0;
    }


static int Widgetclear_damage(lua_State *L) /* same as damage("nothing") */
    {
    Fl_Widget *p = check_Widget(L, 1);
    Fl_Damage c = check_Damage(L, 2);
    p->clear_damage(c);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Protected methods                                                            |
 *------------------------------------------------------------------------------*/
/* Some protected methods are exposed so that they can be used by "subclasses" 
 * in Lua (see subclass.c). 
 */

class Fl_WidgetEXPOSER : Fl_Widget {
    public:
    void draw_box() const { Fl_Widget::draw_box(); }
    void draw_box(Fl_Boxtype t, Fl_Color c) const { Fl_Widget::draw_box(t, c); }
    void draw_box(Fl_Boxtype t, int x,int y,int w,int h, Fl_Color c) const
             { Fl_Widget::draw_box(t, x, y, w, h, c); }
    void draw_backdrop() const { Fl_Widget::draw_backdrop(); }
    void draw_focus() { Fl_Widget::draw_focus(); }
    void draw_focus(Fl_Boxtype t, int x,int y,int w,int h) { Fl_Widget::draw_focus(t,x,y,w,h); }
    void draw_label() const { Fl_Widget::draw_label(); }
    void draw_label(int x, int y, int w, int h) const { Fl_Widget::draw_label(x, y, w, h); }
};


static int Widgetdraw_box(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    if(lua_isnone(L, 2))
        {
        ((Fl_WidgetEXPOSER*)p)->draw_box();
        return 0;
        }
    Fl_Boxtype t = check_Boxtype(L, 2);
    if(lua_isnone(L, 4))
        {
        ((Fl_WidgetEXPOSER*)p)->draw_box(t, check_Color(L, 3));
        return 0;
        }
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);
    int w = luaL_checkinteger(L, 5);
    int h = luaL_checkinteger(L, 6);
    ((Fl_WidgetEXPOSER*)p)->draw_box(t, x, y, w, h, check_Color(L, 7));
    return 0;
    }

static int Widgetdraw_backdrop(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    ((Fl_WidgetEXPOSER*)p)->draw_backdrop();
    return 0;
    }

static int Widgetdraw_focus(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
    if(lua_isnone(L, 2))
        {
        ((Fl_WidgetEXPOSER*)p)->draw_focus();
        return 0;
        }
    Fl_Boxtype t = check_Boxtype(L, 2);
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);
    int w = luaL_checkinteger(L, 5);
    int h = luaL_checkinteger(L, 6);
    ((Fl_WidgetEXPOSER*)p)->draw_focus(t, x, y, w, h);
    return 0;
    }

static int Widgetdraw_label(lua_State *L)
    {
    Fl_Widget *p = check_Widget(L, 1);
#if 1 /* protected */
    if(lua_isnone(L, 2))
        {
        ((Fl_WidgetEXPOSER*)p)->draw_label();
        return 0;
        }
#endif
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int w = luaL_checkinteger(L, 4);
    int h = luaL_checkinteger(L, 5);
#if 1 /* protected */
    if(lua_isnone(L, 6))
        {
        ((Fl_WidgetEXPOSER*)p)->draw_label(x, y, w, h);
        return 0;
        }
#endif
    Fl_Align align = check_Align(L, 6);
    p->draw_label(x, y, w, h, align);
    return 0;
    }

static const struct luaL_Reg Methods[] = 
    {
        { "active", Widgetactive },
        { "active_r", Widgetactive_r },
        { "activate", Widgetactivate },
        { "align", Widgetalign },
        { "argument", Widgetargument },
        { "box", Widgetbox },
        { "callback", Widgetcallback },
        { "changed", Widgetchanged },
        { "clear_active", Widgetclear_active },
        { "clear_changed", Widgetclear_changed },
        { "clear_damage", Widgetclear_damage },
        { "clear_output", Widgetclear_output },
        { "clear_visible", Widgetclear_visible },
        { "clear_visible_focus", Widgetclear_visible_focus },
        { "color", Widgetcolor },
        { "contains", Widgetcontains },
        { "damage", Widgetdamage },
        { "deactivate", Widgetdeactivate },
//      { "default_callback", Widgetdefault_callback },
        { "deimage", Widgetdeimage  },
        { "do_callback", Widgetdo_callback },
        { "draw_label", Widgetdraw_label },
        { "h", Widgeth },
        { "hide", Widgethide },
        { "image", Widgetimage },
        { "inside", Widgetinside },
        { "label", Widgetlabel },
        { "labelcolor", Widgetlabelcolor },
        { "labelfont", Widgetlabelfont },
        { "labelsize", Widgetlabelsize },
        { "labeltype", Widgetlabeltype },
        { "measure_label", MeasureLabel },
        { "output", Widgetoutput },
        { "parent", Widgetparent }, /* do not use (see manual) */
        { "position", Widgetposition },
        { "redraw", Widgetredraw },
        { "redraw_label", Widgetredraw_label },
        { "resize", Widgetresize },
        { "selection_color", Widgetselection_color },
        { "set_active", Widgetset_active },
        { "set_changed", Widgetset_changed },
        { "set_output", Widgetset_output },
        { "set_visible", Widgetset_visible },
        { "set_visible_focus", Widgetset_visible_focus },
        { "show", Widgetshow },
        { "size", Widgetsize },
        { "take_focus", Widgettake_focus },
        { "takesevents", Widgettakesevents },
        { "tooltip", Widgettooltip },
        { "top_window", Widgettop_window },
        { "top_window_offset", Widgettop_window_offset },
        { "type", Widgettype },
        { "user_data", Widgetargument },
        { "visible", Widgetvisible },
        { "visible_focus", Widgetvisible_focus },
        { "visible_r", Widgetvisible_r },
        { "w", Widgetw },
        { "when", Widgetwhen },
        { "window", Widgetwindow },
        { "x", Widgetx },
        { "y", Widgety },
        { "xywh", Widgetxywh },
        { "handle", Widgethandle },
        { "draw", Widgetdraw },
//      { "use_accents_menu", Widgetuse_accents_menu },
        /* exposed protected methods */
        { "draw_backdrop", Widgetdraw_backdrop },
        { "draw_box", Widgetdraw_box },
        { "draw_focus", Widgetdraw_focus },
        { NULL, NULL } /* sentinel */
    };

#define MetaMethods NULL

/*------------------------------------------------------------------------------*
 | Fl_Box                                                                       |
 *------------------------------------------------------------------------------*/

BOX_NEW(Box, New)
BOX_DELETE(Box, Delete)

#define BoxMethods NULL

static const struct luaL_Reg BoxMetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Progress                                                                  |
 *------------------------------------------------------------------------------*/

WIDGET_NEW(Progress, ProgressNew)
WIDGET_DELETE(Progress, ProgressDelete)
GETSET_FLOAT(Progress, maximum, Progressmaximum)
GETSET_FLOAT(Progress, minimum, Progressminimum)
GETSET_FLOAT(Progress, value, Progressvalue)

static const struct luaL_Reg ProgressMethods[] = 
    {
        { "maximum", Progressmaximum },
        { "minimum", Progressminimum },
        { "value", Progressvalue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg ProgressMetaMethods[] = 
    {
        { "__gc",  ProgressDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "box",  New },
        { "progress",  ProgressNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Widget(lua_State *L)
    {
    udata_define(L, MT_Widget, Methods, MetaMethods);

    udata_define(L, MT_Box, BoxMethods, BoxMetaMethods);
    udata_inherit(L, MT_Box, MT_Widget);

    udata_define(L, MT_Progress, ProgressMethods, ProgressMetaMethods);
    udata_inherit(L, MT_Progress, MT_Widget);

    luaL_setfuncs(L, Functions, 0);
    }

#if(0)
// Not needed:
virtual Fl_Group* as_group() {return 0;}
virtual Fl_Window* as_window() {return 0;}
virtual class Fl_Gl_Window* as_gl_window() {return 0;}
#endif
