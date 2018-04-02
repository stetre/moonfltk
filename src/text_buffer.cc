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
#include <errno.h>

#define CHECK_RANGE(arg_start, arg_end)                     \
    int start = checkindex(L, arg_start);                   \
    int end = lua_isnoneornil(L, arg_end) ?                 \
            p->length() : luaL_checkinteger(L, arg_end); /* already has the + 1 in it */\
    if(start > p->length() || (start > end))                \
        return luaL_error(L, "invalid range");

/*------------------------------------------------------------------------------*
 | Fl_Text_Buffer (not a widget)                                                |
 *------------------------------------------------------------------------------*/

static int Text_BufferNew(lua_State *L)
    {
    int requestedSize = luaL_optinteger(L, 1, 0);
    int preferredGapSize = luaL_optinteger(L, 2, 1024);
    Fl_Text_Buffer *p = new Fl_Text_Buffer(requestedSize, preferredGapSize);
    DBG_CREATE(Text_Buffer, p);
    newuserdata(L, p, MT_Text_Buffer);
    return 1;
    }

static int Text_BufferDelete(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    DBG_DELETE(Text_Buffer, p);
    if(freeuserdata(L, p)) delete p;
    return 0;
    }

INT_VOID(Text_Buffer, length, Text_Bufferlength)

static int Text_Buffernext_char(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int ix = lua_isnoneornil(L, 2) ? -1 : checkindex(L, 2);
    ix = p->next_char(ix);
    if(ix >= p->length())
        return 0; // end reached
    pushindex(L, ix);
    pushindex(L, p->next_char(ix)-1); /* char end */
    return 2;
    }

static int Text_Bufferprev_char(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int buflen = p->length();
    int ix = lua_isnoneornil(L, 2) ? buflen : checkindex(L, 2);
    if(ix > p->length()) 
        ix = p->length();
    ix = p->prev_char(ix);
    if(ix < 0)  
        return 0; // beginning reached
    pushindex(L, ix);
    pushindex(L, p->next_char(ix)-1); /* char end */
    return 2;
    }


static int Text_Bufferbyte_at(lua_State *L) /* -> raw byte */
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int ix = checkindex(L, 2);
    if(ix >= p->length())
        return luaL_argerror(L, 2, "out of range");
    unsigned char b = (unsigned char)(p->byte_at(ix));
    lua_pushinteger(L, b);
    return 1;
    }

static int Text_Bufferchar_at(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int ix = checkindex(L, 2);
    if(ix >= p->length())
        return luaL_argerror(L, 2, "out of range");
    ix = p->utf8_align(ix);
    char *buf = p->address(ix);
    int len = p->next_char(ix) - ix;
    unsigned int ucs = p->char_at(ix); /* codepoint, actually */
#if 0
    char buf[4];
    int len = fl_utf8encode(ucs, buf);
#endif
    lua_pushlstring(L, buf, len);
    lua_pushinteger(L, ucs);
    return 2;
    }


static inline int Text_Buffertext(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    if(lua_isnone(L, 2))
        {
        char *s = p->text();
        if(!s) return unexpected(L);
        lua_pushstring(L, s);
        free(s);
        return 1;
        }
    p->text(luaL_checkstring(L, 2));
    return 0;
    }   

static int Text_Bufferinsert(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int pos = checkindex(L, 2);
    const char *s = luaL_checkstring(L, 3);
    p->insert(pos, s);
    return 0;
    }

static int Text_Bufferappend(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    const char *s = luaL_checkstring(L, 2);
    p->append(s);
    return 0;
    }


static int Text_Buffertext_range(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    CHECK_RANGE(2, 3)
    char *s = p->text_range(start, end);
    if(!s) return unexpected(L);
    lua_pushstring(L, s);
    free(s);
    return 1;
    }

static int Text_Bufferremove(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    CHECK_RANGE(2, 3)
    p->remove(start, end);
    return 0;
    }

static int Text_Bufferreplace(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    CHECK_RANGE(2, 3)
    const char *s = luaL_checkstring(L, 4);
    p->replace(start, end, s);
    return 0;
    }

static int Text_Buffercopy(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    Fl_Text_Buffer *from = check_Text_Buffer(L, 2);
    CHECK_RANGE(3, 4)
    int pos = checkindex(L, 5);
    p->copy(from, start, end, pos);
    return 0;
    }

static int Text_Bufferselect(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    CHECK_RANGE(2, 3)
    p->select(start, end);
    return 0;
    }

static int Text_Buffersecondary_select(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    CHECK_RANGE(2, 3)
    p->secondary_select(start, end);
    return 0;
    }

BOOLEAN_VOID(Text_Buffer, selected, Text_Bufferselected)
BOOLEAN_VOID(Text_Buffer, secondary_selected, Text_Buffersecondary_selected)
VOID_VOID(Text_Buffer, unselect, Text_Bufferunselect)
VOID_VOID(Text_Buffer, secondary_unselect, Text_Buffersecondary_unselect)
VOID_VOID(Text_Buffer, unhighlight, Text_Bufferunhighlight)
VOID_VOID(Text_Buffer, remove_selection, Text_Bufferremove_selection)
VOID_VOID(Text_Buffer, remove_secondary_selection, Text_Bufferremove_secondary_selection)
VOID_STRING(Text_Buffer, replace_selection, Text_Bufferreplace_selection)
VOID_STRING(Text_Buffer, replace_secondary_selection, Text_Bufferreplace_secondary_selection)

#define F(method)                               \
static int Text_Buffer##method(lua_State *L)    \
    {                                           \
    int start, end;                             \
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);\
    int rc = p->method(&start, &end);           \
    if(!rc) return 0;                           \
    pushindex(L, start);                        \
    pushindex(L, end-1);                        \
    return 2;                                   \
    }
F(selection_position)
F(secondary_selection_position)
F(highlight_position)
#undef F

#define F(method)                               \
static int Text_Buffer##method(lua_State *L)    \
    {                                           \
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);\
    char *s = p->method();                      \
    if(!s) return unexpected(L);                \
    lua_pushstring(L, s);                       \
    free(s);                                    \
    return 1;                                   \
    }
F(selection_text)
F(secondary_selection_text)
F(highlight_text)
#undef F

static int Text_Bufferhighlight(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushboolean(L, p->highlight());
        return 1;
        }
    CHECK_RANGE(2, 3);
    p->highlight(start, end);
    return 0;
    }


GETSET_INT(Text_Buffer, tab_distance, Text_Buffertab_distance)
VOID_BOOLEAN(Text_Buffer, canUndo, Text_Buffercan_undo)


static int Text_Bufferundo(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int pos;
    if(!p->undo(&pos))
        return 0;
    pushindex(L, pos);
    return 1;
    }

static int Text_Bufferinsertfile(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    const char *fname = luaL_checkstring(L, 2);
    int pos = checkindex(L, 3);
    int buflen = luaL_optinteger(L, 4, 128*1024);
    int rc = p->insertfile(fname, pos, buflen);
    int errnum = errno;
    lua_pushboolean(L, rc == 0);
    if(rc==0)
        return 1;
    lua_pushstring(L, strerror(errnum));
    return 2;
    }

#define F(method)                                   \
static int Text_Buffer##method(lua_State *L)        \
    {                                               \
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);    \
    const char *fname = luaL_checkstring(L, 2);     \
    int buflen = luaL_optinteger(L, 3, 128*1024);   \
    int rc = p->method(fname, buflen);              \
    int errnum = errno;                             \
    lua_pushboolean(L, rc == 0);                    \
    if(rc==0)                                       \
        return 1;                                   \
    lua_pushstring(L, strerror(errnum));            \
    return 2;                                       \
    }
F(appendfile)
F(loadfile)
F(savefile)
#undef F

static int Text_Bufferoutputfile(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    const char *fname = luaL_checkstring(L, 2);
    int start = checkindex(L, 3);
    int end = checkindex(L, 4);
    int buflen = luaL_optinteger(L, 5, 128*1024);
    int rc = p->outputfile(fname, start, end, buflen);
    int errnum = errno;
    lua_pushboolean(L, rc == 0);
    if(rc==0)
        return 1;
    lua_pushstring(L, strerror(errnum));
    return 2;
    }

static int Text_Bufferline_start(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int ix = checkindex(L, 2);
    if(ix > p->length()) return luaL_error(L, "out of range");
    pushindex(L,  p->line_start(ix));
    return 1;
    }

static int Text_Bufferline_end(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int ix = checkindex(L, 2);
    if(ix > p->length()) return luaL_error(L, "out of range");
    pushindex(L,  p->line_end(ix) - 1);
    return 1;
    }

static int Text_Bufferline_text(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int pos = checkindex(L, 2);
    char *s = p->line_text(pos);
    if(!s) return unexpected(L);
    lua_pushstring(L, s);
    free(s);
    return 1;
    }

static int Text_Bufferword(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int ix = checkindex(L, 2);
    if(ix > p->length()) return luaL_error(L, "out of range");
    ix = p->utf8_align(ix);
    int start = p->word_start(ix);
    int end = p->word_end(ix) - 1;
    if(end < start) /* @@ mah ...*/
        { int tmp = start; start = end+1; end = tmp - 1; }
    pushindex(L, start);
    pushindex(L, end);
    return 2;
    }

static int Text_Bufferword_at(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    int ix = checkindex(L, 2);
    if(ix > p->length()) return luaL_error(L, "out of range");
    ix = p->utf8_align(ix);
    int start = p->word_start(ix);
    int end = p->word_end(ix) - 1;
    if(end < start) /* @@ mah ...*/
        { int tmp = start; start = end+1; end = tmp - 1; }
    char *buf = p->address(start);
    int len = end - start +1;
    lua_pushlstring(L, buf, len);
    return 1;
    }


static int Text_Buffercount_displayed_characters(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    CHECK_RANGE(2, 3)
    lua_pushinteger(L, p->count_displayed_characters(start, end));
    return 1;
    }

static int Text_Buffercount_lines(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    CHECK_RANGE(2, 3)
    int count = p->count_lines(start, end); 
    /* NOTE: count_lines() actually counts the number of newline characters ('\n') */
    lua_pushinteger(L, count);
    return 1;
    }

INT_INDEX_INT(Text_Buffer, skip_displayed_characters, Text_Bufferskip_displayed_characters)
INDEX_INDEX_INT(Text_Buffer, skip_lines, Text_Bufferskip_lines)
INDEX_INDEX_INT(Text_Buffer, rewind_lines, Text_Bufferrewind_lines)



#define F(method)                                       \
static int Text_Buffer##method(lua_State *L)            \
    {                                                   \
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);        \
    int start = checkindex(L, 2);                       \
    size_t len;                                         \
    const char *s = luaL_checklstring(L, 3, &len);      \
    unsigned int c = fl_utf8decode(s, s+len, (int*)&len);\
    int pos;                                            \
    if(!p->method(start, c, &pos))                      \
        return 0;                                       \
    pushindex(L, pos);                                  \
    return 1;                                           \
    }
F(findchar_forward)
F(findchar_backward)
#undef F

#define F(method)                               \
static int Text_Buffer##method(lua_State *L)    \
    {                                           \
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);\
    int start = checkindex(L, 2);       \
    const char *s = luaL_checkstring(L, 3);     \
    int matchcase = optboolean(L, 4, 0);        \
    int pos;                                    \
    if(!p->method(start, s, &pos, matchcase))   \
        return 0;                               \
    pushindex(L, pos);                          \
    return 1;                                   \
    }
F(search_forward)
F(search_backward)
#undef F

//BOOLEAN_INT(Text_Buffer, prev_char_clipped, Text_Bufferprev_char_clipped) //@@??
//BOOLEAN_INT(Text_Buffer, next_char_clipped, Text_Buffernext_char_clipped) //@@??
INDEX_INDEX(Text_Buffer, utf8_align, Text_Bufferutf8_align)
  
static int Text_Bufferinput_file_was_transcoded(lua_State *L)
    {
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);
    lua_pushboolean(L, p->input_file_was_transcoded);
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Callbacks                                                                    |
 *------------------------------------------------------------------------------*/
/* Note: FLTK allows to register multiple callbacks (+ arg).
 *       MoonFLTK allows to register a single callback (+ arg):
 *       if multiple callbacks are needed, multiplex it in Lua.
 */

static void Callback_modify(int pos, int nins, int ndel, int nrest, const char* deltext, void* ud_)
    {
    ud_t *ud = (ud_t*)ud_;
    Fl_Text_Buffer *p = (Fl_Text_Buffer*)(ud->obj);
    lua_State *L = main_lua_state;
    if (!L) 
        return;
    if(pushvalue(L, ud->modify) != LUA_TFUNCTION)
        { unexpected(L); return; }
    int nargs = 6;
    push_Text_Buffer(L, p);
    pushindex(L, pos);
    lua_pushinteger(L, nins);
    lua_pushinteger(L, ndel);
    lua_pushinteger(L, nrest);
    lua_pushstring(L, deltext ? deltext : "");
    if(ud->modify_arg != LUA_NOREF)
        { pushvalue(L, ud->modify_arg); nargs++; }
    if(lua_pcall(L, nargs, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }
  
static void Callback_predelete(int pos, int ndel, void* ud_)
    {
    ud_t *ud = (ud_t*)ud_;
    Fl_Text_Buffer *p = (Fl_Text_Buffer*)(ud->obj);
    lua_State *L = main_lua_state;
    if (!L)
        return;
    if(pushvalue(L, ud->predelete) != LUA_TFUNCTION)
        { unexpected(L); return; }
    int nargs = 3;
    push_Text_Buffer(L, p);
    pushindex(L, pos);
    lua_pushinteger(L, ndel);
    if(ud->predelete_arg != LUA_NOREF)
        { pushvalue(L, ud->modify_arg); nargs++; }
    if(lua_pcall(L, nargs, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }
 

#define F(what)                                                 \
static int Text_Buffer_##what##_callback(lua_State *L)          \
    {                                                           \
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);                \
    ud_t *ud = userdata(p);                                     \
    if(ud->what != LUA_NOREF)                                   \
        {                                                       \
        p->remove_##what##_callback(Callback_##what, (void*)ud);\
        unreference(L, ud->what);                               \
        unreference(L, ud->what##_arg);                         \
        }                                                       \
    if(lua_isnoneornil(L, 2)) /*remove */                       \
        return 0;                                               \
    if(!lua_isfunction(L, 2))                                   \
        return luaL_argerror(L, 2, "function expected");        \
    unreference(L, ud->what);                                   \
    reference(L, ud->what, 2);                                  \
    unreference(L, ud->what##_arg);                             \
    if(!lua_isnoneornil(L, 3))                                  \
        reference(L, ud->what##_arg, 3);                        \
    p->add_##what##_callback(Callback_##what, (void*)ud);       \
    return 0;                                                   \
    }                                                           \
                                                                \
static int Text_Buffercall_##what##_callback(lua_State *L)      \
    {                                                           \
    Fl_Text_Buffer *p = check_Text_Buffer(L, 1);                \
    p->call_##what##_callbacks();                               \
    return 0;                                                   \
    }

F(modify)
F(predelete)

#undef F

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Text_BufferMethods[] = 
    {
        { "append", Text_Bufferappend },
        { "appendfile", Text_Bufferappendfile },
        { "byte_at", Text_Bufferbyte_at },
        { "can_undo", Text_Buffercan_undo },
        { "char_at", Text_Bufferchar_at },
        { "copy", Text_Buffercopy },
        { "count_displayed_characters", Text_Buffercount_displayed_characters },
        { "count_lines", Text_Buffercount_lines },
        { "findchar_backward", Text_Bufferfindchar_backward },
        { "findchar_forward", Text_Bufferfindchar_forward },
        { "highlight", Text_Bufferhighlight },
        { "highlight_position", Text_Bufferhighlight_position },
        { "highlight_text", Text_Bufferhighlight_text },
        { "input_file_was_transcoded", Text_Bufferinput_file_was_transcoded },
        { "insert", Text_Bufferinsert },
        { "insertfile", Text_Bufferinsertfile },
        { "length", Text_Bufferlength },
        { "line_end", Text_Bufferline_end },
        { "line_start", Text_Bufferline_start },
        { "line_text", Text_Bufferline_text },
        { "loadfile", Text_Bufferloadfile },
        { "next_char", Text_Buffernext_char },
//      { "next_char_clipped", Text_Buffernext_char_clipped },
        { "outputfile", Text_Bufferoutputfile },
        { "prev_char", Text_Bufferprev_char },
//      { "prev_char_clipped", Text_Bufferprev_char_clipped },
        { "remove", Text_Bufferremove },
        { "remove_secondary_selection", Text_Bufferremove_secondary_selection },
        { "remove_selection", Text_Bufferremove_selection },
        { "replace", Text_Bufferreplace },
        { "replace_secondary_selection", Text_Bufferreplace_secondary_selection },
        { "replace_selection", Text_Bufferreplace_selection },
        { "rewind_lines", Text_Bufferrewind_lines },
        { "savefile", Text_Buffersavefile },
        { "search_backward", Text_Buffersearch_backward },
        { "search_forward", Text_Buffersearch_forward },
        { "secondary_select", Text_Buffersecondary_select },
        { "secondary_selected", Text_Buffersecondary_selected },
        { "secondary_selection_position", Text_Buffersecondary_selection_position },
        { "secondary_selection_text", Text_Buffersecondary_selection_text },
        { "secondary_unselect", Text_Buffersecondary_unselect },
        { "select", Text_Bufferselect },
        { "selected", Text_Bufferselected },
        { "selection_position", Text_Bufferselection_position },
        { "selection_text", Text_Bufferselection_text },
        { "skip_displayed_characters", Text_Bufferskip_displayed_characters },
        { "skip_lines", Text_Bufferskip_lines },
        { "tab_distance", Text_Buffertab_distance },
        { "text", Text_Buffertext },
        { "text_range", Text_Buffertext_range },
        { "undo", Text_Bufferundo },
        { "unhighlight", Text_Bufferunhighlight },
        { "unselect", Text_Bufferunselect },
        { "utf8_align", Text_Bufferutf8_align },
        { "word", Text_Bufferword },
        { "word_at", Text_Bufferword_at },
// callbacks
        { "modify_callback", Text_Buffer_modify_callback },
        { "predelete_callback", Text_Buffer_predelete_callback },
        { "call_modify_callback", Text_Buffercall_modify_callback },
        { "call_predelete_callback", Text_Buffercall_predelete_callback },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Text_BufferMetaMethods[] = 
    {
        { "__gc",  Text_BufferDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Text_BufferFunctions[] = 
    {
        { "text_buffer",  Text_BufferNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Text_Buffer(lua_State *L)
    {
    udata_define(L, MT_Text_Buffer, Text_BufferMethods, Text_BufferMetaMethods);
    luaL_setfuncs(L, Text_BufferFunctions, 0);
    }

#if 0 

//NO (pointers):
//const char *address(int pos) const
//char *address(int pos)
//NO (internal use only(?)):
//const Fl_Text_Selection* primary_selection() const { return &mPrimary; }
//Fl_Text_Selection* primary_selection() { return &mPrimary; }
//const Fl_Text_Selection* secondary_selection() const { return &mSecondary; }
//const Fl_Text_Selection* highlight_selection() const { return &mHighlight; }

//static const char* file_encoding_warning_message; @@
//void (*transcoding_warning_action)(Fl_Text_Buffer*); @@

#endif
