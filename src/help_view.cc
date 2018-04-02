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

#if 0
HELP_VIEW_NEW(Help_View, Help_ViewNew)
HELP_VIEW_DELETE(Help_View, Help_ViewDelete)
#else
WIDGET_NEW(Help_View, Help_ViewNew)
WIDGET_DELETE(Help_View, Help_ViewDelete)
#endif

GETSET_ENUM(Help_View, Font, textfont, Help_Viewtextfont)
GETSET_INTTYPE(Help_View, Fontsize, textsize, Help_Viewtextsize)
GETSET_INTTYPE(Help_View, Color, textcolor, Help_Viewtextcolor)
GETSET_STRING(Help_View, value, Help_Viewvalue)
VOID_STRING(Help_View, load, Help_Viewload)
VOID_INT4(Help_View, resize, Help_Viewresize)

GETSET_INT(Help_View, leftline, Help_Viewleftline)
GETSET_INT(Help_View, scrollbar_size, Help_Viewscrollbar_size)

STRING_VOID(Help_View, directory, Help_Viewdirectory)
STRING_VOID(Help_View, title, Help_Viewtitle)
STRING_VOID(Help_View, filename, Help_Viewfilename)

VOID_VOID(Help_View, clear_selection, Help_Viewclear_selection)
VOID_VOID(Help_View, select_all, Help_Viewselect_all)

static int Help_Viewtopline(lua_State *L)
    {
    Fl_Help_View *p = check_Help_View(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushinteger(L, p->topline());
        return 1;
        }
    if(lua_type(L, 2) == LUA_TSTRING)
        p->topline(luaL_checkstring(L, 2));
    else
        p->topline(luaL_checkinteger(L, 2));
    return 0;
    }

static int Help_Viewsize(lua_State *L)
    {
    Fl_Help_View *p = check_Help_View(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushinteger(L, p->size());
        return 1;
        }
    p->size(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
    }


static int Help_Viewfind(lua_State *L)
    {
    Fl_Help_View *p = check_Help_View(L, 1);
    const char *s = luaL_checkstring(L, 2);
    int pos = luaL_optinteger(L, 3, 0);
    int line = p->find(s, pos);
    if(line == -1)
        return 0;
    lua_pushinteger(L, line);
    return 1;
    }


static const char* Help_Func(Fl_Widget *p, const char *uri)
    {
    lua_State *L = main_lua_state;
    if (!L) return NULL;
    ud_t *ud = userdata(p);
    if(!ud) return NULL;
    if(pushvalue(L, ud->help_func) != LUA_TFUNCTION)
        { unexpected(L); return NULL; }
    push_Help_View(L, (Fl_Help_View*)p); /* downcast should be valid, here... */
    lua_pushstring(L, uri);
    if(lua_pcall(L, 2, 1, 0) != LUA_OK)
        { unexpected(L); return NULL; }
    return luaL_optstring(L, -1, NULL);
#if 0 //@@ there is a SEGV around here, but it may be an FLTK problem
    const char *s = luaL_optstring(L, -1, NULL);
printf("'%s' %d\n", s ? s : "nil");
printf("s==uri? %d\n", s ? (strcmp(s,uri)==0) : 0);
    if(s && strcmp(s, uri)==0)
        return uri;
    return s ? strdup(s) : NULL;
#endif
    }

static int Help_Viewlink(lua_State *L)
    {
    Fl_Help_View *p = check_Help_View(L, 1);
    ud_t *ud = userdata(p);
    if(!lua_isfunction(L, 2))
        return luaL_argerror(L, 2, "function expected");
    unreference(L, ud->help_func);
    reference(L, ud->help_func, 2);
    p->link(Help_Func);
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Help_ViewMethods[] = 
    {
        { "clear_selection", Help_Viewclear_selection },
        { "directory", Help_Viewdirectory },
        { "filename", Help_Viewfilename },
        { "find", Help_Viewfind },
        { "leftline", Help_Viewleftline },
        { "link", Help_Viewlink },
        { "load", Help_Viewload },
        { "resize", Help_Viewresize },
        { "scrollbar_size", Help_Viewscrollbar_size },
        { "select_all", Help_Viewselect_all },
        { "size", Help_Viewsize },
        { "textcolor", Help_Viewtextcolor },
        { "textfont", Help_Viewtextfont },
        { "textsize", Help_Viewtextsize },
        { "title", Help_Viewtitle },
        { "topline", Help_Viewtopline },
        { "value", Help_Viewvalue },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Help_ViewMetaMethods[] = 
    {
        { "__gc",  Help_ViewDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Help_ViewFunctions[] = 
    {
        { "help_view",  Help_ViewNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Help_View(lua_State *L)
    {
    udata_define(L, MT_Help_View, Help_ViewMethods, Help_ViewMetaMethods);
    udata_inherit(L, MT_Help_View, MT_Group);
    luaL_setfuncs(L, Help_ViewFunctions, 0);
    }

