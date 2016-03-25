#!/usr/bin/env lua
-- MoonFLTK example: adjuster.lua
--
-- Derived from the FLTK test/adjuster.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function adjcb(adjuster, box) 
   box:label(adjuster:format())
   box:redraw()
end

win = fl.double_window(320, 100, arg[0])

b1 = fl.box('down box', 20, 30, 80, 25, "\0")
b1:color(fl.WHITE)
a1 = fl.adjuster(20+80, 30, 3*25, 25)
a1:callback(adjcb, b1)
a1:do_callback(b1)

b2 = fl.box('down box', 20+80+4*25, 30, 80, 25)
b2:color(fl.WHITE)
a2 = fl.adjuster(b2:x() + b2:w(), 10, 25, 3*25)
a2:callback(adjcb, b2)
a2:do_callback(b2)

win:done()
win:resizable(win)

win:show(arg[0], arg)
return fl.run()

