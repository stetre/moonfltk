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
#include "keys.h"

static void AddVersions(lua_State *L) //@@DOC
/* Add version strings to the fltk table */
    {
    lua_pushstring(L, "_VERSION");
    lua_pushstring(L, "MoonFLTK " MOONFLTK_VERSION);
    lua_settable(L, -3);

    lua_pushstring(L, "_FLTK_VERSION");
    lua_pushfstring(L, "FLTK %d.%d.%d", 
            FL_MAJOR_VERSION, FL_MINOR_VERSION, FL_PATCH_VERSION);
    lua_settable(L, -3);

    lua_pushstring(L, "_FLTK_ABI_VERSION");
#if defined(FLTK_ABI_VERSION)
    lua_pushinteger(L, FLTK_ABI_VERSION);
#else   
    lua_pushinteger(L, 0); /* i.e. not defined */
#endif  
    lua_settable(L, -3);
    }


static int AddConstants(lua_State *L) /* fl.XXXXX constants for FL_XXXXX defines */
    {
#define ADD(c) do { \
    lua_pushinteger(L, FL_##c); lua_setfield(L, -2, #c); } while(0)
    /* Fonts */
    ADD(HELVETICA);
    ADD(HELVETICA_BOLD);
    ADD(HELVETICA_ITALIC);
    ADD(HELVETICA_BOLD_ITALIC);
    ADD(COURIER);
    ADD(COURIER_BOLD);
    ADD(COURIER_ITALIC);
    ADD(COURIER_BOLD_ITALIC);
    ADD(TIMES);
    ADD(TIMES_BOLD);
    ADD(TIMES_ITALIC);
    ADD(TIMES_BOLD_ITALIC);
    ADD(SYMBOL);
    ADD(SCREEN);
    ADD(SCREEN_BOLD);
    ADD(ZAPF_DINGBATS);
    ADD(FREE_FONT);
    ADD(BOLD);
    ADD(ITALIC);
    ADD(BOLD_ITALIC);
    /* Colors */
    ADD(FOREGROUND_COLOR);
    ADD(BACKGROUND2_COLOR);
    ADD(INACTIVE_COLOR);
    ADD(SELECTION_COLOR);
    ADD(GRAY0);
    ADD(DARK3);
    ADD(DARK2);
    ADD(DARK1);
    ADD(BACKGROUND_COLOR);
    ADD(LIGHT1);
    ADD(LIGHT2);
    ADD(LIGHT3);
    ADD(BLACK);
    ADD(RED);
    ADD(GREEN);
    ADD(YELLOW);
    ADD(BLUE);
    ADD(MAGENTA);
    ADD(CYAN);
    ADD(DARK_RED);
    ADD(DARK_GREEN);
    ADD(DARK_YELLOW);
    ADD(DARK_BLUE);
    ADD(DARK_MAGENTA);
    ADD(DARK_CYAN);
    ADD(WHITE);
    ADD(FREE_COLOR);
    ADD(NUM_FREE_COLOR);
    ADD(GRAY_RAMP);
    ADD(NUM_GRAY);
    ADD(GRAY);
    ADD(COLOR_CUBE);
    ADD(NUM_RED);
    ADD(NUM_GREEN);
    ADD(NUM_BLUE);
    /* Styles */
    ADD(SOLID);
    ADD(DASH);
    ADD(DOT);
    ADD(DASHDOT);
    ADD(DASHDOTDOT);
    ADD(CAP_FLAT);
    ADD(CAP_ROUND);
    ADD(CAP_SQUARE);
    ADD(JOIN_MITER);
    ADD(JOIN_ROUND);
    ADD(JOIN_BEVEL);
    /* Align flags */
    ADD(ALIGN_CENTER);
    ADD(ALIGN_TOP);
    ADD(ALIGN_BOTTOM);
    ADD(ALIGN_LEFT);
    ADD(ALIGN_RIGHT);
    ADD(ALIGN_INSIDE);
    ADD(ALIGN_TEXT_OVER_IMAGE);
    ADD(ALIGN_IMAGE_OVER_TEXT);
    ADD(ALIGN_CLIP);
    ADD(ALIGN_WRAP);
    ADD(ALIGN_IMAGE_NEXT_TO_TEXT);
    ADD(ALIGN_TEXT_NEXT_TO_IMAGE);
    ADD(ALIGN_IMAGE_BACKDROP);
    ADD(ALIGN_TOP_LEFT);
    ADD(ALIGN_TOP_RIGHT);
    ADD(ALIGN_BOTTOM_LEFT);
    ADD(ALIGN_BOTTOM_RIGHT);
    ADD(ALIGN_LEFT_TOP);
    ADD(ALIGN_RIGHT_TOP);
    ADD(ALIGN_LEFT_BOTTOM);
    ADD(ALIGN_RIGHT_BOTTOM);
    ADD(ALIGN_NOWRAP);
    ADD(ALIGN_POSITION_MASK);
    ADD(ALIGN_IMAGE_MASK);
    /* WhenFd */
    ADD(READ);
    ADD(WRITE);
    ADD(EXCEPT);
    /* When flags */
    ADD(WHEN_NEVER);
    ADD(WHEN_CHANGED);
    ADD(WHEN_NOT_CHANGED);
    ADD(WHEN_RELEASE);
    ADD(WHEN_RELEASE_ALWAYS);
    ADD(WHEN_ENTER_KEY);
    ADD(WHEN_ENTER_KEY_ALWAYS);
    ADD(WHEN_ENTER_KEY_CHANGED);
    /* Damage bits */
    ADD(DAMAGE_CHILD);
    ADD(DAMAGE_EXPOSE);
    ADD(DAMAGE_SCROLL);
    ADD(DAMAGE_OVERLAY);
    ADD(DAMAGE_USER1);
    ADD(DAMAGE_USER2);
    ADD(DAMAGE_ALL);
    /* Mode bits */
    ADD(RGB);
    ADD(INDEX);
    ADD(SINGLE);
    ADD(DOUBLE);
    ADD(ACCUM);
    ADD(ALPHA);
    ADD(DEPTH);
    ADD(STENCIL);
    ADD(RGB8);
    ADD(MULTISAMPLE);
    ADD(STEREO);
    ADD(FAKE_SINGLE);
    /* Menu Item flags */
    ADD(MENU_INACTIVE);
    ADD(MENU_TOGGLE);
    ADD(MENU_VALUE);
    ADD(MENU_RADIO);
    ADD(MENU_INVISIBLE);
//  ADD(SUBMENU_POINTER);
    ADD(SUBMENU);
    ADD(MENU_DIVIDER);
    ADD(MENU_HORIZONTAL);
#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
    /* Tree_Item_Draw_Mode */
    ADD(TREE_ITEM_DRAW_DEFAULT);
    ADD(TREE_ITEM_DRAW_LABEL_AND_WIDGET);
    ADD(TREE_ITEM_HEIGHT_FROM_WIDGET);
#endif
    /* Modifiers */
    ADD(SHIFT);
    ADD(CAPS_LOCK);
    ADD(CTRL);
    ADD(ALT);
    ADD(NUM_LOCK);
    ADD(META);
    ADD(SCROLL_LOCK);
    ADD(BUTTON1);
    ADD(BUTTON2);
    ADD(BUTTON3);
#undef ADD
#define ADD(c) do { \
    lua_pushinteger(L, moonfltk##c); lua_setfield(L, -2, #c); } while(0)
    ADD(BUTTON4);
    ADD(BUTTON5);
    ADD(BUTTON6);
    ADD(BUTTON7);
    ADD(BUTTON8);
#undef ADD
#if 0
    /* Fl_Table::TableContext enums, renamed as fl.TABLE_CONTEXT_XXX" */
#define ADD(c) do {\
        lua_pushinteger(L, Fl_Table::CONTEXT_##c);  \
        lua_setfield(L, -2, "TABLE_CONTEXT_"#c);    \
} while(0)
    ADD(NONE);
    ADD(STARTPAGE);
    ADD(ENDPAGE);
    ADD(ROW_HEADER);
    ADD(COL_HEADER);
    ADD(CELL);
    ADD(TABLE);
    ADD(RC_RESIZE);
#undef ADD
#endif
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { NULL, NULL } /* sentinel */
    };

static bool initialized = false;
lua_State* main_lua_state = 0;

extern "C" { /* because it is called by luaopen_moonfltk() */

static int handleClosingLuaState(lua_State *L)
    {
        (void)L;
        main_lua_state = 0;
        return 0;
    }

int moonfltk_open_background_main(lua_State *L);

int moonfltk_open_main(lua_State *L)
    {
    Fl::lock();

    bool isInCorrectMainLuaState;
    
    if (!initialized)
        {
        #if LUA_VERSION_NUM >= 502
            lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
                main_lua_state = lua_tothread(L, -1); // our main state is the current main state
            lua_pop(L, 1);
        #else
            main_lua_state = lua_newthread(L); // our "main state" is a new state
            luaL_ref(L, LUA_REGISTRYINDEX); // remember in registry to prevent garbage collection
        #endif
        
        lua_pushlightuserdata(L, (void*)&initialized); // unique void* key
            lua_newuserdata(L, 1); // sentinel for closing lua state
                lua_newtable(L); // metatable for sentinel
                    lua_pushstring(L, "__gc");
                        lua_pushcfunction(L, handleClosingLuaState);
                    lua_rawset(L, -3); // metatable.__gc = handleClosingLuaState
                lua_setmetatable(L, -2); // sets metatable for sentinal table
        lua_rawset(L, LUA_REGISTRYINDEX); // sets sentinel as value for unique void* in registry
        initialized = true;
        isInCorrectMainLuaState = true;
        }
    else
        {
        // check if initialization has been done in a separate lua main state
        lua_pushlightuserdata(L, (void*)&initialized); // unique void* key
        lua_rawget(L, LUA_REGISTRYINDEX);
            isInCorrectMainLuaState = !lua_isnil(L, -1);
        lua_pop(L, 1);
        }
    
    if (isInCorrectMainLuaState)
        {
        lua_newtable(L); /* the fltk table */
        AddVersions(L);
        AddConstants(L);
    
        /* add fltk functions: */
        luaL_setfuncs(L, Functions, 0);
        moonfltk_open_Fl(L);
        moonfltk_open_Additional(L);
        moonfltk_open_FlColor(L);
        moonfltk_open_FlFont(L);
        moonfltk_open_FlWindow(L);
        moonfltk_open_FlClipboard(L);
        moonfltk_open_FlScreen(L);
        moonfltk_open_FlDraw(L);
        moonfltk_open_FlFile(L);
        moonfltk_open_FlUnicode(L);
        moonfltk_open_FlCommon(L);
        moonfltk_open_FlTooltip(L);
        moonfltk_open_Widget(L);
        moonfltk_open_Button(L);
        moonfltk_open_Clock(L);
        moonfltk_open_Group(L);
        moonfltk_open_Window(L);
        moonfltk_open_Image(L);
        moonfltk_open_Browser(L);
        moonfltk_open_Menu(L);
        moonfltk_open_Input(L);
        moonfltk_open_Valuator(L);
        moonfltk_open_Slider(L);
        moonfltk_open_Tabs(L);
        moonfltk_open_Input_Choice(L);
        moonfltk_open_Color_Chooser(L);
        moonfltk_open_FlEvent(L);
        moonfltk_open_Text_Buffer(L);
        moonfltk_open_Text_Display(L);
        moonfltk_open_Text_Editor(L);
        moonfltk_open_Native_File_Chooser(L);
        moonfltk_open_Handlers(L);
        moonfltk_open_Table(L);
        moonfltk_open_Subclass(L);
        moonfltk_open_Chart(L);
        moonfltk_open_Help_Dialog(L);
        moonfltk_open_Help_View(L);
        moonfltk_open_Tree(L);
    
        Fl::scheme(NULL); /* this is needed otherwise fl_input() does a segv */

        // hold the lock, no Fl::unlock on main thread, this is done by Fl::wait
        return 1;
        }
    else
        {
        Fl::unlock(); // release lock on background thread

        return luaL_error(L, "MoonFLTK is already initialized for another lua state");
        }
    }
    
int moonfltk_open_background_main(lua_State *L)
    {
        lua_newtable(L); /* the fltk table for background threads */

        AddVersions(L);
        AddConstants(L);
        
        moonfltk_open_Fl_for_background(L);
        moonfltk_open_FlFile(L);
        moonfltk_open_FlUnicode(L);
        
        return 1;
    }

} /* extern "C" */
