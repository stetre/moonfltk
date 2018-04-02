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
 | Additional utilities                                                         |
 *------------------------------------------------------------------------------*/

static int Key(lua_State *L)
/* code = fl.key(string)
 * string = fl.key(code)
 */
    {
    if(lua_type(L, 1)==LUA_TSTRING)
        {
        int key = check_Key(L, 1);
        lua_pushinteger(L, key);
        return 1;
        }
    int key = luaL_checkinteger(L, 1);
    push_Key(L, key);
    return 1;
    }

static int Shortcut(lua_State *L)
    {
    if(lua_isnumber(L, 1))
        return push_Shortcut(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, check_Shortcut(L, 1));
    return 1;
    }

static int Alignment(lua_State *L)
    {
    if(lua_isnumber(L, 1))
        return push_Align(L, luaL_checkinteger(L, 1), 0);
    lua_pushinteger(L,  check_Align(L, 1));
    return 1;
    }

static int Whenflags(lua_State *L)
    {
    if(lua_isnumber(L, 1))
        return push_When(L, (Fl_When)luaL_checkinteger(L, 1), 0);
    lua_pushinteger(L,  check_When(L, 1));
    return 1;
    }


static int Linestyle(lua_State *L)
    {
    if(lua_isnumber(L, 1))
        return push_Style(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L,  check_Style(L, 1));
    return 1;
    }

static int Modifiers(lua_State *L)
    {
    if(lua_isnumber(L, 1))
        return push_Modifiers(L, luaL_checkinteger(L, 1), 0);
    int code = check_Modifiers(L, 1);
    lua_pushinteger(L, code);
    return 1;
    }

static int Damagebits(lua_State *L)
    {
    if(lua_isnumber(L, 1))
        return push_Damage(L, (Fl_Damage)luaL_checkinteger(L, 1), 0);
    lua_pushinteger(L,  check_Damage(L, 1));
    return 1;
    }


static int Mode(lua_State *L)
    {
    if(lua_isnumber(L, 1))
        return push_Mode(L, (Fl_Mode)luaL_checkinteger(L, 1), 0);
    lua_pushinteger(L,  check_Mode(L, 1));
    return 1;
    }

static int Menuitemflags(lua_State *L)
    {
    if(lua_isnumber(L, 1))
        return push_Menu_ItemFlags(L, luaL_checkinteger(L, 1), 0);
    lua_pushinteger(L,  check_Menu_ItemFlags(L, 1));
    return 1;
    }

static int Sleep(lua_State *L)
    {
    double seconds = luaL_checknumber(L, 1);
    if(seconds > 0)
        moonfltk_sleep(seconds);
    else if(seconds < 0)
        return luaL_argerror(L, 1, "timeout should be non-negative");
    return 0;
    }

static int Gettime(lua_State *L) /* NONFLTK need this for timer wheel */
/* seconds = fl.gettime() */
    {
    lua_pushnumber(L, moonfltk_now());
    return 1;
    }

static int Unreference(lua_State *L)
/* For advanced uses only. 
 *
 * Unreferences the passed object from the Lua registry, so to allow GC.
 * Once this function has been called, the object is to be considered not
 * valid anymore.
 * Note that the object will not be garbage collected if it is still 
 * referenced elsewhere, e.g. in Lua variables or, more subtly, if it is
 * used as argument to callbacks.
 * Normally you don't need to use this function because objects are
 * GCed automatically when the process exits. You may want to use it in
 * order to reduce memory usage in situations where objects are repeatedly
 * instantiated (although in these cases it may be better to reconsider 
 * the design of the application...).
 */
    {
    void *p = test_Widget(L, 1);
    if(!p) p = test_Image(L, 1); //@@ ragionare sul caso di shared images
//  if(!p) p = test_Device(L, 1); @@ not implemented, so metatable is not defined
    if(!p) p = test_Help_Dialog(L, 1);
    if(!p) p = test_Native_File_Chooser(L, 1);
    if(!p) p = test_Text_Buffer(L, 1);
    if(!p)
        return luaL_argerror(L, 1, "not a valid object");
    userdata_unref(L, p);
    return 0;
    }


static int New_uuid(lua_State *L)
    {
    const char* uuid = Fl_Preferences::newUUID();
    if(!uuid)
        return 0;
    lua_pushstring(L, uuid);
    return 1;
    }

int moonfltk_trace_objects = 0;

static int Trace_objects(lua_State *L)
    {
    moonfltk_trace_objects = checkboolean(L, 1);
    return 0;
    }

static int Trace_stack(lua_State *L)
    {
    const char *s = luaL_optstring(L, 1, "");
    printstack(L, "%s", s);
    return 0;
    }


/*------------------------------------------------------------------------------*
 |                                                                              |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "alignment", Alignment },
        { "damagebits", Damagebits },
        { "gettime",  Gettime },
        { "key", Key },
        { "linestyle", Linestyle },
        { "menuitemflags", Menuitemflags },
        { "mode", Mode },
        { "modifiers", Modifiers },
        { "new_uuid", New_uuid },
        { "trace_objects", Trace_objects },
        { "trace_stack", Trace_stack },
        { "shortcut", Shortcut },
        { "sleep", Sleep },
        { "unreference", Unreference },
        { "whenflags", Whenflags },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_Additional(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

