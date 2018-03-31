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

#if 0 //@@
//int   fl_access(const char *f, int mode)
static int Access(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_chmod(const char *f, int mode)
static int Chmod(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_execvp(const char *file, char *const *argv)
static int Execvp(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//FILE *    fl_fopen(const char *f, const char *mode)
static int Fopen(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//char *    fl_getcwd(char *buf, int maxlen)
static int Getcwd(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//char *    fl_getenv(const char *name)
static int Getenv(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//char  fl_make_path(const char *path)
static int Make_path(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//void  fl_make_path_for_file(const char *path)
static int Make_path_for_file(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_mkdir(const char *f, int mode)
static int Mkdir(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//unsigned int  fl_nonspacing(unsigned int ucs)
static int Nonspacing(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_open(const char *f, int oflags,...)
static int Open(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_rename(const char *f, const char *t)
static int Rename(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_rmdir(const char *f)
static int Rmdir(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_stat(const char *path, struct stat *buffer)
static int Stat(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_system(const char *f)
static int System(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_tolower(unsigned int ucs)
static int Tolower(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_toupper(unsigned int ucs)
static int Toupper(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//unsigned  fl_ucs_to_Utf16(const unsigned ucs, unsigned short *dst, const unsigned dstlen)
static int Ucs_to_Utf16(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_unlink(const char *f)
static int Unlink(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//char *    fl_utf2mbcs(const char *s)
static int Utf2mbcs(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//const char *  fl_utf8back(const char *p, const char *start, const char *end)
static int Utf8back(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_utf8bytes(unsigned ucs)
static int Utf8bytes(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//unsigned  fl_utf8decode(const char *p, const char *end, int *len)
static int Utf8decode(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

static int Utf8encode(lua_State *L) /* redundant with Lua's utf8.char */
    {
    int len;
    char buf[4];
    int ucs = luaL_checkinteger(L, 1);
    len = fl_utf8encode(ucs, buf);
    lua_pushlstring(L, buf, len);
    return 1;
    }


//unsigned  fl_utf8from_mb(char *dst, unsigned dstlen, const char *src, unsigned srclen)
static int Utf8from_mb(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//unsigned  fl_utf8froma(char *dst, unsigned dstlen, const char *src, unsigned srclen)
static int Utf8froma(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//unsigned  fl_utf8fromwc(char *dst, unsigned dstlen, const wchar_t *src, unsigned srclen)
static int Utf8fromwc(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//const char *  fl_utf8fwd(const char *p, const char *start, const char *end)
static int Utf8fwd(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_utf8len(char c)
static int Utf8len(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_utf8len1(char c)
static int Utf8len1(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_utf8locale()
static int Utf8locale(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_utf8test(const char *src, unsigned len)
static int Utf8test(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//unsigned  fl_utf8to_mb(const char *src, unsigned srclen, char *dst, unsigned dstlen)
static int Utf8to_mb(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//unsigned  fl_utf8toa(const char *src, unsigned srclen, char *dst, unsigned dstlen)
static int Utf8toa(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//unsigned  fl_utf8toUtf16(const char *src, unsigned srclen, unsigned short *dst, unsigned dstlen)
static int Utf8toUtf16(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//unsigned  fl_utf8towc(const char *src, unsigned srclen, wchar_t *dst, unsigned dstlen)
static int Utf8towc(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

//int   fl_utf_nb_char(const unsigned char *buf, int len)
static int Utf_nb_char(lua_State *L) // 
    {
    (void)L;
    return 0;
    }

#endif

//int   fl_utf_strcasecmp(const char *s1, const char *s2)
static int Utf_strcasecmp(lua_State *L) // 
    {
    size_t len1;
    const char *s1 = luaL_checklstring(L, 1, &len1);
    size_t len2;
    const char *s2 = luaL_checklstring(L, 2, &len2);
    
    size_t len = (len1 < len2) ? len1 : len2;
    
    int rslt = fl_utf_strncasecmp(s1, s2, len);
    if (rslt != 0)
        {
        lua_pushinteger(L, (rslt < 0) ? -1 : +1);
        }
    else if (len1 != len2)
        {
        lua_pushinteger(L, (len1 < len2) ? -1 : +1);
        }
    else
        {
        lua_pushinteger(L, 0);
        }
    return 1;
    }

#if 0
//int   fl_utf_strncasecmp(const char *s1, const char *s2, int n)
static int Utf_strncasecmp(lua_State *L) // 
    {
    (void)L;
    return 0;
    }
#endif

//int   fl_utf_tolower(const unsigned char *str, int len, char *buf)
static int Utf_tolower(lua_State *L) // 
    {
    size_t len;
    const char *s = luaL_checklstring(L, 1, &len);
    luaL_Buffer buffer;
    char *outPtr = luaL_buffinitsize(L, &buffer, 3 * len + 1);
    int outLen = fl_utf_tolower((const unsigned char*)s, len, outPtr);
    luaL_pushresultsize(&buffer, outLen);
    return 1;
    }

//int   fl_utf_toupper(const unsigned char *str, int len, char *buf)
static int Utf_toupper(lua_State *L) // 
    {
    size_t len;
    const char *s = luaL_checklstring(L, 1, &len);
    luaL_Buffer buffer;
    char *outPtr = luaL_buffinitsize(L, &buffer, 3 * len + 1);
    int outLen = fl_utf_toupper((const unsigned char*)s, len, outPtr);
    luaL_pushresultsize(&buffer, outLen);
    return 1;
    }

#if 0
//int   fl_wcwidth(const char *src)
static int Wcwidth(lua_State *L) // 
    {
    (void)L;
    return 0;
    }
#endif


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
#if 0
        { "access", Access },
        { "chmod", Chmod },
        { "execvp", Execvp },
        { "fopen", Fopen },
        { "getcwd", Getcwd },
        { "getenv", Getenv },
        { "make_path", Make_path },
        { "make_path_for_file", Make_path_for_file },
        { "mkdir", Mkdir },
        { "nonspacing", Nonspacing },
        { "open", Open },
        { "rename", Rename },
        { "rmdir", Rmdir },
        { "stat", Stat },
        { "system", System },
        { "tolower", Tolower },
        { "toupper", Toupper },
        { "ucs_to_utf16", Ucs_to_Utf16 },
        { "unlink", Unlink },
        { "utf2mbcs", Utf2mbcs },
        { "utf8back", Utf8back },
        { "utf8bytes", Utf8bytes },
        { "utf8decode", Utf8decode },
        { "utf8encode", Utf8encode },
        { "utf8from_mb", Utf8from_mb },
        { "utf8froma", Utf8froma },
        { "utf8fromwc", Utf8fromwc },
        { "utf8fwd", Utf8fwd },
        { "utf8len", Utf8len },
        { "utf8len1", Utf8len1 },
        { "utf8locale", Utf8locale },
        { "utf8test", Utf8test },
        { "utf8to_mb", Utf8to_mb },
        { "utf8toa", Utf8toa },
        { "utf8toUtf16", Utf8toUtf16 },
        { "utf8towc", Utf8towc },
        { "utf_nb_char", Utf_nb_char },
#endif
        { "utf_strcasecmp", Utf_strcasecmp },
//      { "utf_strncasecmp", Utf_strncasecmp },
        { "utf_tolower", Utf_tolower },
        { "utf_toupper", Utf_toupper },
#if 0
        { "wcwidth", Wcwidth },
#endif
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_FlUnicode(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

