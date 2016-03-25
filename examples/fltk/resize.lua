#!/usr/bin/env lua
-- MoonFLTK example: resize.lua
--
-- Derived from the FLTK test/resize.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function cb_(b) 
   local w = b:window()
   w:position(w:x()-50,w:y()) 
end

function cb_2(b) 
   local w = b:window()
   w:position(w:x(),w:y()+50) 
end

function cb_1(b) 
   local w = b:window()
   w:position(w:x()+50,w:y()) 
end

function cb_8(b) 
   local w = b:window()
   w:position(w:x(),w:y()-50) 
end

function cb_grow(b) 
   local w = b:window()
   w:size(w:w()+20, w:h()+20) 
end

function cb_shrink(b) 
   local w = b:window()
   w:size(w:w()-20, w:h()-20) 
end

w = fl.double_window(366, 261, arg[0])
o = fl.button(20, 40, 40, 40, "@<-")
o:callback(cb_)
o = fl.button(60, 80, 40, 40, "@2->")
o:callback(cb_2)
o = fl.button(100, 40, 40, 40, "@->")
o:callback(cb_1)
o = fl.button(60, 0, 40, 40, "@8->")
o:callback(cb_8)
o = fl.button(30, 130, 110, 40, "grow")
o:labelfont(1)
o:labelsize(18)
o:callback(cb_grow)
o = fl.button(30, 190, 110, 40, "shrink")
o:labelfont(1)
o:labelsize(18)
o:callback(cb_shrink)
o = fl.box(150, 10, 160, 220, 
"This is a test of program-generated resize() of a window.  The window should "..
"move or resize once when each button is clicked.  The program and window manager " ..
"should not go into fits echoing resizes back and forth!")
o:box('border box')
o:align(132+fl.ALIGN_INSIDE)
w:done()
w:resizable(w)
w:show(argc, argv)
return fl.run()
