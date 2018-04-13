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

void groupclear(lua_State *L, Fl_Group *p)
/* removes and unreferences all children bound to Lua userdata */
    {
    int n = p->children();
    if(n<=0) 
        return;
    for(int i=n-1; i >= 0; --i)
        {
        Fl_Widget *c = p->child(i);
        if(userdata(c))
            {
            p->remove(i);
            userdata_unref(L, c);
            }
        }
    }

static void scrollclear(lua_State *L, Fl_Scroll *p)
/* removes and unreferences all children bound to Lua userdata */
    {
    int n = p->children();
    if(n<=0) 
        return;
    for(int i=n-1; i >= 0; --i)
        {
        Fl_Widget *c = p->child(i);
        /* skip scrollbars */
        if((c == &p->scrollbar) || (c == &p->hscrollbar))
            continue;
        if(userdata(c))
            {
            p->remove(i);
            userdata_unref(L, c);
            }
        }
    }


/*------------------------------------------------------------------------------*
 | Fl_Group exposer (I hate C++ ...)                                            |
 *------------------------------------------------------------------------------*/

class Fl_GroupEXPOSER : Fl_Group {
    public:
    void draw_child(Fl_Widget& widget) const { Fl_Group::draw_child(widget); }
    void draw_children() { Fl_Group::draw_children(); }
    void draw_outside_label(const Fl_Widget& widget) const 
        { Fl_Group::draw_outside_label(widget); }
    void update_child(Fl_Widget& widget) const { Fl_Group::update_child(widget); }
#if 0 /*@@*/
protected:
  void draw();
  int  *sizes();
#endif
};

/*------------------------------------------------------------------------------*
 | Fl_Group                                                                     |
 *------------------------------------------------------------------------------*/

GROUP_NEW(Group, New)
GROUP_DELETE(Group, Delete)

VOID_VOID(Group, begin, Groupbegin)
VOID_VOID(Group, end, Groupend)
INT_VOID(Group, children, Groupchildren)

/* NOTE1: children indices range
 * in C   : 0 .. group:children()-1
 * in Lua : 1 .. group:children
 */
static int Groupchild(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    int n = checkindex(L, 2);
    if((n < 0) || (n >= p->children()))
        return luaL_argerror(L, 2, "out of range");
    Fl_Widget *c = p->child(n);
    if(!c) return 0;
    push_Widget(L, c);
    return 1;
    }

static int Groupfind(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    Fl_Widget *c = test_Widget(L, 2);
    int index = p->find(c);
    pushindex(L, index); /* see NOTE1 */
    return 1;
    }

static int Groupadd(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    Fl_Widget *c = check_Widget(L, 2);
    p->add(c);
    return 0;
    }

static int Groupadd_resizable(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    Fl_Widget *c = check_Widget(L, 2);
    p->add_resizable(*c);
    return 0;
    }

VOID_VOID(Group, init_sizes, Groupinit_sizes)

static int Groupresizable(lua_State *L)
/* NOTE: resizable() -> get
 *       resizable(nil) -> set with a nil value
 */
    {
    Fl_Group *p = check_Group(L, 1);
    if(lua_isnone(L, 2))
        {
        Fl_Widget *w = p->resizable();
        if(!w) return 0;
        push_Widget (L, w);
        return 1;
        }
    if(lua_isnil(L, 2))
        p->resizable(NULL);
    else
        p->resizable(check_Widget(L, 2));
    return 0;
    }

static int Groupinsert(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    Fl_Widget *c = check_Widget(L, 2);
    if(lua_isnumber(L, 3))
        p->insert(*c, checkindex(L, 3)); /* see NOTE1 */
    else
        p->insert(*c, check_Widget(L, 3));
    return 0;
    }

static int Groupremove(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    if(lua_isnumber(L, 2))
        p->remove(checkindex(L, 2)); /* see NOTE1 */
    else
        p->remove(check_Widget(L, 2));
    return 0;
    }

static int Groupclear(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    /* p->clear() deletes the widgets from memory.
     * Widgets are bound to Lua variables and deleted in their __gc metamethod.
     * We need to remove them from the group before calling clear() or deleting 
     * the group itself, to avoid a double delete. */
    groupclear(L, p);
//  p->clear();
    return 0;
    }

GETSET_BOOLEAN(Group, clip_children, Groupclip_children)

static int Current(lua_State *L)
    {
    if(lua_isnone(L, 1))
        {
        Fl_Group *p = Fl_Group::current();
        if(!p) return 0;
        push_Group(L, p);
        return 1;
        }
    Fl_Group *p = check_Group(L, 1);
    Fl_Group::current(p);
    return 0;
    }

static int Groupdraw_child(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    Fl_Widget *w = check_Widget(L, 2);
    ((Fl_GroupEXPOSER*)p)->draw_child(*w);
    return 0;
    }

static int Groupdraw_children(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    ((Fl_GroupEXPOSER*)p)->draw_children();
    return 0;
    }

static int Groupdraw_outside_label(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    Fl_Widget *w = check_Widget(L, 2);
    ((Fl_GroupEXPOSER*)p)->draw_outside_label(*w);
    return 0;
    }


static int Groupupdate_child(lua_State *L)
    {
    Fl_Group *p = check_Group(L, 1);
    Fl_Widget *w = check_Widget(L, 2);
    ((Fl_GroupEXPOSER*)p)->update_child(*w);
    return 0;
    }

static const struct luaL_Reg Methods[] =
    {
        { "add", Groupadd  },
        { "add_resizable", Groupadd_resizable },
        { "begin", Groupbegin },
        { "child", Groupchild },
        { "children", Groupchildren },
        { "clear", Groupclear },
        { "clip_children", Groupclip_children },
        { "done", Groupend }, /* 'end' is a Lua keyword so it cannot be used */
        { "find", Groupfind },
        { "init_sizes", Groupinit_sizes },
        { "insert", Groupinsert },
        { "remove", Groupremove },
        { "resizable", Groupresizable },
// formerly protected:
        { "draw_child", Groupdraw_child },
        { "draw_children", Groupdraw_children },
        { "draw_outside_label", Groupdraw_outside_label},
        { "update_child", Groupupdate_child },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

#if(0)
// Fl_Widget* const* Fl_Group::array() const;
#endif

/*------------------------------------------------------------------------------*
 | Fl_Pack                                                                      |
 *------------------------------------------------------------------------------*/

GROUP_NEW(Pack, PackNew)
GROUP_DELETE(Pack, PackDelete)
WIDGET_TYPE(Pack, Packtype)
GETSET_INT(Pack, spacing, Packspacing)

static const struct luaL_Reg PackMethods[] = 
    {
        { "spacing", Packspacing },
        { "type", Packtype },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg PackMetaMethods[] = 
    {
        { "__gc",  PackDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Scroll                                                                    |
 *------------------------------------------------------------------------------*/

SCROLL_NEW(Scroll, ScrollNew) 
SCROLL_DELETE(Scroll, ScrollDelete)
WIDGET_TYPE(Scroll, Scrolltype)
VOID_INT4(Scroll, resize, Scrollresize)
INT_VOID(Scroll, xposition, Scrollxposition)
INT_VOID(Scroll, yposition, Scrollyposition)
VOID_INT_INT(Scroll, scroll_to, Scrollscroll_to)
GETSET_INT(Scroll, scrollbar_size, Scrollscrollbar_size)

static int Scrollclear(lua_State *L)
    {
    Fl_Scroll *p = check_Scroll(L, 1);
    scrollclear(L, p);
//  p->clear();
    return 0;
    }

static int Scrollscrollbar(lua_State *L)
    {
    Fl_Scroll *p = check_Scroll(L, 1);
    push_Scrollbar(L, &p->scrollbar);
    return 1;
    }

static int Scrollhscrollbar(lua_State *L)
    {
    Fl_Scroll *p = check_Scroll(L, 1);
    push_Scrollbar(L, &p->hscrollbar);
    return 1;
    }

static const struct luaL_Reg ScrollMethods[] = 
    {
        { "clear", Scrollclear },
        { "hscrollbar", Scrollhscrollbar }, /* horizontal scrollbar */
        { "scrollbar", Scrollscrollbar }, /* vertical scrollbar */
        { "scrollbar_size", Scrollscrollbar_size },
        { "scroll_to", Scrollscroll_to },
        { "resize", Scrollresize },
        { "type", Scrolltype },
        { "xposition", Scrollxposition },
        { "yposition", Scrollyposition },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg ScrollMetaMethods[] = 
    {
        { "__gc",  ScrollDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Spinner                                                                   |
 *------------------------------------------------------------------------------*/

GROUP_NEW(Spinner, SpinnerNew)
GROUP_DELETE(Spinner, SpinnerDelete)

WIDGET_TYPE(Spinner, Spinnertype)
GETSET_ENUM(Spinner, Font, textfont, Spinnertextfont)
GETSET_INTTYPE(Spinner, Fontsize, textsize, Spinnertextsize)
GETSET_INTTYPE(Spinner, Color, textcolor, Spinnertextcolor)
GETSET_INTTYPE(Spinner, Color, color, Spinnercolor)
GETSET_INTTYPE(Spinner, Color, selection_color, Spinnerselection_color)
GETSET_DOUBLE(Spinner, maximum, Spinnermaximum)
GETSET_DOUBLE(Spinner, minimum, Spinnerminimum)
GETSET_DOUBLE(Spinner, value, Spinnervalue)
GETSET_DOUBLE(Spinner, step, Spinnerstep)
VOID_INT4(Spinner, resize, Spinnerresize)
VOID_DOUBLE_DOUBLE(Spinner, range, Spinnerrange)

static int Spinnerformat(lua_State *L)
    {
    Fl_Spinner *p = check_Spinner(L, 1);
    if(lua_isnone(L, 2))
        {
        const char *s = p->format();
        if(!s) return 0;
        lua_pushstring(L, s);
        return 1;
        }
    p->format(luaL_checkstring(L, 2));
    return 0;
    }

static const struct luaL_Reg SpinnerMethods[] = 
    {
        { "color", Spinnercolor },
        { "format", Spinnerformat },
        { "maximum", Spinnermaximum },
        { "minimum", Spinnerminimum },
        { "range", Spinnerrange },
        { "resize", Spinnerresize },
        { "selection_color", Spinnerselection_color },
        { "step", Spinnerstep },
        { "textcolor", Spinnertextcolor },
        { "textfont", Spinnertextfont },
        { "textsize", Spinnertextsize },
        { "type", Spinnertype },
        { "value", Spinnervalue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg SpinnerMetaMethods[] = 
    {
        { "__gc",  SpinnerDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Fl_Tile                                                                      |
 *------------------------------------------------------------------------------*/

GROUP_NEW(Tile, TileNew)
GROUP_DELETE(Tile, TileDelete)
VOID_INT4(Tile, resize, Tileresize)

#if 1
VOID_INT4(Tile, position, Tileposition)
#else
static int Tileposition(lua_State *L)
    {
    Fl_Tile *p = check_Tile(L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    if(lua_isnone(L, 4))
        {
        p->position(x,y); // no matching function... we lost widget:position(x,y)
        return 0;
        }
    int w = luaL_checkinteger(L, 4);
    int h = luaL_checkinteger(L, 5);
    p->position(x,y,w,h);
    return 0;
    }
#endif

static const struct luaL_Reg TileMethods[] = 
    {
        { "resize", Tileresize },
        { "position", Tileposition },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg TileMetaMethods[] = 
    {
        { "__gc",  TileDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Wizard                                                                    |
 *------------------------------------------------------------------------------*/

GROUP_NEW(Wizard, WizardNew)
GROUP_DELETE(Wizard, WizardDelete)
GETSET_POINTER(Wizard, Widget, value, WizardValue)
VOID_VOID(Wizard, next, WizardNext)
VOID_VOID(Wizard, prev, WizardPrev)

static const struct luaL_Reg WizardMethods[] = 
    {
        { "next", WizardNext },
        { "prev", WizardPrev },
        { "value", WizardValue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg WizardMetaMethods[] = 
    {
        { "__gc",  WizardDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "current_group", Current },
        { "group",  New },
        { "pack",  PackNew },
        { "scroll",  ScrollNew },
        { "spinner",  SpinnerNew },
        { "tile",  TileNew },
        { "wizard",  WizardNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Group(lua_State *L)
    {
    udata_define(L, MT_Group, Methods, MetaMethods);
    udata_inherit(L, MT_Group, MT_Widget);

    udata_define(L, MT_Pack, PackMethods, PackMetaMethods);
    udata_inherit(L, MT_Pack, MT_Group);

    udata_define(L, MT_Scroll, ScrollMethods, ScrollMetaMethods);
    udata_inherit(L, MT_Scroll, MT_Group);

    udata_define(L, MT_Spinner, SpinnerMethods, SpinnerMetaMethods);
    udata_inherit(L, MT_Spinner, MT_Group);

    udata_define(L, MT_Tile, TileMethods, TileMetaMethods);
    udata_inherit(L, MT_Tile, MT_Group);

    udata_define(L, MT_Wizard, WizardMethods, WizardMetaMethods);
    udata_inherit(L, MT_Wizard, MT_Group);

    luaL_setfuncs(L, Functions, 0);
    }

