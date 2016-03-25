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

#if 0
//static void   Fl::default_atclose(Fl_Window *, void *)
static int Default_atclose(lua_State *L) //@@ serve?
    {
    (void)L;
    return 0;
    }
#endif


//@@ What if a window to be pushed is not bound to a userdata?
//   (e.g. a window created with a common dialog ...).

static int First_window(lua_State *L)
    {
    if(lua_isnone(L, 1))
        {
        Fl_Window *p = Fl::first_window();
        if(p) return 0;
        push_Window(L, p);
        return 1;
        }
    Fl::first_window(check_Window(L, 1));
    return 0;
    }

static int Next_window(lua_State *L)
    {
    Fl_Window *p = Fl::next_window(check_Window(L, 1));
    if(!p) return 0;
    push_Window(L, p);
    return 1;
    }

static int Grab(lua_State *L)
    {
    if(lua_isnone(L, 1))
        {
        Fl_Window *p = Fl::grab();
        if(p) return 0;
        push_Window(L, p);
        return 1;
        }
    Fl::grab(check_Window(L, 1));
    return 0;
    }



static int Modal(lua_State *L) 
    {
    Fl_Window *p = Fl::modal();
    if(!p) return 0;
    push_Window(L, p);
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
//      { "default_atclose", Default_atclose },
        { "first_window", First_window },
        { "grab", Grab },
        { "modal", Modal },
        { "next_window", Next_window },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_FlWindow(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

