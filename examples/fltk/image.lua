#!/usr/bin/env lua
-- MoonFLTK example: image.lua
--
-- Derived from the FLTK test/image.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

width = 100
height = 100

function make_image()
   local p = {}
   for y = 0, height-1 do
      local Y = y/(height-1)
      for x = 0, width-1 do
         local X = x/(width-1)
         local r = math.floor(255*((1-X)*(1-Y))) -- red in upper-left
         local g = math.floor(255*((1-X)*Y)) -- green in lower-left
         local b = math.floor(255*(X*Y))  -- blue in lower-right
         X = X - 0.5
         Y = Y - 0.5
         local alpha = math.floor(255 * math.sqrt(X * X + Y * Y))
         if alpha > 255 then alpha = 255 end
         p[#p+1] = string.pack("BBBB", r, g, b , alpha)
         Y = Y + 0.5
      end
   end
   image = table.concat(p) -- len = 4*width*height
end

function button_cb()
   local i = 0
   if leftb:value() then i = i | fl.ALIGN_LEFT end
   if rightb:value() then i = i | fl.ALIGN_RIGHT end
   if topb:value() then i = i | fl.ALIGN_TOP end
   if bottomb:value() then i = i | fl.ALIGN_BOTTOM end
   if insideb:value() then i = i | fl.ALIGN_INSIDE end
   if overb:value() then i = i | fl.ALIGN_TEXT_OVER_IMAGE end
   b:align(i)
   if (inactb:value()) then 
      b:deactivate()
   else 
      b:activate()
   end
  window:redraw() 
end


-- main

fl.visual('rgb')

window = fl.double_window(400,400, arg[0])
window:color(fl.WHITE)
b = fl.button(140,160,120,120,"Image w/Alpha")

make_image()
rgb = fl.rgb_image(image, width, height,4)
dergb = rgb:copy()
dergb:inactive()

b:image(rgb)
b:deimage(dergb)

leftb = fl.toggle_button(25,50,50,25,"left")
leftb:callback(button_cb)
rightb = fl.toggle_button(75,50,50,25,"right")
rightb:callback(button_cb)
topb = fl.toggle_button(125,50,50,25,"top")
topb:callback(button_cb)
bottomb = fl.toggle_button(175,50,50,25,"bottom")
bottomb:callback(button_cb)
insideb = fl.toggle_button(225,50,50,25,"inside")
insideb:callback(button_cb)
overb = fl.toggle_button(25,75,100,25,"text over")
overb:callback(button_cb)
inactb = fl.toggle_button(125,75,100,25,"inactive")
inactb:callback(button_cb)
window:resizable(window)
window:done()
window:show(argc, argv)
return fl.run()

