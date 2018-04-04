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

#ifndef funcDEFINED
#define funcDEFINED

/********************************************************************************
 * Ad-hoc code generators                                                       *
 ********************************************************************************/

/*------------------------------------------------------------------------------*
 | Constructors and destructors                                                 |
 *------------------------------------------------------------------------------*/

#define WIDGET_NEW(T, Func)                     \
/* Generic widget constructor:  T(x, y, w, h, label=0) */\
static int Func(lua_State *L)                   \
    {                                           \
    int arg = 1;                                \
    int x, y, w, h;                             \
    const char* label;                          \
    x = luaL_checkinteger(L, arg++);            \
    y = luaL_checkinteger(L, arg++);            \
    w = luaL_checkinteger(L, arg++);            \
    h = luaL_checkinteger(L, arg++);            \
    label = luaL_optstring(L, arg, NULL);       \
    Fl_##T *p = new Fl_##T(x, y, w, h, NULL);   \
    if(label) p->copy_label(label); /* see NOTE below */\
    DBG_CREATE(T,p);                            \
    newuserdata(L, p, MT_##T);                  \
    return 1;                                   \
    }

/* NOTE: since widget constructors just store the label pointer,
 *       and since strings passed by the scripts may be GC'ed,
 *       we pass label=NULL to the constructor and then we set 
 *       the label with copy_label(), which strdup()s it.
 */

#define WIDGET_DELETE(T, Func)                      \
/* Generic widget destructor */                     \
static int Func(lua_State *L)                       \
    {                                               \
    Fl_##T *p = check_##T(L, 1);                    \
    DBG_DELETE(T,p);                                \
    if(freeuserdata(L, p)) delete p;                \
    return 0;                                       \
    }

#define BOX_DELETE WIDGET_DELETE
#define BOX_NEW(T, Func)                            \
static int Func(lua_State *L)                       \
/* box(x, y, w, z [, label]) */                     \
/* box(boxtype, x, y, w, z [, label]) */            \
    {                                               \
    Fl_##T *p;                                      \
    Fl_Boxtype boxtype;                             \
    int x, y, w, h, bt = 0;                         \
    const char* label;                              \
    int arg = 1;                                    \
    int n = lua_gettop(L);                          \
    if (n >= 6 || (n == 5 && lua_isinteger(L, 5)))  \
        { boxtype = check_Boxtype(L, arg++); bt=1; }\
    x = luaL_checkinteger(L, arg++);                \
    y = luaL_checkinteger(L, arg++);                \
    w = luaL_checkinteger(L, arg++);                \
    h = luaL_checkinteger(L, arg++);                \
    label = luaL_optstring(L, arg, "");             \
    if(bt)                                          \
        p = new Fl_##T(boxtype, x, y, w, h, NULL);  \
    else                                            \
        p = new Fl_##T(x, y, w, h, NULL);           \
    if(label) p->copy_label(label);                 \
    DBG_CREATE(T,p);                                \
    newuserdata(L, p, MT_##T);                      \
    return 1;                                       \
    }

#define BUTTON_NEW WIDGET_NEW
#define BUTTON_DELETE WIDGET_DELETE

#define SLIDER_NEW WIDGET_NEW
#define SLIDER_DELETE WIDGET_DELETE

#define ROLLER_NEW WIDGET_NEW
#define ROLLER_DELETE WIDGET_DELETE

#define GROUP_NEW WIDGET_NEW
#define GROUP_DELETE(T, Func)                       \
/* Generic destructor (removes all elements before deleting) */\
static int Func(lua_State *L)                       \
    {                                               \
    Fl_##T *p = check_##T(L, 1);                    \
    DBG_DELETE(T,p);                                \
    groupclear(L, p);                               \
    if(freeuserdata(L, p)) delete p;                \
    return 0;                                       \
    }

#define WINDOW_DELETE GROUP_DELETE
#define WINDOW_NEW(T, Func)                     \
/* Generic window constructor: */               \
/* T(x, y, w, h, label=0) */                    \
/* T(w, h, label=0) */                          \
static int Func(lua_State *L)                   \
    {                                           \
    Fl_##T *p;                                  \
    int arg = 1;                                \
    int x, y, w, h;                             \
    const char* label;                          \
    x = luaL_checkinteger(L, arg++);            \
    y = luaL_checkinteger(L, arg++);            \
    if(lua_isinteger(L, arg))                   \
        {                                       \
        w = luaL_checkinteger(L, arg++);        \
        h = luaL_checkinteger(L, arg++);        \
        }                                       \
    else                                        \
        { w = x; h = y; }                       \
    label = luaL_optstring(L, arg, NULL);       \
    if(arg==5)                                  \
        p = new Fl_##T(x, y, w, h, NULL);       \
    else                                        \
        p = new Fl_##T(w, h, NULL);             \
    if(label) p->copy_label(label);             \
    DBG_CREATE(T,p);                            \
    newuserdata(L, p, MT_##T);                  \
    return 1;                                   \
    }


/* Constructors and destructors for Fl_Group descendants having scrollbars.
 * The scrollbars are children of the group, automatically created/deleted.
 *
 * There are two problems to deal with:
 * - if the scrollbars need to be directly accessible from Lua, we need to
 *   bind them to userdata in the constructor and unbind in the destructor.
 * - in the destructor, in order to avoid double deletes we have to remove
 *   all the children widgets but the scrollbars from the group before
 *   deleting it.
 *
 *   The 'facts' to consider here are: 
 *   - widgets bound to userdata are garbage collected (-> deleted)
 *   - a group, when deleted, automatically deletes its children
 *   - a group with scrollbars typically deletes them even if they have
 *     been removed (or there may be no means to remove them at all)
 *
 *   So the policy is: when deleting a group widget, we empty it first,
 *   leaving in only the automatically created children (the scrollbars).
 *   FLTK takes care of deleting the empty group and the scrollbars, while
 *   we (MoonFLTK) take care of deleting any other child.
 *
 */

#define HAS_SCROLLBARS_NEW(T, Func)             \
static int Func(lua_State *L)                   \
    {                                           \
    int arg = 1;                                \
    int x, y, w, h;                             \
    const char* label;                          \
    x = luaL_checkinteger(L, arg++);            \
    y = luaL_checkinteger(L, arg++);            \
    w = luaL_checkinteger(L, arg++);            \
    h = luaL_checkinteger(L, arg++);            \
    label = luaL_optstring(L, arg, NULL);       \
    Fl_##T *p = new Fl_##T(x, y, w, h, NULL);   \
    if(label) p->copy_label(label);             \
    DBG_CREATE(T,p);                            \
    newuserdata(L, &p->scrollbar, MT_Scrollbar);\
    newuserdata(L, &p->hscrollbar, MT_Scrollbar);\
    lua_pop(L, 2);                              \
    newuserdata(L, p, MT_##T);                  \
    return 1;                                   \
    }

#define HAS_SCROLLBARS_DELETE(T, Func, clearfunc)   \
static int Func(lua_State *L)                       \
    {                                               \
    Fl_##T *p = check_##T(L, 1);                    \
    DBG_DELETE(T,p);                                \
    /* the next call to freeuserdata() will return 0, preventing deletion: */\
    /* (we must not delete p->scrollbar and p->hscrollbar) */\
    freeuserdata(L, &p->scrollbar);                 \
    freeuserdata(L, &p->hscrollbar);                \
    clearfunc(L, p);                                \
    if(freeuserdata(L, p)) delete p;                \
    return 0;                                       \
    }

#define BROWSER_NEW     HAS_SCROLLBARS_NEW
#define BROWSER_DELETE(T, Func) HAS_SCROLLBARS_DELETE(T, Func, browserclear)
#define SCROLL_NEW  HAS_SCROLLBARS_NEW
#define SCROLL_DELETE(T, Func) HAS_SCROLLBARS_DELETE(T, Func, scrollclear)

//@@ also has scrollbars, but named differently (damn...) and private (I hate C++...):
#define HELP_VIEW_NEW       GROUP_NEW 
#define HELP_VIEW_DELETE    GROUP_DELETE


#define TABLE_NEW WIDGET_NEW
#define TABLE_DELETE(T, Func)                       \
static int Func(lua_State *L)                       \
    {                                               \
    Fl_##T *p = check_##T(L, 1);                    \
    DBG_DELETE(T,p);                                \
    tableclear(L, p);                               \
    if(freeuserdata(L, p)) delete p;                \
    return 0;                                       \
    }

#define TREE_NEW WIDGET_NEW
#define TREE_DELETE(T, Func)                        \
static int Func(lua_State *L)                       \
    {                                               \
    Fl_##T *p = check_##T(L, 1);                    \
    DBG_DELETE(T,p);                                \
    treeclear(L, p);                                \
    if(freeuserdata(L, p)) delete p;                \
    return 0;                                       \
    }



/*------------------------------------------------------------------------------*
 | Generic methods for Fl_Xxx widgets                                           |
 *------------------------------------------------------------------------------*/

#define WIDGET_TYPE(T, Func)                    \
static int Func(lua_State *L)                   \
/* set: p->type(code) or p->type(string') */    \
/* get: code, string = p->type()        */      \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    if(lua_isnone(L, 2))                        \
        {                                       \
        int t = p->type();                      \
        lua_pushinteger(L, t);                  \
        push_##T##Rtti(L, p->type());           \
        return 2;                               \
        }                                       \
    if(lua_type(L, 2) == LUA_TNUMBER)           \
        p->type(luaL_checkinteger(L, 2));       \
    else                                        \
        p->type(check_##T##Rtti(L, 2));         \
    return 0;                                   \
    }

/* label() method (not virtual: we need to use this for every subclass that overrides it) */
//@@ void label(const char* label, const char* iconlabel); // platform dependent
//@@ void label(Fl_Labeltype a, const char* b) {label_.type = a; label_.value = b;}
#define WIDGET_LABEL(T, Func)                       \
static int Func(lua_State *L)                       \
    {                                               \
    const char *label;                              \
    Fl_##T *p = check_##T(L, 1);                    \
    if(lua_isnone(L, 2)) /* get */                  \
        {                                           \
        label = p->label();                         \
        if (!label) return 0;                       \
        lua_pushstring(L, label);                   \
        return 1;                                   \
        }                                           \
    label = luaL_checkstring(L, 2);                 \
    p->copy_label(label); /* p->label(label); */    \
    return 0;                                       \
    }

#define WINDOW_SHOW(T, Func)                    \
static int Func(lua_State *L)                   \
/* overrides widget->show() */                  \
/* show()               */                      \
/* show(progname)       */                      \
/* show(progname, args) */                      \
/* show(args)           */                      \
    {                                           \
    Fl_##T *window = check_##T(L, 1);           \
    if(!lua_isnoneornil(L, 2))                  \
        {                                       \
        int table_index = 2;                    \
        char **argv;                            \
        const char *arg = NULL;                 \
        int i, argc = 1;                        \
        if(!lua_istable(L, 2))                  \
            {                                   \
            table_index = 3;                    \
            arg = luaL_checkstring(L, 2);       \
            }                                   \
        if(lua_istable(L, table_index))         \
            argc += luaL_len(L, table_index);   \
        argv = (char**)malloc(sizeof(char*) * argc);        \
        argv[0] = strdup(arg ? arg : "moonfltk");           \
        if(!argv)                                           \
            return luaL_error(L, "cannot allocate memory"); \
        if(argc > 1)                                        \
            {                                               \
            for(i = 1; i < argc; i++)                       \
                {                                           \
                lua_geti(L, table_index, i);                \
                arg = luaL_checkstring(L, -1);              \
                argv[i] = strdup(arg);                      \
                if(!argv[i])                                \
                    return luaL_error(L, "cannot allocate memory");\
                lua_pop(L, 1);                              \
                }                                           \
            }                                               \
        /* show(argc, argv) opens displays and           */ \
        /* re-evealuates global settings                 */ \
        /* (e.g. boxtype drawing styles)                 */ \
        window->show(argc, argv);                           \
        for(i = 0; i < argc; i++)                           \
            free(argv[i]);                                  \
        free(argv);                                         \
        }                                                   \
    else                                                    \
        /* show() does not re-evaluate global settings.  */ \
        window->show();                                     \
    return 0;                                               \
    }


/*------------------------------------------------------------------------------*
 | Generic methods for Fl_Xxx classes                                           |
 *------------------------------------------------------------------------------*/

#define BOOLEAN_EVENT(T, method, Func)          \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushboolean(L, p->method(check_Event(L, 2)));\
    return 1;                                   \
    }   


#define VOID_VOID(T, method, Func)              \
/* void T::method() */                          \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    p->method();                                \
    return 0;                                   \
    }   

#define VOID_BOOLEAN(T, method, Func)           \
/* void T::method(boolean) */                   \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    p->method(checkboolean(L, 2));              \
    return 0;                                   \
    }   


#define VOID_INT(T, method, Func)               \
/* void T::method(int) */                       \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    p->method(luaL_checkinteger(L, 2));         \
    return 0;                                   \
    }   

#define VOID_INT(T, method, Func)               \
/* void T::method(int) */                       \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    p->method(luaL_checkinteger(L, 2));         \
    return 0;                                   \
    }   

#define VOID_INT_INT(T, method, Func)           \
/* void T::method(int, int) */                      \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    p->method(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));            \
    return 0;                                   \
    }   

#define VOID_INDEX_INDEX(T, method, Func)       \
/* void T::method(int, int) */                  \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    p->method(checkindex(L, 2), checkindex(L, 3));  \
    return 0;                                   \
    }   

#define VOID_INT4(T, method, Func)      \
static int Func(lua_State *L)           \
    {                                   \
    Fl_##T *p = check_##T(L, 1);        \
    int x = luaL_checkinteger(L, 2);    \
    int y = luaL_checkinteger(L, 3);    \
    int w = luaL_checkinteger(L, 4);    \
    int h = luaL_checkinteger(L, 5);    \
    p->method(x,y,w,h);                 \
    return 0;                           \
    }

#define VOID_NUMBER(T, method, Func)            \
/* void T::method(double) */                    \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    p->method(luaL_checknumber(L, 2));          \
    return 0;                                   \
    }   

#define VOID_FLOAT VOID_NUMBER
#define VOID_DOUBLE VOID_NUMBER

#define VOID_STRING(T, method, Func)            \
/* void T::method(conts char*) */               \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    p->method(luaL_checkstring(L, 2));          \
    return 0;                                   \
    }   

#define VOID_NUMBER_NUMBER(T, method, Func)     \
/* void T::method(double, double) */            \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    p->method(luaL_checknumber(L, 2), luaL_checknumber(L, 3)); \
    return 0;                                   \
    }   

#define VOID_FLOAT_FLOAT VOID_NUMBER_NUMBER
#define VOID_DOUBLE_DOUBLE VOID_NUMBER_NUMBER

#define BOOLEAN_VOID(T, method, Func)           \
/* boolean T::method() */                       \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushboolean(L,  p->method());           \
    return 1;                                   \
    }   

#define BOOLEAN_INT(T, method, Func)            \
/* boolean T::method(int) */                    \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushboolean(L,  p->method(luaL_checkinteger(L, 2)));\
    return 1;                                   \
    }   

#define STRING_VOID(T, method, Func)            \
/* const char* T::method() */                   \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    const char *s = p->method();                \
    if(!s) return 0;                            \
    lua_pushstring(L,  s);                      \
    return 1;                                   \
    }   

#define INT_VOID(T, method, Func)               \
/* int T::method() */                           \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushinteger(L,  p->method());           \
    return 1;                                   \
    }   

#define INDEX_VOID(T, method, Func)             \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    pushindex(L,  p->method());                 \
    return 1;                                   \
    }   


#define INT_INT(T, method, Func)                \
/* int T::method(int) */                        \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushinteger(L,  p->method(luaL_checkinteger(L, 2)));\
    return 1;                                   \
    }   

#define INDEX_INDEX(T, method, Func)            \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    pushindex(L,  p->method(checkindex(L, 2))); \
    return 1;                                   \
    }   


#define INT_INDEX(T, method, Func)              \
/* int T::method(int) */                        \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushinteger(L,  p->method(checkindex(L, 2)));\
    return 1;                                   \
    }   

#define UINT_INDEX(T, method, Func)             \
/* int T::method(int) */                        \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushinteger(L,  (unsigned int)(p->method(checkindex(L, 2))));\
    return 1;                                   \
    }   


#define INT_INT_INT(T, method, Func)            \
/* int T::method(int, int) */                   \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushinteger(L,  p->method(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3)));\
    return 1;                                   \
    }   

#define BOOLEAN_INT_INT(T, method, Func)        \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushboolean(L,  p->method(luaL_checkinteger(L, 2), luaL_checkinteger(L, 3)));\
    return 1;                                   \
    }   

#define BOOLEAN_INDEX_INDEX(T, method, Func)        \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushboolean(L,  p->method(checkindex(L, 2), checkindex(L, 3)));\
    return 1;                                   \
    }   

#define INT_INDEX_INDEX(T, method, Func)        \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushinteger(L,  p->method(checkindex(L, 2), checkindex(L, 3)));\
    return 1;                                   \
    }   

#define INT_INDEX_INT(T, method, Func)          \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushinteger(L,  p->method(checkindex(L, 2), luaL_checkinteger(L, 3)));\
    return 1;                                   \
    }   


#define INDEX_INDEX_INT(T, method, Func)        \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    pushindex(L,  p->method(checkindex(L, 2), luaL_checkinteger(L, 3)));\
    return 1;                                   \
    }   


#define NUMBER_VOID(T, method, Func)            \
/* double T::method() */                        \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushnumber(L,  p->method());            \
    return 1;                                   \
    }   

#define FLOAT_VOID NUMBER_VOID
#define DOUBLE_VOID NUMBER_VOID

#define NUMBER_NUMBER(T, method, Func)          \
/* double T::method(double) */                  \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    lua_pushnumber(L,  p->method(luaL_checknumber(L, 2)));  \
    return 1;                                   \
    }   

#define FLOAT_FLOAT NUMBER_NUMBER
#define DOUBLE_DOUBLE NUMBER_NUMBER

#define POINTER_VOID(T, V, method, Func)        \
/* V* T::method() */                            \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    Fl_##V *v = p->method();                    \
    if(!v) return 0;                            \
    push_##V(L, v);                             \
    return 1;                                   \
    }


#define GETSET_BOOLEAN(T, method, Func)         \
static inline int Func(lua_State *L)            \
/* boolean T::method() , void T::method(boolean) */\
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    if(lua_isnone(L, 2))                        \
        {                                       \
        lua_pushboolean(L, p->method());        \
        return 1;                               \
        }                                       \
    p->method(checkboolean(L, 2));              \
    return 0;                                   \
    }   

#define GETSET_BOOLEAN_INT(T, method, Func)     \
static inline int Func(lua_State *L)            \
/* boolean T::method(int) , void T::method(int, boolean) */\
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    int n = luaL_checkinteger(L, 2);            \
    if(lua_isnone(L, 3))                        \
        {                                       \
        lua_pushboolean(L, p->method(n));       \
        return 1;                               \
        }                                       \
    p->method(n, checkboolean(L, 3));           \
    return 0;                                   \
    }   


#define GETSET_INT(T, method, Func)             \
static inline int Func(lua_State *L)            \
/* int T::method() , void T::method(int)     */ \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    if(lua_isnone(L, 2))                        \
        {                                       \
        lua_pushinteger(L, p->method());        \
        return 1;                               \
        }                                       \
    p->method(luaL_checkinteger(L, 2));         \
    return 0;                                   \
    }   

#define GETSET_INT_INT(T, method, Func)     \
static inline int Func(lua_State *L)            \
/* int T::method(int) , void T::method(int, int) */\
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    int n = luaL_checkinteger(L, 2);            \
    if(lua_isnone(L, 3))                        \
        {                                       \
        lua_pushinteger(L, p->method(n));       \
        return 1;                               \
        }                                       \
    p->method(n, luaL_checkinteger(L, 3));      \
    return 0;                                   \
    }   


#define GETSET_INDEX(T, method, Func)           \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    if(lua_isnone(L, 2))                        \
        {                                       \
        pushindex(L, p->method());              \
        return 1;                               \
        }                                       \
    p->method(checkindex(L, 2));                \
    return 0;                                   \
    }   

#define GETSET_INT_INDEX(T, method, Func)       \
static inline int Func(lua_State *L)            \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    int n = checkindex(L, 2);                   \
    if(lua_isnone(L, 3))                        \
        {                                       \
        lua_pushinteger(L, p->method(n));       \
        return 1;                               \
        }                                       \
    p->method(n, luaL_checkinteger(L, 3));      \
    return 0;                                   \
    }   



#define GETSET_NUMBER(T, method, Func)          \
static inline int Func(lua_State *L)            \
/* double T::method() , void T::method(double)  */  \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    if(lua_isnone(L, 2))                        \
        {                                       \
        lua_pushnumber(L, p->method());         \
        return 1;                               \
        }                                       \
    p->method(luaL_checknumber(L, 2));          \
    return 0;                                   \
    }   

#define GETSET_FLOAT GETSET_NUMBER
#define GETSET_DOUBLE GETSET_NUMBER

#define GETSET_ASCII(T, method, Func)           \
/* char T::method() , void T::method(char) */   \
/* where char is represented in Lua as len=1 string */\
static int Func(lua_State *L)           \
    {                                   \
    Fl_##T *p = check_##T(L, 1);        \
    if(lua_isnone(L, 2))                \
        {                               \
        pushascii(L, p->method());      \
        return 1;                       \
        }                               \
    p->method(checkascii(L, 2));        \
    return 0;                           \
    }

#define GETSET_INTTYPE(T, V, method, Func)          \
/* V T::method() , void T::method(V)     */         \
/* where V is an integer type (enum or alias of an integer )*/\
static int Func(lua_State *L)                       \
    {                                               \
    Fl_##T *p = check_##T(L, 1);                    \
    if(lua_isnone(L, 2))                            \
        {                                           \
        push_##V(L, p->method());                   \
        return 1;                                   \
        }                                           \
    p->method( check_##V(L, 2));                    \
    return 0;                                       \
    }

#define GETSET_ENUM GETSET_INTTYPE

#define GETSET_POINTER(T, V, method, Func)          \
/* V* T::method() , void T::method(V*)  */          \
/* where V is an Fl_ type */                        \
static int Func(lua_State *L)                       \
    {                                               \
    Fl_##T *p = check_##T(L, 1);                    \
    if(lua_isnone(L, 2))                            \
        {                                           \
        Fl_##V *v = p->method();                    \
        if(!v) return 0;                            \
        push_##V(L, v);                             \
        return 1;                                   \
        }                                           \
    p->method(check_##V(L, 2));                     \
    return 0;                                       \
    }

#define GETSET_POINTER_OPT(T, V, method, Func)      \
/* V* T::method() , void T::method(V*)  */          \
/* where V is an Fl_ type */                        \
static int Func(lua_State *L)                       \
    {                                               \
    Fl_##T *p = check_##T(L, 1);                    \
    if(lua_isnone(L, 2))                            \
        {                                           \
        Fl_##V *v = p->method();                    \
        if(!v) return 0;                            \
        push_##V(L, v);                             \
        return 1;                                   \
        }                                           \
    p->method(test_##V(L, 2)); /* may be nil */     \
    return 0;                                       \
    }


#define GETSET_STRING(T, method, Func)          \
static inline int Func(lua_State *L)            \
/* const char* T::method() , void T::method(const char*)     */ \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    if(lua_isnone(L, 2))                        \
        {                                       \
        const char *s = p->method();            \
        if(!s) return 0;                        \
        lua_pushstring(L, s);                   \
        return 1;                               \
        }                                       \
    p->method(luaL_checkstring(L, 2));          \
    return 0;                                   \
    }   

#define GETSET_STRING_OPT(T, method, Func)          \
static inline int Func(lua_State *L)            \
/* const char* T::method() , void T::method(const char*)     */ \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    if(lua_isnone(L, 2))                        \
        {                                       \
        const char *s = p->method();            \
        if(!s) return 0;                        \
        lua_pushstring(L, s);                   \
        return 1;                               \
        }                                       \
    p->method(luaL_optstring(L, 2, NULL));      \
    return 0;                                   \
    }   


/*------------------------------------------------------------------------------*
 | Generic functions (or static methods)                                        |
 *------------------------------------------------------------------------------*/

#define FUNC_VOID_VOID(func, Func)  \
static int Func(lua_State *L)       \
    {                               \
    (void)L;                        \
    func();                         \
    return 0;                       \
    }

#define FUNC_VOID_INT3(func, Func)      \
static int Func(lua_State *L)           \
    {                                   \
    int a = luaL_checkinteger(L, 1);    \
    int b = luaL_checkinteger(L, 2);    \
    int c = luaL_checkinteger(L, 3);    \
    func(a,b,c);                        \
    return 0;                           \
    }

#define FUNC_VOID_INT4(func, Func)      \
static int Func(lua_State *L)           \
    {                                   \
    int a = luaL_checkinteger(L, 1);    \
    int b = luaL_checkinteger(L, 2);    \
    int c = luaL_checkinteger(L, 3);    \
    int d = luaL_checkinteger(L, 4);    \
    func(a,b,c,d);                      \
    return 0;                           \
    }

#define FUNC_VOID_INT5(func, Func)      \
static int Func(lua_State *L)           \
    {                                   \
    int a = luaL_checkinteger(L, 1);    \
    int b = luaL_checkinteger(L, 2);    \
    int c = luaL_checkinteger(L, 3);    \
    int d = luaL_checkinteger(L, 4);    \
    int e = luaL_checkinteger(L, 5);    \
    func(a,b,c,d,e);                    \
    return 0;                           \
    }

#define FUNC_VOID_NUMBER(func, Func)    \
static int Func(lua_State *L)           \
    {                                   \
    double a = luaL_checknumber(L, 1);  \
    func(a);                            \
    return 0;                           \
    }

#define FUNC_VOID_NUMBER2(func, Func)   \
static int Func(lua_State *L)           \
    {                                   \
    double a = luaL_checknumber(L, 1);  \
    double b = luaL_checknumber(L, 2);  \
    func(a,b);                          \
    return 0;                           \
    }

#define FUNC_VOID_NUMBER3(func, Func)   \
static int Func(lua_State *L)           \
    {                                   \
    double a = luaL_checknumber(L, 1);  \
    double b = luaL_checknumber(L, 2);  \
    double c = luaL_checknumber(L, 3);  \
    func(a,b,c);                        \
    return 0;                           \
    }

#define FUNC_VOID_NUMBER6(func, Func)   \
static int Func(lua_State *L)           \
    {                                   \
    double a = luaL_checknumber(L, 1);  \
    double b = luaL_checknumber(L, 2);  \
    double c = luaL_checknumber(L, 3);  \
    double d = luaL_checknumber(L, 4);  \
    double e = luaL_checknumber(L, 5);  \
    double f = luaL_checknumber(L, 6);  \
    func(a,b,c,d,e,f);                  \
    return 0;                           \
    }

#define FUNC_VOID_NUMBER8(func, Func)   \
static int Func(lua_State *L)           \
    {                                   \
    double a = luaL_checknumber(L, 1);  \
    double b = luaL_checknumber(L, 2);  \
    double c = luaL_checknumber(L, 3);  \
    double d = luaL_checknumber(L, 4);  \
    double e = luaL_checknumber(L, 5);  \
    double f = luaL_checknumber(L, 6);  \
    double g = luaL_checknumber(L, 7);  \
    double h = luaL_checknumber(L, 8);  \
    func(a,b,c,d,e,f,g,h);              \
    return 0;                           \
    }


#define FUNC_VOID_FLOAT FUNC_VOID_NUMBER
#define FUNC_VOID_DOUBLE FUNC_VOID_NUMBER
#define FUNC_VOID_FLOAT2 FUNC_VOID_NUMBER2
#define FUNC_VOID_DOUBLE2 FUNC_VOID_NUMBER2
#define FUNC_VOID_FLOAT3 FUNC_VOID_NUMBER3
#define FUNC_VOID_DOUBLE3 FUNC_VOID_NUMBER3
#define FUNC_VOID_FLOAT6 FUNC_VOID_NUMBER6
#define FUNC_VOID_DOUBLE6 FUNC_VOID_NUMBER6
#define FUNC_VOID_FLOAT8 FUNC_VOID_NUMBER8
#define FUNC_VOID_DOUBLE8 FUNC_VOID_NUMBER8

#define FUNC_INT_VOID(func, Func)   \
static int Func(lua_State *L)       \
    {                               \
    lua_pushinteger(L, func());     \
    return 1;                       \
    }


#define FUNC_NUMBER_VOID(func, Func)    \
static int Func(lua_State *L)           \
    {                                   \
    lua_pushnumber(L, func());          \
    return 1;                           \
    }

#define FUNC_FLOAT_VOID     FUNC_NUMBER_VOID
#define FUNC_DOUBLE_VOID    FUNC_NUMBER_VOID

#define FUNC_NUMBER_NUMBER2(func, Func) \
static int Func(lua_State *L)           \
    {                                   \
    double a = luaL_checknumber(L, 1);  \
    double b = luaL_checknumber(L, 2);  \
    lua_pushnumber(L, func(a, b));      \
    return 1;                           \
    }

#define FUNC_FLOAT_FLOAT2       FUNC_NUMBER_NUMBER2
#define FUNC_DOUBLE_DOUBLE2     FUNC_NUMBER_NUMBER2

#define FUNC_BOOLEAN_VOID(func, Func)   \
static int Func(lua_State *L)       \
    {                               \
    lua_pushboolean(L, func());     \
    return 1;                       \
    }

#define FUNC_GETSET_INT(func, Func)     \
static int Func(lua_State *L)           \
    {                                   \
    if(lua_isnone(L, 1))                \
        {                               \
        lua_pushinteger(L, func());     \
        return 1;                       \
        }                               \
    func(luaL_checkinteger(L, 1));      \
    return 0;                           \
    }

#define FUNC_GETSET_BOOLEAN(func, Func)     \
static int Func(lua_State *L)           \
    {                                   \
    if(lua_isnone(L, 1))                \
        {                               \
        lua_pushboolean(L, func());     \
        return 1;                       \
        }                               \
    func(checkboolean(L, 1));           \
    return 0;                           \
    }

#define FUNC_GETSET_NUMBER(func, Func)  \
static int Func(lua_State *L)           \
    {                                   \
    if(lua_isnone(L, 1))                \
        {                               \
        lua_pushnumber(L, func());      \
        return 1;                       \
        }                               \
    func(luaL_checknumber(L, 1));       \
    return 0;                           \
    }

#define FUNC_GETSET_FLOAT FUNC_GETSET_NUMBER
#define FUNC_GETSET_DOUBLE FUNC_GETSET_NUMBER


#define FUNC_GETSET_INTTYPE(V, func, Func)  \
static int Func(lua_State *L)               \
    {                                       \
    if(lua_isnone(L, 1))                    \
        {                                   \
        push_##V(L, func());                \
        return 1;                           \
        }                                   \
    func(check_##V(L, 1));                  \
    return 0;                               \
    }

#define FUNC_GETSET_ENUM FUNC_GETSET_INTTYPE


#define FUNC_GETSET_POINTER(V, func, Func)  \
static int Func(lua_State *L)               \
    {                                       \
    if(lua_isnone(L, 1))                    \
        {                                   \
        Fl_##V *v = func();                 \
        if(!v) return 0;                    \
        push_##V(L, v);                     \
        return 1;                           \
        }                                   \
    Fl_##V *v = check_##V(L, 1);            \
    func(v);                                \
    return 0;                               \
    }

#endif /* funcDEFINED */
