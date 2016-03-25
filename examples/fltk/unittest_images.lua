-- MoonFLTK example: unittest_images.lua
--
-- Derived from the FLTK test/unittest_images.cxx example (http://www.fltk.org)
--

-- Note: currently (March 2010) fl.draw_image() supports transparency with
--  alpha channel only on Apple (Mac OS X), but fl.rgb_image:draw()
--  supports transparency on all platforms !

--------- test the image drawing capabilities of this implementation ----------


-- Some parameters for fine tuning for developers - their
-- default values ought to be: CB=true, DX=0, LX=0, IMG=true
CB = true -- 'true' to show the checker board background for alpha images, 'false' otherwise
DX = 0   -- additional (undefined (0)) pixels per line, must be >= 0
         -- ignored (irrelevant), if LX == 0 (see below)
LX = 0   -- 0 for default: ld() = 0, i.e. ld() defaults (internally) to w()*d()
      -- +1: ld() = (w() + DX) * d()
      -- -1 to flip image vertically: ld() = - ((w() + DX) * d())
IMG = true -- 'true' to use fl.rgb_image for drawing images,
      -- 'false' to use fl.draw_image() instead.
      -- Note: as of April 2011, only 1 (fl.rgb_image) works correctly with alpha
      -- channel, 0 (fl.draw_image()) ignores the alpha channel (FLTK 1.3.0).
      -- There are plans to support alpha in fl.draw_image() in FLTK 1.3.x,
      -- but not in FLTK 1.1.x .

local dg = {}
local dga = {}
local drgb = {}
local drgba = {}

for y=0,127 do
   for x=0,127 do
      drgba[#drgba+1] = y<<1
      drgb[#drgb+1] = y<<1
      dga[#dga+1] = y<<1
      dg[#dg+1] = y<<1
      drgba[#drgba+1] = x<<1
      drgb[#drgb+1] = x<<1
      drgba[#drgba+1] = (127-x)<<1
      drgb[#drgb+1] = (127-x)<<1
      drgba[#drgba+1] = x+y
      dga[#dga+1] = x+y
   end
   if DX > 0 and LX ~= 0 then
      for i = 1, 1*DX do dg[#dg+1] = 0 end
      for i = 1, 2*DX do dga[#dga+1] = 0 end
      for i = 1, 3*DX do drgb[#drgb+1] = 0 end
      for i = 1, 4*DX do drgba[#drgba+1] = 0 end
   end
end

function pack(d) -- converts a table of bytes into a binary string
   local t = {}
   for i = 1, #d do t[i] = string.pack("B", d[i]) end
   return table.concat(t)
end

local img_gray = pack(dg)
local img_gray_a = pack(dga)
local img_rgb = pack(drgb)
local img_rgba = pack(drgba)

if LX<0 then 
   img_gray   = img_gray:sub(127*(128+DX) + 1)
   img_gray_a = img_gray_a:sub(127*(128+DX)*2 + 1)
   img_rgb    = img_rgb:sub(127*(128+DX)*3 + 1)
   img_rgba   = img_rgba:sub(127*(128+DX)*4 + 1)
end

                           -- bits,     w,  h, depth, linedata
local i_g    = fl.rgb_image(img_gray  ,128,128,  1,   LX*(128+DX))
local i_ga   = fl.rgb_image(img_gray_a,128,128,  2,   LX*(128+DX)*2)
local i_rgb  = fl.rgb_image(img_rgb,   128,128,  3,   LX*(128+DX)*3)
local i_rgba = fl.rgb_image(img_rgba,  128,128,  4,   LX*(128+DX)*4)




return function(x, y, w, h)
   local label = "drawing images"
   local self = fl.box_sub(x, y, w, h)
    self:label("Testing Image Drawing\n\n" ..
   "This test renders four images, two of them with a checker board\n" ..
   "visible through the graphics. Color and gray gradients should be\n" ..
   "visible. This does not test any image formats such as JPEG.") 
   self:align(fl.ALIGN_INSIDE|fl.ALIGN_BOTTOM|fl.ALIGN_LEFT|fl.ALIGN_WRAP)
   self:box('border box')
   self:override_draw(function()
      self:super_draw()

    -- top left: RGB

   local x, y = self:x(), self:y()
    local xx, yy = x+10, y+10
    fl.color(fl.BLACK) fl.rect(xx, yy, 130, 130)
if IMG then
      i_rgb:draw(xx+1,yy+1)
else
    fl.draw_image(img_rgb, xx+1, yy+1, 128, 128, 3, LX*((128+DX)*3))
end
    fl.draw("RGB", xx+134, yy+64)

    -- bottom left: RGBA

    xx = x+10 yy = y+10+134
    fl.color(fl.BLACK) fl.rect(xx, yy, 130, 130)   -- black frame
    fl.color(fl.WHITE) fl.rectf(xx+1, yy+1, 128, 128) -- white background
if CB then
    fl.color(fl.BLACK) fl.rectf(xx+65, yy+1, 64, 64)
    fl.color(fl.BLACK) fl.rectf(xx+1, yy+65, 64, 64)
end
if IMG then
    i_rgba:draw(xx+1,yy+1)
else
    fl.draw_image(img_rgba, xx+1, yy+1, 128, 128, 4, LX*((128+DX)*4))
end
    fl.color(fl.BLACK) fl.draw("RGBA", xx+134, yy+64)

    -- top right: Gray

    xx = x+10+200 yy = y+10
    fl.color(fl.BLACK) fl.rect(xx, yy, 130, 130)
if IMG then
    i_g:draw(xx+1,yy+1)
else
    fl.draw_image(img_gray, xx+1, yy+1, 128, 128, 1, LX*((128+DX)*1))
end
    fl.draw("Gray", xx+134, yy+64)

    -- bottom right: Gray+Alpha

    xx = x+10+200 yy = y+10+134
    fl.color(fl.BLACK) fl.rect(xx, yy, 130, 130)   -- black frame
    fl.color(fl.WHITE) fl.rectf(xx+1, yy+1, 128, 128) -- white background
if CB then
    fl.color(fl.BLACK) fl.rectf(xx+65, yy+1, 64, 64)
    fl.color(fl.BLACK) fl.rectf(xx+1, yy+65, 64, 64)
end
if IMG then
    i_ga:draw(xx+1,yy+1)
else
    fl.draw_image(img_gray_a, xx+1, yy+1, 128, 128, 2, LX*((128+DX)*2))
end
    fl.color(fl.BLACK) fl.draw("Gray+Alpha", xx+134, yy+64)
   end)

   return { widget = self, label = label }
end
