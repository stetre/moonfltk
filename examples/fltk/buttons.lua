#!/usr/bin/env lua
-- MoonFLTK example: buttons.lua
--
-- Derived from the FLTK test/buttons.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

-- main 
window = fl.window(320,130, arg[0]);
b = fl.button(10, 10, 130, 30, "button");
b:tooltip("this is a tooltip.");
--fl.tooltip_size(10)
fl.return_button(150, 10, 160, 30, "return_button");
fl.repeat_button(10,50,130,30,"repeat_button");
fl.light_button(10,90,130,30,"light_button");
fl.round_button(150,50,160,30,"round_button");
fl.check_button(150,90,160,30,"check_button");
window:done()
window:show(arg[0],arg);
return fl.run();

