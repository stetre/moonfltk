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

/* On the MoonFLTK/script interface, a menu item is identified (and referred to)
 * by the combination of the menu and the pathname. Indices are not used.
 *
 * Menu items are created dynamically via the menu_:add() method, so an item
 * is always associated with a menu.
 *
 * There is no "moonfltk_menu_item" object type. Instead, the Fl_Menu_Item methods
 * are exposed as methods of the "moonfltk_menu_" type. They are renamed with an
 * "item_" prefix and accept the pathname as first argument.
 * (e.g. the Fl_Menu_Item color(c) method becomes menu_:item_color(pathname, c)).
 *
 * Similarly, FLTK menu methods returning/accepting an index map to MoonFLTK
 * menu methods returning/accepting a pathname.
 */

/*------------------------------------------------------------------------------*
 | Fl_Menu_Item test/check/push functions                                       |
 *------------------------------------------------------------------------------*/

#define MAX_PATHNAME_LEN 256
const char *push_Pathname(lua_State *L, Fl_Menu_ *menu, Fl_Menu_Item *item)
 /* if item=NULL, menu->mvalue() is used */
    {
    char pathname[MAX_PATHNAME_LEN+1];
    int rc = menu->item_pathname(pathname, MAX_PATHNAME_LEN, item);
    switch(rc)
        {
        case 0: break; /* ok */
        case -1: luaL_error(L, "cannot find menu item"); return NULL;
        case -2:  unexpected(L); return NULL; /* item name too long */
        default: unexpected(L); return NULL;
        }
    return lua_pushstring(L, pathname[0] == '/' ? pathname + 1 : pathname); /* strip leading '/'*/
    }

Fl_Menu_Item *test_Pathname(lua_State *L, Fl_Menu_ *menu, int arg)
    {
    const char *pathname = luaL_optstring(L, arg, NULL);
    if(!pathname) return NULL;
    return (Fl_Menu_Item*)menu->find_item(pathname);
    }

Fl_Menu_Item * check_Pathname(lua_State *L, Fl_Menu_ *menu, int arg)
    {
    Fl_Menu_Item *item = test_Pathname(L, menu, arg);
    if(!item)
        { luaL_argerror(L, arg, "cannot find item"); return NULL; }
    return item;
    }

/*------------------------------------------------------------------------------*
 | Fl_Menu_Item methods                                                         |
 *------------------------------------------------------------------------------*/


#define BEGIN                                                               \
    int arg = 1;                                                            \
    Fl_Menu_ *menu = check_Menu_(L, arg++);                                 \
    Fl_Menu_Item *item = check_Pathname(L, menu, arg++);                

static int Label(lua_State *L)
    {
    BEGIN
    if(lua_isnone(L, arg))
        {
        const char *text = item->label();
        if(!text) return 0;
        lua_pushstring(L, text);
        return 1;
        }
    if(lua_isnone(L, arg+1))
        {
        Fl_Labeltype labeltype = check_Labeltype(L, arg++);
        item->label(labeltype, luaL_checkstring(L, arg));
        return 0;
        }
    item->label(luaL_checkstring(L, arg));
    return 0;
    }

#define Menu_ItemGETSET_ENUM(V, method, Func)   \
static int Func(lua_State *L)                   \
    {                                           \
    BEGIN                                       \
    if(lua_isnone(L, arg))                      \
        {                                       \
        push_##V(L, item->method());            \
        return 1;                               \
        }                                       \
    item->method(check_##V(L, arg));            \
    return 0;                                   \
    }

#define Menu_ItemGETSET_INT(method, Func)       \
static int Func(lua_State *L)                   \
    {                                           \
    BEGIN                                       \
    if(lua_isnone(L, arg))                      \
        {                                       \
        lua_pushinteger(L, item->method());     \
        return 1;                               \
        }                                       \
    item->method(luaL_checkinteger(L, arg));    \
    return 0;                                   \
    }

#define Menu_ItemVOID_VOID(method, Func)        \
static int Func(lua_State *L)                   \
    {                                           \
    BEGIN                                       \
    item->method();                             \
    return 0;                                   \
    }

#define Menu_ItemBOOLEAN_VOID(method, Func)     \
static int Func(lua_State *L)                   \
    {                                           \
    BEGIN                                       \
    lua_pushboolean(L, item->method());         \
    return 1;                                   \
    }

#define Menu_ItemINT_VOID(method, Func)         \
static int Func(lua_State *L)                   \
    {                                           \
    BEGIN                                       \
    lua_pushinteger(L, item->method());         \
    return 1;                                   \
    }


Menu_ItemGETSET_ENUM(Labeltype, labeltype, Labeltype)
Menu_ItemGETSET_ENUM(Color, labelcolor, Labelcolor)
Menu_ItemGETSET_ENUM(Font, labelfont, Labelfont)
Menu_ItemGETSET_ENUM(Fontsize, labelsize, Labelsize)
Menu_ItemGETSET_INT(shortcut, Shortcut)
Menu_ItemVOID_VOID(set, Set)
Menu_ItemVOID_VOID(clear, Clear)
Menu_ItemVOID_VOID(setonly, Setonly)
Menu_ItemBOOLEAN_VOID(submenu, Submenu)
Menu_ItemBOOLEAN_VOID(checkbox, Checkbox)
Menu_ItemBOOLEAN_VOID(radio, Radio)
Menu_ItemBOOLEAN_VOID(value, Value)
Menu_ItemBOOLEAN_VOID(visible, Visible )
Menu_ItemVOID_VOID(show, Show)
Menu_ItemVOID_VOID(hide, Hide)
Menu_ItemBOOLEAN_VOID(active, Active)
Menu_ItemVOID_VOID(activate, Activate)
Menu_ItemVOID_VOID(deactivate, Deactivate)
Menu_ItemBOOLEAN_VOID(activevisible, Activevisible)
Menu_ItemINT_VOID(size, Size)

static int Callback(lua_State *L)
    {
    BEGIN
    ud_t *ud = (ud_t*)item->user_data();
    /* ud may be NULL (item may be a separator or a (sub)menu) */
    if(lua_isnone(L, arg)) /* get */
        {
        if(!ud) return 0;
        pushvalue(L, ud->cbref);
        return 1;
        }
    if(!ud) 
        return luaL_error(L, "operation not permitted on this item");
    if(!lua_isfunction(L, arg))
        return luaL_argerror(L, arg, "function expected");
    unreference(L, ud->cbref);  /* release old value */
    reference(L, ud->cbref, arg); /*save new one */
    return 0;
    }


static int Argument(lua_State *L)
    {
    BEGIN
    ud_t *ud = (ud_t*)item->user_data();
    if(lua_isnone(L, arg)) /* get */
        {
        if(!ud) return 0;
        pushvalue(L, ud->argref);
        return 1;
        }
    if(!ud) 
        return luaL_error(L, "operation not permitted on this item");
    unreference(L, ud->argref); /* release old value */
    reference(L, ud->argref, arg); /*save new one */
    return 0;
    }

static int Image(lua_State *L)
    {
    BEGIN
    item->image(check_Image(L, arg));
    return 0;
    }

static int Flags(lua_State *L)
    {
    BEGIN
    return push_Menu_ItemFlags(L, item->flags);
    }


static const struct luaL_Reg Menu_ItemMethods[] = 
    {
        { "item_activate", Activate },
        { "item_active", Active },
        { "item_activevisible", Activevisible },
        { "item_argument", Argument },
        { "item_callback", Callback },
        { "item_checkbox", Checkbox },
        { "item_clear", Clear },
        { "item_deactivate", Deactivate },
        { "item_flags", Flags },
        { "item_hide", Hide },
        { "item_image", Image },
        { "item_label", Label },
        { "item_labelcolor", Labelcolor },
        { "item_labelfont", Labelfont },
        { "item_labelsize", Labelsize },
        { "item_labeltype", Labeltype },
        { "item_radio", Radio },
        { "item_set", Set },
        { "item_setonly", Setonly },
        { "item_shortcut", Shortcut },
        { "item_show", Show },
        { "item_size", Size },
        { "item_submenu", Submenu },
        { "item_value", Value },
        { "item_visible", Visible },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_Menu_Item(lua_State *L)
    {
    /* Add methods to the 'Menu_' type */
    udata_addmethods(L, MT_Menu_, Menu_ItemMethods);
    }


#if(0)
// NOT EXPOSED Fl_Menu_Item METHODS
// ------------------------------------------------------------------------------

// advance N items, skipping submenus:
const Fl_Menu_Item *next(int=1) const;
Fl_Menu_Item *next(int i=1);
const Fl_Menu_Item *first() const { return next(0); }
Fl_Menu_Item *first() { return next(0); }

// used by menubar:
int measure(int* h, const Fl_Menu_*) const;
void draw(int x, int y, int w, int h, const Fl_Menu_*, int t=0) const;

//@@TODO?
// popup menus without using an Fl_Menu_ widget:
const Fl_Menu_Item* popup(int X, int Y, const char *title = 0,  const Fl_Menu_Item* picked=0,
                        const Fl_Menu_* = 0) const;
const Fl_Menu_Item* pulldown(int X, int Y, int W, int H, const Fl_Menu_Item* picked = 0,
                const Fl_Menu_* = 0, const Fl_Menu_Item* title = 0, int menubar=0) const;
const Fl_Menu_Item* test_shortcut() const;
const Fl_Menu_Item* find_shortcut(int *ip=0, const bool require_alt = false) const;

void do_callback(Fl_Widget* o) const {callback_(o, user_data_);}
void do_callback(Fl_Widget* o,void* arg) const {callback_(o, arg);}
void do_callback(Fl_Widget* o,long arg) const {callback_(o, (void*)arg);}

int insert(int,const char*,int,Fl_Callback*,void* =0, int =0);
int add(const char*, int shortcut, Fl_Callback*, void* =0, int = 0);
int add(const char*a, const char* b, Fl_Callback* c, void* d = 0, int e = 0) 

#endif
