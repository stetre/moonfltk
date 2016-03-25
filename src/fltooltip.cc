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
 | Tooltip                                                                      |
 *------------------------------------------------------------------------------*/

FUNC_GETSET_FLOAT(Fl_Tooltip::delay, Tooltipdelay)
FUNC_GETSET_FLOAT(Fl_Tooltip::hoverdelay, Tooltiphoverdelay)
FUNC_BOOLEAN_VOID(Fl_Tooltip::enabled, Tooltipenabled)

static int Tooltipenable(lua_State *L)
    {
    Fl_Tooltip::enable(optboolean(L, 1, 1));
    return 0;
    }

FUNC_VOID_VOID(Fl_Tooltip::disable, Tooltipdisable)
FUNC_GETSET_ENUM(Font, Fl_Tooltip::font, Tooltipfont)
FUNC_GETSET_INTTYPE(Fontsize, Fl_Tooltip::size, Tooltipsize)
FUNC_GETSET_ENUM(Color, Fl_Tooltip::color, Tooltipcolor)
FUNC_GETSET_ENUM(Color, Fl_Tooltip::textcolor, Tooltiptextcolor)

#if defined(FLTK_ABI_VERSION) && (FLTK_ABI_VERSION >= 10301)
FUNC_GETSET_INT(Fl_Tooltip::margin_width, Tooltipmargin_width)
FUNC_GETSET_INT(Fl_Tooltip::margin_height, Tooltipmargin_height)
FUNC_GETSET_INT(Fl_Tooltip::wrap_width, Tooltipwrap_width)
#else
FUNC_INT_VOID(Fl_Tooltip::margin_width, Tooltipmargin_width)
FUNC_INT_VOID(Fl_Tooltip::margin_height, Tooltipmargin_height)
FUNC_INT_VOID(Fl_Tooltip::wrap_width, Tooltipwrap_width)
#endif

FUNC_GETSET_POINTER(Widget, Fl_Tooltip::current, Tooltipcurrent)

#if 0
//@@static void (*enter)(Fl_Widget* w);
//@@static void (*exit)(Fl_Widget *w);
//@@static void Fl_Tooltip::enter_area(Fl_Widget* w, int X, int Y, int W, int H, const char* tip);
static int Tooltipenter_area(lua_State *L)
    {
    (void)L;
    return 0;
    }
#endif

static const struct luaL_Reg Functions[] = 
    {
        { "tooltip_color", Tooltipcolor },
        { "tooltip_current", Tooltipcurrent },
        { "tooltip_delay", Tooltipdelay },
        { "tooltip_disable", Tooltipdisable },
        { "tooltip_enable", Tooltipenable },
        { "tooltip_enabled", Tooltipenabled },
        { "tooltip_font", Tooltipfont },
        { "tooltip_hoverdelay", Tooltiphoverdelay },
        { "tooltip_margin_height", Tooltipmargin_height },
        { "tooltip_margin_width", Tooltipmargin_width },
        { "tooltip_size", Tooltipsize },
        { "tooltip_textcolor", Tooltiptextcolor },
        { "tooltip_wrap_width", Tooltipwrap_width },
//      { "tooltip_enter_area", Tooltipenter_area },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_FlTooltip(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

