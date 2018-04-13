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

void browserclear(lua_State *L, Fl_Browser_ *p)
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
 | Fl_Browser_ (abstract class)                                                 |
 *------------------------------------------------------------------------------*/

WIDGET_TYPE(Browser_, Browser_type)
GETSET_INT(Browser_, position, Browser_position)
GETSET_INT(Browser_, hposition, Browser_hposition)
GETSET_ENUM(Browser_, Font, textfont, Browser_textfont)
GETSET_INTTYPE(Browser_, Fontsize, textsize, Browser_textsize)
GETSET_INTTYPE(Browser_, Color, textcolor, Browser_textcolor)
GETSET_INT(Browser_, scrollbar_size, Browser_scrollbar_size)
VOID_VOID(Browser_, scrollbar_right, Browser_scrollbar_right)
VOID_VOID(Browser_, scrollbar_left, Browser_scrollbar_left)
GETSET_ENUM(Browser_, ScrollbarsMode, has_scrollbar, Browser_has_scrollbar)

static int Browser_sort(lua_State *L)
    {
    Fl_Browser_ *p = check_Browser_(L, 1);
    p->sort(check_SortFlag(L, 2));
    return 0;
    }

#if 0
static int Browser_scrollbar(lua_State *L)
    {
    Fl_Browser_ *p = check_Browser_(L, 1);
    push_Scrollbar(L, &p->scrollbar);
    return 1;
    }

static int Browser_hscrollbar(lua_State *L)
    {
    Fl_Browser_ *p = check_Browser_(L, 1);
    push_Scrollbar(L, &p->hscrollbar);
    return 1;
    }
#endif

static int Browser_deselect(lua_State *L)
    {
    Fl_Browser_ *p = check_Browser_(L, 1);
    int docallbacks = optboolean(L, 2, 0);
    lua_pushboolean(L, p->deselect(docallbacks));
    return 1;
    }

static const struct luaL_Reg Browser_Methods[] = 
    {
        { "deselect", Browser_deselect },
        { "has_scrollbar", Browser_has_scrollbar },
        { "hposition", Browser_hposition },
        { "position", Browser_position },
        { "scrollbar_left", Browser_scrollbar_left },
        { "scrollbar_right", Browser_scrollbar_right },
        { "scrollbar_size", Browser_scrollbar_size },
        { "sort", Browser_sort },
        { "textcolor", Browser_textcolor },
        { "textfont", Browser_textfont },
        { "textsize", Browser_textsize },
        { "type", Browser_type },
        { NULL, NULL } /* sentinel */
    };

#define Browser_MetaMethods NULL

#if(0)
// NOT EXPOSED Fl_Browser_ METHODS 
// ------------------------------------------------------------------------
// farle nelle sottoclassi, specifiche per tipo di item:
int select(void *item,int val=1,int docallbacks=0);
int select_only(void *item,int docallbacks=0);
void display(void *item); // scroll so this item is shown
#endif


/*------------------------------------------------------------------------------*
 | Fl_Browser                                                                   |
 *------------------------------------------------------------------------------*/

BROWSER_NEW(Browser, BrowserNew)
BROWSER_DELETE(Browser, BrowserDelete)

VOID_INT(Browser, remove, Browserremove)

static int Browserclear(lua_State *L)
    {
    Fl_Browser *p = check_Browser(L, 1);
    browserclear(L, p);
    p->clear();
    return 0;
    }

static int Browseradd(lua_State *L)
    {
    Fl_Browser *p = check_Browser(L, 1);
    const char *newtext = luaL_optstring(L, 2, NULL);
    intptr_t data = luaL_optinteger(L, 3, 0);
    p->add(newtext, (void*)data);
    return 0;
    }

static int Browserinsert(lua_State *L)
    {
    Fl_Browser *p = check_Browser(L, 1);
    int line = luaL_checkinteger(L, 2);
    const char *newtext = luaL_optstring(L, 3, NULL);
    intptr_t data = luaL_optinteger(L, 4, 0);
    p->insert(line, newtext, (void*)data);
    return 0;
    }

static int Browserdata(lua_State *L)
    {
    Fl_Browser *p = check_Browser(L, 1);
    int line = luaL_checkinteger(L, 2);
    if(lua_isnone(L, 3))
        {
        lua_pushinteger(L, (intptr_t)p->data(line));
        return 1;
        }
    intptr_t data = luaL_checkinteger(L, 3);
    p->data(line, (void*)data);
    return 0;
    }

VOID_INT_INT(Browser, move, Browsermove)
VOID_INT_INT(Browser, swap, Browserswap)

static int Browsersize(lua_State *L)
    {
    Fl_Browser *p = check_Browser(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushinteger(L, p->size());
        return 1;
        }
    /* Fl_Widget::size() */
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    p->size(w,h);
    return 0;
    }

#include <errno.h>
static int Browserload(lua_State *L)
/* ok, err = load(filename) */
    {
    Fl_Browser *p = check_Browser(L, 1);
    const char *filename = luaL_checkstring(L, 2);
    if(!p->load(filename))
        {
        int errnum = errno;
        lua_pushboolean(L, 0);
        lua_pushstring(L, strerror(errnum));
        return 2;
        }
    lua_pushboolean(L, 1);
    return 1;
    }

GETSET_ASCII(Browser, format_char, Browserformat_char)
GETSET_ASCII(Browser, column_char, Browsercolumn_char)

static int Browserlineposition(lua_State *L)
    {
    Fl_Browser *p = check_Browser(L, 1);
    p->lineposition(luaL_checkinteger(L, 2), check_Line_Position(L, 3));
    return 0;
    }

GETSET_INT(Browser, topline, Browsertopline)
VOID_INT(Browser, bottomline, Browserbottomline)
VOID_INT(Browser, middleline, Browsermiddleline)

GETSET_INT(Browser, value, Browservalue)


static int Browsericon(lua_State *L)
    {
    Fl_Browser *p = check_Browser(L, 1);
    int line = luaL_checkinteger(L, 2);
    if(lua_isnone(L, 3))
        {
        Fl_Image *v = p->icon(line);
        if(!v) return 0;
        push_Image(L, v);
        return 1;
        }
    p->icon(line, check_Image(L, 3));
    return 0;
    }

VOID_INT(Browser, remove_icon, Browserremove_icon)

static int Browsertext(lua_State *L)
    {
    Fl_Browser *p = check_Browser(L, 1);
    int line = luaL_checkinteger(L, 2);
    if(lua_isnone(L, 3))
        {
        const char *text = p->text(line);
        if(!text) return 0;
        lua_pushstring(L, text);
        return 1;
        }
    p->text(line, luaL_optstring(L, 3, NULL));
    return 0;
    }

static int Browserselect(lua_State *L)
    {
    Fl_Browser *p = check_Browser(L, 1);
    int line = luaL_checkinteger(L, 2);
    if(lua_isnone(L, 3))
        {
        lua_pushboolean(L, p->select(line));
        return 1;
        }
    lua_pushboolean(L, p->select(line, checkboolean(L, 3)));
    return 1;
    }


BOOLEAN_INT(Browser, selected, Browserselected)
BOOLEAN_INT(Browser, displayed, Browserdisplayed)
BOOLEAN_INT(Browser, visible, Browservisible)
VOID_INT(Browser, make_visible, Browsermake_visible)

#define F(method, Func)                         \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_Browser *p = check_Browser(L, 1);        \
    if(lua_isnone(L, 2))                    \
        { p->method(); return 0; }              \
    p->method(luaL_checkinteger(L, 2));         \
    return 0;                                   \
    }   
F(show, Browsershow)
F(hide, Browserhide)

#undef F

static int Browsercolumn_widths(lua_State *L)
    {
    int n, v;
    Fl_Browser *p = check_Browser(L, 1);
    if(lua_isnone(L, 2))
        {
        const int *arr = p->column_widths();
        if(!arr) return 0;
        n = 0;
        while(arr[n])
            lua_pushinteger(L, arr[n++]);
        return n;
        }
    int arg = 2;
    while(!lua_isnone(L, arg))
        {
        v = luaL_checkinteger(L, arg);
        if(v<=0)
            return luaL_error(L, "invalid width (must be positive)");
        arg++;
        }   
    n = arg - 2;
    int *arr = (int*)malloc((n+1) * sizeof(int*));
    if(!arr)
        return luaL_error(L, "cannot allocate memory");
    arg = 2;
    for(int i=0; i < n; i++)
        arr[i] = lua_tointeger(L, arg++);
    arr[n] = 0;
    p->column_widths(arr);
    free(arr);
    return 0;
    }


static const struct luaL_Reg BrowserMethods[] = 
    {
        { "add", Browseradd },
        { "bottomline", Browserbottomline },
        { "clear", Browserclear },
        { "column_char", Browsercolumn_char },
        { "column_widths", Browsercolumn_widths },
        { "displayed", Browserdisplayed },
        { "data", Browserdata },
        { "format_char", Browserformat_char },
        { "hide", Browserhide },
        { "icon", Browsericon },
        { "insert", Browserinsert },
        { "lineposition", Browserlineposition },
        { "load", Browserload },
        { "make_visible", Browsermake_visible },
        { "middleline", Browsermiddleline },
        { "move", Browsermove },
        { "remove", Browserremove },
        { "remove_icon", Browserremove_icon },
        { "select", Browserselect },
        { "selected", Browserselected },
        { "show", Browsershow },
        { "swap", Browserswap },
        { "size", Browsersize },
        { "text", Browsertext },
        { "topline", Browsertopline },
        { "visible", Browservisible },
        { "value", Browservalue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg BrowserMetaMethods[] = 
    {
        { "__gc",  BrowserDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | File_Browser                                                                 |
 *------------------------------------------------------------------------------*/

BROWSER_NEW(File_Browser, File_BrowserNew)
BROWSER_DELETE(File_Browser, File_BrowserDelete)
GETSET_INT(File_Browser, iconsize, File_Browsericon_size)
GETSET_INTTYPE(File_Browser, Fontsize, textsize, File_Browsertextsize)
GETSET_ENUM(File_Browser, FileType, filetype, File_Browserfiletype)

static int File_Browserfilter(lua_State *L)
    {
    Fl_File_Browser *p = check_File_Browser(L, 1);
    if(lua_isnone(L, 2))
        {
        const char *pattern = p->filter();
        if(!pattern) return 0;
        lua_pushstring(L, pattern);
        return 1;
        }
    p->filter(luaL_checkstring(L, 2));
    return 0;
    }

static int File_Browserload(lua_State *L)
    {
    Fl_File_Browser *p = check_File_Browser(L, 1);
    const char *directory = luaL_checkstring(L, 2);
    if(lua_isnone(L, 3))
        lua_pushboolean(L, p->load(directory));
    else
        lua_pushboolean(L, p->load(directory, check_File_Sort_F(L, 3)));
    return 1;
    }

static const struct luaL_Reg File_BrowserMethods[] = 
    {
        { "filetype", File_Browserfiletype },
        { "filter", File_Browserfilter },
        { "iconsize", File_Browsericon_size },
        { "load", File_Browserload },
        { "textsize", File_Browsertextsize },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg File_BrowserMetaMethods[] = 
    {
        { "__gc",  File_BrowserDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Hold_Browser                                                                 |
 *------------------------------------------------------------------------------*/

BROWSER_NEW(Hold_Browser, Hold_BrowserNew)
BROWSER_DELETE(Hold_Browser, Hold_BrowserDelete)

#define Hold_BrowserMethods NULL

static const struct luaL_Reg Hold_BrowserMetaMethods[] = 
    {
        { "__gc",  Hold_BrowserDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Multi_Browser                                                                |
 *------------------------------------------------------------------------------*/

BROWSER_NEW(Multi_Browser, Multi_BrowserNew)
BROWSER_DELETE(Multi_Browser, Multi_BrowserDelete)

#define Multi_BrowserMethods NULL

static const struct luaL_Reg Multi_BrowserMetaMethods[] = 
    {
        { "__gc",  Multi_BrowserDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Select_Browser                                                               |
 *------------------------------------------------------------------------------*/

BROWSER_NEW(Select_Browser, Select_BrowserNew)
BROWSER_DELETE(Select_Browser, Select_BrowserDelete)

#define Select_BrowserMethods NULL

static const struct luaL_Reg Select_BrowserMetaMethods[] = 
    {
        { "__gc",  Select_BrowserDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Check_Browser                                                                |
 *------------------------------------------------------------------------------*/

BROWSER_NEW(Check_Browser, Check_BrowserNew)
BROWSER_DELETE(Check_Browser, Check_BrowserDelete)

static int Check_Browseradd(lua_State *L)
/* nitems = add(str [, checked])
 */
    {
    Fl_Check_Browser *p = check_Check_Browser(L, 1);
    const char *str = luaL_optstring(L, 2, NULL);
    if(lua_isnone(L, 3))
        lua_pushinteger(L, p->add(str));
    else
        lua_pushinteger(L, p->add(str, checkboolean(L, 3)));
    return 1;
    }

static int Check_Browsertext(lua_State *L)
    {
    Fl_Check_Browser *p = check_Check_Browser(L, 1);
    const char*text = p->text(luaL_checkinteger(L, 2));
    if(!text)
        return 0;
    lua_pushstring(L, text);
    return 1;
    }

static int Check_Browserclear(lua_State *L)
    {
    Fl_Check_Browser *p = check_Check_Browser(L, 1);
    browserclear(L, p);
    p->clear();
    return 0;
    }

INT_INT(Check_Browser, remove, Check_Browserremove)
GETSET_BOOLEAN_INT(Check_Browser, checked, Check_Browserchecked )
VOID_INT(Check_Browser, set_checked, Check_Browserset_checked )
INT_VOID(Check_Browser, nitems, Check_Browsernitems)
INT_VOID(Check_Browser, nchecked, Check_Browsernchecked )
INT_VOID(Check_Browser, value, Check_Browservalue )
VOID_VOID(Check_Browser, check_all, Check_Browsercheck_all)
VOID_VOID(Check_Browser, check_none, Check_Browsercheck_none)

static const struct luaL_Reg Check_BrowserMethods[] = 
    {
        { "add", Check_Browseradd  },
        { "check_all", Check_Browsercheck_all },
        { "check_none", Check_Browsercheck_none },
        { "checked", Check_Browserchecked },
        { "clear", Check_Browserclear  },
        { "nchecked", Check_Browsernchecked },
        { "nitems", Check_Browsernitems },
        { "remove", Check_Browserremove },
        { "set_checked", Check_Browserset_checked },
        { "text", Check_Browsertext },
        { "value", Check_Browservalue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Check_BrowserMetaMethods[] = 
    {
        { "__gc",  Check_BrowserDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
    //  { "browser_",  Browser_New }, abstract class
        { "browser",  BrowserNew },
        { "file_browser",  File_BrowserNew },
        { "hold_browser",  Hold_BrowserNew },
        { "multi_browser",  Multi_BrowserNew },
        { "select_browser",  Select_BrowserNew },
        { "check_browser",  Check_BrowserNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Browser(lua_State *L)
    {
    udata_define(L, MT_Browser_, Browser_Methods, Browser_MetaMethods);
    udata_inherit(L, MT_Browser_, MT_Group);

    udata_define(L, MT_Browser, BrowserMethods, BrowserMetaMethods);
    udata_inherit(L, MT_Browser, MT_Browser_);

    udata_define(L, MT_File_Browser, File_BrowserMethods, File_BrowserMetaMethods);
    udata_inherit(L, MT_File_Browser, MT_Browser);

    udata_define(L, MT_Hold_Browser, Hold_BrowserMethods, Hold_BrowserMetaMethods);
    udata_inherit(L, MT_Hold_Browser, MT_Browser);

    udata_define(L, MT_Multi_Browser, Multi_BrowserMethods, Multi_BrowserMetaMethods);
    udata_inherit(L, MT_Multi_Browser, MT_Browser);

    udata_define(L, MT_Select_Browser, Select_BrowserMethods, Select_BrowserMetaMethods);
    udata_inherit(L, MT_Select_Browser, MT_Browser);

    udata_define(L, MT_Check_Browser, Check_BrowserMethods, Check_BrowserMetaMethods);
    udata_inherit(L, MT_Check_Browser, MT_Browser_);

    luaL_setfuncs(L, Functions, 0);
    }

