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
 | Fl_Font                                                              |
 *----------------------------------------------------------------------*/

static Fl_Font FreeFont = FL_FREE_FONT; 
/* free index for the next font ( we'll update this in set_font/setfonts() */

Fl_Font check_Font(lua_State *L, int arg)
    {
    Fl_Font font = luaL_checkinteger(L, arg);
    if(!(font < FreeFont))
        return luaL_argerror(L, arg, "invalid font index");
    return font;
    }

int push_Font(lua_State *L, Fl_Font font)
    {
    lua_pushinteger(L, font);
    return 1;
    }



/*------------------------------------------------------------------------------*
 | fl_font functions                                                            |
 *------------------------------------------------------------------------------*/

FUNC_INT_VOID(fl_descent, Descent)

static int Fontt(lua_State *L)
    {
    if(lua_isnone(L, 1))
        return push_Font(L, fl_font());
    fl_font(check_Font(L, 1), check_Fontsize(L, 2));
    return 0;
    }

static int Height(lua_State *L)
    {
    if(lua_isnone(L, 1))
        lua_pushinteger(L, fl_height());
    else
        lua_pushinteger(L, fl_height(check_Font(L, 1), check_Fontsize(L, 2)));
    return 1;
    }

static int Size(lua_State *L)
    {
    push_Fontsize(L, fl_size());
    return 1;
    }

#define F(func, Func)                       \
static int Func(lua_State *L)               \
    {                                       \
    const char *t = luaL_checkstring(L, 1); \
    int n = luaL_optinteger(L, 2, -1);      \
    lua_pushstring(L, func(t,n));           \
    return 1;                               \
    }

F(fl_latin1_to_local, Latin1_to_local)
F(fl_local_to_latin1, Local_to_latin1)
//F(fl_local_to_mac_roman, Local_to_mac_roman)
//F(fl_mac_roman_to_local, Mac_roman_to_local)

#undef F

static int Text_extents(lua_State *L)
    {
    size_t len;
    int dx, dy, w, h;
    const char *s = luaL_checklstring(L, 1, &len);
    fl_text_extents(s,(int)len, dx, dy, w, h);
    lua_pushinteger(L, dx);
    lua_pushinteger(L, dy);
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 4;
    }

static int Width(lua_State *L)
/* Note: double fl_width(unsigned int c) would be redundant ... */
    {
    size_t len;
    const char *s = luaL_checklstring(L, 1, &len);
    lua_pushnumber(L, fl_width(s, (int)len));
    return 1;
    }

static int Get_font(lua_State *L)
    {
    int font = check_Font(L, 1);
    const char *descr = Fl::get_font(font);
    lua_pushstring(L, descr ? descr : "");
    return 1;
    }

static int Get_font_name(lua_State *L)
    {
    int font = check_Font(L, 1);
    int attributes = 0;
    const char *name= Fl::get_font_name(font, &attributes);
    lua_pushstring(L, name ? name : "");
    lua_pushinteger(L, attributes);
    return 2;
    }

static int Get_font_sizes(lua_State *L)
    {
    int *sizep;
    int font = check_Font(L, 1);
    int n = Fl::get_font_sizes(font, sizep);
    if(n<=0) return 0;
    for(int i = 0; i < n; i++)
        lua_pushinteger(L, sizep[i]);
    return n;
    }


static int Set_fonts(lua_State *L)
    {
    const char *s = luaL_optstring(L, 1, NULL);
    FreeFont = Fl::set_fonts(s);
    lua_pushinteger(L, FreeFont);
    return 1;
    }

static int Set_font(lua_State *L)
    {
    Fl_Font index = luaL_checkinteger(L, 1);
    if(index > FreeFont)
        return luaL_argerror(L, 1, "invalid font index");
    if(lua_type(L, 2) == LUA_TNUMBER)
        {
        Fl_Font from = check_Font(L, 2);
        Fl::set_font(index, from);
        }
    else
        {
        int ref;
        const char *s = luaL_checkstring(L, 2);
        if(s[0] != ' ')
            return luaL_argerror(L, 2, "font name should have a leading space");
        reference(L, ref, 2);
        Fl::set_font(index, s);
        if(index == FreeFont) FreeFont++;
        /* The problem here is that set_font() does not copy the string
         * (it should be static). We could strdup() it but it would be tricky 
         * to free() all strdup'd strings at exit... so we reference it on the
         * Lua registry, and leave the GC do the job for us.
         */
        // @@TODO: put the font for index into a lua table and store this table into registry
         (void)ref; /* we won't use this any more */
        }
    return 0;
    }

static int Nfonts(lua_State *L) /* NONFLTK */
    {
    lua_pushinteger(L, FreeFont);
    return 1;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "descent", Descent },
        { "font", Fontt },
        { "height", Height },
        { "get_font", Get_font },
        { "get_font_name", Get_font_name },
        { "get_font_sizes", Get_font_sizes },
        { "latin1_to_local", Latin1_to_local },
        { "local_to_latin1", Local_to_latin1 },
//      { "local_to_mac_roman", Local_to_mac_roman },
//      { "mac_roman_to_local", Mac_roman_to_local },
        { "nfonts", Nfonts },
        { "set_font", Set_font },
        { "set_fonts", Set_fonts },
        { "text_extents", Text_extents },
        { "size", Size },
        { "width", Width },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_FlFont(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

