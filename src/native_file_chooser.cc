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

#define CheckType check_Native_File_ChooserRtti
#define PushType push_Native_File_ChooserRtti

static int CheckOptions(lua_State *L, int arg)
    {
    unsigned int opt = 0;
    const char *s;
    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
        if((strcmp(s,  "no options")==0)) 
            return (int)Fl_Native_File_Chooser::NO_OPTIONS;
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { opt |= CODE; goto done; } while(0)
        CASE(Fl_Native_File_Chooser::SAVEAS_CONFIRM, "saveas confirm");
        CASE(Fl_Native_File_Chooser::NEW_FOLDER, "new folder");
        CASE(Fl_Native_File_Chooser::PREVIEW, "preview");
        CASE(Fl_Native_File_Chooser::USE_FILTER_EXT, "use filter ext");
#undef CASE
        return (int)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }
    return (int)opt;
    }

static int PushOptions(lua_State *L, int opt)
    {
    int n = 0;
#define CASE(CODE,str) if(opt & CODE) do { lua_pushstring(L, str); n++; } while(0)
        CASE(Fl_Native_File_Chooser::NO_OPTIONS, "no options");
        CASE(Fl_Native_File_Chooser::SAVEAS_CONFIRM, "saveas confirm");
        CASE(Fl_Native_File_Chooser::NEW_FOLDER, "new folder");
        CASE(Fl_Native_File_Chooser::PREVIEW, "preview");
        CASE(Fl_Native_File_Chooser::USE_FILTER_EXT, "use filter ext");
#undef CASE
    if(n==0)
        { lua_pushstring(L, "no options"); n++; }
    return n;
    }


/*------------------------------------------------------------------------------*
 | Fl_Native_File_Chooser                                                       |
 *------------------------------------------------------------------------------*/

static int Native_File_ChooserNew(lua_State *L)
    {
    Fl_Native_File_Chooser *p;
    if(lua_isnone(L, 1))
        p = new Fl_Native_File_Chooser();
    else
        p = new Fl_Native_File_Chooser(CheckType(L, 1));
    DBG_CREATE(Native_File_Chooser, p);
    newuserdata(L, p, MT_Native_File_Chooser);
    return 1;
    }

static int Native_File_ChooserDelete(lua_State *L)
    {
    Fl_Native_File_Chooser *p = check_Native_File_Chooser(L, 1);
    DBG_DELETE(Native_File_Chooser, p);
    if(freeuserdata(L, p)) delete p;
    return 0;
    }

static int Native_File_Choosertype(lua_State *L)
    {
    Fl_Native_File_Chooser *p = check_Native_File_Chooser(L, 1);
    if(lua_isnone(L, 2))
        {
        PushType(L, p->type());
        return 1;
        }
    p->type(CheckType(L, 2));
    return 0;
    }

static int Native_File_Chooseroptions(lua_State *L)
    {
    Fl_Native_File_Chooser *p = check_Native_File_Chooser(L, 1);
    if(lua_isnone(L, 2))
        return PushOptions(L, p->options());
    p->options(CheckOptions(L, 2));
    return 0;
    }

static int Native_File_Choosershow(lua_State *L)
    {
    Fl_Native_File_Chooser *p = check_Native_File_Chooser(L, 1);
    int rc = p->show();
    switch(rc)
        {
        case 0: lua_pushstring(L, "ok"); return 1;
        case 1: lua_pushstring(L, "cancel"); return 1;
        case -1: lua_pushfstring(L, "%s", p->errmsg()); return 1;
        }
    return unexpected(L);
    }

INT_VOID(Native_File_Chooser, count, Native_File_Choosercount)

static int Native_File_Chooserfilename(lua_State *L)
    {
    Fl_Native_File_Chooser *p = check_Native_File_Chooser(L, 1);
    const char *f;
    if(lua_isnone(L, 2))
        f = p->filename();
    else
        f = p->filename(checkindex(L, 2));
    if(!f) return 0;
    lua_pushstring(L, f);
    return 1;
    }

static int Native_File_Chooserfilenames(lua_State *L) /* NONFLTK */
    {
    Fl_Native_File_Chooser *p = check_Native_File_Chooser(L, 1);
    int count = p->count();
    for(int i=0; i< count; i++)
        lua_pushstring(L, p->filename(i));
    return count;
    }


GETSET_STRING_OPT(Native_File_Chooser, directory, Native_File_Chooserdirectory)
GETSET_STRING_OPT(Native_File_Chooser, title, Native_File_Choosertitle)
GETSET_STRING_OPT(Native_File_Chooser, filter, Native_File_Chooserfilter)
GETSET_STRING(Native_File_Chooser, preset_file, Native_File_Chooserpreset_file)

INT_VOID(Native_File_Chooser, filters, Native_File_Chooserfilters)
GETSET_INDEX(Native_File_Chooser, filter_value, Native_File_Chooserfilter_value)

STRING_VOID(Native_File_Chooser, errmsg, Native_File_Choosererrmsg)

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Native_File_ChooserMethods[] = 
    {
        { "count", Native_File_Choosercount },
        { "directory", Native_File_Chooserdirectory },
        { "errmsg", Native_File_Choosererrmsg },
        { "filename", Native_File_Chooserfilename },
        { "filenames", Native_File_Chooserfilenames },
        { "filter", Native_File_Chooserfilter },
        { "filter_value", Native_File_Chooserfilter_value },
        { "filters", Native_File_Chooserfilters },
        { "options", Native_File_Chooseroptions },
        { "preset_file", Native_File_Chooserpreset_file },
        { "show", Native_File_Choosershow },
        { "title", Native_File_Choosertitle },
        { "type", Native_File_Choosertype },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Native_File_ChooserMetaMethods[] = 
    {
        { "__gc",  Native_File_ChooserDelete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Native_File_ChooserFunctions[] = 
    {
        { "native_file_chooser",  Native_File_ChooserNew },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Native_File_Chooser(lua_State *L)
    {
    udata_define(L, MT_Native_File_Chooser, Native_File_ChooserMethods, 
            Native_File_ChooserMetaMethods);
    luaL_setfuncs(L, Native_File_ChooserFunctions, 0);
    }

//@@  static const char *file_exists_message;
