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

#if USE_IMAGES

#define IMAGE_NEW(T, Func)                      \
static int Func(lua_State *L)                   \
    {                                           \
    Fl_##T *p;                                  \
    const char* filename = luaL_checkstring(L, 1);  \
    p = new Fl_##T(filename);                   \
    if(p->data() == NULL)                       \
        {                                       \
        delete p;                               \
        return luaL_error(L, "cannot load image from file '%s'", filename);\
        }                                       \
    DBG_CREATE(T, p);                           \
    newuserdata(L, p, MT_##T);                  \
    return 1;                                   \
    }

#define IMAGE_DELETE(T, Func)                   \
static int Func(lua_State *L)                   \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    DBG_DELETE(T, p);                           \
    if(freeuserdata(L, p)) delete p;            \
    return 0;                                   \
    }

#define IMAGE_COPY(T, Func)                     \
/* virtual Fl_Image *copy(int W, int H);    */  \
/* Fl_Image *copy();                        */  \
static int Func(lua_State *L)                   \
    {                                           \
    Fl_##T *orig = check_##T(L, 1);             \
    if(orig->data() == NULL)                    \
        return luaL_error(L, "image has no data");\
    Fl_Image *p;                                \
    if(lua_isnone(L, 2))                        \
        p = orig->copy();                       \
    else                                        \
        {                                       \
        int w = luaL_checkinteger(L, 2);        \
        int h = luaL_checkinteger(L, 3);        \
        p = orig->copy(w, h);                   \
        }                                       \
    if(!p)                                      \
        return luaL_error(L, "image copy failed");\
    DBG_CREATE(T, p);                           \
    newuserdata(L, p, MT_##T);                  \
    return 1;                                   \
    }

#define IMAGE_LABEL(T, Func)                    \
static int Func(lua_State *L)                   \
/* label(widget) */                             \
/* label(menu, pahtname)  for menu item */      \
    {                                           \
    Fl_##T *p = check_##T(L, 1);                \
    if(lua_isnone(L, 3))                    \
        {                                       \
        Fl_Widget *w = check_Widget(L, 2);      \
        p->label(w);                            \
        return 0;                               \
        }                                       \
    Fl_Menu_ *menu = check_Menu_(L, 2);         \
    Fl_Menu_Item *m = check_Pathname(L, menu, 3);\
    p->label(m);                                \
    return 0;                                   \
    }


#define IMAGE_DRAW(T, Func)                 \
static int Func(lua_State *L)               \
/* virtual void draw(int X, int Y, int W, int H, int cx=0, int cy=0); */\
/* void draw(int X, int Y) {draw(X, Y, w(), h(), 0, 0);} */             \
    {                                       \
    int arg = 1;                            \
    Fl_##T *p = check_##T(L, arg++);        \
    int x = luaL_checkinteger(L, arg++);    \
    int y = luaL_checkinteger(L, arg++);    \
    if(lua_isnone(L, arg))                  \
        {                                   \
        p->draw(x, y);                      \
        return 0;                           \
        }                                   \
    int w = luaL_checkinteger(L, arg++);    \
    int h = luaL_checkinteger(L, arg++);    \
    int cx = luaL_optinteger(L, arg++, 0);  \
    int cy = luaL_optinteger(L, arg++, 0);  \
    p->draw(x, y, w, h, cx, cy);            \
    return 0;                               \
    }

#define IMAGE_COLOR_AVERAGE(T, Func)                    \
static int Func(lua_State *L)                           \
/* virtual void color_average(Fl_Color c, float i); */  \
    {                                                   \
    Fl_##T *p = check_##T(L, 1);                        \
    p->color_average(check_Color(L, 2), luaL_checknumber(L, 3));    \
    return 0;                                           \
    }


/*------------------------------------------------------------------------------*
 | Fl_Image                                                                     |
 *------------------------------------------------------------------------------*/

static int ImageNew(lua_State *L)
    {
    Fl_Image *p;
    int arg = 1;
    int w, h,d;
    w = luaL_checkinteger(L, arg++);
    h = luaL_checkinteger(L, arg++);
    d = luaL_checkinteger(L, arg++);
    p = new Fl_Image(w, h, d);
    DBG_CREATE(Image, p);
    newuserdata(L, p, MT_Image);
    return 1;
    }

IMAGE_DELETE(Image, ImageDelete)
IMAGE_COPY(Image, Imagecopy)
INT_VOID(Image, w, Imagew)
INT_VOID(Image, h, Imageh)
INT_VOID(Image, d, Imaged)
INT_VOID(Image, ld, Imageld)
INT_VOID(Image, count, Imagecount)
IMAGE_COLOR_AVERAGE(Image, Imagecolor_average)

VOID_VOID(Image, desaturate, Imagedesaturate)
IMAGE_LABEL(Image, Imagelabel)
IMAGE_DRAW(Image, Imagedraw)
VOID_VOID(Image, uncache, Imageuncache)
VOID_VOID(Image, inactive, Imageinactive)
//@@ const char * const *data() const {return data_;}

static int Imagergb_scaling(lua_State *L)
    {
    if(lua_isnone(L, 1))
        {
        push_RGB_Scaling(L, Fl_Image::RGB_scaling());
        return 1;
        }
    else
    Fl_Image::RGB_scaling(check_RGB_Scaling(L, 1));
    return 0;
    }

static int Imagedata(lua_State *L) //@@DOC
    {
    Fl_Image *p = check_Image(L, 1);
    int count = p->count(); 
    const char * const * data = p->data();
    if(!data || count == 0) /* image has no data */
        return 0;
    else if (count == 1) /* bitmap or color image */
        {
        lua_pushlstring(L, data[0], p->w() * p->h() * p->d());
        return 1;
        } 
    else if (count > 2) // pixmap
        {
        lua_newtable(L);
        for(int i=0; i<count; i++)
            {
            lua_pushstring(L, data[i]);
            lua_seti(L, -2, i+1);
            }
        return 1;
        }
    return unexpected(L);
    }


static const struct luaL_Reg ImageMethods[] = 
    {
        { "color_average", Imagecolor_average },
        { "copy", Imagecopy },
        { "count", Imagecount },
        { "d", Imaged },
        { "desaturate", Imagedesaturate },
        { "draw", Imagedraw },
        { "data", Imagedata },
        { "h", Imageh },
        { "inactive", Imageinactive },
        { "label", Imagelabel },
        { "ld", Imageld },
        { "uncache", Imageuncache },
        { "w", Imagew },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg ImageMetaMethods[] = 
    {
        { "__gc",  ImageDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Bitmap                                                                       |
 *------------------------------------------------------------------------------*/

static int BitmapNew(lua_State *L)
    {
    Fl_Bitmap *p;
    size_t len;
    const char *data = luaL_checklstring(L, 1, &len);
    int w = luaL_checkinteger(L, 2);
    int h = luaL_checkinteger(L, 3);
    /* make a copy of data (Lua may free it) */
    char *bits = (char*)malloc(len *sizeof(char));
    memcpy(bits, data, len *sizeof(char));
    p = new Fl_Bitmap(bits, w, h);
    DBG_CREATE(Bitmap, p);
    ud_t *ud = newuserdata(L, p, MT_Bitmap);
    ud->bits = bits; /* so that freeuserdata() will free() it */
    return 1;
    }

IMAGE_DELETE(Bitmap, BitmapDelete)
IMAGE_COPY(Bitmap, Bitmapcopy)
#if 0
IMAGE_LABEL(Bitmap, Bitmaplabel)
IMAGE_DRAW(Bitmap, Bitmapdraw)
VOID_VOID(Bitmap, uncache, Bitmapuncache)
#endif

static const struct luaL_Reg BitmapMethods[] = 
    {
        { "copy", Bitmapcopy },
#if 0
        { "draw", Bitmapdraw },
        { "label", Bitmaplabel },
        { "uncache", Bitmapuncache },
#endif
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg BitmapMetaMethods[] = 
    {
        { "__gc",  BitmapDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_XBM_Image                                                                 |
 *------------------------------------------------------------------------------*/

IMAGE_NEW(XBM_Image, XBM_ImageNew)
IMAGE_DELETE(XBM_Image, XBM_ImageDelete)
IMAGE_COPY(XBM_Image, XBM_Imagecopy)

static const struct luaL_Reg XBM_ImageMethods[] = 
    {
        { "copy", XBM_Imagecopy },
        { NULL, NULL } /* sentinel */
    };


static const struct luaL_Reg XBM_ImageMetaMethods[] = 
    {
        { "__gc",  XBM_ImageDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Pixmap                                                                       |
 *------------------------------------------------------------------------------*/

static int PixmapNew(lua_State *L)
/* pixmap({str1, str2, str3, ....}) */
    {
    Fl_Pixmap *p;
    int nxpm;
    char** xpm = checkpixmap(L, 1, &nxpm);
    p = new Fl_Pixmap((const char**)xpm);
    DBG_CREATE(Pixmap, p);
    ud_t *ud = newuserdata(L, p, MT_Pixmap);
    ud->xpm = xpm;
    ud->nxpm = nxpm;
    return 1;
    }

IMAGE_DELETE(Pixmap, PixmapDelete)
IMAGE_COPY(Pixmap, Pixmapcopy)
#if 0
IMAGE_LABEL(Pixmap, Pixmaplabel)
IMAGE_DRAW(Pixmap, Pixmapdraw)
VOID_VOID(Pixmap, uncache, Pixmapuncache)
VOID_VOID(Pixmap, desaturate, Pixmapdesaturate)
IMAGE_COLOR_AVERAGE(Pixmap, Pixmapcolor_average)
#endif

static const struct luaL_Reg PixmapMethods[] = 
    {
        { "copy", Pixmapcopy },
#if 0
// non servono (sono tutti dichiarati in image e virtual):
        { "color_average", Pixmapcolor_average },
        { "desaturate", Pixmapdesaturate },
        { "draw", Pixmapdraw },
        { "label", Pixmaplabel },
        { "uncache", Pixmapuncache },
#endif
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg PixmapMetaMethods[] = 
    {
        { "__gc",  PixmapDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_GIF_Image                                                                 |
 *------------------------------------------------------------------------------*/

IMAGE_NEW(GIF_Image, GIF_ImageNew)
IMAGE_DELETE(GIF_Image, GIF_ImageDelete)
IMAGE_COPY(GIF_Image, GIF_Imagecopy)

static const struct luaL_Reg GIF_ImageMethods[] = 
    {
        { "copy", GIF_Imagecopy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg GIF_ImageMetaMethods[] = 
    {
        { "__gc",  GIF_ImageDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | Fl_XPM_Image                                                                 |
 *------------------------------------------------------------------------------*/

IMAGE_NEW(XPM_Image, XPM_ImageNew)
IMAGE_DELETE(XPM_Image, XPM_ImageDelete)
IMAGE_COPY(XPM_Image, XPM_Imagecopy)

static const struct luaL_Reg XPM_ImageMethods[] = 
    {
        { "copy", XPM_Imagecopy },
        { NULL, NULL } /* sentinel */
    };


static const struct luaL_Reg XPM_ImageMetaMethods[] = 
    {
        { "__gc",  XPM_ImageDelete },
        { NULL, NULL } /* sentinel */
    };


/*------------------------------------------------------------------------------*
 | RGB_Image                                                                    |
 *------------------------------------------------------------------------------*/

static int RGB_ImageNew(lua_State *L)
    {
    char *bits = NULL;
    Fl_RGB_Image *p;
    Fl_Pixmap *pxm = test_Pixmap(L, 1);
    if(pxm)
        {
        if(lua_isnone(L, 2))
            p = new Fl_RGB_Image(pxm);
        else
            p = new Fl_RGB_Image(pxm, check_Color(L, 2));
        }
    else
        {
        size_t len;
        const char *data = luaL_checklstring(L, 1, &len);
        int w = luaL_checkinteger(L, 2);
        int h = luaL_checkinteger(L, 3);
        int d = luaL_optinteger(L, 4, 3);
        int ld = luaL_optinteger(L, 5, 0);
        /* make a copy of data (Lua may free it) */
        bits = (char*)malloc(len *sizeof(char));
        memcpy(bits, data, len *sizeof(char));
        p = new Fl_RGB_Image((const uchar*)bits, w, h, d, ld);
        }
    DBG_CREATE(RGB_Image, p);
    ud_t *ud = newuserdata(L, p, MT_RGB_Image);
    ud->bits = bits;
    return 1;
    }

IMAGE_DELETE(RGB_Image, RGB_ImageDelete)
IMAGE_COPY(RGB_Image, RGB_Imagecopy)

#if 0
IMAGE_LABEL(RGB_Image, RGB_Imagelabel)
IMAGE_DRAW(RGB_Image, RGB_Imagedraw)
VOID_VOID(RGB_Image, uncache, RGB_Imageuncache)
VOID_VOID(RGB_Image, desaturate, RGB_Imagedesaturate)
IMAGE_COLOR_AVERAGE(RGB_Image, RGB_Imagecolor_average)
#endif

static const struct luaL_Reg RGB_ImageMethods[] = 
    {
        { "copy", RGB_Imagecopy },
#if 0
        { "color_average", RGB_Imagecolor_average },
        { "desaturate", RGB_Imagedesaturate },
        { "draw", RGB_Imagedraw },
        { "label", RGB_Imagelabel },
        { "uncache", RGB_Imageuncache },
#endif
        { NULL, NULL } /* sentinel */
    };

FUNC_GETSET_INT(Fl_RGB_Image::max_size, RGB_Imagemax_size)

static const struct luaL_Reg RGB_ImageMetaMethods[] = 
    {
        { "__gc",  RGB_ImageDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_BMP_Image                                                                 |
 *------------------------------------------------------------------------------*/

IMAGE_NEW(BMP_Image, BMP_ImageNew)
IMAGE_DELETE(BMP_Image, BMP_ImageDelete)
IMAGE_COPY(BMP_Image, BMP_Imagecopy)

static const struct luaL_Reg BMP_ImageMethods[] = 
    {
        { "copy", BMP_Imagecopy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg BMP_ImageMetaMethods[] = 
    {
        { "__gc",  BMP_ImageDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_JPEG_Image                                                                |
 *------------------------------------------------------------------------------*/

IMAGE_NEW(JPEG_Image, JPEG_ImageNew)
  //@@ fare anche Fl_JPEG_Image(const char *name, const unsigned char *data);
IMAGE_DELETE(JPEG_Image, JPEG_ImageDelete)
IMAGE_COPY(JPEG_Image, JPEG_Imagecopy)

static const struct luaL_Reg JPEG_ImageMethods[] = 
    {
        { "copy", JPEG_Imagecopy },
        { NULL, NULL } /* sentinel */
    };


static const struct luaL_Reg JPEG_ImageMetaMethods[] = 
    {
        { "__gc",  JPEG_ImageDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_PNG_Image                                                                 |
 *------------------------------------------------------------------------------*/

IMAGE_NEW(PNG_Image, PNG_ImageNew)
  //@@ fare anche Fl_PNG_Image (const char *name_png, const unsigned char *buffer, int datasize);
IMAGE_DELETE(PNG_Image, PNG_ImageDelete)
IMAGE_COPY(PNG_Image, PNG_Imagecopy)

static const struct luaL_Reg PNG_ImageMethods[] = 
    {
        { "copy", PNG_Imagecopy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg PNG_ImageMetaMethods[] = 
    {
        { "__gc",  PNG_ImageDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_PNM_Image                                                                 |
 *------------------------------------------------------------------------------*/

IMAGE_NEW(PNM_Image, PNM_ImageNew)
IMAGE_DELETE(PNM_Image, PNM_ImageDelete)
IMAGE_COPY(PNM_Image, PNM_Imagecopy)

static const struct luaL_Reg PNM_ImageMethods[] = 
    {
        { "copy", PNM_Imagecopy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg PNM_ImageMetaMethods[] = 
    {
        { "__gc",  PNM_ImageDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Tiled_Image                                                                  |
 *------------------------------------------------------------------------------*/

static int Tiled_ImageNew(lua_State *L)
    {
    Fl_Tiled_Image *p;
    Fl_Image *img = check_Image(L, 1);
    int w = luaL_optinteger(L, 2, 0);
    int h = luaL_optinteger(L, 3, 0);
    p = new Fl_Tiled_Image(img, w, h);
    DBG_CREATE(Tiled_Image, p);
    newuserdata(L, p, MT_Tiled_Image);
    return 1;
    }

IMAGE_DELETE(Tiled_Image, Tiled_ImageDelete)
IMAGE_COPY(Tiled_Image, Tiled_Imagecopy)
#if 0
IMAGE_DRAW(Tiled_Image, Tiled_Imagedraw)
VOID_VOID(Tiled_Image, desaturate, Tiled_Imagedesaturate)
IMAGE_COLOR_AVERAGE(Tiled_Image, Tiled_Imagecolor_average)
#endif

static int Tiled_Imageimage(lua_State *L)
    {
    Fl_Tiled_Image *p = check_Tiled_Image(L, 1);
    push_Image(L, p->image());
    return 1;
    }

static const struct luaL_Reg Tiled_ImageMethods[] = 
    {
#if 0
        { "color_average", Tiled_Imagecolor_average },
        { "desaturate", Tiled_Imagedesaturate },
        { "draw", Tiled_Imagedraw },
#endif
        { "copy", Tiled_Imagecopy },
        { "image", Tiled_Imageimage },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Tiled_ImageMetaMethods[] = 
    {
        { "__gc",  Tiled_ImageDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Fl_Shared_Image                                                              |
 *------------------------------------------------------------------------------*/

/* The constructor is protected and cannot be called directly 
 * by a program: we have to use get() or find() instead.
 */
//  static Fl_Shared_Image *find(const char *n, int W = 0, int H = 0);
//  static Fl_Shared_Image *get(const char *n, int W = 0, int H = 0);
#define F(func, Func)                                   \
static int Func(lua_State *L)                           \
    {                                                   \
    Fl_Shared_Image *p;                                 \
    const char *name = luaL_checkstring(L, 1);          \
    int w = luaL_optinteger(L, 2, 0);                   \
    int h = luaL_optinteger(L, 3, 0);                   \
    p = Fl_Shared_Image::func(name, w, h);              \
    if(!p)                                              \
        return 0; /* not found */                       \
    DBG_CREATE(T, p);                                   \
    newuserdata(L, p, MT_Shared_Image);                 \
    return 1;                                           \
    }
F(get, Shared_Imageget)
F(find, Shared_Imagefind)
#undef F

IMAGE_COPY(Shared_Image, Shared_Imagecopy)

static int Shared_ImageDelete(lua_State *L)
    {
    Fl_Shared_Image *p = check_Shared_Image(L, 1);
    DBG_DELETE(Shared_Image, p);
    /* No delete, because the image object is automatically deleted
     * when refcount==0. Whe have to release it, instead. */
    if(freeuserdata(L, p)) p->release();
    return 0;
    }

static int Shared_Imagename(lua_State *L)
    {
    Fl_Shared_Image *p = check_Shared_Image(L, 1);
    const char *name = p->name();
    if(!name) return unexpected(L);
    lua_pushstring(L, name);
    return 1;
    }

INT_VOID(Shared_Image, refcount, Shared_Imagerefcount)
VOID_VOID(Shared_Image, reload, Shared_Imagereload)

#if 0 /* 8 yy */
GETSET_INT(Shared_Image, , Shared_Image)
GETSET_BOOLEAN(Shared_Image, , Shared_Image)
VOID_VOID(Shared_Image, , Shared_Image)
BOOLEAN_VOID(Shared_Image, , Shared_Image)
BOOLEAN_INT(Shared_Image, , Shared_Image)
        { "", Shared_Image },
static int Shared_Image(lua_State *L) //@@ 
    {
    Fl_Shared_Image *p = check_Shared_Image(L, 1);
    (void)p;
    return 0;
    }
#endif

FUNC_VOID_VOID(fl_register_images, RegisterImages)

static const struct luaL_Reg Shared_ImageMethods[] = 
    {
        { "copy", Shared_Imagecopy },
        { "name", Shared_Imagename },
        { "refcount", Shared_Imagerefcount },
        { "reload", Shared_Imagereload },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Shared_ImageMetaMethods[] = 
    {
        { "__gc",  Shared_ImageDelete },
        { NULL, NULL } /* sentinel */
    };

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        /* constructors */
        { "image",  ImageNew },
        { "bitmap",  BitmapNew },
        { "xbm_image",  XBM_ImageNew },
        { "pixmap",  PixmapNew },
        { "gif_image",  GIF_ImageNew },
        { "xpm_image",  XPM_ImageNew },
        { "rgb_image",  RGB_ImageNew },
        { "bmp_image",  BMP_ImageNew },
        { "jpeg_image",  JPEG_ImageNew },
        { "png_image",  PNG_ImageNew },
        { "pnm_image",  PNM_ImageNew },
        { "tiled_image",  Tiled_ImageNew },
        { "shared_image_get", Shared_Imageget },
        { "shared_image_find", Shared_Imagefind },
        /* global functions and static methods: */
        { "rgb_scaling", Imagergb_scaling },
        { "rgb_max_size", RGB_Imagemax_size },
        { "register_images", RegisterImages },
        { NULL, NULL } /* sentinel */
    };

void moonfltk_open_Image(lua_State *L)
    {
    udata_define(L, MT_Image, ImageMethods, ImageMetaMethods);

    udata_define(L, MT_Bitmap, BitmapMethods, BitmapMetaMethods);
    udata_inherit(L, MT_Bitmap, MT_Image);

    udata_define(L, MT_XBM_Image, XBM_ImageMethods, XBM_ImageMetaMethods);
    udata_inherit(L, MT_XBM_Image, MT_Bitmap);

    udata_define(L, MT_Pixmap, PixmapMethods, PixmapMetaMethods);
    udata_inherit(L, MT_Pixmap, MT_Image);

    udata_define(L, MT_GIF_Image, GIF_ImageMethods, GIF_ImageMetaMethods);
    udata_inherit(L, MT_GIF_Image, MT_Pixmap);

    udata_define(L, MT_XPM_Image, XPM_ImageMethods, XPM_ImageMetaMethods);
    udata_inherit(L, MT_XPM_Image, MT_Pixmap);

    udata_define(L, MT_RGB_Image, RGB_ImageMethods, RGB_ImageMetaMethods);
    udata_inherit(L, MT_RGB_Image, MT_Image);

    udata_define(L, MT_BMP_Image, BMP_ImageMethods, BMP_ImageMetaMethods);
    udata_inherit(L, MT_BMP_Image, MT_RGB_Image);

    udata_define(L, MT_JPEG_Image, JPEG_ImageMethods, JPEG_ImageMetaMethods);
    udata_inherit(L, MT_JPEG_Image, MT_RGB_Image);

    udata_define(L, MT_PNG_Image, PNG_ImageMethods, PNG_ImageMetaMethods);
    udata_inherit(L, MT_PNG_Image, MT_RGB_Image);

    udata_define(L, MT_PNM_Image, PNM_ImageMethods, PNM_ImageMetaMethods);
    udata_inherit(L, MT_PNM_Image, MT_RGB_Image);

    udata_define(L, MT_Tiled_Image, Tiled_ImageMethods, Tiled_ImageMetaMethods);
    udata_inherit(L, MT_Tiled_Image, MT_Image);

    udata_define(L, MT_Shared_Image, Shared_ImageMethods, Shared_ImageMetaMethods);
    udata_inherit(L, MT_Shared_Image, MT_Image);

    luaL_setfuncs(L, Functions, 0);
    }

#else

void moonfltk_open_Image(lua_State *L)
    {
    (void)L;
    }

#endif /* USE_IMAGES */


#if(0) //@@

// Test function for adding new formats
typedef Fl_Image *(*Fl_Shared_Handler)(const char *name, uchar *header, int headerlen);

//Multi 'costruttore':
static Fl_Shared_Image **images();
static int      num_images();

static void     add_handler(Fl_Shared_Handler f);
static void     remove_handler(Fl_Shared_Handler f);

#endif
