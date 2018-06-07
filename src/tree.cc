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

void treeclear(lua_State *L, Fl_Tree *p)
/* removes and unreferences all children bound to Lua userdata, skips scrollbars */
    {
    int n = p->children();
    if(n<=0) 
        return;
    for(int i=n-1; i >= 0; --i)
        {
        Fl_Widget *c = p->child(i);
        /* skip scrollbars */
        if(p->is_scrollbar(c))
            continue;
        if(userdata(c))
            {
            ((Fl_Group*)p)->remove(i);
            userdata_unref(L, c);
            }
        }
    }


/*------------------------------------------------------------------------------*
 | Fl_Tree_Item test/check/push functions                                       |
 *------------------------------------------------------------------------------*/
/* The Fl_Tree_Item class is not exposed to Lua. 
 * Items are identified by their full pathname and the menu they are in.
 * The methods of the Fl_Tree_Item class are exposed as methods of the fl.tree
 * object (e.g.  Fl_Menu_Item::labelsize(sz) becomes tree:item_labelsize(path, sz))
 */
#define MAX_PATHNAME_LEN 256
static const char *push_Tree_Item(lua_State *L, Fl_Tree *tree, Fl_Tree_Item *item)
/* this actually pushes the full pathname of the item */
    {
    char path[MAX_PATHNAME_LEN+1];
    int sr = tree->showroot(); /* save old showroot() value */
    tree->showroot(1); /* we want the full pathname */
    if(tree->item_pathname(path, MAX_PATHNAME_LEN, item) !=0)
        return NULL; 
    tree->showroot(sr); /* restore */
    return lua_pushstring(L, path);
    }

static Fl_Tree_Item *test_Tree_Item(lua_State *L, Fl_Tree *tree, int arg)
    {
    const char *path = luaL_optstring(L, arg, NULL);
    if(!path) return NULL;
    return (Fl_Tree_Item*)tree->find_item(path);
    }

static Fl_Tree_Item * check_Tree_Item(lua_State *L, Fl_Tree *tree, int arg)
    {
    Fl_Tree_Item *item = test_Tree_Item(L, tree, arg);
    if(!item)
        { luaL_argerror(L, arg, "cannot find tree item"); return NULL; }
    return item;
    }

/*------------------------------------------------------------------------------*
 | Fl_Tree                                                                      |
 *------------------------------------------------------------------------------*/

TREE_NEW(Tree, TreeNew)
TREE_DELETE(Tree, TreeDelete)
WIDGET_TYPE(Tree, Treetype)


/* Note: clear() and clear_children() clear tree items, not children widgets of the
 * tree as an Fl_Group.
 * This is confusing: the tree has children items (nodes of the tree), but being 
 * an Fl_Group it has children in the sense of widgets (e.g. the scrollbars, or
 * any other widget added between begin() and end/done()).
 *
 */
static int Treeclear(lua_State *L) /* clear(), clear_children(path) */
    {
    Fl_Tree *p = check_Tree(L, 1);
    if(lua_isnone(L, 2)) /* clear() */
        {
        p->clear();
        return 0;
        }
    /* clear_children(item) */
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    p->clear_children(item);    
    return 0;
    }

#if 0 /* Tree as a Group */
        { "group_clear", Treegroup_clear },
static int Treegroup_clear(lua_State *L) //@@ 
    {
    Fl_Tree *p = check_Tree(L, 1);
    (void)p;
    return 0;
    }

        { "group_remove", Tree },
static int Treegroup_remove(lua_State *L) //@@ 
    {
    Fl_Tree *p = check_Tree(L, 1);
    (void)p;
    return 0;
    }
#endif

BOOLEAN_EVENT(Tree, handle, Treehandle)
VOID_VOID(Tree, draw, Treedraw)
VOID_VOID(Tree, show_self, Treeshow_self)
VOID_INT4(Tree, resize, Treeresize)

VOID_STRING(Tree, root_label, Treeroot_label)

GETSET_INT(Tree, vposition, Treevposition)
GETSET_INT(Tree, hposition, Treehposition)

GETSET_INT(Tree, scrollbar_size, Treescrollbar_size)
BOOLEAN_VOID(Tree, is_vscroll_visible, Treeis_vscroll_visible)
BOOLEAN_VOID(Tree, is_hscroll_visible, Treeis_hscroll_visible)

GETSET_INTTYPE(Tree, Color, connectorcolor, Treeconnectorcolor)

GETSET_ENUM(Tree, Tree_Connector, connectorstyle, Treeconnectorstyle)

GETSET_ENUM(Tree, Tree_Sort, sortorder, Treesortorder)
GETSET_ENUM(Tree, Boxtype, selectbox, Treeselectbox)
GETSET_ENUM(Tree, Tree_Select, selectmode, Treeselectmode)

GETSET_INT(Tree, marginleft, Treemarginleft)
GETSET_INT(Tree, margintop, Treemargintop)

#define F(method, Func)             \
static int Func(lua_State *L)       \
    {                               \
    Fl_Tree *p = check_Tree(L, 1);  \
    if(lua_isnone(L, 2))            \
        {                           \
        Fl_Image *im = p->method(); \
        if(!im) return 0;           \
        push_Image(L, im);          \
        return 1;                   \
        }                           \
    p->method(check_Image(L, 2));   \
    return 0;                       \
    }

F(usericon, Treeusericon)
F(openicon, Treeopenicon)
F(closeicon, Treecloseicon)
#undef F

#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
GETSET_INT(Tree, marginbottom, Treemarginbottom)
GETSET_INT(Tree, widgetmarginleft, Treewidgetmarginleft)
#endif /*FLTK_ABI_VERSION*/

#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10303)
#endif /*FLTK_ABI_VERSION*/

static int Treeadd(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    const char *path = luaL_checkstring(L, 2);
    if(p->add(path) == NULL)
        return luaL_error(L, "cannot create tree item '%s'", path);
    return 0;
    }

static int Treeinsert_above(lua_State *L)
/* path = insert_above(abovepath, name) */
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    const char *name = luaL_checkstring(L, 3);
    item = p->insert_above(item, name);
    if(!item)
        return luaL_error(L, "cannot insert tree item '%s' above item '%s'", 
            name, lua_tostring(L, 2));
    push_Tree_Item(L, p, item);
    return 1;
    }

static int Treeinsert(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    const char *name = luaL_checkstring(L, 3);
    int pos = checkindex(L, 4);
    if((pos < 0) || (pos > item->children()))
        return luaL_argerror(L, 4, "out of range");
    item = p->insert(item, name, pos);
    if(!item)
        return luaL_error(L, "cannot insert tree item");
    // '%s' as child of '%s' at pos=%d", name, lua_tostring(L, 2), pos+1);
    push_Tree_Item(L, p, item);
    return 1;
    }

static int Treeremove(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    if(p->remove(item) != 0)
        return unexpected(L);
    return 0;
    }

GETSET_INT(Tree, linespacing, Treelinespacing)
GETSET_INT(Tree, openchild_marginbottom, Treeopenchild_marginbottom)
GETSET_INT(Tree, usericonmarginleft, Treeusericonmarginleft)
GETSET_INT(Tree, labelmarginleft, Treelabelmarginleft)
GETSET_INT(Tree, connectorwidth, Treeconnectorwidth)
GETSET_BOOLEAN(Tree, showcollapse, Treeshowcollapse)
GETSET_BOOLEAN(Tree, showroot, Treeshowroot)

static int Treeopen(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    int docallback = optboolean(L, 3, 1);
    lua_pushboolean(L, p->open(item, docallback));
    return 1;
    }

static int Treeopen_toggle(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    int docallback = optboolean(L, 3, 1);
    p->open_toggle(item, docallback);
    return 0;
    }

static int Treeclose(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    int docallback = optboolean(L, 3, 1);
    lua_pushboolean(L, p->close(item, docallback));
    return 1;
    }

static int Treeis_open(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    lua_pushboolean(L, p->is_open(item));
    return 1;
    }

static int Treeis_close(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    lua_pushboolean(L, p->is_close(item));
    return 1;
    }

static int Treeselect(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    int docallback = optboolean(L, 3, 1);
    lua_pushboolean(L, p->select(item, docallback));
    return 1;
    }


static int Treeselect_toggle(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    int docallback = optboolean(L, 3, 1);
    p->select_toggle(item, docallback);
    return 0;
    }


static int Treedeselect(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    int docallback = optboolean(L, 3, 1);
    lua_pushboolean(L, p->deselect(item, docallback));
    return 1;
    }


static int Treeselect_all(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    int docallback = optboolean(L, 3, 1);
    lua_pushboolean(L, p->select_all(item, docallback));
    return 1;
    }

static int Treedeselect_all(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    int docallback = optboolean(L, 3, 1);
    lua_pushboolean(L, p->deselect_all(item, docallback));
    return 1;
    }


static int Treeselect_only(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    int docallback = optboolean(L, 3, 1);
    lua_pushboolean(L, p->select_only(item, docallback));
    return 1;
    }


static int Treeextend_selection_(lua_State *L, int dir)
/* extend_selection_<dir>(from, to [, select(def)/deselect/toggle, visible = false]) */
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *from = check_Tree_Item(L, p, 2);
    Fl_Tree_Item *to = check_Tree_Item(L, p, 3);
    const char* s = luaL_optstring(L, 4, "select");
    int val;
    if(strcmp(s, "deselect")==0) val = 0;
    else if(strcmp(s, "select")==0) val = 1;
    else if(strcmp(s, "toggle")==0) val = 2;
    else return luaL_argerror(L, 4, badvalue(L,s));
    int visible = optboolean(L, 5, 0);
    int n = p->extend_selection_dir(from, to, dir, val, (bool)visible);
    lua_pushinteger(L, n);
    return 1;
    }

static int Treeextend_selection_up(lua_State *L)
    { return Treeextend_selection_(L, FL_Down); }
static int Treeextend_selection_down(lua_State *L)
    { return Treeextend_selection_(L, FL_Up); }
  
static int Treeset_item_focus(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    p->set_item_focus(item);
    return 0;
    }


static int Treeget_item_focus(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = p->get_item_focus();
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


static int Treeis_selected(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    lua_pushboolean(L, p->is_selected(item));
    return 1;
    }


#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
//  Fl_Tree_Item_Reselect_Mode item_reselect_mode() const;
//  void item_reselect_mode(Fl_Tree_Item_Reselect_Mode mode);
static int Treeitem_reselect_mode(lua_State *L) //@@ 
    {
    Fl_Tree *p = check_Tree(L, 1);
    (void)p;
    return 0;
    }


//  Fl_Tree_Item_Draw_Mode item_draw_mode() const;
//  void item_draw_mode(Fl_Tree_Item_Draw_Mode mode);
//  void item_draw_mode(int mode);
static int Treeitem_draw_mode(lua_State *L) //@@ 
    {
    Fl_Tree *p = check_Tree(L, 1);
    (void)p;
    return 0;
    }

#endif

#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10303)
VOID_VOID(Tree, calc_dimensions, Treecalc_dimensions)
VOID_VOID(Tree, calc_tree, Treecalc_tree)
VOID_VOID(Tree, recalc_tree, Treerecalc_tree)
#endif

 
static int Treedisplay(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    p->display(item);
    return 0;
    }


static int Treedisplayed(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    lua_pushboolean(L, p->displayed(item));
    return 1;
    }


static int Treeshow_item(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    if(lua_isnone(L, 3))
        p->show_item(item);
    else
        p->show_item(item, luaL_checkinteger(L, 3)); /* yoffset */
    return 0;
    }

static int Treeshow_item_top(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    p->show_item_top(item);
    return 0;
    }


static int Treeshow_item_middle(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    p->show_item_middle(item);
    return 0;
    }


static int Treeshow_item_bottom(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    p->show_item_bottom(item);
    return 0;
    }

static int Treeroot(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = p->root();
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


static int Treefirst(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = p->first();
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


static int Treefirst_visible_item(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = p->first_visible_item();
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


static int Treenext(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    item = p->next(item);
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


static int Treeprev(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    item = p->prev(item);
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


static int Treelast(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = p->last();
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


static int Treelast_visible_item(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = p->last_visible_item();
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


#if 0 //defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10303)
//  Fl_Tree_Item *next_visible_item(Fl_Tree_Item *start, int dir);  // made public in 1.3.3 ABI
static int Treenext_visible_item(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    (void)p;
    return 0;
    }

#endif

static int Treefirst_selected_item(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = p->first_selected_item();
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


static int Treelast_selected_item(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = p->last_selected_item();
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }


//  Fl_Tree_Item *next_item(Fl_Tree_Item *item, int dir=FL_Down, bool visible=false);
static int Treenext_item_(lua_State *L, int dir, bool visible)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
//  int visible = optboolean(L, 3, 0);
    item = p->next_item(item, dir, (bool)visible);
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }

static int Treenext_visible_item(lua_State *L)
    { return Treenext_item_(L, FL_Down, 1); } 
    
static int Treeprev_visible_item(lua_State *L)
    { return Treenext_item_(L, FL_Up, 1); } 


#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10303)
//  int get_selected_items(Fl_Tree_Item_Array &items);
static int Treeget_selected_items(lua_State *L) //@@ 
    {
    Fl_Tree *p = check_Tree(L, 1);
    (void)p;
    return 0;
    }
#endif

static int Treenext_selected_item_(lua_State *L, int dir)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = test_Tree_Item(L, p, 2);
    item = p->next_selected_item(item, dir);
    if(!item) return 0;
    push_Tree_Item(L, p, item);
    return 1;
    }

static int Treenext_selected_item(lua_State *L)
    { return Treenext_selected_item_(L, FL_Down); } 
    
static int Treeprev_selected_item(lua_State *L)
    { return Treenext_selected_item_(L, FL_Up); } 



/*------------------------------------------------------------------------------*
 | Callback                                                                     |
 *------------------------------------------------------------------------------*/

static void Callback(Fl_Widget *p_, void *not_used)
/* This is the regular widget callback, but for trees, instead of
 * executing it as func(tree, arg), we execute it as
 * func(tree, item, reason, arg)
 */
    {
#define p ((Fl_Tree*)p_) /* dirty downcast */
    lua_State *L = main_lua_state;
    if (!L) 
        return;
    (void)not_used;
    int nargs = 3; 
    ud_t *ud = userdata(p);
    if( p->callback() != Callback )
        return;
    if(pushvalue(L, ud->cbref) != LUA_TFUNCTION)
        { unexpected(L); return; }
    push_Tree(L, p);
    Fl_Tree_Item *item = p->callback_item();
    if(!item)
        { unexpected(L); return; }
    push_Tree_Item(L, p, item);
    push_Tree_Reason(L, p->callback_reason());
    if(ud->argref != LUA_NOREF)
        { pushvalue(L, ud->argref); nargs++; }
    if(lua_pcall(L, nargs, 0, 0) != LUA_OK)
        { lua_error(L); return; }
#undef p
    }

#define SetUserData(L, ud, arg) do {                        \
    unreference((L), (ud)->argref);                         \
    if(!lua_isnoneornil((L), (arg)))                        \
        reference((L), (ud)->argref, (arg));                \
} while(0)

static int Treecallback(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
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
    p->callback(Callback, (void*)NULL);
    return 0;
    }

#if 0
static int Treedo_callback(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    ud_t *ud = userdata(p);
    if(!lua_isnone(L, 2))
        SetUserData(L, ud, 2);
    p->do_callback(p);
    return 0;
    }
#endif

static int Treeargument(lua_State *L) 
    {
    Fl_Tree *p = check_Tree(L, 1);
    ud_t *ud = userdata(p);
    if(lua_isnone(L, 2))
        {
        pushvalue(L, ud->argref);
        return 1;
        }
    SetUserData(L, ud, 2);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Fl_Tree_Item's methods                                                       |
 *------------------------------------------------------------------------------*/

#define F(method, Func) /* VOID_ITEM */             \
static int Func(lua_State *L)                       \
    {                                               \
    Fl_Tree *p = check_Tree(L, 1);                  \
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);  \
    item->method();                                 \
    return 0;                                       \
    }
F(activate, Treeitem_activate)
F(deactivate, Treeitem_deactivate)
#undef F


#define F(method, Func) /* INT_ITEM */              \
static int Func(lua_State *L)                       \
    {                                               \
    Fl_Tree *p = check_Tree(L, 1);                  \
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);  \
    lua_pushinteger(L, item->method());             \
    return 1;                                       \
    }

F(children, Treeitem_children)
F(x, Treeitem_x)
F(y, Treeitem_y)
F(w, Treeitem_w)
F(h, Treeitem_h)
F(label_x, Treeitem_label_x)
F(label_y, Treeitem_label_y)
F(label_w, Treeitem_label_w)
F(label_h, Treeitem_label_h)
F(depth, Treeitem_depth)
#undef F

#define F(method, Func) /* BOOLEAN_ITEM */          \
static int Func(lua_State *L)                       \
    {                                               \
    Fl_Tree *p = check_Tree(L, 1);                  \
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);  \
    lua_pushboolean(L, item->method());             \
    return 1;                                       \
    }

F(has_children, Treeitem_has_children)
F(is_root, Treeitem_is_root)
F(is_activated, Treeitem_is_activated)
F(is_active, Treeitem_is_active)
F(visible, Treeitem_visible)
F(is_visible, Treeitem_is_visible)
F(visible_r, Treeitem_visible_r)
#undef F


#define F(V, method, Func)  /* GETSET_ITEM_ENUM   */\
static int Func(lua_State *L)                       \
    {                                               \
    Fl_Tree *tree = check_Tree(L, 1);               \
    Fl_Tree_Item *item = test_Tree_Item(L, tree, 2);\
    if(lua_isnil(L, 2)) /* Fl_Tree method */        \
        {                                           \
        if(lua_isnone(L, 3))                        \
            {                                       \
            push_##V(L, tree->item_##method());     \
            return 1;                               \
            }                                       \
        tree->item_##method( check_##V(L, 3));      \
        }                                           \
    else /* Fl_Tree_Item method */                  \
        {                                           \
        if(lua_isnone(L, 3))                        \
            {                                       \
            push_##V(L, item->method());            \
            return 1;                               \
            }                                       \
        item->method( check_##V(L, 3));             \
        }                                           \
    return 0;                                       \
    }

F(Font, labelfont, Treeitem_labelfont) 
F(Fontsize, labelsize, Treeitem_labelsize) 
F(Color, labelfgcolor, Treeitem_labelfgcolor) 
F(Color, labelbgcolor, Treeitem_labelbgcolor) 

#undef F

static int Treeitem_label(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    if(lua_isnone(L, 3))
        {
        const char *l = item->label();
        if(!l) return 0;
        lua_pushstring(L, l);
        return 1;
        }
    item->label(luaL_checkstring(L, 3));
    return 0;
    }


static int Treeitem_widget(lua_State *L)
    { 
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    if(lua_isnone(L, 3))
        {
        Fl_Widget *w = item->widget();
        if(!w) return 0;
        push_Widget(L, w);
        return 1;
        }
    item->widget(check_Widget(L, 3));
    return 0;
    }


static int Treeitem_usericon(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    if(lua_isnone(L, 3))
        {
        Fl_Image *img = item->usericon();
        if(!img) return 0;
        push_Image(L, img);
        return 1;
        }
    item->usericon(test_Image(L, 3));
    return 0;
    }

static int Treeitem_parent(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    Fl_Tree_Item *parent = item->parent();
    if(!parent)
        return 0;
    push_Tree_Item(L, p, parent);
    return 1;
    }

static int Treeitem_show_self(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    const char *indent = luaL_optstring(L, 3, "");
    item->show_self(indent);
    return 0;
    }

static int Treeitem_find_child(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    const char *name = luaL_checkstring(L, 3);
    int index = item->find_child(name);
    if(index == -1)
        return 0;
    pushindex(L, index);
    return 1;
    }

static int Treeitem_child(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    int pos = checkindex(L, 3);
    if((pos < 0) || (pos > item->children()))
        return luaL_argerror(L, 3, "out of range");
    item = item->child(pos);
    if(!item)
        return 0; /* unexpected, actually .. */
    push_Tree_Item(L, p, item);
    return 1;
    }

static int Treeitem_swap_children(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    const char *a = luaL_checkstring(L, 3);
    const char *b = luaL_checkstring(L, 4);
    int ax = item->find_child(a);
    if(ax == -1)
        return luaL_argerror(L, 3, "no such child");
    int bx = item->find_child(b);
    if(bx == -1)
        return luaL_argerror(L, 4, "no such child");
    item->swap_children(ax, bx);
    return 0;
    }

static int Treeitem_user_data(lua_State *L)
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    if(lua_isnone(L, 3))
        {
        void* val = item->user_data();
        if(!val)
            return 0;
        lua_pushinteger(L, (intptr_t)val);
        return 1;
        }
    intptr_t val = luaL_checkinteger(L, 3);
    item->user_data((void*)val);
    return 0;
    }


#if 0 /* 8 yy */
GETSET_INT(Tree, item_, Treeitem_)
GETSET_BOOLEAN(Tree, item_, Treeitem_)
VOID_VOID(Tree, item_, Treeitem_)
BOOLEAN_VOID(Tree, item_, Treeitem_)
BOOLEAN_INT(Tree, item_, Treeitem_)
F(, Treeitem_)
        { "item_", Treeitem_ },
static int Treeitem_(lua_State *L) //@@ 
    {
    Fl_Tree *p = check_Tree(L, 1);
    Fl_Tree_Item *item = check_Tree_Item(L, p, 2);
    (void)p;
    return 0;
    }

#endif



/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg TreeMethods[] = 
    {
        { "add", Treeadd },
        { "clear", Treeclear },
        { "closeicon", Treecloseicon },
        { "connectorcolor", Treeconnectorcolor },
        { "connectorstyle", Treeconnectorstyle },
        { "connectorwidth", Treeconnectorwidth },
        { "draw", Treedraw },
        { "extend_selection_up", Treeextend_selection_up },
        { "extend_selection_down", Treeextend_selection_down },
        { "first", Treefirst },
        { "first_selected_item", Treefirst_selected_item },
        { "first_visible_item", Treefirst_visible_item },
        { "get_item_focus", Treeget_item_focus },
        { "handle", Treehandle },
        { "hposition", Treehposition },
        { "insert_above", Treeinsert_above },
        { "insert", Treeinsert },
        { "is_hscroll_visible", Treeis_hscroll_visible },
        { "is_vscroll_visible", Treeis_vscroll_visible },
        { "labelmarginleft", Treelabelmarginleft },
        { "last", Treelast },
        { "last_selected_item", Treelast_selected_item },
        { "last_visible_item", Treelast_visible_item },
        { "linespacing", Treelinespacing },
        { "marginleft", Treemarginleft },
        { "margintop", Treemargintop },
        { "next", Treenext },
        { "next_selected_item", Treenext_selected_item },
        { "next_visible_item", Treenext_visible_item },
        { "openicon", Treeopenicon },
        { "openchild_marginbottom", Treeopenchild_marginbottom },
        { "prev", Treeprev },
        { "prev_selected_item", Treeprev_selected_item },
        { "prev_visible_item", Treeprev_visible_item },
        { "remove", Treeremove },
        { "resize", Treeresize },
        { "root_label", Treeroot_label },
        { "root", Treeroot },
        { "scrollbar_size", Treescrollbar_size },
        { "show_item", Treeshow_item },
        { "show_item_bottom", Treeshow_item_bottom },
        { "show_item_middle", Treeshow_item_middle },
        { "show_item_top", Treeshow_item_top },
        { "selectbox", Treeselectbox },
        { "selectmode", Treeselectmode },
        { "set_item_focus", Treeset_item_focus },
        { "showcollapse", Treeshowcollapse },
        { "showroot", Treeshowroot },
        { "show_self", Treeshow_self },
        { "sortorder", Treesortorder },
        { "usericon", Treeusericon },
        { "usericonmarginleft", Treeusericonmarginleft },
        { "vposition", Treevposition },
#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
        { "item_draw_mode", Treeitem_draw_mode },
        { "item_reselect_mode", Treeitem_reselect_mode },
        { "marginbottom", Treemarginbottom },
        { "widgetmarginleft", Treewidgetmarginleft },
#endif /*FLTK_ABI_VERSION*/
#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10303)
        { "calc_dimensions", Treecalc_dimensions },
        { "calc_tree", Treecalc_tree },
        { "get_selected_items", Treeget_selected_items },
//      { "next_visible_item", Treenext_visible_item }, redundant
        { "recalc_tree", Treerecalc_tree },
#endif /*FLTK_ABI_VERSION*/
/* Fl_Tree_Item methods ----------------------------------------*/
        { "item_activate", Treeitem_activate },
        { "item_child", Treeitem_child },
        { "item_children", Treeitem_children },
//      { "item_clicked", Treeitem_clicked }, DEPRECATED
        { "item_close", Treeclose },
        { "item_deactivate", Treeitem_deactivate },
        { "item_depth", Treeitem_depth },
        { "item_deselect_all", Treedeselect_all },
        { "item_deselect", Treedeselect },
        { "item_display", Treedisplay },
        { "item_displayed", Treedisplayed },
        { "item_find_child", Treeitem_find_child },
        { "item_h", Treeitem_h },
        { "item_has_children", Treeitem_has_children },
        { "item_is_active", Treeitem_is_active },
        { "item_is_activated", Treeitem_is_activated },
        { "item_is_close", Treeis_close },
        { "item_is_open", Treeis_open },
        { "item_is_root", Treeitem_is_root },
        { "item_is_selected", Treeis_selected },
        { "item_is_visible", Treeitem_is_visible },
        { "item_label", Treeitem_label },
        { "item_label_h", Treeitem_label_h },
        { "item_label_w", Treeitem_label_w },
        { "item_label_x", Treeitem_label_x },
        { "item_label_y", Treeitem_label_y },
        { "item_labelbgcolor", Treeitem_labelbgcolor },
        { "item_labelcolor", Treeitem_labelfgcolor }, /* alias */
        { "item_labelfgcolor", Treeitem_labelfgcolor },
        { "item_labelfont", Treeitem_labelfont },
        { "item_labelsize", Treeitem_labelsize },
        { "item_open_toggle", Treeopen_toggle },
        { "item_open", Treeopen },
        { "item_parent", Treeitem_parent },
        { "item_show_self", Treeitem_show_self },
        { "item_select", Treeselect },
        { "item_select_all", Treeselect_all },
        { "item_select_only", Treeselect_only },
        { "item_select_toggle", Treeselect_toggle },
        { "item_swap_children", Treeitem_swap_children },
        { "item_user_data", Treeitem_user_data },
        { "item_usericon", Treeitem_usericon },
        { "item_visible", Treeitem_visible },
        { "item_visible_r", Treeitem_visible_r },
        { "item_w", Treeitem_w },
        { "item_widget", Treeitem_widget },
        { "item_x", Treeitem_x },
        { "item_y", Treeitem_y },
/* widget:callback methods ---------------------------------------*/
        { "callback", Treecallback },
//      { "do_callback", Treedo_callback },
        { "argument", Treeargument },
/* widget:type ---------------------------------------------------*/
        { "type", Treetype },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg TreeMetaMethods[] = 
    {
        { "__gc",  TreeDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg TreeFunctions[] = 
    {
        { "tree",  TreeNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Tree(lua_State *L)
    {
    udata_define(L, MT_Tree, TreeMethods, TreeMetaMethods);
    udata_inherit(L, MT_Tree, MT_Group);
    luaL_setfuncs(L, TreeFunctions, 0);
    }



#if(0) /* NOT IMPLEMENTED/USED/EXPOSED STUFF */
// Fl_Tree /////////////////////////////////////////////////////////////////////
// used internally by MoonFLTK: ------------------------------------------------
int is_scrollbar(Fl_Widget *w); 
Fl_Tree_Item *find_item(const char *path);
const Fl_Tree_Item *find_item(const char *path) const;
  int item_pathname(char *pathname, int pathnamelen, const Fl_Tree_Item *item) const;
  void callback_item(Fl_Tree_Item* item);
  Fl_Tree_Item* callback_item();
  void callback_reason(Fl_Tree_Reason reason);
  Fl_Tree_Reason callback_reason() const;
// not used: ---------------------------------------------------------
void load(class Fl_Preferences&);
const Fl_Tree_Prefs& prefs() const { return _prefs; }
Fl_Tree_Item *add(const char *path, Fl_Tree_Item *newitem=0);
Fl_Tree_Item *first_visible();      // deprecated in ABI 10303
Fl_Tree_Item *last_visible();       // deprecated in ABI 10303
const Fl_Tree_Item* find_clicked(int yonly=0) const;
Fl_Tree_Item* find_clicked(int yonly=0);

// Fl_Tree_Item ////////////////////////////////////////////////////////////////

//@@

#if FLTK_ABI_VERSION >= 10303
Fl_Tree_Item *replace(Fl_Tree_Item *new_item);
Fl_Tree_Item *replace_child(Fl_Tree_Item *olditem, Fl_Tree_Item *newitem);
#endif
int move(int to, int from);
int move(Fl_Tree_Item *item, int op=0, int pos=0);
int move_above(Fl_Tree_Item *item);
int move_below(Fl_Tree_Item *item);
int move_into(Fl_Tree_Item *item, int pos=0);

Fl_Tree_Item *prev();
Fl_Tree_Item *next();
Fl_Tree_Item *next_sibling();
Fl_Tree_Item *prev_sibling();
void update_prev_next(int index);
Fl_Tree_Item *next_visible(Fl_Tree_Prefs &prefs);
Fl_Tree_Item *prev_visible(Fl_Tree_Prefs &prefs);
  
#if FLTK_ABI_VERSION >= 10303
const Fl_Tree_Prefs& prefs() const;
const Fl_Tree *tree() const {
#endif

int event_on_collapse_icon(const Fl_Tree_Prefs &prefs) const;
int event_on_label(const Fl_Tree_Prefs &prefs) const;




// Not supported: (item subclassing) ---------------------------------
virtual int draw_item_content(int render);
void draw(int X, int &Y, int W, Fl_Tree_Item *itemfocus, int &tree_item_xmax, int lastchild=1, int render=1);
void draw(int X, int &Y, int W, Fl_Widget *tree, Fl_Tree_Item *itemfocus, const Fl_Tree_Prefs &prefs, int lastchild=1);
// not used: (redundant) ---------------------------------------------
Fl_Tree_Item *find_child_item(const char *name);
Fl_Tree_Item *find_child_item(char **arr);
Fl_Tree_Item *find_item(char **arr);
Fl_Tree_Item* deparent(int index);
int reparent(Fl_Tree_Item *newchild, int index);
int find_child(Fl_Tree_Item *item);
int swap_children(Fl_Tree_Item *a, Fl_Tree_Item *b);
Fl_Tree_Item *add(const Fl_Tree_Prefs &prefs, const char *new_label, Fl_Tree_Item *newitem);
Fl_Tree_Item *add(const Fl_Tree_Prefs &prefs, const char *new_label);
Fl_Tree_Item *add(const Fl_Tree_Prefs &prefs, char **arr, Fl_Tree_Item *newitem);
Fl_Tree_Item *add(const Fl_Tree_Prefs &prefs, char **arr);
Fl_Tree_Item *insert(const Fl_Tree_Prefs &prefs, const char *new_label, int pos=0);
Fl_Tree_Item *insert_above(const Fl_Tree_Prefs &prefs, const char *new_label);
int remove_child(Fl_Tree_Item *item);
int remove_child(const char *new_label);
void clear_children();
void open();
void close();
int is_open() const;
int is_close() const;
void open_toggle();
void select(int val=1);
void select_toggle();
int select_all();
void deselect();
int deselect_all();
char is_selected() const;
const Fl_Tree_Item* find_clicked(const Fl_Tree_Prefs &prefs, int yonly=0) const;
Fl_Tree_Item* find_clicked(const Fl_Tree_Prefs &prefs, int yonly=0);
void root(Fl_Tree_Item *newitem);
// For internal use only: ---------------------------------------------
void parent(Fl_Tree_Item *val)
#endif

