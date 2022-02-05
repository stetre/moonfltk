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

class Fl_TableEXPOSER : Fl_Table {
    public:
        Fl_Scrollbar *get_vscrollbar() { return Fl_Table::vscrollbar; }
        Fl_Scrollbar *get_hscrollbar() { return Fl_Table::hscrollbar; }
        // use select_xxx instead of current_xxx to get the "last clicked"-cell
        int get_current_row() const { return select_row; }
        int get_current_col() const { return select_col; }
        int find_cell(TableContext context, int R, int C, int &X, int &Y, int &W, int &H)
            { return Fl_Table::find_cell(context, R, C, X, Y, W, H); }
};

#define Vscrollbar(p) ((Fl_TableEXPOSER*)(p))->get_vscrollbar()
#define Hscrollbar(p) ((Fl_TableEXPOSER*)(p))->get_hscrollbar()

void tableclear(lua_State *L, Fl_Table *p)
/* removes and unreferences all children bound to Lua userdata */
    {
    int n = p->children();

    if(n<=0) 
        return;
    p->rows(0);
    p->cols(0);
    for(int i=n-1; i >= 0; --i)
        {
        Fl_Widget *c = p->child(i);
//      if((c == Vscrollbar(p)) || (c == Hscrollbar(p))) continue;
        if(userdata(c))
            {
    //      printf("removing child %d\n", i);
            ((Fl_Group*)p)->remove(i);
            userdata_unref(L, c);
            }
        }
    }

TABLE_NEW(Table, TableNew)
TABLE_DELETE(Table, TableDelete)

static int Tableclear(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    tableclear(L, p);
//  p->clear();
    return 0;
    }

GETSET_ENUM(Table, Boxtype, table_box, Tabletable_box)

GETSET_INT(Table, rows, Tablerows)
GETSET_INT(Table, cols, Tablecols)
  
static int Tablevisible_cells(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    int r1, r2, c1, c2;
    p->visible_cells(r1, r2, c1, c2);
    pushindex(L, r1);
    pushindex(L, r2);
    pushindex(L, c1);
    pushindex(L, c2);
    return 4;
    }

BOOLEAN_VOID(Table, is_interactive_resize, Tableis_interactive_resize)

GETSET_BOOLEAN(Table, row_resize, Tablerow_resize)
GETSET_BOOLEAN(Table, col_resize, Tablecol_resize)
  
GETSET_INT(Table, col_resize_min, Tablecol_resize_min)
GETSET_INT(Table, row_resize_min, Tablerow_resize_min)
  
GETSET_BOOLEAN(Table, row_header, Tablerow_header)
GETSET_BOOLEAN(Table, col_header, Tablecol_header)
  
GETSET_INT(Table, col_header_height, Tablecol_header_height)
GETSET_INT(Table, row_header_width, Tablerow_header_width)
  
GETSET_INTTYPE(Table, Color, row_header_color, Tablerow_header_color)
GETSET_INTTYPE(Table, Color, col_header_color, Tablecol_header_color)
  
GETSET_INT_INDEX(Table, row_height, Tablerow_height)
GETSET_INT_INDEX(Table, col_width, Tablecol_width)
  
VOID_INT(Table, row_height_all, Tablerow_height_all)
VOID_INT(Table, col_width_all, Tablecol_width_all)
  
GETSET_INDEX(Table, row_position, Tablerow_position)
GETSET_INDEX(Table, col_position, Tablecol_position)
  
INDEX_VOID(Table, callback_row, Tablecallback_row)
INDEX_VOID(Table, callback_col, Tablecallback_col)

static int Tablecallback_context(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    return push_TableContext(L, p->callback_context());
    }
 
BOOLEAN_INDEX_INDEX(Table, is_selected, Tableis_selected)

static int Tableget_selection(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    int row_top, col_left, row_bot, col_right;
    p->get_selection(row_top, col_left, row_bot, col_right);
    pushindex(L, row_top);
    pushindex(L, col_left);
    pushindex(L, row_bot);
    pushindex(L, col_right);
    return 4;
    }

static int Tableset_selection(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    p->set_selection(checkindex(L, 2),checkindex(L, 3),checkindex(L, 4),checkindex(L, 5));
    return 0;
    }

static int Tablemove_cursor(lua_State *L)
    {
	Fl_Table *p = check_Table(L, 1);
    int r = luaL_checkinteger(L, 2);
    int c = luaL_checkinteger(L, 3);
    int rc = 0;
    if(lua_isnone(L, 4))
        rc = p->move_cursor(r, c);
    else
        {
        int shiftselect = checkboolean(L, 4);
        rc = p->move_cursor(r, c, shiftselect);
        }   
    lua_pushboolean(L, rc);
    return 1;
    }
  
static int Tablecurrent_row(lua_State *L) 
    {
    Fl_TableEXPOSER *p = (Fl_TableEXPOSER*) check_Table(L, 1);
    int r = p->get_current_row();
    if (r >= 0) {
        pushindex(L, r);
        return 1;
    }
    return 0;
    }


static int Tablecurrent_col(lua_State *L) 
    {
    Fl_TableEXPOSER *p = (Fl_TableEXPOSER*) check_Table(L, 1);
    int c = p->get_current_col();
    if (c >= 0) {
        pushindex(L, c);
        return 1;
    }
    return 0;
    }

#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
GETSET_INT(Table, scrollbar_size, Tablescrollbar_size)
#endif

#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10303)
GETSET_BOOLEAN(Table, tab_cell_nav, Tabletab_cell_nav)
#endif

VOID_INT4(Table, resize, Tableresize)
VOID_VOID(Table, draw, Tabledraw)

static int Tablefind_cell(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    Fl_Table::TableContext context = check_TableContext(L, 2);
    int r = checkindex(L, 3);
    int c = checkindex(L, 4);
    int x, y, w, h;
    int rc = ((Fl_TableEXPOSER*)(p))->find_cell(context, r, c, x, y, w, h);
    if(rc == -1) /* out of range */
        return 0;   
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 4;
    }

VOID_VOID(Table, init_sizes, Tableinit_sizes)
VOID_VOID(Table, begin, Tablebegin)
VOID_VOID(Table, end, Tabledone)

INT_VOID(Table, children, Tablechildren)

static int Tableadd(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    Fl_Widget *w = check_Widget(L, 2);
    p->add(w);
    return 0;
    }


static int Tableinsert(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    Fl_Widget *c = check_Widget(L, 2);
    if(lua_isnumber(L, 3))
        p->insert(*c, checkindex(L, 3));
    else
        p->insert(*c, check_Widget(L, 3));
    return 0;
    }

static int Tableremove(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    Fl_Widget *w = check_Widget(L, 2);
    p->remove(*w);
    return 0;
    }

#if 0
// NO?  Fl_Widget * const *array()
static int Tablearray(lua_State *L) //@@ 
    {
    Fl_Table *p = check_Table(L, 1);
    (void)p;
    return 0;
    }
#endif

  
static int Tablechild(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    int n = checkindex(L, 2);
    if((n < 0) || (n >= p->children()))
        return luaL_argerror(L, 2, "out of range");
    Fl_Widget *c = p->child(n);
    if(!c) return 0;
    push_Widget(L, c);
    return 1;
    }


static int Tablefind(lua_State *L)
    {
    Fl_Table *p = check_Table(L, 1);
    Fl_Widget *c = test_Widget(L, 2);
    int index = p->find(c);
    pushindex(L, index);
    return 1;
    }



static const struct luaL_Reg TableMethods[] = 
    {
        { "callback_context", Tablecallback_context },
        { "callback_col", Tablecallback_col },
        { "callback_row", Tablecallback_row },
        { "col_header", Tablecol_header },
        { "col_header_color", Tablecol_header_color },
        { "col_header_height", Tablecol_header_height },
        { "col_position", Tablecol_position },
        { "col_resize", Tablecol_resize },
        { "col_resize_min", Tablecol_resize_min },
        { "col_width", Tablecol_width },
        { "col_width_all", Tablecol_width_all },
        { "cols", Tablecols },
        { "find_cell", Tablefind_cell },
        { "get_selection", Tableget_selection },
        { "is_interactive_resize", Tableis_interactive_resize },
        { "is_selected", Tableis_selected },
        { "move_cursor", Tablemove_cursor },
        { "current_row", Tablecurrent_row },
        { "current_col", Tablecurrent_col },
        { "row_header", Tablerow_header },
        { "row_header_color", Tablerow_header_color },
        { "row_header_width", Tablerow_header_width },
        { "row_height", Tablerow_height },
        { "row_height_all", Tablerow_height_all },
        { "row_position", Tablerow_position },
        { "row_resize", Tablerow_resize },
        { "row_resize_min", Tablerow_resize_min },
        { "rows", Tablerows },
#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
        { "scrollbar_size", Tablescrollbar_size },
#endif
        { "set_selection", Tableset_selection },
        { "table_box", Tabletable_box },
#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10303)
        { "tab_cell_nav", Tabletab_cell_nav },
#endif
        { "visible_cells", Tablevisible_cells },
        /* group or widget methods --------------------*/
        { "add", Tableadd },
//      { "array", Tablearray },
        { "begin", Tablebegin },
        { "child", Tablechild },
        { "children", Tablechildren },
        { "clear", Tableclear },
        { "draw", Tabledraw },
        { "done", Tabledone },
        { "find", Tablefind },
        { "init_sizes", Tableinit_sizes },
        { "insert", Tableinsert },
        { "remove", Tableremove },
        { "resize", Tableresize },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg TableMetaMethods[] = 
    {
        { "__gc",  TableDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Table_Row                                                                 |
 *------------------------------------------------------------------------------*/

TABLE_NEW(Table_Row, Table_RowNew)
TABLE_DELETE(Table_Row, Table_RowDelete)

static int Table_Rowclear(lua_State *L)
    {
    Fl_Table_Row *p = check_Table_Row(L, 1);
    tableclear(L, p);
//  p->clear();
    return 0;
    }

static int Table_Rowtype(lua_State *L) //WIDGET_TYPE
    {
    Fl_Table_Row *p = check_Table_Row(L, 1);
    if(lua_isnone(L, 2))
        {
        int t = p->type();
        lua_pushinteger(L, t);
        push_Table_RowRtti(L, p->type());
        return 2;
        }
    if(lua_type(L, 2) == LUA_TNUMBER)
        p->type((Fl_Table_Row::TableRowSelectMode)luaL_checkinteger(L, 2)); /* I hate C++ */
    else
        p->type(check_Table_RowRtti(L, 2));
    return 0;
    }

GETSET_INT(Table_Row, rows, Table_Rowrows)

static int Table_Rowrow_selected(lua_State *L)
    {
    Fl_Table_Row *p = check_Table_Row(L, 1);
    int rc = p->row_selected(checkindex(L, 2));
    switch(rc)
        {
        case -1:    lua_pushnil(L); 
                    lua_pushstring(L, "out of range");
                     return 2;
        case 1:     lua_pushboolean(L, 1); break;
        case 0:     lua_pushboolean(L, 0); break;
        default:
            return unexpected(L);
        }
    return 1;
    }

static int Table_Rowselect_all_rows(lua_State *L)
    {
    Fl_Table_Row *p = check_Table_Row(L, 1);
    const char *s = luaL_optstring(L, 2, "select");
    int flag = 1;
    if(strcmp(s, "deselect") == 0) flag = 0;
    else if(strcmp(s, "select") == 0) flag = 1;
    else if(strcmp(s, "toggle") == 0) flag = 2;
    else return luaL_argerror(L, 2, badvalue(L, s));
    p->select_all_rows(flag);
    return 0;
    }

static int Table_Rowselect_row(lua_State *L)
    {
    Fl_Table_Row *p = check_Table_Row(L, 1);
    int row = checkindex(L, 2);
    const char *s = luaL_optstring(L, 3, "select");
    int flag = 1;
    if(strcmp(s, "deselect") == 0) flag = 0;
    else if(strcmp(s, "select") == 0) flag = 1;
    else if(strcmp(s, "toggle") == 0) flag = 2;
    else return luaL_argerror(L, 3, badvalue(L, s));
    int rc = p->select_row(row, flag);
    switch(rc)
        {
        case -1:    lua_pushnil(L); lua_pushstring(L, "out of range"); return 2;
        case 1:     lua_pushboolean(L, 1); break; /* no change */
        case 0:     lua_pushboolean(L, 0); break; /* changed */
        default:
            return unexpected(L);
        }
    return 1;
    }
  
static const struct luaL_Reg Table_RowMethods[] = 
    {
        { "clear", Table_Rowclear },
        { "row_selected", Table_Rowrow_selected },
        { "rows", Table_Rowrows },
        { "select_all_rows", Table_Rowselect_all_rows },
        { "select_row", Table_Rowselect_row },
        { "type", Table_Rowtype },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Table_RowMetaMethods[] = 
    {
        { "__gc",  Table_RowDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "table",  TableNew },
        { "table_row",  Table_RowNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Table(lua_State *L)
    {
    udata_define(L, MT_Table, TableMethods, TableMetaMethods);
    udata_inherit(L, MT_Table, MT_Group);

    udata_define(L, MT_Table_Row, Table_RowMethods, Table_RowMetaMethods);
    udata_inherit(L, MT_Table_Row, MT_Table);

    luaL_setfuncs(L, Functions, 0);
    }




