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
 | Fl_Option                                                            |
 *----------------------------------------------------------------------*/

Fl::Fl_Option check_Option(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
    CASE(Fl::OPTION_ARROW_FOCUS, "arrow focus");
    CASE(Fl::OPTION_VISIBLE_FOCUS, "visible focus");
    CASE(Fl::OPTION_DND_TEXT, "dnd text");
    CASE(Fl::OPTION_SHOW_TOOLTIPS, "show tooltips");
    CASE(Fl::OPTION_FNFC_USES_GTK, "uses gtk");
//    CASE(Fl::OPTION_LAST, "last");
#undef CASE
    return (Fl::Fl_Option)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Option(lua_State *L, Fl::Fl_Option option)
    {
    switch(option)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
    CASE(Fl::OPTION_ARROW_FOCUS, "arrow focus");
    CASE(Fl::OPTION_VISIBLE_FOCUS, "visible focus");
    CASE(Fl::OPTION_DND_TEXT, "dnd text");
    CASE(Fl::OPTION_SHOW_TOOLTIPS, "show tooltips");
    CASE(Fl::OPTION_FNFC_USES_GTK, "uses gtk");
//    CASE(Fl::OPTION_LAST, "last");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | Fl_Boxtype                                                           |
 *----------------------------------------------------------------------*/

Fl_Boxtype check_Boxtype(lua_State *L, int arg)
    {
    if (lua_type(L, arg) == LUA_TSTRING)
        {
        const char *s = lua_tostring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_NO_BOX, "no box");
        CASE(FL_FLAT_BOX, "flat box");
        CASE(FL_UP_BOX, "up box");
        CASE(FL_DOWN_BOX, "down box");
        CASE(FL_UP_FRAME, "up frame");
        CASE(FL_DOWN_FRAME, "down frame");
        CASE(FL_THIN_UP_BOX, "thin up box");
        CASE(FL_THIN_DOWN_BOX, "thin down box");
        CASE(FL_THIN_UP_FRAME, "thin up frame");
        CASE(FL_THIN_DOWN_FRAME, "thin down frame");
        CASE(FL_ENGRAVED_BOX, "engraved box");
        CASE(FL_EMBOSSED_BOX, "embossed box");
        CASE(FL_ENGRAVED_FRAME, "engraved frame");
        CASE(FL_EMBOSSED_FRAME, "embossed frame");
        CASE(FL_BORDER_BOX, "border box");
        CASE(FL_SHADOW_BOX, "shadow box");
        CASE(FL_BORDER_FRAME, "border frame");
        CASE(FL_SHADOW_FRAME, "shadow frame");
        CASE(FL_ROUNDED_BOX, "rounded box");
        CASE(FL_RSHADOW_BOX, "rshadow box");
        CASE(FL_ROUNDED_FRAME, "rounded frame");
        CASE(FL_RFLAT_BOX, "rflat box");
        CASE(FL_ROUND_UP_BOX, "round up box");
        CASE(FL_ROUND_DOWN_BOX, "round down box");
        CASE(FL_DIAMOND_UP_BOX, "diamond up box");
        CASE(FL_DIAMOND_DOWN_BOX, "diamond down box");
        CASE(FL_OVAL_BOX, "oval box");
        CASE(FL_OSHADOW_BOX, "oshadow box");
        CASE(FL_OVAL_FRAME, "oval frame");
        CASE(FL_OFLAT_BOX, "oflat box");
        CASE(FL_PLASTIC_UP_BOX, "plastic up box");
        CASE(FL_PLASTIC_DOWN_BOX, "plastic down box");
        CASE(FL_PLASTIC_UP_FRAME, "plastic up frame");
        CASE(FL_PLASTIC_DOWN_FRAME, "plastic down frame");
        CASE(FL_PLASTIC_THIN_UP_BOX, "plastic thin up box");
        CASE(FL_PLASTIC_THIN_DOWN_BOX, "plastic thin down box");
        CASE(FL_PLASTIC_ROUND_UP_BOX, "plastic round up box");
        CASE(FL_PLASTIC_ROUND_DOWN_BOX, "plastic round down box");
        CASE(FL_GTK_UP_BOX, "gtk up box");
        CASE(FL_GTK_DOWN_BOX, "gtk down box");
        CASE(FL_GTK_UP_FRAME, "gtk up frame");
        CASE(FL_GTK_DOWN_FRAME, "gtk down frame");
        CASE(FL_GTK_THIN_UP_BOX, "gtk thin up box");
        CASE(FL_GTK_THIN_DOWN_BOX, "gtk thin down box");
        CASE(FL_GTK_THIN_UP_FRAME, "gtk thin up frame");
        CASE(FL_GTK_THIN_DOWN_FRAME, "gtk thin down frame");
        CASE(FL_GTK_ROUND_UP_BOX, "gtk round up box");
        CASE(FL_GTK_ROUND_DOWN_BOX, "gtk round down box");
        CASE(FL_GLEAM_UP_BOX, "gleam up box");
        CASE(FL_GLEAM_DOWN_BOX, "gleam down box");
        CASE(FL_GLEAM_UP_FRAME, "gleam up frame");
        CASE(FL_GLEAM_DOWN_FRAME, "gleam down frame");
        CASE(FL_GLEAM_THIN_UP_BOX, "gleam thin up box");
        CASE(FL_GLEAM_THIN_DOWN_BOX, "gleam thin down box");
        CASE(FL_GLEAM_ROUND_UP_BOX, "gleam round up box");
        CASE(FL_GLEAM_ROUND_DOWN_BOX, "gleam round down box");
        CASE(FL_FREE_BOXTYPE, "free boxtype");
#undef CASE
        return (Fl_Boxtype)luaL_argerror(L, arg, badvalue(L,s));
        }
    else
        {
        int i = luaL_checkinteger(L, arg);
        if (i < 0 || i > 255) 
            luaL_argerror(L, arg, "string or integer between 0 and 255 expected");
        return (Fl_Boxtype)i;
        }
    }

int push_Boxtype(lua_State *L, Fl_Boxtype boxtype)
    {
    int bt;
#define CASE(CODE,str) if(boxtype==CODE) do { lua_pushstring(L, str); return 1; } while(0)
        CASE(FL_NO_BOX, "no box");
        CASE(FL_FLAT_BOX, "flat box");
        CASE(FL_UP_BOX, "up box");
        CASE(FL_DOWN_BOX, "down box");
        CASE(FL_UP_FRAME, "up frame");
        CASE(FL_DOWN_FRAME, "down frame");
        CASE(FL_THIN_UP_BOX, "thin up box");
        CASE(FL_THIN_DOWN_BOX, "thin down box");
        CASE(FL_THIN_UP_FRAME, "thin up frame");
        CASE(FL_THIN_DOWN_FRAME, "thin down frame");
        CASE(FL_ENGRAVED_BOX, "engraved box");
        CASE(FL_EMBOSSED_BOX, "embossed box");
        CASE(FL_ENGRAVED_FRAME, "engraved frame");
        CASE(FL_EMBOSSED_FRAME, "embossed frame");
        CASE(FL_BORDER_BOX, "border box");
        CASE(FL_SHADOW_BOX, "shadow box");
        CASE(FL_BORDER_FRAME, "border frame");
        CASE(FL_SHADOW_FRAME, "shadow frame");
        CASE(FL_ROUNDED_BOX, "rounded box");
        CASE(FL_RSHADOW_BOX, "rshadow box");
        CASE(FL_ROUNDED_FRAME, "rounded frame");
        CASE(FL_RFLAT_BOX, "rflat box");
        CASE(FL_ROUND_UP_BOX, "round up box");
        CASE(FL_ROUND_DOWN_BOX, "round down box");
        CASE(FL_DIAMOND_UP_BOX, "diamond up box");
        CASE(FL_DIAMOND_DOWN_BOX, "diamond down box");
        CASE(FL_OVAL_BOX, "oval box");
        CASE(FL_OSHADOW_BOX, "oshadow box");
        CASE(FL_OVAL_FRAME, "oval frame");
        CASE(FL_OFLAT_BOX, "oflat box");
        CASE(FL_PLASTIC_UP_BOX, "plastic up box");
        CASE(FL_PLASTIC_DOWN_BOX, "plastic down box");
        CASE(FL_PLASTIC_UP_FRAME, "plastic up frame");
        CASE(FL_PLASTIC_DOWN_FRAME, "plastic down frame");
        CASE(FL_PLASTIC_THIN_UP_BOX, "plastic thin up box");
        CASE(FL_PLASTIC_THIN_DOWN_BOX, "plastic thin down box");
        CASE(FL_PLASTIC_ROUND_UP_BOX, "plastic round up box");
        CASE(FL_PLASTIC_ROUND_DOWN_BOX, "plastic round down box");
        CASE(FL_GTK_UP_BOX, "gtk up box");
        CASE(FL_GTK_DOWN_BOX, "gtk down box");
        CASE(FL_GTK_UP_FRAME, "gtk up frame");
        CASE(FL_GTK_DOWN_FRAME, "gtk down frame");
        CASE(FL_GTK_THIN_UP_BOX, "gtk thin up box");
        CASE(FL_GTK_THIN_DOWN_BOX, "gtk thin down box");
        CASE(FL_GTK_THIN_UP_FRAME, "gtk thin up frame");
        CASE(FL_GTK_THIN_DOWN_FRAME, "gtk thin down frame");
        CASE(FL_GTK_ROUND_UP_BOX, "gtk round up box");
        CASE(FL_GTK_ROUND_DOWN_BOX, "gtk round down box");
        CASE(FL_GLEAM_UP_BOX, "gleam up box");
        CASE(FL_GLEAM_DOWN_BOX, "gleam down box");
        CASE(FL_GLEAM_UP_FRAME, "gleam up frame");
        CASE(FL_GLEAM_DOWN_FRAME, "gleam down frame");
        CASE(FL_GLEAM_THIN_UP_BOX, "gleam thin up box");
        CASE(FL_GLEAM_THIN_DOWN_BOX, "gleam thin down box");
        CASE(FL_GLEAM_ROUND_UP_BOX, "gleam round up box");
        CASE(FL_GLEAM_ROUND_DOWN_BOX, "gleam round down box");
        CASE(FL_FREE_BOXTYPE, "free boxtype");
#undef CASE
//      default:
//          return unexpected(L);
//      }
    bt = (int)boxtype; /* to avoid -Wtautological-constant-compare warning (clang) */
    if (0 <= bt && bt <= 255)
        {
        lua_pushinteger(L, bt);
        return 1;
        }
    return unexpected(L);
    }

/*----------------------------------------------------------------------*
 | Fl_Labeltype                                                         |
 *----------------------------------------------------------------------*/

Fl_Labeltype check_Labeltype(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_NORMAL_LABEL, "normal");
        CASE(FL_NO_LABEL, "none");
        CASE(FL_SHADOW_LABEL, "shadow");
        CASE(FL_ENGRAVED_LABEL, "engraved");
        CASE(FL_EMBOSSED_LABEL, "embossed");
        //CASE(_FL_MULTI_LABEL, "multi");
        //CASE(_FL_ICON_LABEL, "icon");
        //CASE(_FL_IMAGE_LABEL, "image");
        //CASE(FL_FREE_LABELTYPE, "free");
#undef CASE
    return (Fl_Labeltype)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Labeltype(lua_State *L, Fl_Labeltype labeltype)
    {
#define CASE(CODE,str) if(labeltype==CODE) do { lua_pushstring(L, str); return 1; } while(0)
        CASE(FL_NORMAL_LABEL, "normal");
        CASE(FL_NO_LABEL, "none");
        CASE(FL_SHADOW_LABEL, "shadow");
        CASE(FL_ENGRAVED_LABEL, "engraved");
        CASE(FL_EMBOSSED_LABEL, "embossed");
        //CASE(_FL_MULTI_LABEL, "multi");
        //CASE(_FL_ICON_LABEL, "icon");
        //CASE(_FL_IMAGE_LABEL, "image");
        //CASE(FL_FREE_LABELTYPE, "free");
#undef CASE
    return unexpected(L);
    }


/*----------------------------------------------------------------------*
 | Fl_Cursor                                                            |
 *----------------------------------------------------------------------*/

Fl_Cursor check_Cursor(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_CURSOR_DEFAULT, "default");
        CASE(FL_CURSOR_ARROW, "arrow");
        CASE(FL_CURSOR_CROSS, "cross");
        CASE(FL_CURSOR_WAIT, "wait");
        CASE(FL_CURSOR_INSERT, "insert");
        CASE(FL_CURSOR_HAND, "hand");
        CASE(FL_CURSOR_HELP, "help");
        CASE(FL_CURSOR_MOVE, "move");
        CASE(FL_CURSOR_NS, "ns");
        CASE(FL_CURSOR_WE, "we");
        CASE(FL_CURSOR_NWSE, "nwse");
        CASE(FL_CURSOR_NESW, "nesw");
        CASE(FL_CURSOR_N, "n");
        CASE(FL_CURSOR_NE, "ne");
        CASE(FL_CURSOR_E, "e");
        CASE(FL_CURSOR_SE, "se");
        CASE(FL_CURSOR_S, "s");
        CASE(FL_CURSOR_SW, "sw");
        CASE(FL_CURSOR_W, "w");
        CASE(FL_CURSOR_NW, "nw");
        CASE(FL_CURSOR_NONE, "none");
#undef CASE
    return (Fl_Cursor)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Cursor(lua_State *L, Fl_Cursor cursor)
    {
    switch(cursor)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_CURSOR_DEFAULT, "default");
        CASE(FL_CURSOR_ARROW, "arrow");
        CASE(FL_CURSOR_CROSS, "cross");
        CASE(FL_CURSOR_WAIT, "wait");
        CASE(FL_CURSOR_INSERT, "insert");
        CASE(FL_CURSOR_HAND, "hand");
        CASE(FL_CURSOR_HELP, "help");
        CASE(FL_CURSOR_MOVE, "move");
        CASE(FL_CURSOR_NS, "ns");
        CASE(FL_CURSOR_WE, "we");
        CASE(FL_CURSOR_NWSE, "nwse");
        CASE(FL_CURSOR_NESW, "nesw");
        CASE(FL_CURSOR_N, "n");
        CASE(FL_CURSOR_NE, "ne");
        CASE(FL_CURSOR_E, "e");
        CASE(FL_CURSOR_SE, "se");
        CASE(FL_CURSOR_S, "s");
        CASE(FL_CURSOR_SW, "sw");
        CASE(FL_CURSOR_W, "w");
        CASE(FL_CURSOR_NW, "nw");
        CASE(FL_CURSOR_NONE, "none");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 | Fl_Browser_ anonymous enum for has_scrollbar()                       |
 *----------------------------------------------------------------------*/

uchar check_ScrollbarsMode(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(0, "none");
        CASE(Fl_Browser_::HORIZONTAL, "horizontal");
        CASE(Fl_Browser_::VERTICAL, "vertical");
        CASE(Fl_Browser_::BOTH, "both");
        CASE(Fl_Browser_::ALWAYS_ON, "always on");
        CASE(Fl_Browser_::HORIZONTAL_ALWAYS, "horizontal always");
        CASE(Fl_Browser_::VERTICAL_ALWAYS, "vertical always");
        CASE(Fl_Browser_::BOTH_ALWAYS, "both always");
#undef CASE
    return (uchar)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_ScrollbarsMode(lua_State *L, uchar val)
    {
    switch(val)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(0, "none");
        CASE(Fl_Browser_::HORIZONTAL, "horizontal");
        CASE(Fl_Browser_::VERTICAL, "vertical");
        CASE(Fl_Browser_::BOTH, "both");
        CASE(Fl_Browser_::ALWAYS_ON, "always on");
        CASE(Fl_Browser_::HORIZONTAL_ALWAYS, "horizontal always");
        CASE(Fl_Browser_::VERTICAL_ALWAYS, "vertical always");
        CASE(Fl_Browser_::BOTH_ALWAYS, "both always");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 | Fl_Browser::Fl_Line_Position                                         |
 *----------------------------------------------------------------------*/

Fl_Browser::Fl_Line_Position check_Line_Position (lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(Fl_Browser::TOP, "top");
        CASE(Fl_Browser::MIDDLE, "middle");
        CASE(Fl_Browser::BOTTOM, "bottom");
#undef CASE
    return (Fl_Browser::Fl_Line_Position)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Line_Position (lua_State *L, Fl_Browser::Fl_Line_Position val)
    {
    switch(val)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(Fl_Browser::TOP, "top");
        CASE(Fl_Browser::MIDDLE, "middle");
        CASE(Fl_Browser::BOTTOM, "bottom");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 | Fl_RGB_Scaling                                                       |
 *----------------------------------------------------------------------*/

Fl_RGB_Scaling check_RGB_Scaling(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_RGB_SCALING_NEAREST, "nearest");
        CASE(FL_RGB_SCALING_BILINEAR, "bilinear");
#undef CASE
    return (Fl_RGB_Scaling)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_RGB_Scaling(lua_State *L, Fl_RGB_Scaling alg)
    {
    switch(alg)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_RGB_SCALING_NEAREST, "nearest");
        CASE(FL_RGB_SCALING_BILINEAR, "bilinear");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 | Fl_File_Browser::filetype anon. enum.                                |
 *----------------------------------------------------------------------*/

int check_FileType(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(Fl_File_Browser::FILES, "files");
        CASE(Fl_File_Browser::DIRECTORIES, "directories");
#undef CASE
    return (int)luaL_argerror(L, --arg, badvalue(L,s));
    }

int push_FileType(lua_State *L, int t)
    {
    switch(t)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(Fl_File_Browser::FILES, "files");
        CASE(Fl_File_Browser::DIRECTORIES, "directories");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 | Fl_Text_Display anonymous enums                                      |
 *----------------------------------------------------------------------*/

int check_Text_Display_Cursor_Style(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
    CASE(Fl_Text_Display::NORMAL_CURSOR, "normal");
    CASE(Fl_Text_Display::CARET_CURSOR, "caret");
    CASE(Fl_Text_Display::DIM_CURSOR, "dim");
    CASE(Fl_Text_Display::BLOCK_CURSOR, "block");
    CASE(Fl_Text_Display::HEAVY_CURSOR, "heavy");
    CASE(Fl_Text_Display::SIMPLE_CURSOR, "simple");
#undef CASE
    return (int)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Text_Display_Cursor_Style(lua_State *L, int val)
    {
    switch(val)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(Fl_Text_Display::NORMAL_CURSOR, "normal");
        CASE(Fl_Text_Display::CARET_CURSOR, "caret");
        CASE(Fl_Text_Display::DIM_CURSOR, "dim");
        CASE(Fl_Text_Display::BLOCK_CURSOR, "block");
        CASE(Fl_Text_Display::HEAVY_CURSOR, "heavy");
        CASE(Fl_Text_Display::SIMPLE_CURSOR, "simple");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


int check_Text_Display_Wrap_Mode(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
    CASE(Fl_Text_Display::WRAP_NONE, "none");
    CASE(Fl_Text_Display::WRAP_AT_COLUMN, "at column");
    CASE(Fl_Text_Display::WRAP_AT_PIXEL, "at pixel");
    CASE(Fl_Text_Display::WRAP_AT_BOUNDS, "at bounds");
#undef CASE
    return (int)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Text_Display_Wrap_Mode(lua_State *L, int val)
    {
    switch(val)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(Fl_Text_Display::WRAP_NONE, "none");
        CASE(Fl_Text_Display::WRAP_AT_COLUMN, "at column");
        CASE(Fl_Text_Display::WRAP_AT_PIXEL, "at pixel");
        CASE(Fl_Text_Display::WRAP_AT_BOUNDS, "at bounds");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 |  Color_Chooser Mode                                                  |
 *----------------------------------------------------------------------*/

int check_Color_ChooserMode(lua_State *L, int arg) 
    {
    const char *s = luaL_optstring(L, arg, "default");
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(-1, "default");
        CASE(0, "rgb");
        CASE(1, "byte");
        CASE(2, "hex");
        CASE(3, "hsv");
#undef CASE
    return luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Color_ChooserMode(lua_State *L, int mode)
    {
    switch(mode)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(0, "rgb");
        CASE(1, "byte");
        CASE(2, "hex");
        CASE(3, "hsv");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 | Fl_Beep                                                              |
 *----------------------------------------------------------------------*/

Fl_Beep check_Beep(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_BEEP_DEFAULT, "default");
        CASE(FL_BEEP_MESSAGE, "message");
        CASE(FL_BEEP_ERROR, "error");
        CASE(FL_BEEP_QUESTION, "question");
        CASE(FL_BEEP_PASSWORD, "password");
        CASE(FL_BEEP_NOTIFICATION, "notification");
#undef CASE
    return (Fl_Beep)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Beep(lua_State *L, Fl_Beep type)
    {
    switch(type)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_BEEP_DEFAULT, "default");
        CASE(FL_BEEP_MESSAGE, "message");
        CASE(FL_BEEP_ERROR, "error");
        CASE(FL_BEEP_QUESTION, "question");
        CASE(FL_BEEP_PASSWORD, "password");
        CASE(FL_BEEP_NOTIFICATION, "notification");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 | Fl_Event                                                             |
 *----------------------------------------------------------------------*/

int check_Event(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_NO_EVENT, "no event");
        CASE(FL_PUSH, "push");
        CASE(FL_RELEASE,"release");
        CASE(FL_ENTER,"enter");
        CASE(FL_LEAVE,"leave");
        CASE(FL_DRAG,"drag");
        CASE(FL_FOCUS,"focus");
        CASE(FL_UNFOCUS,"unfocus");
        CASE(FL_KEYDOWN,"keydown");
    //  CASE(FL_KEYBOARD,"keyboard"); use keydown
        CASE(FL_KEYUP,"keyup");
        CASE(FL_CLOSE,"close");
        CASE(FL_MOVE,"move");
        CASE(FL_SHORTCUT,"shortcut");
        CASE(FL_DEACTIVATE,"deactivate");
        CASE(FL_ACTIVATE,"activate");
        CASE(FL_HIDE,"hide");
        CASE(FL_SHOW,"show");
        CASE(FL_PASTE,"paste");
        CASE(FL_SELECTIONCLEAR,"selectionclear");
        CASE(FL_MOUSEWHEEL,"mousewheel");
        CASE(FL_DND_ENTER,"dnd enter");
        CASE(FL_DND_DRAG,"dnd drag");
        CASE(FL_DND_LEAVE,"dnd leave");
        CASE(FL_DND_RELEASE,"dnd release");
        CASE(FL_SCREEN_CONFIGURATION_CHANGED,"screen configuration changed");
        CASE(FL_FULLSCREEN,"fullscreen");
#undef CASE
    return luaL_argerror(L, arg, badvalue(L, s));
    }


int push_Event(lua_State *L, int event)
    {
    switch(event)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_NO_EVENT, "no event");
        CASE(FL_PUSH, "push");
        CASE(FL_RELEASE,"release");
        CASE(FL_ENTER,"enter");
        CASE(FL_LEAVE,"leave");
        CASE(FL_DRAG,"drag");
        CASE(FL_FOCUS,"focus");
        CASE(FL_UNFOCUS,"unfocus");
        CASE(FL_KEYDOWN,"keydown");
    //  CASE(FL_KEYBOARD,"keyboard"); use keydown
        CASE(FL_KEYUP,"keyup");
        CASE(FL_CLOSE,"close");
        CASE(FL_MOVE,"move");
        CASE(FL_SHORTCUT,"shortcut");
        CASE(FL_DEACTIVATE,"deactivate");
        CASE(FL_ACTIVATE,"activate");
        CASE(FL_HIDE,"hide");
        CASE(FL_SHOW,"show");
        CASE(FL_PASTE,"paste");
        CASE(FL_SELECTIONCLEAR,"selectionclear");
        CASE(FL_MOUSEWHEEL,"mousewheel");
        CASE(FL_DND_ENTER,"dnd enter");
        CASE(FL_DND_DRAG,"dnd drag");
        CASE(FL_DND_LEAVE,"dnd leave");
        CASE(FL_DND_RELEASE,"dnd release");
        CASE(FL_SCREEN_CONFIGURATION_CHANGED,"screen configuration changed");
        CASE(FL_FULLSCREEN,"fullscreen");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 | Mouse Button                                                         |
 *----------------------------------------------------------------------*/

int check_Mouse_Button(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_LEFT_MOUSE, "left");
        CASE(FL_MIDDLE_MOUSE, "middle");
        CASE(FL_RIGHT_MOUSE, "right");
#undef CASE
    return luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Mouse_Button(lua_State *L, int b)
    {
    switch(b)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_LEFT_MOUSE, "left");
        CASE(FL_MIDDLE_MOUSE, "middle");
        CASE(FL_RIGHT_MOUSE, "right");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

/*----------------------------------------------------------------------*
 | Fl_File_Sort_F                                                       |
 *----------------------------------------------------------------------*/

Fl_File_Sort_F *check_File_Sort_F(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(fl_alphasort, "alphasort");
        CASE(fl_casealphasort, "casealphasort");
        CASE(fl_casenumericsort, "casenumericsort");
        CASE(fl_numericsort, "numericsort");
#undef CASE
    luaL_argerror(L, arg, badvalue(L,s));
    return NULL;
    }

int push_File_Sort_F(lua_State *L, Fl_File_Sort_F *func)
    {
#define CASE(CODE,str) if(func==CODE) { lua_pushstring(L, str); return 1; }
        CASE(fl_alphasort, "alphasort");
        CASE(fl_casealphasort, "casealphasort");
        CASE(fl_casenumericsort, "casenumericsort");
        CASE(fl_numericsort, "numericsort");
#undef CASE
    return unexpected(L);
    }

/*----------------------------------------------------------------------*
 | Fl_Table::TableContext                                               |
 *----------------------------------------------------------------------*/

Fl_Table::TableContext check_TableContext(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(Fl_Table::CONTEXT_NONE, "none");
        CASE(Fl_Table::CONTEXT_STARTPAGE, "startpage");
        CASE(Fl_Table::CONTEXT_ENDPAGE, "endpage");
        CASE(Fl_Table::CONTEXT_ROW_HEADER, "row header");
        CASE(Fl_Table::CONTEXT_COL_HEADER, "col header");
        CASE(Fl_Table::CONTEXT_CELL, "cell");
        CASE(Fl_Table::CONTEXT_TABLE, "table");
        CASE(Fl_Table::CONTEXT_RC_RESIZE, "rc resize");
#undef CASE
    return (Fl_Table::TableContext)luaL_argerror(L, --arg, badvalue(L,s));
    }

int push_TableContext(lua_State *L, Fl_Table::TableContext option)
    {
    switch(option)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(Fl_Table::CONTEXT_NONE, "none");
        CASE(Fl_Table::CONTEXT_STARTPAGE, "startpage");
        CASE(Fl_Table::CONTEXT_ENDPAGE, "endpage");
        CASE(Fl_Table::CONTEXT_ROW_HEADER, "row header");
        CASE(Fl_Table::CONTEXT_COL_HEADER, "col header");
        CASE(Fl_Table::CONTEXT_CELL, "cell");
        CASE(Fl_Table::CONTEXT_TABLE, "table");
        CASE(Fl_Table::CONTEXT_RC_RESIZE, "rc resize");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | Browser Sort Flag                                                    |
 *----------------------------------------------------------------------*/

int check_SortFlag(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_SORT_ASCENDING, "ascending");
        CASE(FL_SORT_DESCENDING, "descending");
#undef CASE
    return luaL_argerror(L, arg, badvalue(L,s));
    }

int push_SortFlag(lua_State *L, int opt)
    {
    switch(opt)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FL_SORT_ASCENDING, "ascending");
        CASE(FL_SORT_DESCENDING, "descending");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | Fl_Tree_Connector                                                    |
 *----------------------------------------------------------------------*/

Fl_Tree_Connector check_Tree_Connector(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_TREE_CONNECTOR_NONE, "none");
        CASE(FL_TREE_CONNECTOR_DOTTED, "dotted");
        CASE(FL_TREE_CONNECTOR_SOLID, "solid");
#undef CASE
    return (Fl_Tree_Connector)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Tree_Connector(lua_State *L, Fl_Tree_Connector val)
    {
#define CASE(CODE,str) if(val==CODE) do { lua_pushstring(L, str); return 1; } while(0)
        CASE(FL_TREE_CONNECTOR_NONE, "none");
        CASE(FL_TREE_CONNECTOR_DOTTED, "dotted");
        CASE(FL_TREE_CONNECTOR_SOLID, "solid");
#undef CASE
    return unexpected(L);
    }


/*----------------------------------------------------------------------*
 | Fl_Tree_Reason                                                       |
 *----------------------------------------------------------------------*/

Fl_Tree_Reason check_Tree_Reason(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_TREE_REASON_NONE, "none");
        CASE(FL_TREE_REASON_SELECTED, "selected");
        CASE(FL_TREE_REASON_DESELECTED, "deselected");
#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
        CASE(FL_TREE_REASON_RESELECTED, "reselected");
#endif /*FLTK_ABI_VERSION*/
        CASE(FL_TREE_REASON_OPENED, "opened");
        CASE(FL_TREE_REASON_CLOSED, "closed");
        CASE(FL_TREE_REASON_DRAGGED, "dragged");
#undef CASE
    return (Fl_Tree_Reason)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Tree_Reason(lua_State *L, Fl_Tree_Reason val)
    {
#define CASE(CODE,str) if(val==CODE) do { lua_pushstring(L, str); return 1; } while(0)
        CASE(FL_TREE_REASON_NONE, "none");
        CASE(FL_TREE_REASON_SELECTED, "selected");
        CASE(FL_TREE_REASON_DESELECTED, "deselected");
#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
        CASE(FL_TREE_REASON_RESELECTED, "reselected");
#endif /*FLTK_ABI_VERSION*/
        CASE(FL_TREE_REASON_OPENED, "opened");
        CASE(FL_TREE_REASON_CLOSED, "closed");
        CASE(FL_TREE_REASON_DRAGGED, "dragged");
#undef CASE
    return unexpected(L);
    }

/*----------------------------------------------------------------------*
 | Fl_Tree_Sort                                                         |
 *----------------------------------------------------------------------*/

Fl_Tree_Sort check_Tree_Sort(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_TREE_SORT_NONE, "none");
        CASE(FL_TREE_SORT_ASCENDING, "ascending");
        CASE(FL_TREE_SORT_DESCENDING, "descending");
#undef CASE
    return (Fl_Tree_Sort)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Tree_Sort(lua_State *L, Fl_Tree_Sort val)
    {
#define CASE(CODE,str) if(val==CODE) do { lua_pushstring(L, str); return 1; } while(0)
        CASE(FL_TREE_SORT_NONE, "none");
        CASE(FL_TREE_SORT_ASCENDING, "ascending");
        CASE(FL_TREE_SORT_DESCENDING, "descending");
#undef CASE
    return unexpected(L);
    }

/*----------------------------------------------------------------------*
 | Fl_Tree_Select                                                       |
 *----------------------------------------------------------------------*/

Fl_Tree_Select check_Tree_Select(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_TREE_SELECT_NONE, "none");
        CASE(FL_TREE_SELECT_SINGLE, "single");
        CASE(FL_TREE_SELECT_MULTI, "multi");
        CASE(FL_TREE_SELECT_SINGLE_DRAGGABLE, "single draggable");
#undef CASE
    return (Fl_Tree_Select)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Tree_Select(lua_State *L, Fl_Tree_Select val)
    {
#define CASE(CODE,str) if(val==CODE) do { lua_pushstring(L, str); return 1; } while(0)
        CASE(FL_TREE_SELECT_NONE, "none");
        CASE(FL_TREE_SELECT_SINGLE, "single");
        CASE(FL_TREE_SELECT_MULTI, "multi");
        CASE(FL_TREE_SELECT_SINGLE_DRAGGABLE, "single draggable");
#undef CASE
    return unexpected(L);
    }

/*----------------------------------------------------------------------*
 | Fl_Tree_Item_Reselect_Mode                                           |
 *----------------------------------------------------------------------*/

#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)

Fl_Tree_Item_Reselect_Mode check_Tree_Item_Reselect_Mode(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FL_TREE_SELECTABLE_ONCE, "once");
        CASE(FL_TREE_SELECTABLE_ALWAYS, "always");
#undef CASE
    return (Fl_Tree_Item_Reselect_Mode)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Tree_Item_Reselect_Mode(lua_State *L, Fl_Tree_Item_Reselect_Mode val)
    {
#define CASE(CODE,str) if(val==CODE) do { lua_pushstring(L, str); return 1; } while(0)
        CASE(FL_TREE_SELECTABLE_ONCE, "once");
        CASE(FL_TREE_SELECTABLE_ALWAYS, "always");
#undef CASE
    return unexpected(L);
    }

#endif


