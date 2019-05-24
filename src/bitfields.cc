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
 | Fl_Align                                                             |
 *----------------------------------------------------------------------*/

Fl_Align check_Align(lua_State *L, int arg)
/* Accepts an integer or a list of strings starting from index=arg */
    {
    Fl_Align align = 0;
    const char *s;
    
    if(lua_type(L, arg) == LUA_TNUMBER)
        return (Fl_Align)luaL_checkinteger(L, arg);

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { align |= CODE; goto done; } while(0)
        CASE(0, "default"); /* for default values (nil is used for get()) */
        // -----------
        CASE(FL_ALIGN_CENTER, "center");
        CASE(FL_ALIGN_TOP, "top");
        CASE(FL_ALIGN_BOTTOM, "bottom");
        CASE(FL_ALIGN_LEFT, "left");
        CASE(FL_ALIGN_RIGHT, "right");
        // -----------
        CASE(FL_ALIGN_INSIDE, "inside");
        CASE(0, "outside");
        // -----------
        CASE(FL_ALIGN_TEXT_OVER_IMAGE, "text over image");
        CASE(0, "image over text");
        CASE(FL_ALIGN_IMAGE_NEXT_TO_TEXT, "image next to text");
        CASE(FL_ALIGN_TEXT_NEXT_TO_IMAGE, "text next to image");
        CASE(FL_ALIGN_IMAGE_BACKDROP, "image backdrop");
        // -----------
        CASE(FL_ALIGN_CLIP, "clip");
        CASE(0, "no clip");
        CASE(FL_ALIGN_WRAP, "wrap");
        CASE(0, "no wrap");
#undef CASE
        return (Fl_Align)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return align;
    }

int push_Align(lua_State *L, Fl_Align align, int pushcode)
/* Pushes an integer followed by a list of strings */
    {
    if(pushcode)
        { lua_pushinteger(L, align); return 1; }

    int n = 0;
    Fl_Align position = align & FL_ALIGN_POSITION_MASK;
#define CASE(CODE,str) do { if( position & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(FL_ALIGN_TOP, "top");
        CASE(FL_ALIGN_BOTTOM, "bottom");
        CASE(FL_ALIGN_LEFT, "left");
        CASE(FL_ALIGN_RIGHT, "right");
#undef CASE
    if(position==0) { lua_pushstring(L, "center"); n++; }

    lua_pushstring(L, align & FL_ALIGN_INSIDE ? "inside" : "outside"); n++;

    Fl_Align image = align & FL_ALIGN_IMAGE_MASK;
    switch(image)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_ALIGN_TEXT_OVER_IMAGE, "text over image");
        CASE(FL_ALIGN_IMAGE_NEXT_TO_TEXT, "image next to text");
        CASE(FL_ALIGN_TEXT_NEXT_TO_IMAGE, "text next to image");
        CASE(FL_ALIGN_IMAGE_BACKDROP, "image backdrop");
#undef CASE
        default:
            lua_pushstring(L, "image over text"); 
        }
    n++;

    lua_pushstring(L, align & FL_ALIGN_CLIP ? "clip" : "no clip"); n++;
    lua_pushstring(L, align & FL_ALIGN_WRAP ? "wrap" : "no wrap"); n++;
    return n;
    }

/*----------------------------------------------------------------------*
 | WhenFd                                                               |
 *----------------------------------------------------------------------*/

int check_WhenFd(lua_State *L, int arg)
    {
    unsigned int flags = 0;
    const char *s;

    if(lua_type(L, arg) == LUA_TNUMBER)
        return luaL_checkinteger(L, arg);

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(FL_READ, "read");
        CASE(FL_WRITE, "write");
        CASE(FL_EXCEPT, "except");
#undef CASE
        return (int)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return (int)flags;
    }

int push_WhenFd(lua_State *L, int flags, int pushcode)
    {
    if(pushcode)
        { lua_pushinteger(L, flags); return 1; }
    int n = 0;
#define CASE(CODE,str) if(flags & CODE) do { lua_pushstring(L, str); n++; } while(0)
        CASE(FL_READ, "read");
        CASE(FL_WRITE, "write");
        CASE(FL_EXCEPT, "except");
#undef CASE
    return n;
    }


/*----------------------------------------------------------------------*
 | Fl_When                                                              |
 *----------------------------------------------------------------------*/

Fl_When check_When(lua_State *L, int arg)
/* Accepts an integer or a list of strings starting from index=arg */
    {
    unsigned int when = 0;
    const char *s;

    if(lua_type(L, arg) == LUA_TNUMBER)
        return (Fl_When)luaL_checkinteger(L, arg);

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { when |= CODE; goto done; } while(0)
        CASE(FL_WHEN_NEVER, "never");
        CASE(FL_WHEN_CHANGED, "changed");
        CASE(FL_WHEN_NOT_CHANGED, "not changed");
        CASE(FL_WHEN_RELEASE, "release");
        CASE(FL_WHEN_ENTER_KEY, "enter key");
        //CASE(FL_WHEN_RELEASE_ALWAYS, "release always");        release | not changed
        //CASE(FL_WHEN_ENTER_KEY_ALWAYS, "enter key always");    enter key | not changed
        //CASE(FL_WHEN_ENTER_KEY_CHANGED, "enter key changed"); enter key | changed
#undef CASE
        return (Fl_When)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return (Fl_When)when;
    }

int push_When(lua_State *L, Fl_When when, int pushcode)
/* Pushes an integer followed by a list of strings */
    {
    if(pushcode)
        { lua_pushinteger(L, when); return 1; }

    int n = 0;

#define CASE(CODE,str) if(when & CODE) do { lua_pushstring(L, str); n++; } while(0)
//      CASE(FL_WHEN_NEVER, "never");
        CASE(FL_WHEN_CHANGED, "changed");
        CASE(FL_WHEN_NOT_CHANGED, "not changed");
        CASE(FL_WHEN_RELEASE, "release");
        CASE(FL_WHEN_ENTER_KEY, "enter key");
//      CASE(FL_WHEN_RELEASE_ALWAYS, "release always");
//      CASE(FL_WHEN_ENTER_KEY_ALWAYS, "enter key always");
//      CASE(FL_WHEN_ENTER_KEY_CHANGED, "enter key changed");
#undef CASE
    if(n==0)
        { lua_pushstring(L, "never"); return 1; }
    return n;
    }


/*----------------------------------------------------------------------*
 | Line Style                                                           |
 *----------------------------------------------------------------------*/

int check_Style(lua_State *L, int arg)
/* Accepts an integer or a list of strings starting from index=arg */
    {
    int line, cap, join;
    const char *s;
    
    if(lua_type(L, arg) == LUA_TNUMBER)
        return luaL_checkinteger(L, arg);

    line = cap = join = 0;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { line = CODE; goto done; } while(0)
        CASE(FL_SOLID, "solid");
        CASE(FL_DASH, "dash");
        CASE(FL_DOT, "dot");
        CASE(FL_DASHDOT, "dashdot");
        CASE(FL_DASHDOTDOT, "dashdotdot");
#undef CASE
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { cap = CODE; goto done; } while(0)
        CASE(FL_CAP_FLAT, "cap flat");
        CASE(FL_CAP_ROUND, "cap round");
        CASE(FL_CAP_SQUARE, "cap square");
#undef CASE
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { join = CODE; goto done; } while(0)
        CASE(FL_JOIN_MITER, "join miter");
        CASE(FL_JOIN_ROUND, "join round");
        CASE(FL_JOIN_BEVEL, "join bevel");
#undef CASE
        return luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return line | cap | join;
    }


int push_Style(lua_State *L, int style, int pushcode)
/* Pushes an integer followed by a list of strings */
    {
    if(pushcode)
        { lua_pushinteger(L, style); return 1; }

    int n = 0;
    int line = style & 0x0f;
    switch(line)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); n++; break
        CASE(FL_DASH, "dash");
        CASE(FL_DOT, "dot");
        CASE(FL_DASHDOT, "dashdot");
        CASE(FL_DASHDOTDOT, "dashdotdot");
        default:
            lua_pushstring(L, "solid");
            n++;
#undef CASE
        }

    int cap = style & 0xf00;
    switch(cap)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); n++; break
        CASE(FL_CAP_FLAT, "cap flat");
        CASE(FL_CAP_ROUND, "cap round");
        CASE(FL_CAP_SQUARE, "cap square");
        default:
            ;
#undef CASE
        }

    int join = style & 0xf000;
    switch(join)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); n++; break
        CASE(FL_JOIN_MITER, "join miter");
        CASE(FL_JOIN_ROUND, "join round");
        CASE(FL_JOIN_BEVEL, "join bevel");
        default:
            ;
#undef CASE
        }

    return n;
    }


/*----------------------------------------------------------------------*
 | Fl_Damage                                                            |
 *----------------------------------------------------------------------*/

Fl_Damage check_Damage(lua_State *L, int arg)
    {
    unsigned int flags = 0;
    const char *s;

    if(lua_type(L, arg) == LUA_TNUMBER)
        return (Fl_Damage)luaL_checkinteger(L, arg);

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(0, "nothing");
        CASE(FL_DAMAGE_CHILD, "child");
        CASE(FL_DAMAGE_EXPOSE, "expose");
        CASE(FL_DAMAGE_SCROLL, "scroll");
        CASE(FL_DAMAGE_OVERLAY, "overlay");
        CASE(FL_DAMAGE_USER1, "user1");
        CASE(FL_DAMAGE_USER2, "user2");
        CASE(FL_DAMAGE_ALL, "all");
#undef CASE
        return (Fl_Damage)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return (Fl_Damage)flags;
    }

int push_Damage(lua_State *L, Fl_Damage flags, int pushcode)
    {
    if(pushcode)
        { lua_pushinteger(L, flags); return 1; }

    int n = 0;
#define CASE(CODE,str) if(flags & CODE) do { lua_pushstring(L, str); n++; } while(0)
        CASE(FL_DAMAGE_CHILD, "child");
        CASE(FL_DAMAGE_EXPOSE, "expose");
        CASE(FL_DAMAGE_SCROLL, "scroll");
        CASE(FL_DAMAGE_OVERLAY, "overlay");
        CASE(FL_DAMAGE_USER1, "user1");
        CASE(FL_DAMAGE_USER2, "user2");
        CASE(FL_DAMAGE_ALL, "all");
#undef CASE
    if(n==0)
        { lua_pushstring(L, "nothing"); return 1; }
    return n;
    }


/*----------------------------------------------------------------------*
 | Fl_Menu_Item flags                                                   |
 *----------------------------------------------------------------------*/

int check_Menu_ItemFlags(lua_State *L, int arg)
    {
    unsigned int flags = 0;
    const char *s;

    if(lua_type(L, arg) == LUA_TNUMBER)
        return luaL_checkinteger(L, arg);

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(FL_MENU_INACTIVE, "inactive");
        CASE(FL_MENU_TOGGLE, "toggle");
        CASE(FL_MENU_VALUE, "value");
        CASE(FL_MENU_RADIO, "radio");
        CASE(FL_MENU_INVISIBLE, "invisible");
//      CASE(FL_SUBMENU_POINTER, "submenu pointer"); this is only for static menus, I suppose
        CASE(FL_SUBMENU, "submenu");
        CASE(FL_MENU_DIVIDER, "divider");
        CASE(FL_MENU_HORIZONTAL, "horizontal");
#undef CASE
        return (int)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return (int)flags;
    }

int push_Menu_ItemFlags(lua_State *L, int flags, int pushcode)
    {
    if(pushcode)
        { lua_pushinteger(L, flags); return 1; }
    int n = 0;
#define CASE(CODE,str) if(flags & CODE) do { lua_pushstring(L, str); n++; } while(0)
        CASE(FL_MENU_INACTIVE, "inactive");
        CASE(FL_MENU_TOGGLE, "toggle");
        CASE(FL_MENU_VALUE, "value");
        CASE(FL_MENU_RADIO, "radio");
        CASE(FL_MENU_INVISIBLE, "invisible");
//      CASE(FL_SUBMENU_POINTER, "submenu pointer");
        CASE(FL_SUBMENU, "submenu");
        CASE(FL_MENU_DIVIDER, "divider");
        CASE(FL_MENU_HORIZONTAL, "horizontal");
#undef CASE
    return n;
    }

/*----------------------------------------------------------------------*
 | Fl_Mode                                                              |
 *----------------------------------------------------------------------*/

Fl_Mode check_Mode(lua_State *L, int arg)
    {
    unsigned int opt = 0;
    const char *s;

    if(lua_type(L, arg) == LUA_TNUMBER)
        return (Fl_Mode)luaL_checkinteger(L, arg);

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { opt |= CODE; goto done; } while(0)
        CASE(FL_RGB, "rgb");
        CASE(FL_INDEX, "index");
        CASE(FL_SINGLE, "single");
        CASE(FL_DOUBLE, "double");
        CASE(FL_ACCUM, "accum");
        CASE(FL_ALPHA, "alpha");
        CASE(FL_DEPTH, "depth");
        CASE(FL_STENCIL, "stencil");
        CASE(FL_RGB8, "rgb8");
        CASE(FL_MULTISAMPLE, "multisample");
        CASE(FL_STEREO, "stereo");
        CASE(FL_FAKE_SINGLE, "fake single");
#undef CASE
        return (Fl_Mode)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return (Fl_Mode)opt;
    }

int push_Mode(lua_State *L, Fl_Mode opt, int pushcode)
    {
    if(pushcode)
        { lua_pushinteger(L, opt); return 1; }

    int n = 0;
#define ALT(CODE1, str0, str1) do { lua_pushstring(L, (opt & CODE1)? str1 : str0); n++; } while(0)
        ALT(FL_INDEX, "rgb", "index");
        ALT(FL_DOUBLE, "single", "double");
#undef ALT
#define CASE(CODE,str) if(opt & CODE) do { lua_pushstring(L, str); n++; } while(0)
        CASE(FL_ACCUM, "accum");
        CASE(FL_ALPHA, "alpha");
        CASE(FL_DEPTH, "depth");
        CASE(FL_STENCIL, "stencil");
        CASE(FL_RGB8, "rgb8");
        CASE(FL_MULTISAMPLE, "multisample");
        CASE(FL_STEREO, "stereo");
        CASE(FL_FAKE_SINGLE, "fake single");
#undef CASE
    return n;
    }

/*----------------------------------------------------------------------*
 | Fl_Tree_Item_Draw_Mode                                               |
 *----------------------------------------------------------------------*/

#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)

Fl_Tree_Item_Draw_Mode check_Tree_Item_Draw_Mode(lua_State *L, int arg)
    {
    unsigned int flags = 0;
    const char *s;

    if(lua_type(L, arg) == LUA_TNUMBER)
        return (Fl_Tree_Item_Draw_Mode )luaL_checkinteger(L, arg);

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(FL_TREE_ITEM_DRAW_DEFAULT, "default");
        CASE(FL_TREE_ITEM_DRAW_LABEL_AND_WIDGET, "label and widget");
        CASE(FL_TREE_ITEM_HEIGHT_FROM_WIDGET, "height from widget");
#undef CASE
        return (Fl_Tree_Item_Draw_Mode )luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return (Fl_Tree_Item_Draw_Mode )flags;
    }

int push_Tree_Item_Draw_Mode(lua_State *L, Fl_Tree_Item_Draw_Mode flags, int pushcode)
    {
    if(pushcode)
        { lua_pushinteger(L, flags); return 1; }

    int n = 0;
#define CASE(CODE,str) if(flags & CODE) do { lua_pushstring(L, str); n++; } while(0)
        CASE(FL_TREE_ITEM_DRAW_DEFAULT, "default");
        CASE(FL_TREE_ITEM_DRAW_LABEL_AND_WIDGET, "label and widget");
        CASE(FL_TREE_ITEM_HEIGHT_FROM_WIDGET, "height from widget");
#undef CASE
    if(n==0)
        { lua_pushstring(L, "default"); return 1; }
    return n;
    }

#endif

