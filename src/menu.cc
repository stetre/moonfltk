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
 | Test, check and push functions based on the item index                       |
 *------------------------------------------------------------------------------*/

#if 0
static int TestIndex(lua_State *L, Fl_Menu_ *menu, int arg)
/* Given the menu and the pathname at arg, returns the item's index or -1 on error */
    {
//  const char *pathname = luaL_optstring(L, arg, NULL); 
    const char *pathname = lua_isnoneornil(L, arg) ? NULL : luaL_checkstring(L, arg);
    if(pathname) 
        return menu->find_index(pathname);
    return -1;
    }
#endif

static int CheckIndex(lua_State *L, Fl_Menu_ *menu, int arg)
/* Given the menu and the pathname at arg, returns the item's index or raises an error */
    {
//  const char *pathname = luaL_optstring(L, arg, NULL); 
    const char *pathname = lua_isnoneornil(L, arg) ? NULL : luaL_checkstring(L, arg);
    if(pathname) 
        return menu->find_index(pathname);
    return luaL_argerror(L, arg, "cannot find item");
    }

static const char *PushIndex(lua_State *L, Fl_Menu_ *menu, int index)
/* Given the menu and the index, pushes the pathname (or nil) on the Lua stack */
    {
    if(index==-1)
        { lua_pushnil(L); return NULL; }
    Fl_Menu_Item *array = (Fl_Menu_Item*)menu->menu();
    return push_Pathname(L, menu, &array[index]);
    }

/*------------------------------------------------------------------------------*
 | Menu / menu item deletion                                                    |
 *------------------------------------------------------------------------------*/

static void UdDelete(lua_State *L, Fl_Menu_ *menu, Fl_Menu_Item *item)
/* Deletes the ud_t structure associated with the menu item (if any) */
    {
    ud_t *ud = (ud_t*)item->user_data();
    if(!ud) return;
    if(moonfltk_trace_objects)
        {
        const char *pathname =  push_Pathname(L, menu, item);
        printf("deleting Menu_Item '%s' %p\n", pathname, (void*)ud);
        }
    item->user_data(NULL);
    unreference(L, ud->cbref);
    unreference(L, ud->argref);
    free(ud);   
    }

#define MENU_NEW WIDGET_NEW
#define MENU_DELETE(T, Func)                            \
/* Generic menu destructor */                           \
static int Func(lua_State *L)                           \
    {                                                   \
    Fl_##T *menu = check_##T(L, 1);                     \
    DBG_DELETE(T, menu);                                \
    int n = menu->size();                               \
    Fl_Menu_Item *item = (Fl_Menu_Item*)menu->menu();   \
    for(int i=0; i<n; i++)                              \
        UdDelete(L, menu, &item[i]);                    \
    if(freeuserdata(L, menu)) delete menu;              \
    return 0;                                           \
    }

static int SubmenuUdDelete(lua_State *L, Fl_Menu_ *menu, Fl_Menu_Item *item)
/* if item is a sumenu, deletes the ud_t's of all the nested items 
 * (but not that of the item itself, if it has one) */
    {
    if(!item->submenu())
        return 0;
    int n = item->size(); 
    /* unfortunately, n is not the size of the sub menu, but the number of items
     * to the end of the menu...
     */
    int level = 0; /* keep track of inner submenus */
    item++; n--; /* skip the item itself (we may just want to clear it) */
    while(n--)
        {
        //DBG("menu item '%s', level = %d\n", push_Pathname(L, menu, item), level);
        if(item->label() == NULL)
            if(--level < 0) break;
        if(item->submenu()) level++;
        UdDelete(L, menu, item);
        item++;
        }
    return 0;
    }

static int ItemDelete(lua_State *L, Fl_Menu_ *menu, Fl_Menu_Item *item)
/* Deletes an item */
    {
    int index = menu->find_index(item);
    SubmenuUdDelete(L, menu, item);
    UdDelete(L, menu, item);
    menu->remove(index);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Callback                                                                     |
 *------------------------------------------------------------------------------*/

/* The C callback is the same for every menu item. It executes the item-specific
 * Lua callback (if it has one) which is stored on the Lua register at the
 * reference stored in the ud_t associated as user_data with the item.
 *
 * The callback is executed as: func(menu, pathname, argument)
 *
 * where (menu, pathname) identify the menu item and 'argument' is the optional
 * user argument (or 'user data'... to many of these around here...).
 */
static void CommonCallback(Fl_Widget *menu_, void *ud_)
    {
    lua_State *L = main_lua_state;
    if (!L) return;
    Fl_Menu_ *menu = (Fl_Menu_*)menu_;
    ud_t *ud = (ud_t*)ud_; /* note that this is the ud bound to the menu item, not to the menu */
    //printf("Menu callback: menu: %p, menu_item: %p\n", userdata(menu), ud);
    if(!ud)
        { unexpected(L); return; }
    if(ud->cbref == LUA_NOREF) 
        return; /* item has no callback */

    /* Retrieve the Lua callback from ud->cbref and execute it */
    if(pushvalue(L, ud->cbref) != LUA_TFUNCTION)
        { unexpected(L); return; }
    int nargs = 2;
    push_Menu_(L, menu);
    push_Pathname(L, menu, NULL); /* by passing NULL, menu->mvalue() will be used */
    if(ud->argref != LUA_NOREF)
        { pushvalue(L, ud->argref); nargs++; }
    if(lua_pcall(L, nargs, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }

/*------------------------------------------------------------------------------*
 | Fl_Menu_ (abstract class)                                                    |
 *------------------------------------------------------------------------------*/

static int Menu_add(lua_State *L)
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);

    if(test_Pathname(L, menu, 2))
        return luaL_argerror(L, 2, "item pathname in use");

    /* Now we can use the pathname ... */
    const char *pathname = luaL_checkstring(L, 2);

    /* Should we check that the shortcut is valid? Unfortunately the add() 
     * function has no mechanism to signal errors... */
    int shortcut = luaL_optinteger(L, 3, 0);

    bool setCallback = false;
    int cbref = LUA_NOREF;
    if(!lua_isnoneornil(L, 4))
        {
        if(!lua_isfunction(L,4))
            return luaL_argerror(L, 4, "function or nil expected");
        reference(L, cbref, 4);
        setCallback = true;
        }

    int argref = LUA_NOREF;
    if(!lua_isnoneornil(L, 5))
        reference(L, argref, 5);

    int flags = check_Menu_ItemFlags(L, 6); 

    ud_t *ud = (ud_t*)malloc(sizeof(ud_t));
    if(!ud)
        return luaL_error(L, "cannot allocate memory");
    memset(ud, 0, sizeof(ud_t));
    ud->cbref = cbref;
    ud->argref = argref;
    
    int index = menu->add(pathname, shortcut, setCallback ? CommonCallback : NULL, setCallback ? ud : NULL, flags);
    if(moonfltk_trace_objects) 
        printf("added Menu_Item '%s' (index=%d) %p\n", pathname, index, (void*)ud);
    if(index==0) menu->value(0);
/*    lua_pushstring(L, pathname); */
    lua_pushinteger(L, index);
    return 1;
    }

int menuclear(lua_State *L, Fl_Menu_ *menu)
    {
    /* delete all ud_t before clearing the menu */
    int n = menu->size();
    Fl_Menu_Item *item = (Fl_Menu_Item*)menu->menu();
    if (n > 0)
        {
        for(int i=0; i<n-1; i++) // -1 to avoid calling UdDelete on the terminiator item. See: https://www.fltk.org/doc-1.3/classFl__Menu__.html#a56d014765ec71a15ecca109348180f45 for more information.
            UdDelete(L, menu, item+i);
        }
    return 0;
    }

static int Menu_clear(lua_State *L)
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    menuclear(L, menu);
    menu->clear();
    return 0;
    }

static int Menu_clear_submenu(lua_State *L)
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    Fl_Menu_Item *item = check_Pathname(L, menu, 2);
    if(!item->submenu())
        return luaL_error(L, "item is not a submenu");
    int index = menu->find_index(item);
    SubmenuUdDelete(L, menu, item);
    menu->clear_submenu(index);
    return 0;
    }

static int Menu_remove(lua_State *L)
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    Fl_Menu_Item *item = check_Pathname(L, menu, 2);
    return ItemDelete(L, menu, item);
    }

GETSET_ENUM(Menu_, Boxtype, down_box, Menu_down_box)
GETSET_ENUM(Menu_, Font, textfont, Menu_textfont)
GETSET_INTTYPE(Menu_, Fontsize, textsize, Menu_textsize)
GETSET_INTTYPE(Menu_, Color, textcolor, Menu_textcolor)
VOID_VOID(Menu_, global, Menu_global)

static int Menu_replace(lua_State *L)
/* replace(pathname, newlabel)
 */
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    int index = CheckIndex(L, menu, 2);
    const char* label = luaL_checkstring(L, 3);
    menu->replace(index, label);
    return 0;
    }

static int Menu_shortcut(lua_State *L)
/* shortcut(pathname, newshortcut)
 */
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    int index = CheckIndex(L, menu, 2);
    int shortcut = luaL_checkinteger(L, 3);
    menu->shortcut(index, shortcut);
    return 0;
    }

static int Menu_text(lua_State *L)
/* text(pathname) -> label
 * text()         -> label of the last picked item
 */
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    const char *text;
    if(lua_isnone(L, 2))
        text = menu->text();
    else
        {
        int index = CheckIndex(L, menu, 2);
        text = menu->text(index);
        }
    if(!text) return 0;
    lua_pushstring(L, text);
    return 1;
    }

static int Menu_value(lua_State *L)
/* pathname = value()
 * changed = value(pathname)
 */
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    if(lua_isnone(L, 2))
        {
        PushIndex(L, menu, menu->value());
        return 1;
        }
    int index = CheckIndex(L, menu, 2);
    lua_pushboolean(L, menu->value(index));
    return 1;
    }

static int Menu_size(lua_State *L)
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushinteger(L, menu->size());
        return 1;
        }
    /* Fl_Widget::size() */
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    menu->size(w,h);
    return 0;
    }

static int Menu_mvalue(lua_State *L)
/* pathname = mvalue()   (last picked)
 */
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    push_Pathname(L, menu, NULL);
    return 1;
    }

static int Menu_test_shortcut(lua_State *L)
/* pathname = test_shortcut() 
 */
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    Fl_Menu_Item *item = (Fl_Menu_Item *)menu->test_shortcut();
    if(!item) return 0;
    push_Pathname(L, menu, item);
    return 1;
    }

static int Menu_mode(lua_State *L)
/* flag1, ... = value()
 * mode(flag1, ...)
 */
    {
    Fl_Menu_ *menu = check_Menu_(L, 1);
    int index = CheckIndex(L, menu, 2);
    if(lua_isnone(L, 3))
        return push_Menu_ItemFlags(L, menu->mode(index));
    int flags = push_Menu_ItemFlags(L, 3);
    menu->mode(index, flags);
    return 0;
    }

static const struct luaL_Reg Menu_Methods[] = 
    {
        { "add",  Menu_add},
        { "clear", Menu_clear },
        { "clear_submenu", Menu_clear_submenu },
        { "down_box", Menu_down_box },
        { "global", Menu_global },
        { "mode", Menu_mode },
        { "mvalue", Menu_mvalue  },
        { "remove", Menu_remove },
        { "replace", Menu_replace },
        { "shortcut", Menu_shortcut },
        { "size", Menu_size },
        { "test_shortcut", Menu_test_shortcut },
        { "text", Menu_text },
        { "textfont", Menu_textfont },
        { "textsize", Menu_textsize },
        { "textcolor", Menu_textcolor },
        { "value", Menu_value  },
        { NULL, NULL } /* sentinel */
    };

#define Menu_MetaMethods NULL

#if(0)
// NOT EXPOSED Fl_Menu_ METHODS
// ------------------------------------------------------------------------------------
// used only internally by MoonFLTK:
int item_pathname(char *name, int namelen, const Fl_Menu_Item *finditem=0) const;
int find_index(const char *name) const;
int find_index(const Fl_Menu_Item *item) const;
int find_index(Fl_Callback *cb) const;
const Fl_Menu_Item* find_item(const char *name);
const Fl_Menu_Item* find_item(Fl_Callback*);
const Fl_Menu_Item *menu() const
// no static menus:
void menu(const Fl_Menu_Item *m);
// use add() instead of insert() (redundant):
int insert(int index, const char* a, const char* b, Fl_Callback* c, void* d = 0, int e = 0) 
int insert(int index, const char*, int shortcut, Fl_Callback*, void* = 0, int = 0);
// meaningful for static menus, which are not used in MoonFLTK:
void copy(const Fl_Menu_Item *m, void* user_data = 0); 
// 'string' shortcuts (e.g. "^a" for Ctrl-a) are not supported by MoonFLTK (redundant):
int  add(const char* a, const char* b, Fl_Callback* c, void* d = 0, int e = 0) 
//@@ servono?
const Fl_Menu_Item* picked(const Fl_Menu_Item*);
int  add(const char *);

#endif

/*------------------------------------------------------------------------------*
 | Fl_Choice                                                                    |
 *------------------------------------------------------------------------------*/

MENU_NEW(Choice, ChoiceNew)

int flcommon_Choice(lua_State *L);
static int Choice(lua_State *L)
    { return (lua_type(L, 1) == LUA_TSTRING) ? flcommon_Choice(L) : ChoiceNew(L); }

MENU_DELETE(Choice, ChoiceDelete)

static int Choice_value(lua_State *L)
/* pathname = value()
 * changed = value(pathname)
 */
    {
    Fl_Choice *menu = check_Choice(L, 1);
    if(lua_isnone(L, 2))
        {
        PushIndex(L, menu, menu->value());
        return 1;
        }
    int index = CheckIndex(L, menu, 2);
    lua_pushboolean(L, menu->value(index));
    return 1;
    }

static const struct luaL_Reg ChoiceMethods[] = 
    {
        { "value", Choice_value  },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg ChoiceMetaMethods[] = 
    {
        { "__gc",  ChoiceDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Fl_Menu_Bar                                                                  |
 *------------------------------------------------------------------------------*/

MENU_NEW(Menu_Bar, Menu_BarNew)
MENU_DELETE(Menu_Bar, Menu_BarDelete)

#define Menu_BarMethods NULL

static const struct luaL_Reg Menu_BarMetaMethods[] = 
    {
        { "__gc",  Menu_BarDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Menu_Button                                                               |
 *------------------------------------------------------------------------------*/

MENU_NEW(Menu_Button, Menu_ButtonNew)
MENU_DELETE(Menu_Button, Menu_ButtonDelete)
WIDGET_TYPE(Menu_Button, Menu_Buttontype)

static int Menu_Buttonpopup(lua_State *L)
/* pathname = popup() 
 */
    {
    Fl_Menu_Button *menu = check_Menu_Button(L, 1);
    Fl_Menu_Item *item = (Fl_Menu_Item *)menu->popup();
    if(!item) return 0; /* the user dismissed the popup menu... */
    push_Pathname(L, menu, item);
    return 1;
    }


static const struct luaL_Reg Menu_ButtonMethods[] = 
    {
        { "popup", Menu_Buttonpopup },
        { "type", Menu_Buttontype },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Menu_ButtonMetaMethods[] = 
    {
        { "__gc",  Menu_ButtonDelete },
        { NULL, NULL } /* sentinel */
    };



/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Menu_Functions[] = 
    {
        { "choice",  Choice },
        { "menu_bar",  Menu_BarNew },
        { "menu_button",  Menu_ButtonNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Menu(lua_State *L)
    {
    udata_define(L, MT_Menu_, Menu_Methods, Menu_MetaMethods);
    udata_inherit(L, MT_Menu_, MT_Widget);

    moonfltk_open_Menu_Item(L);

    udata_define(L, MT_Choice, ChoiceMethods, ChoiceMetaMethods);
    udata_inherit(L, MT_Choice, MT_Menu_);

    udata_define(L, MT_Menu_Bar, Menu_BarMethods, Menu_BarMetaMethods);
    udata_inherit(L, MT_Menu_Bar, MT_Menu_);

    udata_define(L, MT_Menu_Button, Menu_ButtonMethods, Menu_ButtonMetaMethods);
    udata_inherit(L, MT_Menu_Button, MT_Menu_);

    luaL_setfuncs(L, Menu_Functions, 0);
    }

