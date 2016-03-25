#!/usr/bin/env lua
-- MoonFLTK example: pixmap.lua
--
-- Derived from the FLTK test/pixmap.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

imgfile = "pixmaps/porsche.xpm"

function button_cb()
   local i = 0
   if leftb:value() then i = i | fl.ALIGN_LEFT end
   if rightb:value() then i = i | fl.ALIGN_RIGHT end
   if topb:value() then i = i | fl.ALIGN_TOP end
   if bottomb:value() then i = i | fl.ALIGN_BOTTOM end
   if insideb:value() then i = i | fl.ALIGN_INSIDE end
   if overb:value() then i = i | fl.ALIGN_TEXT_OVER_IMAGE end
   b:align(i)
   if inactb:value() then b:deactivate() else b:activate() end
   window:redraw()
end

-- main -----------------------

window = fl.double_window(400,400)
b = fl.button(140,160,120,120,"Pixmap")
pixmap = fl.xpm_image(imgfile)
depixmap = pixmap:copy()
depixmap:inactive()

b:image(pixmap)
b:deimage(depixmap)

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
fl.visual('rgb')
window:resizable(window)
window:done()
window:show(arg[0],arg)
return fl.run()

