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

/*------------------------------------------------------------------------------*
 | Help_Dialog (not a widget)                                                   |
 *------------------------------------------------------------------------------*/

static int Help_DialogNew(lua_State *L)                 
    {                                           
    Fl_Help_Dialog *p = new Fl_Help_Dialog();   
    DBG_CREATE(Help_Dialog,p);                          
    newuserdata(L, p, MT_Help_Dialog);                  
    return 1;                                   
    }

static int Help_DialogDelete(lua_State *L)                      
    {                                               
    Fl_Help_Dialog *p = check_Help_Dialog(L, 1);                    
    DBG_DELETE(Help_Dialog,p);                              
    if(freeuserdata(L, p)) delete p;                
    return 0;                                       
    }

WINDOW_SHOW(Help_Dialog, Help_Dialogshow)
VOID_VOID(Help_Dialog, hide, Help_Dialoghide)
INT_VOID(Help_Dialog, h, Help_Dialogh)
INT_VOID(Help_Dialog, w, Help_Dialogw)
INT_VOID(Help_Dialog, x, Help_Dialogx)
INT_VOID(Help_Dialog, y, Help_Dialogy)
VOID_INT_INT(Help_Dialog, position, Help_Dialogposition)
VOID_INT4(Help_Dialog, resize, Help_Dialogresize)
GETSET_INTTYPE(Help_Dialog, Fontsize, textsize, Help_Dialogtextsize)
BOOLEAN_VOID(Help_Dialog, visible, Help_Dialogvisible)
GETSET_STRING(Help_Dialog, value, Help_Dialogvalue)
VOID_STRING(Help_Dialog, load, Help_Dialogload)


static int Help_Dialogtopline(lua_State *L)
    {
    Fl_Help_Dialog *p = check_Help_Dialog(L, 1);
    if(lua_type(L, 2) == LUA_TSTRING)
        p->topline(luaL_checkstring(L, 2));
    else
        p->topline(luaL_checkinteger(L, 2));
    return 0;
    }


static const struct luaL_Reg Help_DialogMethods[] = 
    {
        { "h", Help_Dialogh },
        { "hide", Help_Dialoghide },
        { "load", Help_Dialogload },
        { "position", Help_Dialogposition },
        { "resize", Help_Dialogresize },
        { "show", Help_Dialogshow },
        { "textsize", Help_Dialogtextsize },
        { "topline", Help_Dialogtopline },
        { "value", Help_Dialogvalue },
        { "visible", Help_Dialogvisible },
        { "w", Help_Dialogw },
        { "x", Help_Dialogx },
        { "y", Help_Dialogy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Help_DialogMetaMethods[] = 
    {
        { "__gc",  Help_DialogDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Help_DialogFunctions[] = 
    {
        { "help_dialog",  Help_DialogNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Help_Dialog(lua_State *L)
    {
    udata_define(L, MT_Help_Dialog, Help_DialogMethods, Help_DialogMetaMethods);
    luaL_setfuncs(L, Help_DialogFunctions, 0);
    }

