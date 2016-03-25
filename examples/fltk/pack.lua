#!/usr/bin/env lua
-- MoonFLTK example: pack.lua
--
-- Derived from the FLTK test/pack.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function type_cb(_, v)
   for i = 1, pack:children() do -- notice that indices start from 1, not from 0
      o = pack:child(i)
      o:resize(0,0,25,25)
   end
   pack:resize(scroll:x(),scroll:y(),scroll:w(),scroll:h())
   pack:parent():redraw()
   pack:type(v)
   pack:redraw()
end

function spacing_cb(o) 
   pack:spacing(o:value())
   scroll:redraw()
end

w = fl.double_window(360, 370, arg[0])
scroll = fl.scroll(10,10,340,285)
pack = fl.pack(10, 10, 340, 285)
pack:box("down frame")
fl.button(35, 35, 25, 25, "b1")
fl.button(45, 45, 25, 25, "b2")
fl.button(55, 55, 25, 25, "b3")
fl.button(65, 65, 25, 25, "b4")
fl.button(75, 75, 25, 25, "b5")
fl.button(85, 85, 25, 25, "b6")
fl.button(95, 95, 25, 25, "b7")
fl.button(105, 105, 25, 25, "b8")
fl.button(115, 115, 25, 25, "b9")
fl.button(125, 125, 25, 25, "b10")
fl.button(135, 135, 25, 25, "b11")
fl.button(145, 145, 25, 25, "b12")
fl.button(155, 155, 25, 25, "b13")
fl.button(165, 165, 25, 25, "b14")
fl.button(175, 175, 25, 25, "b15")
fl.button(185, 185, 25, 25, "b16")
fl.button(195, 195, 25, 25, "b17")
fl.button(205, 205, 25, 25, "b18")
fl.button(215, 215, 25, 25, "b19")
fl.button(225, 225, 25, 25, "b20")
fl.button(235, 235, 25, 25, "b21")
fl.button(245, 245, 25, 25, "b22")
fl.button(255, 255, 25, 25, "b23")
fl.button(265, 265, 25, 25, "b24")
pack:done()
w:resizable(pack)
scroll:done()

o = fl.radio_light_button(10, 305, 165, 25, "HORIZONTAL")
--o = fl.light_button(10, 305, 165, 25, "HORIZONTAL")
--o:type("radio")
o:callback(type_cb, "horizontal")

--o = fl.radio_light_button(185, 305, 165, 25, "VERTICAL")
o = fl.light_button(185, 305, 165, 25, "VERTICAL")
o:type("radio")
o:value(true)
o:callback(type_cb, "vertical")

o = fl.value_slider(100, 335, 250, 25, "Spacing: ")
o:align("left")
o:type("horizontal")
o:range(0,30)
o:step(1)
o:callback(spacing_cb)

w:done()
w:show(arg[0], arg)
return fl.run()


