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

#define GET_BUFFER                                          \
    Fl_Text_Buffer *buf = p->buffer();                      \
    if(!buf)                                                \
        return luaL_error(L, "text_display has no text_buffer");    \

#define CHECK_RANGE(arg_start, arg_end)                     \
    GET_BUFFER                                              \
    int start = checkindex(L, arg_start);                   \
    int end = lua_isnoneornil(L, arg_end) ?                 \
            buf->length() : luaL_checkinteger(L, arg_end);  \
    if(start > buf->length() || (start > end))              \
        return luaL_error(L, "invalid range");

/*------------------------------------------------------------------------------*
 | Text_Display                                                                 |
 *------------------------------------------------------------------------------*/

GROUP_NEW(Text_Display, Text_DisplayNew)
GROUP_DELETE(Text_Display, Text_DisplayDelete)

GETSET_POINTER_OPT(Text_Display, Text_Buffer, buffer, Text_Displaybuffer) 
//@@ unreference/delete the old buffer, if any?

static int Text_Displayredisplay_range(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    CHECK_RANGE(2, 3)
    p->redisplay_range(start, end);
    return 0;
    }


#if 0 // @@ capire meglio
//VOID_INDEX_INDEX(Text_Display, scroll, Text_Displayscroll)
DOUBLE_DOUBLE(Text_Display, x_to_col, Text_Displayx_to_col)
DOUBLE_DOUBLE(Text_Display, col_to_x, Text_Displaycol_to_x)
#endif

VOID_STRING(Text_Display, insert, Text_Displayinsert)
VOID_STRING(Text_Display, overstrike, Text_Displayoverstrike)
GETSET_INDEX(Text_Display, insert_position, Text_Displayinsert_position)
  
static int Text_Displayposition_to_xy(lua_State *L)
    {
    int x, y;
    Fl_Text_Display *p = check_Text_Display(L, 1);
    GET_BUFFER
    int ix = checkindex(L, 2);
    if(ix > buf->length())
        return luaL_error(L, "out of range");
    if(!p->position_to_xy(ix, &x, &y))
        return 0;
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    return 2;
    }

BOOLEAN_INT_INT(Text_Display, in_selection, Text_Displayin_selection)
VOID_VOID(Text_Display, show_insert_position, Text_Displayshow_insert_position)
BOOLEAN_VOID(Text_Display, move_right, Text_Displaymove_right)
BOOLEAN_VOID(Text_Display, move_left, Text_Displaymove_left)
BOOLEAN_VOID(Text_Display, move_up, Text_Displaymove_up)
BOOLEAN_VOID(Text_Display, move_down, Text_Displaymove_down)


static int Text_Displaycount_lines(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    CHECK_RANGE(2, 3)
    int opt = checkboolean(L, 4);
    lua_pushinteger(L, p->count_lines(start, end, opt));
    return 1;
    }


static int Text_Displayline_start(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    GET_BUFFER
    int ix = checkindex(L, 2);
    if(ix > buf->length())
        return luaL_error(L, "out of range");
    pushindex(L,  p->line_start(ix));
    return 1;
    }

static int Text_Displayline_end(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    GET_BUFFER
    int ix = checkindex(L, 2);
    if(ix > buf->length())
        return luaL_error(L, "out of range");
    pushindex(L, p->line_end(ix, optboolean(L, 3, 0)) - 1);
    return 1;
    }

static int Text_Displayskip_lines(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    GET_BUFFER
    int ix = checkindex(L, 2);
    if(ix > buf->length())
        return luaL_error(L, "out of range");
    int nlines = luaL_checkinteger(L, 3);
    int opt = checkboolean(L, 4);
    pushindex(L, p->skip_lines(ix, nlines, opt));
    return 1;
    }

VOID_VOID(Text_Display, next_word, Text_Displaynext_word)
VOID_VOID(Text_Display, previous_word, Text_Displayprevious_word)

static int Text_Displayshow_cursor(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    p->show_cursor(optboolean(L, 2, 1));
    return 0;
    }

GETSET_INT(Text_Display, shortcut, Text_Displayshortcut)
  
GETSET_INTTYPE(Text_Display, Color, cursor_color, Text_Displaycursor_color)
GETSET_INT(Text_Display, scrollbar_width, Text_Displayscrollbar_width)
GETSET_ENUM(Text_Display, Font, textfont, Text_Displaytextfont)
GETSET_INTTYPE(Text_Display, Fontsize, textsize, Text_Displaytextsize)
GETSET_INTTYPE(Text_Display, Color, textcolor, Text_Displaytextcolor)
  
static int Text_Displaywrap_mode(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    int wrap = check_Text_Display_Wrap_Mode(L, 2);
    int margin = luaL_checkinteger(L, 3); 
    p->wrap_mode(wrap, margin);
    return 0;
    }

static int Text_Displayscrollbar_align(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    if(lua_isnone(L, 2)) /* get */  
        return push_Align(L, p->scrollbar_align()); /* multiple return values */
    Fl_Align align = check_Align(L, 2);
    p->scrollbar_align(align);
    return 0;
    }

VOID_VOID(Text_Display, hide_cursor, Text_Displayhide_cursor)

static int Text_Displaycursor_style(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    p->cursor_style(check_Text_Display_Cursor_Style(L, 2));
    return 0;
    }

 
VOID_INT4(Text_Display, resize, Text_Displayresize)
GETSET_INT(Text_Display, linenumber_width, Text_Displaylinenumber_width)
GETSET_ENUM(Text_Display, Font, linenumber_font, Text_Displaylinenumber_font)
GETSET_INTTYPE(Text_Display, Fontsize, linenumber_size, Text_Displaylinenumber_size)
GETSET_INTTYPE(Text_Display, Color, linenumber_fgcolor, Text_Displaylinenumber_fgcolor)
GETSET_INTTYPE(Text_Display, Color, linenumber_bgcolor, Text_Displaylinenumber_bgcolor)
GETSET_STRING(Text_Display, linenumber_format, Text_Displaylinenumber_format)

static int Text_Displaylinenumber_align(lua_State *L)
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    if(lua_isnone(L, 2)) /* get */  
        return push_Align(L, p->linenumber_align()); /* multiple return values */
    Fl_Align align = check_Align(L, 2);
    p->linenumber_align(align);
    return 0;
    }

//@@ METTERE A POSTO:
INDEX_INDEX_INT(Text_Display, rewind_lines, Text_Displayrewind_lines)


static int Text_Displayword_start(lua_State *L) /* moves the cursor */
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    GET_BUFFER
    int ix = checkindex(L, 2);
    if(ix >= buf->length())
        return luaL_error(L, "out of range");
    pushindex(L, p->word_start(ix));
    return 1;
    }
 
static int Text_Displayword_end(lua_State *L) /* moves the cursor */
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    GET_BUFFER
    int ix = checkindex(L, 2);
    if(ix >= buf->length())
        return luaL_error(L, "out of range");
    pushindex(L, p->word_end(ix));
    return 1;
    }


//"Nobody knows what this function does" (FLTK manual): 
//int wrapped_column(int row, int column) const;
//int wrapped_row(int row) const;
 
/*------------------------------------------------------------------------------*
 | Highlight                                                                    |
 *------------------------------------------------------------------------------*/

#if 0
  typedef void (*Unfinished_Style_Cb)(int, void *);
  struct Style_Table_Entry { Fl_Color color; Fl_Font font; Fl_Fontsize size; unsigned attr; };
  void highlight_data(Fl_Text_Buffer *styleBuffer, const Style_Table_Entry *styleTable,
                      int nStyles, char unfinishedStyle,
                      Unfinished_Style_Cb unfinishedHighlightCB, void *cbArg);
#endif
  
static void Callback(int pos, void* ud_)
    {
    ud_t *ud = (ud_t*)ud_;
    Fl_Text_Display *p = (Fl_Text_Display*)(ud->obj);
    lua_State *L = main_lua_state;
    if (!L)
        return;
    if(pushvalue(L, ud->style) != LUA_TFUNCTION)
        { unexpected(L); return; }
    push_Text_Display(L, p);
    pushindex(L, pos);
    if(lua_pcall(L, 2, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }

static int Text_Displayhighlight_data(lua_State *L)
    {
#define ST struct Fl_Text_Display::Style_Table_Entry /* I hate C++ */
    Fl_Text_Display *p = check_Text_Display(L, 1);
    ud_t * ud = userdata(p);
    if(ud->style_table)
        {
        //@@ the FLTK manual states that this function can be used to 
        //   remove the style data,  but it does not explain how...
        //   Passing 0 as stylebuffer would cause a SEGV, I'm afraid...
        return luaL_error(L, "duplicated call");
        }
    Fl_Text_Buffer *style_buffer = check_Text_Buffer(L, 2);
    if(lua_type(L, 3) != LUA_TTABLE)
        return luaL_argerror(L, 3, "style table expected");
    // check and get the style table
    int nstyles = luaL_len(L, 3); // style_table size
    ud->style_table = malloc(nstyles*sizeof(ST));
    memset(ud->style_table, 0, nstyles*sizeof(ST)); 

    ST* stable = (ST*)ud->style_table;
    for(int i = 0; i < nstyles; i++)
        {
        lua_rawgeti(L, 3, i+1);
        int table_index = lua_gettop(L);
        if(lua_type(L, table_index ) != LUA_TTABLE)
            return luaL_argerror(L, 3, "malformatted style table");
        lua_rawgeti(L, table_index, 1);
        stable[i].color = luaL_checkinteger(L, -1);
        lua_rawgeti(L, table_index, 2);
        stable[i].font = luaL_checkinteger(L, -1);
        lua_rawgeti(L, table_index, 3);
        stable[i].size = luaL_checkinteger(L, -1);
        lua_rawgeti(L, table_index, 4);
        stable[i].attr = luaL_optinteger(L, -1, 0);
        lua_pop(L, 5);
        }

    if(lua_isnoneornil(L, 4)) /* don't use callback */
        {
        p->highlight_data(style_buffer, (ST*)ud->style_table, nstyles, 'A', 0, 0);
        return 0;
        }

    char unfstyle = checkascii(L, 4);
    if((unfstyle < 'A') or (unfstyle > ('A' + nstyles -1)))
        return luaL_argerror(L, 4, "out of range");

    if(lua_type(L, 5) != LUA_TFUNCTION)
        return luaL_argerror(L, 5, "function expected");
    reference(L, ud->style, 5);
    p->highlight_data(style_buffer, (ST*)ud->style_table, nstyles, unfstyle, Callback, (void*)ud);
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

#if 0 /* 8 yy */
GETSET_INT(Text_Display, , Text_Display)
        { "", Text_Display },
(Text_Display, , Text_Display)
GETSET_INT(Text_Display, , Text_Display)
        { "", Text_Display },
static int Text_Display(lua_State *L) //@@ 
    {
    Fl_Text_Display *p = check_Text_Display(L, 1);
    (void)p;
    return 0;
    }

#endif


static const struct luaL_Reg Text_DisplayMethods[] = 
    {
        { "buffer", Text_Displaybuffer },
//      { "col_to_x", Text_Displaycol_to_x },
        { "count_lines", Text_Displaycount_lines },
        { "cursor_color", Text_Displaycursor_color },
        { "cursor_style", Text_Displaycursor_style },
        { "hide_cursor", Text_Displayhide_cursor },
        { "highlight_data", Text_Displayhighlight_data },
        { "in_selection", Text_Displayin_selection },
        { "insert", Text_Displayinsert },
        { "insert_position", Text_Displayinsert_position },
        { "line_end", Text_Displayline_end },
        { "line_start", Text_Displayline_start },
        { "linenumber_align", Text_Displaylinenumber_align },
        { "linenumber_bgcolor", Text_Displaylinenumber_bgcolor },
        { "linenumber_fgcolor", Text_Displaylinenumber_fgcolor },
        { "linenumber_font", Text_Displaylinenumber_font },
        { "linenumber_format", Text_Displaylinenumber_format },
        { "linenumber_size", Text_Displaylinenumber_size },
        { "linenumber_width", Text_Displaylinenumber_width },
        { "move_down", Text_Displaymove_down },
        { "move_left", Text_Displaymove_left },
        { "move_right", Text_Displaymove_right },
        { "move_up", Text_Displaymove_up },
        { "next_word", Text_Displaynext_word },
        { "overstrike", Text_Displayoverstrike },
        { "position_to_xy", Text_Displayposition_to_xy },
        { "previous_word", Text_Displayprevious_word },
        { "redisplay_range", Text_Displayredisplay_range },
        { "resize", Text_Displayresize },
        { "rewind_lines", Text_Displayrewind_lines },
//      { "scroll", Text_Displayscroll },
        { "scrollbar_align", Text_Displayscrollbar_align },
        { "scrollbar_width", Text_Displayscrollbar_width },
        { "shortcut", Text_Displayshortcut },
        { "show_cursor", Text_Displayshow_cursor },
        { "show_insert_position", Text_Displayshow_insert_position },
        { "skip_lines", Text_Displayskip_lines },
        { "textcolor", Text_Displaytextcolor },
        { "textfont", Text_Displaytextfont },
        { "textsize", Text_Displaytextsize },
        { "word_start", Text_Displayword_start },
        { "word_end", Text_Displayword_end },
        { "wrap_mode", Text_Displaywrap_mode },
//      { "x_to_col", Text_Displayx_to_col },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Text_DisplayMetaMethods[] = 
    {
        { "__gc",  Text_DisplayDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Text_DisplayFunctions[] = 
    {
        { "text_display",  Text_DisplayNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Text_Display(lua_State *L)
    {
    udata_define(L, MT_Text_Display, Text_DisplayMethods, Text_DisplayMetaMethods);
    udata_inherit(L, MT_Text_Display, MT_Group);
    luaL_setfuncs(L, Text_DisplayFunctions, 0);
    }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#if(0)

class FL_EXPORT Fl_Text_Display: public Fl_Group {
public:
  /**
   the character position is the left edge of a character, whereas 
   the cursor is thought to be between the centers of two consecutive
   characters.
   */
  enum {
    CURSOR_POS, 
    CHARACTER_POS
  };
  
  /** 
   drag types - they match Fl::event_clicks() so that single clicking to
   start a collection selects by character, double clicking selects by
   word and triple clicking selects by line.
   */
  enum {
    DRAG_NONE = -2,
    DRAG_START_DND = -1,
    DRAG_CHAR = 0, 
    DRAG_WORD = 1, 
    DRAG_LINE = 2
  };
  
  friend void fl_text_drag_me(int pos, Fl_Text_Display* d);
  virtual int handle(int e);
  
//   int position_style(int lineStartPos, int lineLen, int lineIndex) const; @@ ?!

};

#endif
