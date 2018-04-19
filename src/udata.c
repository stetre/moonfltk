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

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "tree.h"
#include "udata.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "compat-5.3.h"

struct moonfltk_udata_s {
    RB_ENTRY(moonfltk_udata_s) entry;
    uintptr_t id;     /* object id (search key) */
    /* references on the Lua registry */
    int ref;    /* the correspoding userdata */
    void *mem;  /* userdata memory area allocated and released by Lua */
};

#define UNEXPECTED_ERROR "unexpected error (%s, %d)", __FILE__, __LINE__

static int cmp(udata_t *udata1, udata_t *udata2) /* the compare function */
    { return (udata1->id < udata2->id ? -1 : udata1->id > udata2->id); } 

static RB_HEAD(udatatree_s, udata_s) Head = RB_INITIALIZER(&Head);

RB_PROTOTYPE_STATIC(udatatree_s, udata_s, entry, cmp) 
RB_GENERATE_STATIC(udatatree_s, udata_s, entry, cmp) 
 
static udata_t *udata_remove(udata_t *udata) 
    { return RB_REMOVE(udatatree_s, &Head, udata); }
static udata_t *udata_insert(udata_t *udata) 
    { return RB_INSERT(udatatree_s, &Head, udata); }
static udata_t *udata_search(uintptr_t id) 
    { udata_t tmp; tmp.id = id; return RB_FIND(udatatree_s, &Head, &tmp); }
static udata_t *udata_first(uintptr_t id) 
    { udata_t tmp; tmp.id = id; return RB_NFIND(udatatree_s, &Head, &tmp); }
#if 0
static udata_t *udata_next(udata_t *udata)
    { return RB_NEXT(udatatree_s, &Head, udata); }
static udata_t *udata_prev(udata_t *udata)
    { return RB_PREV(udatatree_s, &Head, udata); }
static udata_t *udata_min(void)
    { return RB_MIN(udatatree_s, &Head); }
static udata_t *udata_max(void)
    { return RB_MAX(udatatree_s, &Head); }
static udata_t *udata_root(void)
    { return RB_ROOT(&Head); }
#endif

void *udata_new(lua_State *L, size_t size, const void *ptr, const char *mt)
/* 
 * Creates a new Lua userdata, optionally sets its metatable to mt (if != NULL),
 * associates the userdata with the passed ptr and pushes the userdata on the stack.
 *
 * The userdata can be subsquently pushed on the Lua stack with udata_push(L, ptr).
 * (This is useful to bind C/C++ objects to Lua userdata).
 *
 * If ptr=NULL, the pointer to the memory area allocated by Lua is used as ptr
 * (this function returnes it).
 */
    {
    udata_t *udata;
//  printf("new object %p\n", ptr);
    if((udata = (udata_t*)malloc(sizeof(udata_t))) == NULL) 
        { luaL_error(L, "cannot allocate memory"); return NULL; }
    memset(udata, 0, sizeof(udata_t));
    udata->mem = lua_newuserdata(L, size);
    if(!udata->mem)
        {
        free(udata);
        luaL_error(L, "lua_newuserdata error"); 
        return NULL;
        }
    udata->id = ptr!=NULL ? (uintptr_t)ptr : (uintptr_t)(udata->mem);
    if(udata_search(udata->id))
        { 
        free(udata);
        luaL_error(L, "duplicated object %p", ptr); 
        return NULL; 
        }
    /* create a reference for later push's */
    lua_pushvalue(L, -1); /* the newly created userdata */
    udata->ref = luaL_ref(L, LUA_REGISTRYINDEX);
    udata_insert(udata);
    if(mt)
        {
        luaL_getmetatable(L, mt);
        lua_setmetatable(L, -2);
        }
    return udata->mem;
    }

void *udata_mem(const void* ptr)
    {
    udata_t *udata = udata_search((uintptr_t)ptr);
    return udata ? udata->mem : NULL;
    }

int udata_unref(lua_State *L, const void *ptr)
/* unreference udata so that it will be garbage collected */
    {
//  printf("unref object %p\n", ptr);
    udata_t *udata = udata_search((uintptr_t)ptr);
    if(!udata) 
        return luaL_error(L, "invalid object pointer %p", ptr);
    if(udata->ref != LUA_NOREF)
        {
        luaL_unref(L, LUA_REGISTRYINDEX, udata->ref);
        udata->ref = LUA_NOREF;
        }
    return 0;
    }

int udata_free(lua_State *L, const void *ptr)
/* this should be called in the __gc metamethod
 */
    {
    udata_t *udata = udata_search((uintptr_t)ptr);
//  printf("free object %p\n", ptr);
    if(!udata) 
        return luaL_error(L, "invalid object pointer %p", ptr);
    /* release all references */
    if(udata->ref != LUA_NOREF)
        luaL_unref(L, LUA_REGISTRYINDEX, udata->ref);
    udata_remove(udata);
    free(udata);
    /* mem is released by Lua at garbage collection */
    return 0;
    }

int udata_push(lua_State *L, const void *ptr)
    {
    udata_t *udata = udata_search((uintptr_t)ptr);
//  printf("push object %p\n", ptr);
    if(!udata) 
        return luaL_error(L, "invalid object pointer %p", ptr);
    if(udata->ref == LUA_NOREF)
        return luaL_error(L, "unreferenced object");
    if(lua_rawgeti(L, LUA_REGISTRYINDEX, udata->ref) != LUA_TUSERDATA)
        return luaL_error(L, UNEXPECTED_ERROR);
    return 1; /* one value pushed */
    }

int udata_push_ifvalid(lua_State *L, const void *ptr)
    {
    udata_t *udata = udata_search((uintptr_t)ptr);
//  printf("push object %p\n", ptr);
    if(!udata) 
        return 0;
    if(udata->ref == LUA_NOREF)
        return luaL_error(L, "unreferenced object");
    if(lua_rawgeti(L, LUA_REGISTRYINDEX, udata->ref) != LUA_TUSERDATA)
        return luaL_error(L, UNEXPECTED_ERROR);
    return 1; /* one value pushed */
    }

void udata_free_all(void)
/* free all without unreferencing (for atexit()) */
    {
    udata_t *udata;
    while((udata = udata_first(0)))
        {
        udata_remove(udata);
        free(udata);
        }
    }

static int is_subclass(lua_State *L, int arg, int mt_index)
    {
    int ok;
    if(luaL_getmetafield(L, arg, "__index") == LUA_TNIL)
        return 0;
    if(lua_rawequal(L, mt_index, lua_gettop(L)))
        {
        lua_pop(L, 1);
        return 1;
        }
    ok = is_subclass(L, lua_gettop(L), mt_index);
    lua_pop(L, 1);
    return ok;
    }

void *udata_test(lua_State *L, int arg, const char *mt)
/* same as luaL_testudata(), but succeeds also if the userdata has not
 * mt as metatable but as ancestor
 */ 
    {
    void *mem;
    int ok;
    if((mem = luaL_testudata(L, arg, mt)))
        return mem;
    
    if((mem = lua_touserdata(L, arg)) == NULL)
        return NULL;

    if(luaL_getmetatable(L, mt)!=LUA_TTABLE)
        { luaL_error(L, "cannot find metatable '%s'", mt); return NULL; }
    ok = is_subclass(L, arg, lua_gettop(L));
    lua_pop(L, 1);
    return ok ? mem : NULL;
    }

/*------------------------------------------------------------------------------*
 | newmetatable for 'class' userdata                                            |
 *------------------------------------------------------------------------------*/

int udata_define(lua_State *L, const char* mt, const luaL_Reg* methods, const luaL_Reg* metamethods)
    {
    /* create the metatable */
    if(!luaL_newmetatable(L, mt))
        return luaL_error(L, "cannot create metatable '%s'", mt);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    if(metamethods)
        /* add metamethods */
        luaL_setfuncs(L, metamethods, 0);
    if(methods)
        {
        /* add methods */
        luaL_getsubtable(L, -1, "__index");
        luaL_setfuncs(L, methods, 0);
        lua_pop(L, 1);
        }
    lua_pop(L, 1);
    return 0;
    }

int udata_inherit(lua_State *L, const char *sub, const char *super)
/* Sets  metatable(sub).__index = metatable(super)
 *
 * This way, if one accesses a field/method of a 'sub' object which is not defined in
 * the 'sub' metatable, Lua searches for it in the 'super' metatable (i.e., the 'sub'
 * type inherits from 'super').
 */
    {
    if(luaL_getmetatable(L, sub)!=LUA_TTABLE)
        return luaL_error(L, "cannot find metatable '%s'", sub);
    luaL_getsubtable(L, -1, "__index");
    if(luaL_getmetatable(L, super)!=LUA_TTABLE)
        return luaL_error(L, "cannot find metatable '%s'", super);
    lua_setmetatable(L, -2);
    lua_pop(L, 2);
    return 0;
    }

int udata_addmethods(lua_State *L, const char* mt, const luaL_Reg* methods)
/* Adds methods to the metatable mt */
    {
    if(luaL_getmetatable(L, mt)!=LUA_TTABLE)
        return luaL_error(L, "cannot find metatable '%s'", mt);
    if(methods)
        {
        /* add methods */
        luaL_getsubtable(L, -1, "__index");
        luaL_setfuncs(L, methods, 0);
        lua_pop(L, 1);
        }
    lua_pop(L, 1);
    return 0;
    }

