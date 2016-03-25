#!/usr/bin/env lua
-- MoonFLTK example: clock.lua
--
-- Derived from the FLTK test/clock.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

w1 = fl.double_window(220, 220, "clock")
c1 = fl.clock(0, 0, 220, 220)
w1:resizable(c1)
w1:done()

w2 = fl.double_window(220, 220, "round_clock")
c2 = fl.round_clock(0, 0, 220, 220)
w2:resizable(c2)
w2:done()

w1:xclass("Fl_Clock")
w2:xclass("Fl_Clock")
w1:show()
w2:show()
return fl.run()
