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

/********************************************************************************
 * internal common header                                                       *
 ********************************************************************************/

#ifndef internalDEFINED
#define internalDEFINED


#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "moonfltk.h"

extern "C" {
#include "compat-5.3.h"
}

#ifdef BEGIN_FLTK_INCLUDE
    BEGIN_FLTK_INCLUDE // {
#endif

#include <FL/Fl.H>

#ifdef END_FLTK_INCLUDE
    END_FLTK_INCLUDE   // }
#endif

#include "udata.h"

#ifndef USE_GL
#define USE_GL 0
#endif

#ifndef USE_IMAGES
#define USE_IMAGES 0
#endif


#define TOSTR_(x) #x
#define TOSTR(x) TOSTR_(x)

#if 0
//#pragma message ("LUAVER="TOSTR(LUAVER))
#if LUA_VERSION_NUM < 503
#error "MoonFLTK requires Lua version >= 5.3"
#endif
#endif

#define main_lua_state moonfltk_main_lua_state
extern lua_State* main_lua_state;

/* Note: all the dynamic symbols of this library (should) start with 'moonfltk_' .
 * The only exception is the luaopen_moonfltk() function, which is searched for
 * with that name by Lua.
 * MoonFLTK's string references on the Lua registry also start with 'moonfltk_'.
 */

#define ud_t moonfltk_ud_t
typedef struct {
    void *obj;  /* the object (FL_##T*) bound to this userdata */
    int refcnt; /* reference counter(for shared images) */
    /* references on the Lua registry --------------------*/
    int cbref;  /* callback */
    int argref; /* callback argument */
    /* widget methods override */
    int draw;
    int handle;
    int hide;
    int show;
    int resize;
    int draw_overlay;
    int draw_cell;
    /* text buffer */
    int modify;
    int modify_arg;
    int predelete;
    int predelete_arg;
    /* text display */
    int style;
    void* style_table;
    /* text editor */
    int key_func;
    /* text editor */
    int help_func;
    /* for dinamically allocated images ---------------------- */
    char *bits;
    char **xpm; /* array of string pointers for XPM images */
    int nxpm; /* no. of strings in xpm */
} moonfltk_ud_t;

#include "mt.h"
#include "func.h"

#if 0
/* .cc */
#define  moonfltk_
#endif

/* utils.cc */
double moonfltk_now(void);
void moonfltk_sleep(double seconds);
#define noprintf moonfltk_noprintf
int noprintf(const char *fmt, ...);
#define printstack moonfltk_printstack
void printstack(lua_State *L, const char* fmat, ...);
#define notavailable moonfltk_notavailable
int notavailable(lua_State *L, ...);
#define checkboolean moonfltk_checkboolean
int checkboolean(lua_State *L, int arg);
#define optboolean moonfltk_optboolean
int optboolean(lua_State *L, int arg, int d);
#define testchar moonfltk_testchar
int testchar(lua_State *L, int arg, char *c_);
#define checkascii moonfltk_checkascii
char checkascii(lua_State *L, int arg);
#define pushascii moonfltk_pushascii
int pushascii(lua_State *L, char c);
#define checkindex moonfltk_checkindex
int checkindex(lua_State *L, int arg);
#define optindex moonfltk_optindex
int optindex(lua_State *L, int arg, int optval);
#define pushindex moonfltk_pushindex
void pushindex(lua_State *L, int val);
#define checkpositiveinteger moonfltk_checkpositiveinteger
int checkpositiveinteger(lua_State *L, int arg);
#define checkoption_hint moonfltk_checkoption_hint 
int checkoption_hint(lua_State *L, int arg, const char *def, const char *const lst[]);
#define checkpixmap moonfltk_checkpixmap
char** checkpixmap(lua_State *L, int arg, int *n);
#define freepixmap moonfltk_freepixmap
void freepixmap(char **xpm, int nxpm);
#define newuserdata moonfltk_newuserdata
ud_t *newuserdata(lua_State *L, void *ptr, const char *mt);
#define freeuserdata moonfltk_freeuserdata
int freeuserdata(lua_State *L, void *ptr);
#define userdata(ptr) (ud_t*)udata_mem(ptr)
#define userdata_unref(L, ptr) udata_unref((L),(ptr))

/* menu.cc */
#define menuclear moonfltk_menuclear
int menuclear(lua_State *L, Fl_Menu_ *menu);
const char *push_Pathname(lua_State *L, Fl_Menu_ *menu, Fl_Menu_Item *item);
Fl_Menu_Item *test_Pathname(lua_State *L, Fl_Menu_ *menu, int arg);
Fl_Menu_Item * check_Pathname(lua_State *L, Fl_Menu_ *menu, int arg);


/* main.c */
void moonfltk_open_Fl(lua_State *L);
void moonfltk_open_Fl_for_background(lua_State *L);
void moonfltk_open_Additional(lua_State *L);
void moonfltk_open_FlColor(lua_State *L);
void moonfltk_open_FlFont(lua_State *L);
void moonfltk_open_FlWindow(lua_State *L);
void moonfltk_open_FlEvent(lua_State *L);
void moonfltk_open_FlClipboard(lua_State *L);
void moonfltk_open_FlScreen(lua_State *L);
void moonfltk_open_FlDraw(lua_State *L);
void moonfltk_open_FlFile(lua_State *L);
void moonfltk_open_FlUnicode(lua_State *L);
void moonfltk_open_FlCommon(lua_State *L);
void moonfltk_open_FlTooltip(lua_State *L);
void moonfltk_open_Widget(lua_State *L);
void moonfltk_open_Button(lua_State *L);
void moonfltk_open_Clock(lua_State *L);
void moonfltk_open_Group(lua_State *L);
void moonfltk_open_Window(lua_State *L);
void moonfltk_open_Image(lua_State *L);
void moonfltk_open_Browser(lua_State *L);
void moonfltk_open_Menu_Item(lua_State *L);
void moonfltk_open_Menu(lua_State *L);
void moonfltk_open_Input(lua_State *L);
void moonfltk_open_Valuator(lua_State *L);
void moonfltk_open_Slider(lua_State *L);
void moonfltk_open_Tabs(lua_State *L);
void moonfltk_open_Input_Choice(lua_State *L);
void moonfltk_open_Color_Chooser(lua_State *L);
void moonfltk_open_Subclass(lua_State *L);
void moonfltk_open_Text_Buffer(lua_State *L);
void moonfltk_open_Text_Display(lua_State *L);
void moonfltk_open_Text_Editor(lua_State *L);
void moonfltk_open_Native_File_Chooser(lua_State *L);
void moonfltk_open_Handlers(lua_State *L);
void moonfltk_open_Table(lua_State *L);
void moonfltk_open_Chart(lua_State *L);
void moonfltk_open_Help_Dialog(lua_State *L);
void moonfltk_open_Help_View(lua_State *L);
void moonfltk_open_Tree(lua_State *L);

#define tableclear moonfltk_tableclear
void tableclear(lua_State *L, Fl_Table *p);
#define groupclear moonfltk_groupclear
void groupclear(lua_State *L, Fl_Group *p);
#define browserclear moonfltk_browserclear
void browserclear(lua_State *L, Fl_Browser_ *p);

/*------------------------------------------------------------------------------*
 | Enumerations                                                                 |
 *------------------------------------------------------------------------------*/

/* widget::type() method -------------------------------------------------------*/
uchar check_WidgetRtti(lua_State *L, int arg);
int push_WidgetRtti(lua_State *L, uchar type);
uchar check_PackRtti(lua_State *L, int arg);
int push_PackRtti(lua_State *L, uchar type);
uchar check_ScrollRtti(lua_State *L, int arg);
int push_ScrollRtti(lua_State *L, uchar type);
uchar check_Browser_Rtti(lua_State *L, int arg);
int push_Browser_Rtti(lua_State *L, uchar type);
uchar check_Menu_ButtonRtti(lua_State *L, int arg);
int push_Menu_ButtonRtti(lua_State *L, uchar type);
uchar check_InputRtti(lua_State *L, int arg);
int push_InputRtti(lua_State *L, int val_);
uchar check_SpinnerRtti(lua_State *L, int arg);
#define push_SpinnerRtti push_InputRtti
uchar check_ButtonRtti(lua_State *L, int arg);
int push_ButtonRtti(lua_State *L, uchar type);
int check_Native_File_ChooserRtti(lua_State *L, int arg);
int push_Native_File_ChooserRtti(lua_State *L, int val);
uchar check_SliderRtti(lua_State *L, int arg);
int push_SliderRtti(lua_State *L, uchar type);
uchar check_ScrollbarRtti(lua_State *L, int arg);
int push_ScrollbarRtti(lua_State *L, uchar type);
uchar check_CounterRtti(lua_State *L, int arg);
int push_CounterRtti(lua_State *L, uchar type);
uchar check_DialRtti(lua_State *L, int arg);
int push_DialRtti(lua_State *L, uchar type);
uchar check_RollerRtti(lua_State *L, int arg);
int push_RollerRtti(lua_State *L, uchar type);
Fl_Table_Row::TableRowSelectMode check_Table_RowRtti(lua_State *L, int arg);
int push_Table_RowRtti(lua_State *L, Fl_Table_Row::TableRowSelectMode val);
uchar check_ChartRtti(lua_State *L, int arg);
int push_ChartRtti(lua_State *L, uchar type);
uchar check_TreeRtti(lua_State *L, int arg);
int push_TreeRtti(lua_State *L, uchar type);
#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
Fl_Tree_Item_Draw_Mode check_Tree_Item_Draw_Mode(lua_State *L, int arg);
int push_Tree_Item_Draw_Mode(lua_State *L, Fl_Tree_Item_Draw_Mode val, int pushcode=1);
#endif

/* bitfields ---------------------------------------------------------------------*/
Fl_Align check_Align(lua_State *L, int arg);
int push_Align(lua_State *L, Fl_Align align, int pushcode=1);
Fl_When check_When(lua_State *L, int arg);
int push_When(lua_State *L, Fl_When when, int pushcode=1);
int check_WhenFd(lua_State *L, int arg);
int push_WhenFd(lua_State *L, int flags, int pushcode);
int check_Style(lua_State *L, int arg);
int push_Style(lua_State *L, int style, int pushcode=1);
Fl_Damage check_Damage(lua_State *L, int arg);
int push_Damage(lua_State *L, Fl_Damage flags, int pushcode=1);
int check_Menu_ItemFlags(lua_State *L, int arg);
int push_Menu_ItemFlags(lua_State *L, int flags, int pushcode=1);
Fl_Mode check_Mode(lua_State *L, int arg);
int push_Mode(lua_State *L, Fl_Mode opt, int pushcode=1);

/* enumerations ----------------------------------------------------------------*/

int check_Key(lua_State *L, int arg);
int push_Key(lua_State *L, int key_);
int check_Modifiers(lua_State *L, int arg);
int push_Modifiers(lua_State *L, int state, int pushcode=1);
int check_Shortcut(lua_State *L, int arg);
int push_Shortcut(lua_State *L, int val);
Fl::Fl_Option check_Option(lua_State *L, int arg);
int push_Option(lua_State *L, Fl::Fl_Option option);
Fl_Color check_Color(lua_State *L, int arg);
int push_Color(lua_State *L, Fl_Color color);
Fl_Boxtype check_Boxtype(lua_State *L, int arg);
int push_Boxtype(lua_State *L, Fl_Boxtype boxtype);


Fl_Labeltype check_Labeltype(lua_State *L, int arg);
int push_Labeltype(lua_State *L, Fl_Labeltype labeltype);
Fl_Font check_Font(lua_State *L, int arg);
int push_Font(lua_State *L, Fl_Font font);
#define check_Fontsize(L, arg) (Fl_Fontsize)luaL_optinteger((L),(arg),FL_NORMAL_SIZE)
#define push_Fontsize lua_pushinteger
Fl_Table::TableContext check_TableContext(lua_State *L, int arg);
int push_TableContext(lua_State *L, Fl_Table::TableContext flags);
Fl_Cursor check_Cursor(lua_State *L, int arg);
int push_Cursor(lua_State *L, Fl_Cursor cursor);
uchar check_ScrollbarsMode(lua_State *L, int arg);
int push_ScrollbarsMode(lua_State *L, uchar val);
Fl_Browser::Fl_Line_Position check_Line_Position (lua_State *L, int arg);
int push_Line_Position (lua_State *L, Fl_Browser::Fl_Line_Position val);
Fl_RGB_Scaling check_RGB_Scaling(lua_State *L, int arg);
int push_RGB_Scaling(lua_State *L, Fl_RGB_Scaling alg);
int check_FileType(lua_State *L, int arg);
int push_FileType(lua_State *L, int t);
Fl_File_Sort_F* check_File_Sort_F(lua_State *L, int arg);
int push_File_Sort_F(lua_State *L, Fl_File_Sort_F *func);
int check_Event(lua_State *L, int arg);
int push_Event(lua_State *L, int event);
int check_Mouse_Button(lua_State *L, int arg);
int push_Mouse_Button(lua_State *L, int b);
int check_Color_ChooserMode(lua_State *L, int arg);
int push_Color_ChooserMode(lua_State *L, int mode);
int check_Text_Display_Cursor_Style(lua_State *L, int arg);
int push_Text_Display_Cursor_Style(lua_State *L, int val);
int check_Text_Display_Wrap_Mode(lua_State *L, int arg);
int push_Text_Display_Wrap_Mode(lua_State *L, int val);
Fl_Beep check_Beep(lua_State *L, int arg);
int push_Beep(lua_State *L, Fl_Beep type);
int check_SortFlag(lua_State *L, int arg);
int push_SortFlag(lua_State *L, int opt);
Fl_Tree_Connector check_Tree_Connector(lua_State *L, int arg);
int push_Tree_Connector(lua_State *L, Fl_Tree_Connector val);
Fl_Tree_Reason check_Tree_Reason(lua_State *L, int arg);
int push_Tree_Reason(lua_State *L, Fl_Tree_Reason val);
Fl_Tree_Sort check_Tree_Sort(lua_State *L, int arg);
int push_Tree_Sort(lua_State *L, Fl_Tree_Sort val);
Fl_Tree_Select check_Tree_Select(lua_State *L, int arg);
int push_Tree_Select(lua_State *L, Fl_Tree_Select val);
#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
Fl_Tree_Item_Reselect_Mode check_Tree_Item_Reselect_Mode(lua_State *L, int arg);
int push_Tree_Item_Reselect_Mode(lua_State *L, Fl_Tree_Item_Reselect_Mode val);
#endif

/*------------------------------------------------------------------------------*
 | Debug and other utilities                                                    |
 *------------------------------------------------------------------------------*/

/* Dynamic referencing on the Lua registry */

#define reference(L, dst, arg) do {                 \
    lua_pushvalue((L), (arg));                      \
    (dst) = luaL_ref((L), LUA_REGISTRYINDEX);       \
} while(0)

#define unreference(L, ref) do {                    \
    if((ref)!=LUA_NOREF) {                          \
        luaL_unref((L), LUA_REGISTRYINDEX, (ref));  \
        (ref) = LUA_NOREF; }                        \
} while(0)

#define pushvalue(L, ref) /* returns LUA_TXXX */    \
    lua_rawgeti((L), LUA_REGISTRYINDEX, (ref)) 

extern int moonfltk_trace_objects;
#define DBG_CREATE(T, p) \
    if(moonfltk_trace_objects) printf("creating "#T" %p\n", (void*)(p))
#define DBG_DELETE(T, p) \
    if(moonfltk_trace_objects) printf("deleting "#T" %p\n", (void*)(p))



/* If this is printed, it denotes a suspect bug: */
#define UNEXPECTED_ERROR "unexpected error (%s, %d)", __FILE__, __LINE__
#define MEMORY_ERROR "cannot allocate memory (%s, %d)", __FILE__, __LINE__
#define NOT_AVAILABLE do { return luaL_error(L, "function not available"); } while(0)

#define unexpected(L) luaL_error((L), UNEXPECTED_ERROR)
#define memoryerror(L) luaL_error((L), MEMORY_ERROR)
#define badvalue(L,s)   lua_pushfstring((L), "invalid value '%s'", (s))

#if defined(DEBUG) 
/*--------------------------------------------------------------------*/

#define TR_STACK(L) printstack((L),"%s %d\n",__FILE__,__LINE__)

#define checkoption checkoption_hint
#define DBG printf

#define TR() do {                                           \
    printf("trace %s %d\n",__FILE__,__LINE__);              \
} while(0)

#define BK() do {                                           \
    printf("break %s %d\n",__FILE__,__LINE__);              \
    getchar();                                              \
} while(0)

#else  /* NO DEBUG */
/*--------------------------------------------------------------------*/

#define TR_STACK(L) (void)(L)
#define checkoption luaL_checkoption
#define DBG noprintf
#define TR()
#define BK()

#endif /* DEBUG */
/*--------------------------------------------------------------------*/

#endif /* internalDEFINED */
