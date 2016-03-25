#!/usr/bin/env lua
-- MoonFLTK example: iconize.lua
--
-- Derived from the FLTK test/iconize.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function iconize_cb(_, w)
  w:iconize()
end

function show_cb(_, w) 
  w:show()
end

function hide_cb(_, w)
  w:hide()
end

function window_cb()
  os.exit(true, true)
end

mainw = fl.window(200,200, arg[0])
mainw:done()
mainw:show(argc,argv)

control = fl.window(120,120)

hide_button = fl.button(0,0,120,30,"hide()")
hide_button:callback(hide_cb, mainw)

iconize_button = fl.button(0,30,120,30,"iconize()")
iconize_button:callback(iconize_cb, mainw)

show_button = fl.button(0,60,120,30,"show()")
show_button:callback(show_cb, mainw)

show_button2 = fl.button (0,90,120,30,"show this")
show_button2:callback(show_cb, control)

control:done()
control:show()
control:callback(window_cb)
return fl.run()
