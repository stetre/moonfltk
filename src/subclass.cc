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
 | Fl_XxxSUB implementations for Fl_Xxx virtual methods                 |
 *----------------------------------------------------------------------*/

/* These macros implement methods for Fl_XxxSUB classes, which override
 * the corresponding methods of the Fl_Xxx class they are derived from.
 *
 * The task of the generic Fl_XxxSUB::yyy() method is to execute the Lua
 * function registered by the user for the affected object via the 
 * override_yyy() method.
 *
 * If no Lua function has been registered, the method executes the 
 * corresponding method of the superclass (i.e., Fl_Xxx::yyy()).
 */


#define SUB_VOID_VOID(T, what)                              \
void Fl_##T##SUB::what()                                    \
    {                                                       \
    ud_t *ud = userdata(this);                              \
    if(!ud) return;                                         \
    if(ud->what == LUA_NOREF)                               \
        { Fl_##T::what(); return; }                         \
    lua_State *L = main_lua_state;                          \
    if (!L) return;                                         \
    if(pushvalue(L, ud->what) != LUA_TFUNCTION)             \
        { unexpected(L); return; }                          \
    push_Widget(L, this);                                   \
    if(lua_pcall(L, 1, 0, 0) != LUA_OK)                     \
        { lua_error(L); return; }                           \
    }

#define SUB_HANDLE(T)                                       \
int Fl_##T##SUB::handle(int event)                          \
    {                                                       \
    ud_t *ud = userdata(this);                              \
    if(!ud) return 0;                                       \
    if(ud->handle == LUA_NOREF)                             \
        { return Fl_##T::handle(event); }                   \
    lua_State *L = main_lua_state;                          \
    if (!L) return 0;                                       \
    if(pushvalue(L, ud->handle) != LUA_TFUNCTION)           \
        { unexpected(L); return 0; }                        \
    push_Widget(L, this);                                   \
    push_Event(L, event);                                   \
    if(lua_pcall(L, 2, 1, 0) != LUA_OK)                     \
        { lua_error(L); return 0; }                         \
    return checkboolean(L, -1);                             \
    }

#define SUB_RESIZE(T)                                       \
void Fl_##T##SUB::resize(int x, int y, int w, int h)        \
    {                                                       \
    ud_t *ud = userdata(this);                              \
    if(!ud) return;                                         \
    if(ud->resize == LUA_NOREF)                             \
        { Fl_##T::resize(x,y,w,h); return; }                \
    lua_State *L = main_lua_state;                          \
    if (!L) return;                                         \
    if(pushvalue(L, ud->resize) != LUA_TFUNCTION)           \
        { unexpected(L); return; }                          \
    push_Widget(L, this);                                   \
    lua_pushinteger(L, x);                                  \
    lua_pushinteger(L, y);                                  \
    lua_pushinteger(L, w);                                  \
    lua_pushinteger(L, h);                                  \
    if(lua_pcall(L, 5, 0, 0) != LUA_OK)                     \
        { lua_error(L); return; }                           \
    }

#define SUB_DRAWETC(T)                                      \
    SUB_VOID_VOID(T, draw)                                  \
    SUB_VOID_VOID(T, hide)                                  \
    SUB_VOID_VOID(T, show)                                  \
    SUB_HANDLE(T)                                           \
    SUB_RESIZE(T)


#define SUB_DRAW_CELL(T)                                    \
void Fl_##T##SUB::draw_cell(TableContext context, int r, int c, int x, int y, int w, int h) \
    {                                                       \
    ud_t *ud = userdata(this);                              \
    if(!ud) return;                                         \
    if(ud->draw_cell == LUA_NOREF)                          \
        { Fl_##T::draw_cell(context,r,c,x,y,w,h); return; } /*@@NO: error */ \
    lua_State *L = main_lua_state;                          \
    if (!L) return;                                         \
    if(pushvalue(L, ud->draw_cell) != LUA_TFUNCTION)        \
        { unexpected(L); return; }                          \
    push_Widget(L, this);                                   \
    push_TableContext(L, context);                          \
    pushindex(L, r);                                        \
    pushindex(L, c);                                        \
    lua_pushinteger(L, x);                                  \
    lua_pushinteger(L, y);                                  \
    lua_pushinteger(L, w);                                  \
    lua_pushinteger(L, h);                                  \
    if(lua_pcall(L, 8, 0, 0) != LUA_OK)                     \
        { lua_error(L); return; }                           \
    }



/*----------------------------------------------------------------------*
 | override_yyy()                                                       |
 *----------------------------------------------------------------------*/
/* The fl.xxx_sub() function creates an object of the Fl_XxxSUB class,
 * wich is a subclass of the Fl_Xxx class.
 * Such an object has methods named override_yyy() that it can use to
 * register Lua functions to override the yyy() method.
 *
 * e.g.: 
 *      win = fl.window_sub() -- creates an object of the Fl_WindowSUB class
 *                            -- which is a subclass of Fl_Window.
 *      function mydraw() ... end
 *      win:override_draw(mydraw) -- override the draw() method
 *
 * The Lua function receives the object as first argument, and possibly
 * other parameters (this depends on the overriden method, and the same 
 * holds for the values the function is expected to return).
 *
 */ 

#define OVERRIDE(T, what)                                   \
static int T##_override_##what(lua_State *L)                \
/* override_what([func]) */                                 \
    {                                                       \
    Fl_##T##SUB *p = check_##T##SUB(L, 1);                  \
    ud_t *ud = userdata(p);                                 \
    if(lua_isnoneornil(L, 2))                               \
        {                                                   \
        unreference(L, ud->what);                           \
        return 0;                                           \
        }                                                   \
    if(!lua_isfunction(L, 2))                               \
        return luaL_argerror(L, 2, "function expected");    \
    unreference(L, ud->what);                               \
    reference(L, ud->what, 2);                              \
    return 0;                                               \
    }

#define OVERRIDE_DRAWETC(T)     \
    OVERRIDE(T, draw)           \
    OVERRIDE(T, handle)         \
    OVERRIDE(T, hide)           \
    OVERRIDE(T, show)           \
    OVERRIDE(T, resize)

#define REGISTER_OVERRIDE(T, what)      \
        { "override_"#what, T##_override_##what },

#define REGISTER_OVERRIDE_DRAWETC(T)    \
    REGISTER_OVERRIDE(T, draw)          \
    REGISTER_OVERRIDE(T, handle)        \
    REGISTER_OVERRIDE(T, hide)          \
    REGISTER_OVERRIDE(T, show)          \
    REGISTER_OVERRIDE(T, resize)

/*----------------------------------------------------------------------*
 | super_yyy()                                                          |
 *----------------------------------------------------------------------*/
/* Bindings for the super_xxx() methods */

#define SUPER_VOID_VOID(T, what)            \
static int T##_super_##what(lua_State *L)   \
    {                                       \
    Fl_##T##SUB *p = check_##T##SUB(L, 1);  \
    DBG("executing super_"#what"()\n");     \
    p->super_##what();                      \
    return 0;                               \
    }
    
#define SUPER_DRAW(T) SUPER_VOID_VOID(T, draw)
#define SUPER_HIDE(T) SUPER_VOID_VOID(T, hide)
#define SUPER_SHOW(T) SUPER_VOID_VOID(T, show)

#define SUPER_HANDLE(T)                     \
static int T##_super_handle(lua_State *L)   \
    {                                       \
    Fl_##T##SUB *p = check_##T##SUB(L, 1);  \
    int e = check_Event(L, 2);              \
    DBG("executing super_handle()\n");      \
    lua_pushboolean(L, p->super_handle(e)); \
    return 1;                               \
    }

#define SUPER_RESIZE(T)                     \
static int T##_super_resize(lua_State *L)   \
    {                                       \
    Fl_##T##SUB *p = check_##T##SUB(L, 1);  \
    int x = luaL_checkinteger(L, 2);        \
    int y = luaL_checkinteger(L, 3);        \
    int w = luaL_checkinteger(L, 4);        \
    int h = luaL_checkinteger(L, 5);        \
    DBG("executing super_resize()\n");      \
    p->super_resize(x, y, w, h);            \
    return 0;                               \
    }

#define REGISTER_SUPER(T, what) \
        { "super_"#what, T##_super_##what },

#define SUPER_DRAWETC(T)        \
    SUPER_DRAW(T)               \
    SUPER_HIDE(T)               \
    SUPER_SHOW(T)               \
    SUPER_HANDLE(T)             \
    SUPER_RESIZE(T)

#define REGISTER_SUPER_DRAWETC(T)   \
    REGISTER_SUPER(T, draw)         \
    REGISTER_SUPER(T, handle)       \
    REGISTER_SUPER(T, hide)         \
    REGISTER_SUPER(T, show)         \
    REGISTER_SUPER(T, resize)

#define DRAWETC(T)          \
    SUB_DRAWETC(T)          \
    OVERRIDE_DRAWETC(T)     \
    SUPER_DRAWETC(T)

#define REGISTER_DRAWETC(T)     \
    REGISTER_OVERRIDE_DRAWETC(T)\
    REGISTER_SUPER_DRAWETC(T)


/*----------------------------------------------------------------------*
 | Fl_Widget subclass                                                   |
 *----------------------------------------------------------------------*/

void Fl_Widget::draw() 
/* Implementation for the pure-virtual method (we need this for the default behavior). */
    {
    fprintf(stderr, "warning: Fl_Widget::draw() was called (you should override it)\n");
    }

WIDGET_NEW(WidgetSUB, WidgetSUBNew)
WIDGET_DELETE(WidgetSUB, WidgetSUBDelete)
DRAWETC(Widget)

static const struct luaL_Reg WidgetSUBMethods[] = 
    {
        REGISTER_DRAWETC(Widget)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg WidgetSUBMetaMethods[] = 
    {
        { "__gc",  WidgetSUBDelete },
        { NULL, NULL } /* sentinel */
    };


/*----------------------------------------------------------------------*
 | Fl_Box subclass                                                      |
 *----------------------------------------------------------------------*/

BOX_NEW(BoxSUB, BoxSUBNew)
BOX_DELETE(BoxSUB, BoxSUBDelete)
DRAWETC(Box)

static const struct luaL_Reg BoxSUBMethods[] = 
    {
        REGISTER_DRAWETC(Box)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg BoxSUBMetaMethods[] = 
    {
        { "__gc",  BoxSUBDelete },
        { NULL, NULL } /* sentinel */
    };


/*----------------------------------------------------------------------*
 | Fl_Button subclass                                                   |
 *----------------------------------------------------------------------*/

BUTTON_NEW(ButtonSUB, ButtonSUBNew)
BUTTON_DELETE(ButtonSUB, ButtonSUBDelete)
DRAWETC(Button)

static const struct luaL_Reg ButtonSUBMethods[] = 
    {
        REGISTER_DRAWETC(Button)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg ButtonSUBMetaMethods[] = 
    {
        { "__gc",  ButtonSUBDelete },
        { NULL, NULL } /* sentinel */
    };

/*----------------------------------------------------------------------*
 | Fl_Group subclass                                                    |
 *----------------------------------------------------------------------*/

GROUP_NEW(GroupSUB, GroupSUBNew)
GROUP_DELETE(GroupSUB, GroupSUBDelete)
DRAWETC(Group)

static const struct luaL_Reg GroupSUBMethods[] = 
    {
        REGISTER_DRAWETC(Group)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg GroupSUBMetaMethods[] = 
    {
        { "__gc",  GroupSUBDelete },
        { NULL, NULL } /* sentinel */
    };

/*----------------------------------------------------------------------*
 | Fl_Browser subclass                                                  |
 *----------------------------------------------------------------------*/

BROWSER_NEW(BrowserSUB, BrowserSUBNew)
BROWSER_DELETE(BrowserSUB, BrowserSUBDelete)
DRAWETC(Browser)

static const struct luaL_Reg BrowserSUBMethods[] = 
    {
        REGISTER_DRAWETC(Browser)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg BrowserSUBMetaMethods[] = 
    {
        { "__gc",  BrowserSUBDelete },
        { NULL, NULL } /* sentinel */
    };

#if 0
/*----------------------------------------------------------------------*
 | Fl_File_Browser subclass                                             |
 *----------------------------------------------------------------------*/

BROWSER_NEW(File_BrowserSUB, File_BrowserSUBNew)
BROWSER_DELETE(File_BrowserSUB, File_BrowserSUBDelete)
DRAWETC(File_Browser)

static const struct luaL_Reg File_BrowserSUBMethods[] = 
    {
        REGISTER_DRAWETC(File_Browser)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg File_BrowserSUBMetaMethods[] = 
    {
        { "__gc",  File_BrowserSUBDelete },
        { NULL, NULL } /* sentinel */
    };

/*----------------------------------------------------------------------*
 | Fl_Hold_Browser subclass                                             |
 *----------------------------------------------------------------------*/

BROWSER_NEW(Hold_BrowserSUB, Hold_BrowserSUBNew)
BROWSER_DELETE(Hold_BrowserSUB, Hold_BrowserSUBDelete)
DRAWETC(Hold_Browser)

static const struct luaL_Reg Hold_BrowserSUBMethods[] = 
    {
        REGISTER_DRAWETC(Hold_Browser)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Hold_BrowserSUBMetaMethods[] = 
    {
        { "__gc",  Hold_BrowserSUBDelete },
        { NULL, NULL } /* sentinel */
    };

/*----------------------------------------------------------------------*
 | Fl_Multi_Browser subclass                                            |
 *----------------------------------------------------------------------*/

BROWSER_NEW(Multi_BrowserSUB, Multi_BrowserSUBNew)
BROWSER_DELETE(Multi_BrowserSUB, Multi_BrowserSUBDelete)
DRAWETC(Multi_Browser)

static const struct luaL_Reg Multi_BrowserSUBMethods[] = 
    {
        REGISTER_DRAWETC(Multi_Browser)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Multi_BrowserSUBMetaMethods[] = 
    {
        { "__gc",  Multi_BrowserSUBDelete },
        { NULL, NULL } /* sentinel */
    };


/*----------------------------------------------------------------------*
 | Fl_Select_Browser subclass                                           |
 *----------------------------------------------------------------------*/

BROWSER_NEW(Select_BrowserSUB, Select_BrowserSUBNew)
BROWSER_DELETE(Select_BrowserSUB, Select_BrowserSUBDelete)
DRAWETC(Select_Browser)

static const struct luaL_Reg Select_BrowserSUBMethods[] = 
    {
        REGISTER_DRAWETC(Select_Browser)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Select_BrowserSUBMetaMethods[] = 
    {
        { "__gc",  Select_BrowserSUBDelete },
        { NULL, NULL } /* sentinel */
    };


/*----------------------------------------------------------------------*
 | Fl_Check_Browser subclass                                            |
 *----------------------------------------------------------------------*/

BROWSER_NEW(Check_BrowserSUB, Check_BrowserSUBNew)
BROWSER_DELETE(Check_BrowserSUB, Check_BrowserSUBDelete)
DRAWETC(Check_Browser)

static const struct luaL_Reg Check_BrowserSUBMethods[] = 
    {
        REGISTER_DRAWETC(Check_Browser)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Check_BrowserSUBMetaMethods[] = 
    {
        { "__gc",  Check_BrowserSUBDelete },
        { NULL, NULL } /* sentinel */
    };

#endif


/*----------------------------------------------------------------------*
 | Fl_Table subclass                                                    |
 *----------------------------------------------------------------------*/

TABLE_NEW(TableSUB, TableSUBNew)
TABLE_DELETE(TableSUB, TableSUBDelete)
DRAWETC(Table)

SUB_DRAW_CELL(Table)
OVERRIDE(Table, draw_cell)

static const struct luaL_Reg TableSUBMethods[] = 
    {
        REGISTER_DRAWETC(Table)
        REGISTER_OVERRIDE(Table, draw_cell)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg TableSUBMetaMethods[] = 
    {
        { "__gc",  TableSUBDelete },
        { NULL, NULL } /* sentinel */
    };

/*----------------------------------------------------------------------*
 | Fl_Table_Row subclass                                                |
 *----------------------------------------------------------------------*/

TABLE_NEW(Table_RowSUB, Table_RowSUBNew)
TABLE_DELETE(Table_RowSUB, Table_RowSUBDelete)
DRAWETC(Table_Row)

SUB_DRAW_CELL(Table_Row)
OVERRIDE(Table_Row, draw_cell)

static const struct luaL_Reg Table_RowSUBMethods[] = 
    {
        REGISTER_DRAWETC(Table_Row)
        REGISTER_OVERRIDE(Table_Row, draw_cell)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Table_RowSUBMetaMethods[] = 
    {
        { "__gc",  Table_RowSUBDelete },
        { NULL, NULL } /* sentinel */
    };



/*----------------------------------------------------------------------*
 | Fl_Tabs subclass                                                     |
 *----------------------------------------------------------------------*/

GROUP_NEW(TabsSUB, TabsSUBNew)
GROUP_DELETE(TabsSUB, TabsSUBDelete)
DRAWETC(Tabs)

static const struct luaL_Reg TabsSUBMethods[] = 
    {
        REGISTER_DRAWETC(Tabs)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg TabsSUBMetaMethods[] = 
    {
        { "__gc",  TabsSUBDelete },
        { NULL, NULL } /* sentinel */
    };


/*----------------------------------------------------------------------*
 | Fl_Window subclass                                                   |
 *----------------------------------------------------------------------*/

WINDOW_NEW(WindowSUB, WindowSUBNew)
WINDOW_DELETE(WindowSUB, WindowSUBDelete)
DRAWETC(Window)

static const struct luaL_Reg WindowSUBMethods[] = 
    {
        REGISTER_DRAWETC(Window)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg WindowSUBMetaMethods[] = 
    {
        { "__gc",  WindowSUBDelete },
        { NULL, NULL } /* sentinel */
    };

/*----------------------------------------------------------------------*
 | Fl_Double_Window subclass                                            |
 *----------------------------------------------------------------------*/

WINDOW_NEW(Double_WindowSUB, Double_WindowSUBNew)
WINDOW_DELETE(Double_WindowSUB, Double_WindowSUBDelete)
DRAWETC(Double_Window)

static const struct luaL_Reg Double_WindowSUBMethods[] = 
    {
        REGISTER_DRAWETC(Double_Window)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Double_WindowSUBMetaMethods[] = 
    {
        { "__gc",  Double_WindowSUBDelete },
        { NULL, NULL } /* sentinel */
    };

/*----------------------------------------------------------------------*
 | Fl_Overlay_Window subclass                                           |
 *----------------------------------------------------------------------*/

void Fl_Overlay_Window::draw_overlay() 
/* Implementation for the pure-virtual method (we need this for the default behavior). */
    {
    fprintf(stderr, "warning: Fl_Overlay_Window::draw_overlay() was called (you should override it)\n");
    }

WINDOW_NEW(Overlay_WindowSUB, Overlay_WindowSUBNew)
WINDOW_DELETE(Overlay_WindowSUB, Overlay_WindowSUBDelete)
DRAWETC(Overlay_Window)

SUB_VOID_VOID(Overlay_Window, draw_overlay)
OVERRIDE(Overlay_Window, draw_overlay)

static const struct luaL_Reg Overlay_WindowSUBMethods[] = 
    {
        REGISTER_DRAWETC(Overlay_Window)
        REGISTER_OVERRIDE(Overlay_Window, draw_overlay)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Overlay_WindowSUBMetaMethods[] = 
    {
        { "__gc",  Overlay_WindowSUBDelete },
        { NULL, NULL } /* sentinel */
    };


/*----------------------------------------------------------------------*
 | Fl_Gl_Window subclass                                                |
 *----------------------------------------------------------------------*/

#if USE_GL

WINDOW_NEW(Gl_WindowSUB, Gl_WindowSUBNew)
WINDOW_DELETE(Gl_WindowSUB, Gl_WindowSUBDelete)
DRAWETC(Gl_Window)

static const struct luaL_Reg Gl_WindowSUBMethods[] = 
    {
        REGISTER_DRAWETC(Gl_Window)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Gl_WindowSUBMetaMethods[] = 
    {
        { "__gc",  Gl_WindowSUBDelete },
        { NULL, NULL } /* sentinel */
    };

#endif

/*----------------------------------------------------------------------*
 | Fl_Single_Window subclass                                            |
 *----------------------------------------------------------------------*/

WINDOW_NEW(Single_WindowSUB, Single_WindowSUBNew)
WINDOW_DELETE(Single_WindowSUB, Single_WindowSUBDelete)
DRAWETC(Single_Window)

static const struct luaL_Reg Single_WindowSUBMethods[] = 
    {
        REGISTER_DRAWETC(Single_Window)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Single_WindowSUBMetaMethods[] = 
    {
        { "__gc",  Single_WindowSUBDelete },
        { NULL, NULL } /* sentinel */
    };


/*----------------------------------------------------------------------*
 | Fl_Menu_Window subclass                                              |
 *----------------------------------------------------------------------*/

WINDOW_NEW(Menu_WindowSUB, Menu_WindowSUBNew)
WINDOW_DELETE(Menu_WindowSUB, Menu_WindowSUBDelete)
DRAWETC(Menu_Window)

static const struct luaL_Reg Menu_WindowSUBMethods[] = 
    {
        REGISTER_DRAWETC(Menu_Window)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Menu_WindowSUBMetaMethods[] = 
    {
        { "__gc",  Menu_WindowSUBDelete },
        { NULL, NULL } /* sentinel */
    };

/*----------------------------------------------------------------------*
 | Fl_Roller subclass                                                   |
 *----------------------------------------------------------------------*/

ROLLER_NEW(RollerSUB, RollerSUBNew)
ROLLER_DELETE(RollerSUB, RollerSUBDelete)
DRAWETC(Roller)

static const struct luaL_Reg RollerSUBMethods[] = 
    {
        REGISTER_DRAWETC(Roller)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg RollerSUBMetaMethods[] = 
    {
        { "__gc",  RollerSUBDelete },
        { NULL, NULL } /* sentinel */
    };


/*----------------------------------------------------------------------*
 | Fl_Slider subclass                                                   |
 *----------------------------------------------------------------------*/

SLIDER_NEW(SliderSUB, SliderSUBNew)
SLIDER_DELETE(SliderSUB, SliderSUBDelete)
DRAWETC(Slider)

static const struct luaL_Reg SliderSUBMethods[] = 
    {
        REGISTER_DRAWETC(Slider)
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg SliderSUBMetaMethods[] = 
    {
        { "__gc",  SliderSUBDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "widget_sub",  WidgetSUBNew },
        { "box_sub",  BoxSUBNew },
        { "button_sub",  ButtonSUBNew },
        { "group_sub",  GroupSUBNew },
        { "browser_sub",  BrowserSUBNew },
#if 0
        { "file_browser_sub",  File_BrowserSUBNew },
        { "hold_browser_sub",  Hold_BrowserSUBNew },
        { "multi_browser_sub",  Multi_BrowserSUBNew },
        { "select_browser_sub",  Select_BrowserSUBNew },
        { "check_browser_sub",  Check_BrowserSUBNew },
#endif
        { "tabs_sub",  TabsSUBNew },
        { "table_sub",  TableSUBNew },
        { "table_row_sub",  Table_RowSUBNew },
        { "window_sub",  WindowSUBNew },
        { "double_window_sub",  Double_WindowSUBNew },
        { "overlay_window_sub",  Overlay_WindowSUBNew },
#if USE_GL
        { "gl_window_sub",  Gl_WindowSUBNew },
#endif
        { "single_window_sub",  Single_WindowSUBNew },
        { "menu_window_sub",  Menu_WindowSUBNew },
        { "roller_sub",  RollerSUBNew },
        { "slider_sub",  SliderSUBNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Subclass(lua_State *L)
    {
    udata_define(L, MT_WidgetSUB, WidgetSUBMethods, WidgetSUBMetaMethods);
    udata_inherit(L, MT_WidgetSUB, MT_Widget);

    udata_define(L, MT_BoxSUB, BoxSUBMethods, BoxSUBMetaMethods);
    udata_inherit(L, MT_BoxSUB, MT_Box);

    udata_define(L, MT_ButtonSUB, ButtonSUBMethods, ButtonSUBMetaMethods);
    udata_inherit(L, MT_ButtonSUB, MT_Button);

    udata_define(L, MT_GroupSUB, GroupSUBMethods, GroupSUBMetaMethods);
    udata_inherit(L, MT_GroupSUB, MT_Group);

    udata_define(L, MT_BrowserSUB, BrowserSUBMethods, BrowserSUBMetaMethods);
    udata_inherit(L, MT_BrowserSUB, MT_Browser);

#if 0
    udata_define(L, MT_File_BrowserSUB, File_BrowserSUBMethods, File_BrowserSUBMetaMethods);
    udata_inherit(L, MT_File_BrowserSUB, MT_File_Browser);

    udata_define(L, MT_Hold_BrowserSUB, Hold_BrowserSUBMethods, Hold_BrowserSUBMetaMethods);
    udata_inherit(L, MT_Hold_BrowserSUB, MT_Hold_Browser);

    udata_define(L, MT_Multi_BrowserSUB, Multi_BrowserSUBMethods, Multi_BrowserSUBMetaMethods);
    udata_inherit(L, MT_Multi_BrowserSUB, MT_Multi_Browser);

    udata_define(L, MT_Select_BrowserSUB, Select_BrowserSUBMethods, Select_BrowserSUBMetaMethods);
    udata_inherit(L, MT_Select_BrowserSUB, MT_Select_Browser);

    udata_define(L, MT_Check_BrowserSUB, Check_BrowserSUBMethods, Check_BrowserSUBMetaMethods);
    udata_inherit(L, MT_Check_BrowserSUB, MT_Check_Browser);
#endif

    udata_define(L, MT_TableSUB, TableSUBMethods, TableSUBMetaMethods);
    udata_inherit(L, MT_TableSUB, MT_Table);

    udata_define(L, MT_Table_RowSUB, Table_RowSUBMethods, Table_RowSUBMetaMethods);
    udata_inherit(L, MT_Table_RowSUB, MT_Table_Row);

    udata_define(L, MT_TabsSUB, TabsSUBMethods, TabsSUBMetaMethods);
    udata_inherit(L, MT_TabsSUB, MT_Tabs);

    udata_define(L, MT_WindowSUB, WindowSUBMethods, WindowSUBMetaMethods);
    udata_inherit(L, MT_WindowSUB, MT_Window);

    udata_define(L, MT_Double_WindowSUB, Double_WindowSUBMethods, Double_WindowSUBMetaMethods);
    udata_inherit(L, MT_Double_WindowSUB, MT_Double_Window);

    udata_define(L, MT_Overlay_WindowSUB, Overlay_WindowSUBMethods, 
                            Overlay_WindowSUBMetaMethods);
    udata_inherit(L, MT_Overlay_WindowSUB, MT_Overlay_Window);

#if USE_GL
    udata_define(L, MT_Gl_WindowSUB, Gl_WindowSUBMethods, Gl_WindowSUBMetaMethods);
    udata_inherit(L, MT_Gl_WindowSUB, MT_Gl_Window);
#endif

    udata_define(L, MT_Single_WindowSUB, Single_WindowSUBMethods, Single_WindowSUBMetaMethods);
    udata_inherit(L, MT_Single_WindowSUB, MT_Single_Window);

    udata_define(L, MT_Menu_WindowSUB, Menu_WindowSUBMethods, Menu_WindowSUBMetaMethods);
    udata_inherit(L, MT_Menu_WindowSUB, MT_Menu_Window);

    udata_define(L, MT_RollerSUB, RollerSUBMethods, RollerSUBMetaMethods);
    udata_inherit(L, MT_RollerSUB, MT_Roller);

    udata_define(L, MT_SliderSUB, SliderSUBMethods, SliderSUBMetaMethods);
    udata_inherit(L, MT_SliderSUB, MT_Slider);

    luaL_setfuncs(L, Functions, 0);
    }


