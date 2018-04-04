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

//static void Fl::args(int argc, char **argv);
// args(progname)
// args(args)
// args(progname [, args])
static int Args(lua_State *L)
    {
    int table_index = 1;
    char **argv;
    const char *arg = NULL;
    int i, argc = 1;
    if(!lua_istable(L, 1))
        {
        table_index = 2;
        arg = luaL_checkstring(L, 1);
        }
    if(lua_istable(L, table_index))
        argc += luaL_len(L, table_index);
    argv = (char**)malloc(sizeof(char*) * argc);
    argv[0] = strdup(arg ? arg : "moonfltk");
    if(!argv)
        return luaL_error(L, "cannot allocate memory");
    if(argc > 1)
        {
        for(i = 1; i < argc; i++)
            {
            lua_geti(L, table_index, i);
            arg = luaL_checkstring(L, -1);
            argv[i] = strdup(arg);
            if(!argv[i])
                return luaL_error(L, "cannot allocate memory");
            lua_pop(L, 1);
            }
        }
    Fl::args(argc, argv);
    for(i = 0; i < argc; i++)
        free(argv[i]);
    free(argv);
    return 0;
    }


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

// Handling thread messages that can be send to the main thread.
// 
// The FLTK documentation says: 
//      The default message handler saves the last message which can be 
//      accessed using the Fl::thread_message() function.
//
// With this we can get the problem, that dropped message pointers 
// will not be freed on the main thread leading to a memory leak.
//
// To solve this, we implement a message handler, that handles a
// message queue. FLTK itself manages a queue of these message handlers. 

struct AwakeMessage {
    size_t        len;
    AwakeMessage* next;
};
static AwakeMessage* firstAwakeMessage = 0; // should only be accessed on main thread
static AwakeMessage* lastAwakeMessage  = 0; // should only be accessed on main thread

// this message handler is invoked by FLTK on the main thread
static void handleAwake(void *data)
    {
    if (data != 0)
        {
        AwakeMessage* m = (AwakeMessage*) data;
        if (lastAwakeMessage != 0)
            {
            lastAwakeMessage->next = m;
            lastAwakeMessage = m;
            }
        else
            {
            firstAwakeMessage = m;
            lastAwakeMessage  = m;
            }
        }
    }

// Awakes the FLTK main thread.
//
// If invoked with an optional message string argument, this function will
// also queues this message string. The queued message string can be obtained
// using the function fl.thread_mesesage.
//
// If message strings are queued, fl.thread_message should be called to 
// remove messages from the queue.
// 
// Returns true if the message could be queued or no message was given,
// returns false, if the message could not be queued.
//
// In all cases the main FLTK thread will be awaken from wait.
//
static int Awake(lua_State *L)
    {
        int isOk = 1;

        if (lua_gettop(L) == 0 || lua_isnil(L, 1))
            Fl::awake();
        else if (lua_isstring(L, 1))
            {
            size_t len;
            const char* s = lua_tolstring(L, 1, &len);
            AwakeMessage* m = (AwakeMessage*)malloc(sizeof(AwakeMessage) + len); 
            if (m != 0)
                {
                // do not use firstAwakeMessage and lastAwakeMessage,
                // since code here can be running on any thread.
                memcpy(m + 1, s, len);
                m->len  = len;
                m->next = 0;
                isOk = (Fl::awake(handleAwake, m) == 0);
                if (!isOk)
                    // Handler could not be queued, but 
                    // FLTK has awaken the main thread.
                    free(m);
                }
            else
                {
                // out of memory ->
                // awake the main thread without message handler
                isOk = 0;
                Fl::awake();
                }
            }
        else
            return luaL_argerror(L, 1, "string expected");
        lua_pushboolean(L, isOk);
        return 1;
    }

// Obtains next message string that was put into
// the queue with the function fl.awake.
// The returned message string is removed from the queue.
// Returns nil if there are no more message strings in the queue.
//
static int Thread_message(lua_State *L)
    {
        Fl::lock(); // lock is needed, because Thread_message 
                    // could be invoked from any thread.
        if (firstAwakeMessage != 0)
            {
            AwakeMessage* m = firstAwakeMessage;
            firstAwakeMessage = m->next;
            if (lastAwakeMessage == m)
                lastAwakeMessage = 0;
            lua_pushlstring(L, (char*)(m + 1), m->len);
            free(m);
            Fl::unlock();
            return 1;
            }
        else
            {
            Fl::unlock();
            return 0;
            }
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
        { "args", Args },
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
        { "thread_message",  Thread_message },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg BackgroundFunctions[] = 
    {
        { "awake", Awake },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Fl(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

void moonfltk_open_Fl_for_background(lua_State *L)
    {
    luaL_setfuncs(L, BackgroundFunctions, 0);
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
