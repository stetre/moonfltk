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

#include "fldraw_boxtype.h" // for boxtype_FunctionRefs


/*----------------------------------------------------------------------*
 | Draw Functions                                                       |
 *----------------------------------------------------------------------*/

FUNC_VOID_INT4(fl_push_clip, Push_clip)
FUNC_VOID_VOID(fl_push_no_clip, Push_no_clip)
FUNC_VOID_VOID(fl_pop_clip, Pop_clip)
FUNC_VOID_VOID(fl_push_matrix, Push_matrix)
FUNC_VOID_VOID(fl_pop_matrix, Pop_matrix)
FUNC_VOID_VOID(fl_begin_complex_polygon, Begin_complex_polygon)
FUNC_VOID_VOID(fl_end_complex_polygon, End_complex_polygon)
FUNC_VOID_VOID(fl_gap, Gap)
FUNC_VOID_VOID(fl_begin_line, Begin_line)
FUNC_VOID_VOID(fl_end_line, End_line)
FUNC_VOID_VOID(fl_begin_loop, Begin_loop)
FUNC_VOID_VOID(fl_end_loop, End_loop)
FUNC_VOID_VOID(fl_begin_points, Begin_points)
FUNC_VOID_VOID(fl_end_points, End_points)
FUNC_VOID_VOID(fl_begin_polygon, Begin_polygon)
FUNC_VOID_VOID(fl_end_polygon, End_polygon)
FUNC_BOOLEAN_VOID(fl_can_do_alpha_blending, Can_do_alpha_blending)
FUNC_VOID_VOID(fl_overlay_clear, Overlay_clear)
FUNC_VOID_VOID(fl_reset_spot, Reset_spot)
FUNC_VOID_VOID(fl_restore_clip, Restore_clip)
FUNC_VOID_INT4(fl_set_status, Set_status)
FUNC_DOUBLE_DOUBLE2(fl_transform_dx, Transform_dx)
FUNC_DOUBLE_DOUBLE2(fl_transform_dy, Transform_dy)
FUNC_DOUBLE_DOUBLE2(fl_transform_x, Transform_x)
FUNC_DOUBLE_DOUBLE2(fl_transform_y, Transform_y)
FUNC_VOID_DOUBLE2(fl_transformed_vertex, Transformed_vertex)
FUNC_VOID_DOUBLE6(fl_mult_matrix, Mult_matrix)
FUNC_VOID_DOUBLE3(fl_circle, Circle)
FUNC_VOID_DOUBLE8(fl_curve, Curve)
FUNC_VOID_DOUBLE(fl_rotate, Rotate)
FUNC_VOID_DOUBLE2(fl_translate, Translate)
FUNC_VOID_DOUBLE2(fl_vertex, Vertex)

static int Scale(lua_State *L)
    {
    if(lua_isnone(L, 2))
        fl_scale(luaL_checknumber(L, 1));
    else
        fl_scale(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
    return 0;
    }

static int Rect(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    if(lua_isnone(L, 5))
        fl_rect(x, y, w, h);
    else
        fl_rect(x, y, w, h, check_Color(L, 5));
    return 0;
    }

static int Rectf(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    if(lua_isnone(L, 5))
        fl_rectf(x, y, w, h);
    else if(lua_isnone(L, 6))
        fl_rectf(x, y, w, h, check_Color(L, 5));
    else
        {
        int r = luaL_checkinteger(L, 5);
        int g = luaL_checkinteger(L, 6);
        int b = luaL_checkinteger(L, 7);
        fl_rectf(x, y, w, h, r, g, b);
        }   
    return 0;
    }

static int Arc(lua_State *L)
    {
    if(lua_isnone(L, 6))
        {
        double x = luaL_checknumber(L, 1);
        double y = luaL_checknumber(L, 2);
        double r = luaL_checknumber(L, 3);
        double start = luaL_checknumber(L, 4);
        double end = luaL_checknumber(L, 5);
        fl_arc(x, y, r, start, end);
        }
    else
        {
        int x = luaL_checkinteger(L, 1);
        int y = luaL_checkinteger(L, 2);
        int w = luaL_checkinteger(L, 3);
        int h = luaL_checkinteger(L, 4);
        double a1 = luaL_checknumber(L, 5);
        double a2 = luaL_checknumber(L, 6);
        fl_arc(x, y, w, h, a1, a2);
        }
    return 0;
    }


#define F(func, Func)                   \
static int Func(lua_State *L)           \
    {                                   \
    int v1 = luaL_checkinteger(L, 1);   \
    int v2 = luaL_checkinteger(L, 2);   \
    int v3 = luaL_checkinteger(L, 3);   \
    if(lua_isnone(L, 4))            \
        {                               \
        func(v1, v2, v3);               \
        return 0;                       \
        }                               \
    int v4 = luaL_checkinteger(L, 4);   \
    if(lua_isnone(L, 5))            \
        {                               \
        func(v1, v2, v3, v4);           \
        return 0;                       \
        }                               \
    int v5 = luaL_checkinteger(L, 5);   \
    func(v1, v2, v3, v4, v5);           \
    return 0;                           \
    }

F(fl_xyline, Xyline)
F(fl_yxline, Yxline)

#undef F


static int DrawCb = LUA_NOREF;
static lua_State *DrawStateXXX = NULL; /* DrawState() is a global function in Windows .. */
static void DrawCallthis(const char *str, int n, int x, int y)
    {
    lua_State *L = DrawStateXXX;
    if(pushvalue(L, DrawCb) != LUA_TFUNCTION)
        { unexpected(L); return; }
    lua_pushlstring(L, str, n);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    if(lua_pcall(L, 3, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }

static int Draw(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        {
        size_t len;
        int angle = luaL_checkinteger(L, 1);
        const char *str = luaL_checklstring(L, 2, &len);
        fl_draw(angle, str, (int)len, luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
        return 0;
        }
    size_t len;
    const char *str = luaL_checklstring(L, 1, &len);
    if(lua_isnone(L, 5))
        {
        fl_draw(str, (int)len, luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
        return 0;
        }
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int w = luaL_checkinteger(L, 4);
    int h = luaL_checkinteger(L, 5);
    int align = luaL_checkinteger(L, 6); /* only integer code alignment, here */
    if(lua_isnone(L, 7))
        {
        fl_draw(str, x, y, w, h, align); 
        return 0;
        }
    Fl_Image *img = test_Image(L, 7);
    int draw_symbols = optboolean(L, 8, 1);
    if(lua_isnoneornil(L, 9))
        {
        fl_draw(str, x, y, w, h, align, img, draw_symbols);
        return 0;
        }
    if(!lua_isfunction(L, 9))
        return luaL_argerror(L, 9, "function expected");
    unreference(L, DrawCb);
    reference(L, DrawCb, 9);
    DrawStateXXX = L;
    fl_draw(str, x, y, w, h, align, DrawCallthis, img, draw_symbols);
    return 0;
    }

static int Line_style(lua_State *L)
/* line_style(style [, width, dash1, dash2, ... ]) */
    {
    int style = luaL_checkinteger(L, 1);    
    if(lua_isnone(L, 2))
        {
        fl_line_style(style);
        return 0;
        }
    int width = luaL_checkinteger(L, 2);
    if(lua_isnone(L, 3))
        {
        fl_line_style(style, width);
        return 0;
        }
    char *dashes;
    int arg = 3;
    while(!lua_isnone(L, arg))
        luaL_checkinteger(L, arg++);
    int ndashes = arg - 3;
    if((ndashes % 2) != 0)
        return luaL_argerror(L, 3, "odd number of dashes");
    dashes = (char*)malloc((ndashes+1)*sizeof(char));
    if(!dashes)
        return memoryerror(L);
    arg = 3;
    for(int i = 0; i < ndashes; i++)
        dashes[i] = lua_tointeger(L, arg++);
    dashes[ndashes] = 0;
    fl_line_style(style, width, dashes);
    free(dashes);   
    return 0;
    }

#if 0
static int Chord(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    double a1 = luaL_checknumber(L, 5);
    double a2 = luaL_checknumber(L, 6);
    fl_chord(x, y, w, h, a1, a2); //does not exist in FLTK 1.3.3
    return 0;
    }
#endif

static int Clip_box(lua_State *L)
    {
    int X, Y, W, H;
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    fl_clip_box(x, y, w, h, X, Y, W, H);
    lua_pushinteger(L, X);
    lua_pushinteger(L, Y);
    lua_pushinteger(L, W);
    lua_pushinteger(L, H);
    return 4;
    }

static int Cursorr(lua_State *L)
    {
    Fl_Cursor c = check_Cursor(L, 1);
    if(lua_isnone(L, 2))
        {
        fl_cursor(c);
        return 0;
        }
    Fl_Color fg = check_Color(L, 2);
    if(lua_isnone(L, 3))
        {
        fl_cursor(c, fg);
        return 0;
        }
    Fl_Color bg = check_Color(L, 3);
    fl_cursor(c, fg, bg);
    return 0;
    }

static int Draw_box(lua_State *L)
    {
    Fl_Boxtype bt = check_Boxtype(L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int w = luaL_checkinteger(L, 4);
    int h = luaL_checkinteger(L, 5);
    Fl_Color c = check_Color(L, 6);
    fl_draw_box(bt, x, y, w, h, c);
    return 0;
    }

static void boxtypeDrawFunction(uchar n, int x, int y, int w, int h, Fl_Color color)
    {
    lua_State* L = main_lua_state;
    int luaRef = boxtype_FunctionRefs[n].luaRef;
    if (L == 0 || luaRef == LUA_NOREF)
        return;
    int type = lua_rawgeti(L, LUA_REGISTRYINDEX, luaRef);
    if (type != LUA_TFUNCTION) 
        { lua_pop(L, 1); return; }
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    lua_pushinteger(L, color);
    lua_call(L, 5, 0);
    }
    
// typedef void (Fl_Box_Draw_F)(int x, int y, int w, int h, Fl_Color color);
// static void Fl::set_boxtype(Fl_Boxtype, Fl_Box_Draw_F*,uchar,uchar,uchar,uchar);
// static void Fl::set_boxtype(Fl_Boxtype, Fl_Boxtype from);
static int Set_boxtype(lua_State *L)
    {
    Fl_Boxtype bt = check_Boxtype(L, 1);
    if (lua_isfunction(L, 2))
        {
        uchar dx = (uchar)luaL_checkinteger(L, 3);
        uchar dy = (uchar)luaL_checkinteger(L, 4);
        uchar dw = (uchar)luaL_checkinteger(L, 5);
        uchar dh = (uchar)luaL_checkinteger(L, 6);

        if (boxtype_FunctionRefs[bt].luaRef != LUA_NOREF)
            luaL_unref(L, LUA_REGISTRYINDEX, boxtype_FunctionRefs[bt].luaRef);
        lua_pushvalue(L, 2);
        boxtype_FunctionRefs[bt].luaRef = luaL_ref(L, LUA_REGISTRYINDEX);
        Fl::set_boxtype(bt, boxtype_FunctionRefs[bt].func, dx, dy, dw, dh);
        }
    else
        {   
        Fl_Boxtype fromBt = check_Boxtype(L, 2);

        if (boxtype_FunctionRefs[bt].luaRef != LUA_NOREF)
            luaL_unref(L, LUA_REGISTRYINDEX, boxtype_FunctionRefs[bt].luaRef);
        Fl_Box_Draw_F* fromFunc = Fl::get_boxtype(fromBt);
        if (   boxtype_FunctionRefs[fromBt].func   == fromFunc
            && boxtype_FunctionRefs[fromBt].luaRef != LUA_NOREF)
            {
            lua_rawgeti(L, LUA_REGISTRYINDEX, boxtype_FunctionRefs[fromBt].luaRef);
            boxtype_FunctionRefs[bt].luaRef = luaL_ref(L, LUA_REGISTRYINDEX);
            Fl::set_boxtype(bt, boxtype_FunctionRefs[fromBt].func, 
                            Fl::box_dx(fromBt), 
                            Fl::box_dy(fromBt), 
                            Fl::box_dw(fromBt), 
                            Fl::box_dh(fromBt));
            }
        else
            Fl::set_boxtype(bt, fromBt);
        }
    return 0;
    }

static int native_boxtype_draw_function(lua_State *L)
{
    Fl_Box_Draw_F* func = (Fl_Box_Draw_F*)lua_touserdata(L, lua_upvalueindex(1));
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    Fl_Color c =  check_Color(L, 5);
    func(x, y, w, h, c);
    return 0;
}

// static Fl_Box_Draw_F *Fl::get_boxtype(Fl_Boxtype);
static int Get_boxtype(lua_State *L)
{
    Fl_Boxtype bt = check_Boxtype(L, 1);
    Fl_Box_Draw_F* func = Fl::get_boxtype(bt);
    if (   boxtype_FunctionRefs[bt].func   == func
        && boxtype_FunctionRefs[bt].luaRef != LUA_NOREF)
        {
        lua_rawgeti(L, LUA_REGISTRYINDEX, boxtype_FunctionRefs[bt].luaRef);
        }
    else
        {
        lua_pushlightuserdata(L, (void*)func);
        lua_pushcclosure(L, native_boxtype_draw_function, 1);
        }
    return 1;
}

static int Boxtype_code(lua_State *L)
    {
    Fl_Boxtype bt = check_Boxtype(L, 1);
    lua_pushinteger(L, bt);
    return 1;
    }

//typedef void (*Fl_Draw_Image_Cb)(void* data,int x,int y,int w,uchar* buf);
//void  fl_draw_image(Fl_Draw_Image_Cb cb, void *data, int X, int Y, int W, int H, int D=3) @@
static int Draw_image(lua_State *L)
    {
    size_t len;
    const uchar *data = (uchar*)luaL_checklstring(L, 1, &len);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int w = luaL_checkinteger(L, 4);
    int h = luaL_checkinteger(L, 5);
    int d = luaL_optinteger(L, 6, 3);
    int l = luaL_optinteger(L, 7, 0);
//@@TODO: data length check
//  if(len < (w*h*d)) return luaL_argerror(L, 1, "to few bytes in data");
    fl_draw_image(data, x, y, w, h, d, l);
    return 0;
    }

//void  fl_draw_image_mono(Fl_Draw_Image_Cb cb, void *data, int X, int Y, int W, int H, int D=1) @@
static int Draw_image_mono(lua_State *L)
    {
    size_t len;
    const uchar *data = (uchar*)luaL_checklstring(L, 1, &len);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int w = luaL_checkinteger(L, 4);
    int h = luaL_checkinteger(L, 5);
    int d = luaL_optinteger(L, 6, 3);
    int l = luaL_optinteger(L, 7, 0);
//@@TODO: data length check
    fl_draw_image_mono(data, x, y, w, h, d, l);
    return 0;
    }


static int Draw_pixmap(lua_State *L)
    {
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    Fl_Color color = FL_GRAY;
    if(!lua_isnone(L, 4))
        color = check_Color(L, 4);
    int nxpm;
    char **xpm = checkpixmap(L, 1, &nxpm);

    int rc = fl_draw_pixmap(xpm, x, y, color);
    freepixmap(xpm, nxpm);
    if(rc == 0)
        return luaL_argerror(L, 1, "image parsing error");
    return 0;
    }


#define F(func, Func)                                   \
static int Func(lua_State *L)                           \
    {                                                   \
    size_t len;                                         \
    const char *s = luaL_checklstring(L, 1, &len);      \
    if((len%4)!=0)                                      \
        return luaL_argerror(L, 1, "string length is not a multiple of 4"); \
    int x = luaL_checkinteger(L, 2);                    \
    int y = luaL_checkinteger(L, 3);                    \
    int w = luaL_checkinteger(L, 4);                    \
    int h = luaL_checkinteger(L, 5);                    \
    func(s, x, y, w, h);                                \
    return 0;                                           \
    }

F(fl_frame, Frame)
F(fl_frame2, Frame2)

#undef F

static int Line(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int x1 = luaL_checkinteger(L, 3);
    int y1 = luaL_checkinteger(L, 4);
    if(lua_isnone(L, 5))
        {
        fl_line(x, y, x1, y1);
        return 0;
        }
    int x2 = luaL_checkinteger(L, 5);
    int y2 = luaL_checkinteger(L, 6);
    fl_line(x, y, x1, y1, x2, y2);
    return 0;
    }


#define F(func, Func)                   \
static int Func(lua_State *L)           \
    {                                   \
    int x = luaL_checkinteger(L, 1);    \
    int y = luaL_checkinteger(L, 2);    \
    int x1 = luaL_checkinteger(L, 3);   \
    int y1 = luaL_checkinteger(L, 4);   \
    int x2 = luaL_checkinteger(L, 5);   \
    int y2 = luaL_checkinteger(L, 6);   \
    if(lua_isnone(L, 7))                \
        {                               \
        func(x, y, x1, y1, x2, y2);     \
        return 0;                       \
        }                               \
    int x3 = luaL_checkinteger(L, 7);   \
    int y3 = luaL_checkinteger(L, 8);   \
    func(x, y, x1, y1, x2, y2, x3, y3); \
    return 0;                           \
    }
F(fl_loop, Loop)
F(fl_polygon, Polygon)

#undef F


static int Measure(lua_State *L)
/* w, h = measure(str[, w, draw_symbols]) */
    {
    const char *str = luaL_checkstring(L, 1);
    int w = luaL_optinteger(L, 2, 0);
    int draw_symbols = optboolean(L, 3, 1); 
    int h = 0;
    fl_measure(str, w, h, draw_symbols);
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 2;
    }


static int Measure_pixmap(lua_State *L)
    {
    int nxpm, w, h;
    char **xpm = checkpixmap(L, 1, &nxpm);
    int rc = fl_measure_pixmap(xpm, w, h);
    freepixmap(xpm, nxpm);
    if(rc == 0)
        return luaL_argerror(L, 1, "image parsing error");
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 2;
    }

static int Not_clipped(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    lua_pushboolean(L, fl_not_clipped(x, y, w, h));
    return 1;
    }

static int Overlay_rect(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    fl_overlay_rect(x, y, w, h);
    return 0;
    }

static int Pie(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    double a1 = luaL_checknumber(L, 5);
    double a2 = luaL_checknumber(L, 6);
    fl_pie(x, y, w, h, a1, a2);
    return 0;
    }

static int Point(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    fl_point(x, y);
    return 0;
    }

static int Read_image(lua_State *L)
/* data = read_image(x, y, w, h [,alpha]) 
 * data is a binary string
 */
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = checkpositiveinteger(L, 3);
    int h = checkpositiveinteger(L, 4);
    int alpha = optboolean(L, 5, 0);
    
    size_t bufsz = alpha ? w*h*4 : w*h*3;
    uchar *buf = (uchar*)malloc(bufsz);
    fl_read_image(buf, x, y, w, h, alpha);
    lua_pushlstring(L, (char*)buf, bufsz);
    free(buf);
    return 1;
    }

static int Rtl_draw(lua_State *L)
    {
    size_t len;
    const char *str = luaL_checklstring(L, 1, &len);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    fl_rtl_draw(str, (int)len, x, y);
    return 0;
    }


static int ScrollCb = LUA_NOREF;
static lua_State *ScrollState = NULL;
static void ScrollDraw_area(void * data, int x, int y, int w, int h)
    {
    (void)data; /* not used */
    lua_State *L = ScrollState;
    if(pushvalue(L, ScrollCb) != LUA_TFUNCTION)
        { unexpected(L); return; }
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    if(lua_pcall(L, 4, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    }


static int Scroll(lua_State *L)
    {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int dx = luaL_checkinteger(L, 5);
    int dy = luaL_checkinteger(L, 6);
    if(!lua_isfunction(L, 7))
        return luaL_argerror(L, 7, "function expected");
    unreference(L, ScrollCb);
    reference(L, ScrollCb, 7);
    ScrollState = L;
    fl_scroll(x, y, w, h, dx, dy, ScrollDraw_area, NULL);
    return 0;
    }


static int Set_spot(lua_State *L)
    {
    int font = check_Font(L, 1);
    int size = luaL_checkinteger(L, 2);
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);
    int w = luaL_checkinteger(L, 5);
    int h = luaL_checkinteger(L, 6);
    if(lua_isnoneornil(L, 7))
        fl_set_spot(font, size, x, y, w, h);
    else
        fl_set_spot(font, size, x, y, w, h, check_Window(L, 7));
    return 0;
    }

#define F(func, Func)                       \
static int Func(lua_State *L)               \
    {                                       \
    Fl_Boxtype t = check_Boxtype(L, 1);     \
    lua_pushinteger(L, func(t));            \
    return 1;                               \
    }
F(Fl::box_dx, Box_dx)
F(Fl::box_dy, Box_dy)
F(Fl::box_dw, Box_dw)
F(Fl::box_dh, Box_dh)
#undef F

FUNC_BOOLEAN_VOID(Fl::draw_box_active, Draw_box_active)

FUNC_GETSET_INT(Fl::scrollbar_size, Scrollbar_size)

//NO: unsigned int fl_old_shortcut(const char *s)
//NO: const char*  fl_shortcut_label(unsigned int shortcut, const char **eom)
static int Shortcut_label(lua_State *L)
    {
    unsigned int shortcut = luaL_checkinteger(L, 1);
    const char *label = fl_shortcut_label(shortcut);    
    lua_pushstring(L, label ? label : "");
    return 1;
    }

#if 0
/* For this to be implemented in Lua we need the drawit() to pass the symbol name too, 
 * e.g. void drawit(name, color)
 * so we can know which symbol it refers to.
 */
//int fl_add_symbol(const char *name, void(*drawit)(Fl_Color), int scalable) @@ UNBINDABLE
static int Add_symbol(lua_State *L) 
    {
    const char *label = luaL_checkstring(L, 1);
    (void)L;
    return 0;
    }

#endif

static int Draw_symbol(lua_State *L)
    {
    const char *label = luaL_checkstring(L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int w = luaL_checkinteger(L, 4);
    int h = luaL_checkinteger(L, 5);
    Fl_Color color = check_Color(L, 6);
    lua_pushboolean(L, fl_draw_symbol(label, x, y, w, h, color));
    return 1;
    }

#if 0
//const char*   fl_expand_text(const char *from, char *buf, int maxbuf, double maxw, int &n, double &width, int wrap, int draw_symbols=0)
static int Expand_text(lua_State *L) //@@ 
    {
    (void)L;
    return 0;
    }
#endif


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/


static const struct luaL_Reg Functions[] = 
    {
//      { "add_symbol", Add_symbol },
        { "arc", Arc },
        { "begin_complex_polygon", Begin_complex_polygon },
        { "begin_line", Begin_line },
        { "begin_loop", Begin_loop },
        { "begin_points", Begin_points },
        { "begin_polygon", Begin_polygon },
        { "box_dh", Box_dh },
        { "box_dw", Box_dw },
        { "box_dx", Box_dx },
        { "box_dy", Box_dy },
        { "can_do_alpha_blending", Can_do_alpha_blending },
//      { "chord", Chord },
        { "circle", Circle },
        { "clip_box", Clip_box },
        { "cursor", Cursorr },
        { "curve", Curve },
        { "draw", Draw  },
        { "draw_box", Draw_box },
        { "draw_box_active", Draw_box_active },
        { "draw_image", Draw_image },
        { "draw_image_mono", Draw_image_mono },
        { "draw_pixmap", Draw_pixmap },
        { "draw_symbol", Draw_symbol },
        { "end_complex_polygon", End_complex_polygon },
        { "end_line", End_line },
        { "end_loop", End_loop },
        { "end_points", End_points },
        { "end_polygon", End_polygon },
//      { "expand_text", Expand_text },
        { "frame", Frame },
        { "frame2", Frame2 },
        { "gap", Gap },
        { "line", Line },
        { "line_style", Line_style },
        { "loop", Loop },
        { "measure", Measure },
        { "measure_pixmap", Measure_pixmap },
        { "mult_matrix", Mult_matrix },
        { "not_clipped", Not_clipped },
        { "overlay_clear", Overlay_clear },
        { "overlay_rect", Overlay_rect },
        { "pie", Pie },
        { "point", Point },
        { "polygon", Polygon },
        { "pop_clip", Pop_clip },
        { "pop_matrix", Pop_matrix },
        { "push_clip", Push_clip },
        { "push_matrix", Push_matrix },
        { "push_no_clip", Push_no_clip },
        { "read_image", Read_image },
        { "rect", Rect },
        { "rectf", Rectf },
        { "reset_spot", Reset_spot },
        { "restore_clip", Restore_clip },
        { "rotate", Rotate },
        { "rtl_draw", Rtl_draw },
        { "scale", Scale },
        { "scroll", Scroll },
        { "scrollbar_size", Scrollbar_size },
        { "set_boxtype", Set_boxtype },
        { "get_boxtype", Get_boxtype },
        { "boxtype_code", Boxtype_code },
        { "set_spot", Set_spot },
        { "set_status", Set_status },
        { "shortcut_label", Shortcut_label },
        { "transform_dx", Transform_dx },
        { "transform_dy",  Transform_dy},
        { "transform_x", Transform_x },
        { "transform_y",  Transform_y},
        { "transformed_vertex", Transformed_vertex },
        { "translate", Translate },
        { "vertex", Vertex },
        { "xyline", Xyline },
        { "yxline", Yxline },
        { NULL, NULL } /* sentinel */
    };


void moonfltk_open_FlDraw(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);

    /* Set free boxtypes to a default value, to avoid segv in case an integer boxtype code
     * is used which was not previously set with set_boxtype. */
    for(int bt = FL_FREE_BOXTYPE; bt<256; bt++)
        {
        Fl::set_boxtype((Fl_Boxtype)bt, Fl::get_boxtype(FL_NO_BOX),
            Fl::box_dx(FL_NO_BOX), Fl::box_dy(FL_NO_BOX), Fl::box_dw(FL_NO_BOX), Fl::box_dh(FL_NO_BOX));
        }
    }

#if 0 //@@
void    Fl_Quartz_Graphics_Driver::copy_offscreen(int x, int y, int w, int h, Fl_Offscreen pixmap, int srcx, int srcy)

void    fl_begin_offscreen(Fl_Offscreen ctx)
void    fl_end_offscreen()
void    fl_copy_offscreen(int x, int y, int w, int h, Fl_Offscreen pixmap, int srcx, int srcy)
Fl_Offscreen    fl_create_offscreen(int w, int h)
void    fl_delete_offscreen(Fl_Offscreen ctx)

void    fl_clip_region(Fl_Region r)
Fl_Region   fl_clip_region()


//@@TODO: custom Fl_Boxtype
typedef void (Fl_Box_Draw_F)(int x, int y, int w, int h, Fl_Color color);
static Fl_Box_Draw_F *Fl::get_boxtype(Fl_Boxtype);
static void Fl::set_boxtype(Fl_Boxtype, Fl_Box_Draw_F*,uchar,uchar,uchar,uchar);
static void Fl::set_boxtype(Fl_Boxtype, Fl_Boxtype from);

//@@TODO: custom Fl_Label
typedef void (Fl_Label_Draw_F)(const Fl_Label *label, int x, int y, int w, int h, Fl_Align align);
typedef void (Fl_Label_Measure_F)(const Fl_Label *label, int &width, int &height);
/** Sets the functions to call to draw and measure a specific labeltype. */
static void Fl::set_labeltype(Fl_Labeltype,Fl_Label_Draw_F*,Fl_Label_Measure_F*);
static void Fl::set_labeltype(Fl_Labeltype, Fl_Labeltype from); // is it defined ?

#endif
