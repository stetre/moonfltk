#!/usr/bin/env lua
-- MoonFLTK example: icon.lua
--
-- Derived from the FLTK test/icon.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function choice_cb(_menu, _pn, c)
   icon:color_average(c, 0.0)
   win:icon(icon)
end

win = fl.double_window(400,300)
buffer = string.rep('\0', 32*32*3) --uchar buffer[32*32*3]
icon = fl.rgb_image(buffer, 32, 32, 3)

choice = fl.choice(80,100,200,25,"Colour:")
choice:add("Red",0,choice_cb, fl.RED)
choice:add("Green",0,choice_cb,fl.GREEN)
choice:add("Blue",0,choice_cb,fl.BLUE)
choice:callback(choice_cb)
choice:when('release', 'not changed')

win:done()
win:show(arg[0],arg)
return fl.run()
