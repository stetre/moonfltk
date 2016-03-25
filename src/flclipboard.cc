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


static int check_ClipboardEndpoint(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
    CASE(0, "selection buffer");
    CASE(1, "clipboard");
#undef CASE
    return luaL_argerror(L, arg, badvalue(L,s));
    }

static int push_ClipboardEndpoint(lua_State *L, int val)
    {
    switch(val)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
    CASE(0, "selection buffer");
    CASE(1, "clipboard");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

const char* check_ClipboardType(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(str) if((strcmp(s, str)==0)) return str
        CASE(Fl::clipboard_plain_text);
        CASE(Fl::clipboard_image);
#undef CASE
    luaL_argerror(L, arg, badvalue(L,s));
    return NULL;
    }


static int CnRef = LUA_NOREF;
//static int CnArg = LUA_NOREF; not used
static lua_State *CnState = NULL;
static void CnHandler(int source, void *data)
    {
#define L CnState
    (void)data; /* not used */
    if(pushvalue(L, CnRef) != LUA_TFUNCTION)
        { unexpected(L); return; }
    push_ClipboardEndpoint(L, source);
    if(lua_pcall(L, 1, 0, 0) != LUA_OK)
        { unexpected(L); return; }
#undef L
    }

#define CnRemove(L) do {                        \
    if(CnRef != LUA_NOREF)                      \
        {                                       \
        Fl::remove_clipboard_notify(CnHandler); \
        unreference(L, CnRef);                  \
        CnState = NULL;                         \
        }                                       \
} while(0)

static int Add_clipboard_notify(lua_State *L)
    {
    CnRemove(L);
    if(lua_isnoneornil(L, 1))
        return 0;
    if(!lua_isfunction(L, 1))
        return luaL_argerror(L, 1, "function expected");
    reference(L, CnRef, 1);
    CnState = L;
    Fl::add_clipboard_notify(CnHandler);
    return 0;
    }

static int Remove_clipboard_notify(lua_State *L)
    {
    CnRemove(L);
    return 0;
    }

static int Dnd(lua_State *L)
    {
    (void)L;
    Fl::dnd(); /* return value? */
    return 0;
    }

static int Clipboard_contains(lua_State *L)
    {
    const char *type = check_ClipboardType(L, 1);
    lua_pushboolean(L, Fl::clipboard_contains(type));
    return 1;
    }

static int Copy(lua_State *L)
    {
    size_t len;
    int destination;
    const char *stuff = luaL_checklstring(L, 1, &len);
    if(lua_isnoneornil(L, 2))
        destination = 0; /* selection buffer */
    else
        destination = check_ClipboardEndpoint(L, 2);
    Fl::copy(stuff, (int)len, destination);
    return 0;
    }


static int Paste(lua_State *L)
    {
    Fl_Widget *receiver = check_Widget(L, 1);
    int source = check_ClipboardEndpoint(L, 2);
    if(lua_isnoneornil(L, 3))
        {
        Fl::paste(*receiver, source);
        return 0;
        }
    const char *type = check_ClipboardType(L, 3);
    Fl::paste(*receiver, source, type);
    return 0;
    }

static int Selection(lua_State *L)
    {
    size_t len;
    Fl_Widget *owner = check_Widget(L, 1);
    const char *stuff = luaL_checklstring(L, 2, &len);
    Fl::selection(*owner, stuff, (int)len);
    return 0;
    }


FUNC_GETSET_BOOLEAN(Fl::dnd_text_ops, Dnd_text_ops)

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "add_clipboard_notify", Add_clipboard_notify },
        { "clipboard_contains", Clipboard_contains },
        { "copy", Copy },
        { "dnd", Dnd },
        { "dnd_text_ops", Dnd_text_ops },
        { "paste", Paste },
        { "remove_clipboard_notify", Remove_clipboard_notify },
        { "selection", Selection },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_FlClipboard(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }


