#!/usr/bin/env lua
-- MoonFLTK example: fast_slow.lua
--
-- Derived from the FLTK test/fast_slow.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function cb_control(o)
   fast:value(o:value())
   if not fl.pushed() then slow:value(o:value()) end
end

w = fl.double_window(318, 443, arg[0])

control = fl.slider(90, 200, 30, 200, "move\nthis")
control:callback(cb_control)
control:when('changed', 'release', 'not changed')
control:labelsize(10)

fast = fl.slider(140, 200, 30, 200, "fast\nredraw")
fast:set_output()
fast:labelsize(10)

slow = fl.slider(190, 200, 30, 200, "slow\nredraw")
slow:set_output()
slow:labelsize(10)

box = fl.box(10, 10, 300, 180, [[
The left slider has changed('changed', 'release', 'not changed') so it produces a callback on both drag and release mouse events.
The middle slider (representing a widget with low overhead) is changed on every mouse movement.
The right slider (representing a widget with high overhead) is only updated when the mouse is released, by checking if fl.pushed() is nil.
]])
box:box("down box")
box:color(53)
box:selection_color(fl.DARK1)
box:labelfont(fl.COURIER)
box:labelsize(12)
box:align('wrap', 'left', 'inside')

w:done()
w:resizable(w)
w:show(arg[0], arg)
return fl.run()
