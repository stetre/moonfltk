#!/usr/bin/env lua
-- MoonFLTK example: twowin.lua
--
-- Derived from the FLTK test/twowin.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function cb1() b2:take_focus() end
function cb2() b1:take_focus() end

win1 = fl.double_window(200, 200)
bb1 = fl.button(10, 10, 100, 100, "b1")
bb1:callback(cb1)
b1 = fl.input(10, 150, 100, 25)
win1:label("win1")
win1:done()

win2 = fl.double_window(200, 200)
bb2 = fl.button(10, 10, 100, 100, "b2")
bb2:callback(cb2)
b2 = fl.input(10, 150, 100, 25)
win2:label("win2")
win2:done()

win1:position(200, 200)
win2:position(400, 200)

win1:show()
win2:show()
return fl.run()
