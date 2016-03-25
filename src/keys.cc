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

/*----------------------------------------------------------------------*
 | Keys                                                                 |
 *----------------------------------------------------------------------*/

int check_Key(lua_State *L, int arg)
    {
    size_t len;
    if(lua_type(L, arg) != LUA_TSTRING)
        return luaL_argerror(L, arg, "string expected");

    const char *s = luaL_checklstring(L, arg, &len);

    /* printable ASCII */
    if(len==1) /* 0-127 */
        {
        unsigned char c = (unsigned char)s[0];
        //printf("len=1 %d\n", s[0]);
        if(c & 0x80)
            return luaL_argerror(L, arg, "invalid key");
        //@@ exclude non printable ?
        return c;
        }
    if(len==2) /* 128-255 */
        {
        unsigned char c0 = (unsigned char)s[0];
        unsigned char c1 = (unsigned char)s[1];
        //printf("len=2 %d %d\n", c0, c1);
        if(c0==0xc2) /* 194;128 .. 194;191 --> 128 .. 191 */
            {
            if((c1<128) || (c1>191))
                return luaL_argerror(L, arg, "invalid key");
            return c1;
            }
        if(c0==0xc3) /* 195;128 .. 195;191 --> 192 .. 255 */
            {
            if((c1<128) || (c1>191))
                return luaL_argerror(L, arg, "invalid key");
            return c1 + 64;
            }
        }   

#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(moonfltk_Button1, "button1");
        CASE(moonfltk_Button2, "button2");
        CASE(moonfltk_Button3, "button3");
        CASE(moonfltk_Button4, "button4");
        CASE(moonfltk_Button5, "button5");
        CASE(moonfltk_Button6, "button6");
        CASE(moonfltk_Button7, "button7");
        CASE(moonfltk_Button8, "button8");
        CASE(FL_BackSpace, "backspace");
        CASE(FL_Tab, "tab");
        CASE(FL_Iso_Key, "iso key");
        CASE(FL_Enter, "enter");
        CASE(FL_Pause, "pause");
        CASE(FL_Scroll_Lock, "scroll lock");
        CASE(FL_Escape, "escape");
        CASE(FL_Kana, "kana");
        CASE(FL_Eisu, "eisu");
        CASE(FL_Yen, "yen");
        CASE(FL_JIS_Underscore, "jis underscore");
        CASE(FL_Home, "home");
        CASE(FL_Left, "left");
        CASE(FL_Up, "up");
        CASE(FL_Right, "right");
        CASE(FL_Down, "down");
        CASE(FL_Page_Up, "page up");
        CASE(FL_Page_Down, "page down");
        CASE(FL_End, "end");
        CASE(FL_Print, "print");
        CASE(FL_Insert, "insert");
        CASE(FL_Menu, "menu");
        CASE(FL_Help, "help");
        CASE(FL_Num_Lock, "num lock");
//      CASE(FL_KP, "kp");
        CASE(moonfltk_KP0, "kp 0");
        CASE(moonfltk_KP1, "kp 1");
        CASE(moonfltk_KP2, "kp 2");
        CASE(moonfltk_KP3, "kp 3");
        CASE(moonfltk_KP4, "kp 4");
        CASE(moonfltk_KP5, "kp 5");
        CASE(moonfltk_KP6, "kp 6");
        CASE(moonfltk_KP7, "kp 7");
        CASE(moonfltk_KP8, "kp 8");
        CASE(moonfltk_KP9, "kp 9");
        CASE(moonfltk_KP_Add, "kp +");
        CASE(moonfltk_KP_Sub, "kp -");
        CASE(moonfltk_KP_Mul, "kp *");
        CASE(moonfltk_KP_Div, "kp /");
        CASE(moonfltk_KP_Dot, "kp .");
        CASE(moonfltk_KP_Eq, "kp =");
        CASE(FL_KP_Enter, "kp enter");
        CASE(moonfltk_KP_Middle, "kp middle");
//      CASE(FL_KP_Last, "kp last");
        CASE(moonfltk_F1, "f1");
        CASE(moonfltk_F2, "f2");
        CASE(moonfltk_F3, "f3");
        CASE(moonfltk_F4, "f4");
        CASE(moonfltk_F5, "f5");
        CASE(moonfltk_F6, "f6");
        CASE(moonfltk_F7, "f7");
        CASE(moonfltk_F8, "f8");
        CASE(moonfltk_F9, "f9");
        CASE(moonfltk_F10, "f10");
        CASE(moonfltk_F11, "f11");
        CASE(moonfltk_F12, "f12");
        CASE(moonfltk_F13, "f13");
        CASE(moonfltk_F14, "f14");
        CASE(moonfltk_F15, "f15");
        CASE(moonfltk_F16, "f16");
        CASE(moonfltk_F17, "f17");
        CASE(moonfltk_F18, "f18");
        CASE(moonfltk_F19, "f19");
        CASE(moonfltk_F20, "f20");
//      CASE(FL_F_Last, "f last");
        CASE(FL_Shift_L, "shift l");
        CASE(FL_Shift_R, "shift r");
        CASE(FL_Control_L, "control l");
        CASE(FL_Control_R, "control r");
        CASE(FL_Caps_Lock, "caps lock");
        CASE(FL_Meta_L, "meta l");
        CASE(FL_Meta_R, "meta r");
        CASE(FL_Alt_L, "alt l");
        CASE(FL_Alt_R, "alt r");
        CASE(moonfltk_Alt_Gr, "alt gr");
        CASE(FL_Delete, "delete");
        CASE(FL_Volume_Down , "volume down");
        CASE(FL_Volume_Mute , "volume mute");
        CASE(FL_Volume_Up, "volume up");
        CASE(FL_Media_Play, "media play");
        CASE(FL_Media_Stop, "media stop");
        CASE(FL_Media_Prev, "media prev");
        CASE(FL_Media_Next, "media next");
        CASE(FL_Home_Page, "home page");
        CASE(FL_Mail, "mail");
        CASE(FL_Search, "search");
        CASE(FL_Back, "back");
        CASE(FL_Forward , "forward");
        CASE(FL_Stop, "stop");
        CASE(FL_Refresh , "refresh");
        CASE(FL_Sleep , "sleep");
        CASE(FL_Favorites, "favorites");
#undef CASE
    return (int)luaL_argerror(L, arg, badvalue(L,s));
    }

int push_Key(lua_State *L, int key_)
    {
    int key = key_ & FL_KEY_MASK;

    //printf("key 0x%.8x\n", key_);
    
    if((key & 0xff) == key)
        {
        unsigned char c = (unsigned char)key;
        if(c < 127) /* ASCII */
            lua_pushlstring(L, (char*)&c, 1);
        else /* 128-255: convert to utf8 */
            {
            int len;
            char buf[4];
            int ucs = (int)c;
            len = fl_utf8encode(ucs, buf);
            lua_pushlstring(L, buf, len);
            }
        return 1;
        }

    switch(key)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(moonfltk_Button1, "button1");
        CASE(moonfltk_Button2, "button2");
        CASE(moonfltk_Button3, "button3");
        CASE(moonfltk_Button4, "button4");
        CASE(moonfltk_Button5, "button5");
        CASE(moonfltk_Button6, "button6");
        CASE(moonfltk_Button7, "button7");
        CASE(moonfltk_Button8, "button8");
        CASE(FL_BackSpace, "backspace");
        CASE(FL_Tab, "tab");
        CASE(FL_Iso_Key, "iso key");
        CASE(FL_Enter, "enter");
        CASE(FL_Pause, "pause");
        CASE(FL_Scroll_Lock, "scroll lock");
        CASE(FL_Escape, "escape");
        CASE(FL_Kana, "kana");
        CASE(FL_Eisu, "eisu");
        CASE(FL_Yen, "yen");
        CASE(FL_JIS_Underscore, "jis underscore");
        CASE(FL_Home, "home");
        CASE(FL_Left, "left");
        CASE(FL_Up, "up");
        CASE(FL_Right, "right");
        CASE(FL_Down, "down");
        CASE(FL_Page_Up, "page up");
        CASE(FL_Page_Down, "page down");
        CASE(FL_End, "end");
        CASE(FL_Print, "print");
        CASE(FL_Insert, "insert");
        CASE(FL_Menu, "menu");
        CASE(FL_Help, "help");
        CASE(FL_Num_Lock, "num lock");
//      CASE(FL_KP, "kp");
        CASE(moonfltk_KP0, "kp 0");
        CASE(moonfltk_KP1, "kp 1");
        CASE(moonfltk_KP2, "kp 2");
        CASE(moonfltk_KP3, "kp 3");
        CASE(moonfltk_KP4, "kp 4");
        CASE(moonfltk_KP5, "kp 5");
        CASE(moonfltk_KP6, "kp 6");
        CASE(moonfltk_KP7, "kp 7");
        CASE(moonfltk_KP8, "kp 8");
        CASE(moonfltk_KP9, "kp 9");
        CASE(moonfltk_KP_Add, "kp +");
        CASE(moonfltk_KP_Sub, "kp -");
        CASE(moonfltk_KP_Mul, "kp *");
        CASE(moonfltk_KP_Div, "kp /");
        CASE(moonfltk_KP_Dot, "kp .");
        CASE(moonfltk_KP_Eq, "kp =");
        CASE(FL_KP_Enter, "kp enter");
        CASE(moonfltk_KP_Middle, "kp middle");
//      CASE(FL_KP_Last, "kp last");
        CASE(moonfltk_F1, "f1");
        CASE(moonfltk_F2, "f2");
        CASE(moonfltk_F3, "f3");
        CASE(moonfltk_F4, "f4");
        CASE(moonfltk_F5, "f5");
        CASE(moonfltk_F6, "f6");
        CASE(moonfltk_F7, "f7");
        CASE(moonfltk_F8, "f8");
        CASE(moonfltk_F9, "f9");
        CASE(moonfltk_F10, "f10");
        CASE(moonfltk_F11, "f11");
        CASE(moonfltk_F12, "f12");
        CASE(moonfltk_F13, "f13");
        CASE(moonfltk_F14, "f14");
        CASE(moonfltk_F15, "f15");
        CASE(moonfltk_F16, "f16");
        CASE(moonfltk_F17, "f17");
        CASE(moonfltk_F18, "f18");
        CASE(moonfltk_F19, "f19");
        CASE(moonfltk_F20, "f20");
//      CASE(FL_F_Last, "f last");
        CASE(FL_Shift_L, "shift l");
        CASE(FL_Shift_R, "shift r");
        CASE(FL_Control_L, "control l");
        CASE(FL_Control_R, "control r");
        CASE(FL_Caps_Lock, "caps lock");
        CASE(FL_Meta_L, "meta l");
        CASE(FL_Meta_R, "meta r");
        CASE(FL_Alt_L, "alt l");
        CASE(FL_Alt_R, "alt r");
        CASE(moonfltk_Alt_Gr, "alt gr");
        CASE(FL_Delete, "delete");
        CASE(FL_Volume_Down , "volume down");
        CASE(FL_Volume_Mute , "volume mute");
        CASE(FL_Volume_Up, "volume up");
        CASE(FL_Media_Play, "media play");
        CASE(FL_Media_Stop, "media stop");
        CASE(FL_Media_Prev, "media prev");
        CASE(FL_Media_Next, "media next");
        CASE(FL_Home_Page, "home page");
        CASE(FL_Mail, "mail");
        CASE(FL_Search, "search");
        CASE(FL_Back, "back");
        CASE(FL_Forward , "forward");
        CASE(FL_Stop, "stop");
        CASE(FL_Refresh , "refresh");
        CASE(FL_Sleep , "sleep");
        CASE(FL_Favorites, "favorites");
#undef CASE
        default:
            lua_pushstring(L, "???");
            printf("unknown key 0x%.8x\n", key_);
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | Modifiers                                                            |
 *----------------------------------------------------------------------*/

int check_Modifiers(lua_State *L, int arg)
/* Accepts an integer or a list of strings starting from index=arg */
    {
    unsigned int state = 0;
    const char *s;

    if(lua_type(L, arg) == LUA_TNUMBER)
        return luaL_checkinteger(L, arg) & 0xffff0000;

    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { state |= CODE; goto done; } while(0)
        CASE(FL_SHIFT, "Shift");
        CASE(FL_CAPS_LOCK, "Caps Lock");
        CASE(FL_CTRL, "Ctrl");
        CASE(FL_ALT, "Alt");
        CASE(FL_NUM_LOCK, "Num Lock");
        CASE(FL_META, "Meta");
        CASE(moonfltkSPARE_MODIFIER, "???");
        CASE(FL_SCROLL_LOCK, "Scroll Lock");
        CASE(FL_BUTTON1, "Button1");
        CASE(FL_BUTTON2, "Button2");
        CASE(FL_BUTTON3, "Button3");
        CASE(moonfltkBUTTON4, "Button4");
        CASE(moonfltkBUTTON5, "Button5");
        CASE(moonfltkBUTTON6, "Button6");
        CASE(moonfltkBUTTON7, "Button7");
        CASE(moonfltkBUTTON8, "Button8");
#undef CASE
        return luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return state;
    }

int push_Modifiers(lua_State *L, int state, int pushcode)
    {
    state = state & 0xffff0000; //~(FL_KEY_MASK);
    if(pushcode)
        { lua_pushinteger(L, state); return 1; }

    int n = 0;
#define CASE(CODE,str) if(state & CODE) do { lua_pushstring(L, str); n++; } while(0)
        CASE(FL_SHIFT, "Shift");
        CASE(FL_CAPS_LOCK, "Caps Lock");
        CASE(FL_CTRL, "Ctrl");
        CASE(FL_ALT, "Alt");
        CASE(FL_NUM_LOCK, "Num Lock");
        CASE(FL_META, "Meta");
        CASE(moonfltkSPARE_MODIFIER, "???");
        CASE(FL_SCROLL_LOCK, "Scroll Lock");
        CASE(FL_BUTTON1, "Button1");
        CASE(FL_BUTTON2, "Button2");
        CASE(FL_BUTTON3, "Button3");
        CASE(moonfltkBUTTON4, "Button4");
        CASE(moonfltkBUTTON5, "Button5");
        CASE(moonfltkBUTTON6, "Button6");
        CASE(moonfltkBUTTON7, "Button7");
        CASE(moonfltkBUTTON8, "Button8");
#undef CASE
    if(n==0)
        { lua_pushstring(L, "never"); return 1; }
    return n;
    }

/*----------------------------------------------------------------------*
 | Shortcut                                                             |
 *----------------------------------------------------------------------*/

int check_Shortcut(lua_State *L, int arg)
    {
    int flags = 0;
    int key = -1;
    const char *s;

    while(!lua_isnoneornil(L, arg))
        {
        s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(FL_SHIFT, "Shift");
        CASE(FL_CAPS_LOCK, "Caps Lock");
        CASE(FL_CTRL, "Ctrl");
        CASE(FL_ALT, "Alt");
        CASE(FL_NUM_LOCK, "Num Lock");
        CASE(FL_META, "Meta");
        CASE(FL_SCROLL_LOCK, "Scroll Lock");
        CASE(FL_BUTTON1, "Button1");
        CASE(FL_BUTTON2, "Button2");
        CASE(FL_BUTTON3, "Button3");
        CASE(moonfltkBUTTON4, "Button4");
        CASE(moonfltkBUTTON5, "Button5");
        CASE(moonfltkBUTTON6, "Button6");
        CASE(moonfltkBUTTON7, "Button7");
        CASE(moonfltkBUTTON8, "Button8");
//      CASE(FL_BUTTONS, "Buttons");
//      CASE(FL_COMMAND, "Command"); /* alias for "ctrl" */
//      CASE(FL_CONTROL, "Control"); /* alias for "meta" */
#undef CASE
        if(key == -1)
            key = check_Key(L, arg);
        else
            return (int)luaL_argerror(L, arg, badvalue(L,s));
        done: arg++;
        }
    if(key == -1) 
        return luaL_argerror(L, arg, "key is missing");
    return flags + key;
    }

int push_Shortcut(lua_State *L, int val)
    {
    int n = 0;
    int flags = val & ~FL_KEY_MASK;
    int key = val & FL_KEY_MASK;
#define CASE(CODE,str) if(flags & CODE) do { lua_pushstring(L, str); n++; } while(0)
        CASE(FL_SHIFT, "Shift");
        CASE(FL_CAPS_LOCK, "Caps Lock");
        CASE(FL_CTRL, "Ctrl");
        CASE(FL_ALT, "Alt");
        CASE(FL_NUM_LOCK, "Num Lock");
        CASE(FL_META, "Meta");
        CASE(FL_SCROLL_LOCK, "Scroll Lock");
#undef CASE
#if 0
    if((flags & FL_BUTTONS) == FL_BUTTONS)
        { lua_pushstring(L, "Buttons"); n++; }
    else
#endif
        {
#define CASE(CODE,str) if(flags & CODE) do { lua_pushstring(L, str); n++; } while(0)
        CASE(FL_BUTTON1, "Button1");
        CASE(FL_BUTTON2, "Button2");
        CASE(FL_BUTTON3, "Button3");
        CASE(moonfltkBUTTON4, "Button4");
        CASE(moonfltkBUTTON5, "Button5");
        CASE(moonfltkBUTTON6, "Button6");
        CASE(moonfltkBUTTON7, "Button7");
        CASE(moonfltkBUTTON8, "Button8");
#undef CASE
        }
    push_Key(L, key);
    return n + 1;
    }


