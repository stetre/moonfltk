#!/usr/bin/env lua
-- MoonFLTK example: tiled_image.lua
--
-- Derived from the FLTK test/tiled_image.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

--imgfile = "pixmaps/tile.xpm"
imgfile = "pixmaps/porsche.xpm" -- this shows tiling better..

fl.visual('rgb')

window = fl.double_window(400,400)
group = fl.group(0,0,400,400)
group:image(fl.tiled_image(fl.xpm_image(imgfile)))
group:align(fl.ALIGN_INSIDE)

b = fl.button(340,365,50,25,"Close")
b:callback(function() window:hide() end)

group:done()

window:resizable(group)
window:done()
window:show(arg[0], arg)

return fl.run()
