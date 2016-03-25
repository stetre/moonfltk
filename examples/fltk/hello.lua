#!/usr/bin/env lua
-- MoonFLTK example: hello.lua
--
-- Derived from the FLTK test/hello.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

win = fl.window(340, 180, arg[0])
box = fl.box(20, 40, 300, 100, "Hello, World!");
box:box('up box')
box:labelfont(fl.BOLD + fl.ITALIC)
box:labelsize(36)
box:labeltype('shadow')
win:done() -- 'end' is a keyword in Lua
win:show(arg[0], arg)

return fl.run()

