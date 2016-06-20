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

FUNC_DOUBLE_VOID(Fl::version, Version)

static int Option(lua_State *L)
    {
    Fl::Fl_Option opt = check_Option(L, 1);
    if(lua_isnone(L, 2))
        {
        lua_pushboolean(L, Fl::option(opt));
        return 1;
        }
    int val = checkboolean(L, 2);
    Fl::option(opt, val);
    return 0;
    }

static int Wait(lua_State *L)
/* boolean wait()
 * boolean wait([time])  returns nil if an error occurred
 */
    {
    if(lua_isnone(L, 1))
        {
        lua_pushboolean(L, Fl::wait());
        return 1;
        }
    double time = luaL_checknumber(L, 1);
    double rc = Fl::wait(time);
    if(rc<0)
        return 0; /* an error occurred */
    lua_pushboolean(L, rc > 0);
    return 1;
    }

static int Check(lua_State *L)
    {
    lua_pushboolean(L, Fl::check());
    return 1;
    }

static int Ready(lua_State *L)
    {
    lua_pushboolean(L, Fl::ready());
    return 1;
    }

static int Run(lua_State *L)
    {
    lua_pushboolean(L, (Fl::run() == 0));   
    return 1;
    }

#if 0
static int Readqueue(lua_State *L)
    {
    Fl_Widget *w = Fl::readqueue();
    if(!w)
        return 0;
    push_Widget(L, w);
    return 1;
    }
#endif


FUNC_BOOLEAN_VOID(Fl::damage, Damage)
FUNC_VOID_VOID(Fl::redraw, Redraw)
FUNC_VOID_VOID(Fl::flush, Flush)


static int Scheme(lua_State *L)
    {
    if(lua_isnone(L, 1))
        {
        const char *s = Fl::scheme();
        if(!s) return 0;
        lua_pushstring(L, s);
        return 1;
        }
    Fl::scheme(luaL_optstring(L, 1, NULL));
    return 0;
    }

static int Is_scheme(lua_State *L)
    {
    const char *s = luaL_checkstring(L, 1);
    lua_pushboolean(L, Fl::is_scheme(s));
    return 1;
    }

FUNC_VOID_VOID(Fl::reload_scheme, Reload_scheme)

static int Displayyyy(lua_State *L)
    {
    Fl::display(luaL_checkstring(L, 1));
    return 0;
    }

static int Visuallll(lua_State *L)
    {
    Fl_Mode mode = check_Mode(L, 1);
    lua_pushboolean(L, Fl::visual(mode));
    return 1;
    }


static int Gl_visual(lua_State *L) 
    {
#if USE_GL
 // static int gl_visual(int, int *alist=0);
 // See Fl_Gl_Window for a list of additional values for the argument.
    Fl_Mode mode = check_Mode(L, 1);
    lua_pushboolean(L, Fl::gl_visual(mode /*@@, alist*/));
#else
    lua_pushboolean(L, 0);
#endif
    return 1;
    }

FUNC_GETSET_BOOLEAN(Fl::visible_focus, Visible_focus)


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "check", Check },
        { "damage", Damage },
        { "display", Displayyyy },
        { "flush", Flush },
        { "gl_visual", Gl_visual },
        { "is_scheme", Is_scheme },
        { "option", Option  },
//      { "readqueue", Readqueue },
        { "ready", Ready },
        { "redraw", Redraw },
        { "reload_scheme", Reload_scheme },
        { "scheme", Scheme  },
        { "run", Run },
        { "version", Version },
        { "visible_focus", Visible_focus },
        { "visual", Visuallll },
        { "wait", Wait },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_Fl(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

#if(0) //@@ not/not yet implemented stuff:
// argument parsers:
typedef int (*Fl_Args_Handler)(int argc, char **argv, int &i);
static int Fl::arg(int argc, char **argv, int& i);
static int Fl::args(int argc, char **argv, int& i, Fl_Args_Handler cb = 0);
static void Fl::args(int argc, char **argv);
static const char* const Fl::help;

// Multithreading ----------------------------------------------------
typedef void (*Fl_Awake_Handler)(void *data);
static int Fl::lock();
static void Fl::unlock();
static void Fl::awake(void* message = 0);
static int Fl::awake(Fl_Awake_Handler cb, void* message = 0);
static void* Fl::thread_message(); // platform dependent

// Widget deletion / Fl_Widget_Tracker class --------------------------
static void Fl::delete_widget(Fl_Widget *w);
static void Fl::do_widget_deletion();
static void Fl::watch_widget_pointer(Fl_Widget *&w);
static void Fl::release_widget_pointer(Fl_Widget *&w);
static void Fl::clear_widget_pointer(Fl_Widget const *w);
class FL_EXPORT Fl_Widget_Tracker { };

// Cairo -------------------------------------------------------------
static cairo_t * Fl::cairo_make_current(Fl_Window* w);
static void Fl::cairo_autolink_context(bool alink) {cairo_state_.autolink(alink);}
static bool Fl::cairo_autolink_context() {return cairo_state_.autolink();}
static cairo_t * Fl::cairo_cc() { return cairo_state_.cc(); }
static void Fl::cairo_cc(cairo_t * c, bool own=false){ cairo_state_.cc(c, own); } 

#endif
