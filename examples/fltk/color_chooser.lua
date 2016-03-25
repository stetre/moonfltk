#!/usr/bin/env lua
-- MoonFLTK example: color_chooser.lua
--
-- Derived from the FLTK test/color_chooser.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

width = 75
height = 75
image = {}

floor = math.floor

function make_image()
  for y = 0, height-1 do
      local Y = y/(height-1)
      for x = 0, width-1 do
         local X = x/(width-1)
         image[#image+1] = floor(255*((1-X)*(1-Y))) -- red in upper-left
         image[#image+1] = floor(255*((1-X)*Y)) -- green in lower-left
         image[#image+1] = floor(255*(X*Y))  -- blue in lower-right
         --print(image[#image-2], image[#image-1], image[#image])
      end
   end
   -- Convert the array of bytes to a binary string:
   for i=1,#image do 
      image[i] = string.pack("B", image[i]) 
   end
   image = table.concat(image)
end

function Pens(X,Y,W,H,L)
   local pens = fl.box_sub(X,Y,W,H,L)
   pens:override_draw(function(_)
      assert(_ == pens)
      -- use every color in the gray ramp:
      for i = 0, W-1 do
         fl.color(fl.GRAY_RAMP+i)
         fl.line(pens:x()+i, pens:y(), pens:x()+i, pens:y()+pens:h())
      end
   end)
   return pens
end

c = fl.GRAY
fullcolor_cell = fl.FREE_COLOR


function cb1(_, b)
   c = fl.show_colormap(c)
   b:color(c)
   b:parent():redraw()
end

function cb2(_, bx)
   local  rgbi, r, g, b = fl.get_color(c)
   r, g, b =  fl.color_chooser_b("New color:",r,g,b,'hsv')
   if not r then return end
   c = fullcolor_cell

   fl.set_color(fullcolor_cell,r,g,b)
   bx:color(fullcolor_cell)
   bx:parent():redraw()
end

-- main ----------------------------------------------------
fl.set_color(fullcolor_cell,145,159,170)
window = fl.window(400,400, arg[0])
box = fl.box(50,50,300,300)
box:box('thin down box')
c = fullcolor_cell
box:color(c)
b1 = fl.button(140,120,120,30,"fl.show_colormap")
b1:callback(cb1,box)
b1:labelsize(12)
b2 = fl.button(140,160,120,30,"fl.choose_color")
b2:callback(cb2,box)
b2:labelsize(12)
image_box = fl.box(140,200,120,120)
make_image()
fl.rgb_image(image, width, height):label(image_box)
b = fl.box(140,320,120,0,"Example of fl.draw_image()")
b:labelsize(12)
p = Pens(80,200,3*8,120,"lines")
p:labelsize(12)
p:align(fl.ALIGN_TOP)
window:show(argc,argv)
return fl.run()

