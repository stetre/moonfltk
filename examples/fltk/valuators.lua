#!/usr/bin/env lua
-- MoonFLTK example: valuators.lua
--
-- Derived from the FLTK test/valuators.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

--fl.scheme("gtk+")
--fl.scheme("plastic")
--fl.scheme("gleam")

function callback(o)
   print(o:label(), o:value())
end

w = fl.double_window(580, 510, arg[0])
w:color(43)
w:selection_color(43)

-------------------------------------------------------------------

o = fl.box(10, 10, 280, 210, "sliders")
o:box("engraved box")
o:labelfont(1)
o:align("top", "inside")

o = fl.slider(30, 45, 20, 145, "vertical")
o:tooltip("Vertical Slider")
o:selection_color(1)
o:labelsize(8)
o:callback(callback)
o:align("top")

o = fl.slider(70, 55, 20, 145, "vertical fill")
o:type( "vertical fill")
o:selection_color(1)
o:labelsize(8)
o:callback(callback)

o = fl.slider(105, 45, 20, 145, "vertical nice")
o:type("vertical nice")
o:box("flat box")
o:color(10)
o:selection_color(1)
o:labelsize(8)
o:callback(callback)
o:align("top")

o = fl.slider(140, 80, 130, 20, "horizontal")
o:type("horizontal")
o:selection_color(1)
o:labelsize(8)
o:callback(callback)

o = fl.slider(140, 120, 130, 20, "horizontal fill")
o:type("horizontal fill")
o:selection_color(1)
o:labelsize(8)
o:callback(callback)

o = fl.slider(140, 160, 130, 20, "horizontal nice")
o:type("horizontal nice")
o:box("flat box")
o:color(10)
o:selection_color(1)
o:labelsize(8)
o:callback(callback)

-------------------------------------------------------------

o = fl.box(10, 230, 280, 210, "value sliders")
o:box("engraved box")
o:labelfont(1)
o:align("top", "inside")

o = fl.value_slider(30, 260, 30, 145, "vertical")
o:tooltip("Value Slider")
o:selection_color(1)
o:labelsize(8)
o:callback(callback)
o:align("top")

o = fl.value_slider(70, 275, 30, 140, "vertical fill")
o:type("vertical fill")
o:selection_color(1)
o:labelsize(8)
o:callback(callback)

o = fl.value_slider(110, 260, 20, 145, "vertical nice")
o:type("vertical nice")
o:box("flat box")
o:color(10)
o:selection_color(1)
o:labelsize(8)
o:callback(callback)
o:align("top")

o = fl.value_slider(140, 290, 130, 20, "horizontal")
o:type("horizontal")
o:selection_color(1)
o:labelsize(8)
o:callback(callback)

o = fl.value_slider(140, 330, 130, 20, "horizontal fill")
o:type("horizontal fill")
o:selection_color(1)
o:labelsize(8)
o:callback(callback)

o = fl.value_slider(140, 370, 130, 20, "horizontal nice")
o:type("horizontal nice")
o:box("flat box")
o:color(10)
o:selection_color(1)
o:labelsize(8)
o:callback(callback)

-----------------------------------------------------------

o = fl.box(10, 450, 135, 50, "value input")
o:box("engraved box")
o:labelfont(1)
o:align("top" , "inside")

o = fl.value_input(30, 470, 105, 25, "in")
o:tooltip("Value Input")
o:labelsize(8)
o:maximum(100)
o:step(0.1)
o:callback(callback)

-----------------------------------------------------------

o = fl.box(155, 450, 135, 50, "value output")
o:box("engraved box")
o:labelfont(1)
o:align("top" , "inside")

o = fl.value_output(170, 470, 105, 25, "out")
o:tooltip("Value Output")
o:labelsize(8)
o:maximum(100)
o:step(0.1)
o:callback(callback)

-----------------------------------------------------------

o = fl.box(300, 10, 130, 120, "scrollbars")
o:box("engraved box")
o:labelfont(1)
o:align("top", "left", "inside")

o = fl.scrollbar(305, 65, 95, 20, "horizontal")
o:tooltip("Horizontal Scrollbar")
o:type("horizontal")
o:labelsize(8)
o:maximum(100)
o:value(20)
o:callback(callback)

o = fl.scrollbar(400, 20, 20, 105, "vertical")
o:tooltip("Vertical Scrollbar")
o:labelsize(8)
o:maximum(100)
o:callback(callback)
o:align("top")

-----------------------------------------------------------

o = fl.box(440, 10, 130, 120, "adjusters")
o:box("engraved box")
o:labelfont(1)
o:align("top" , "inside")

o = fl.adjuster(450, 60, 75, 25, "w()>h()")
o:tooltip("Horizontal Adjuster")
o:labelsize(8)
o:callback(callback)

o = fl.adjuster(530, 35, 25, 75, "w()<h()")
o:tooltip("Vertical Adjuster")
o:labelsize(8)
o:callback(callback)

-------------------------------------------------------------------

o = fl.box(300, 140, 130, 120, "counters")
o:box("engraved box")
o:labelfont(1)
o:align("top", "inside")

o = fl.counter(310, 175, 110, 25, "normal")
o:tooltip("Standard Counter")
o:labelsize(8)
o:callback(callback)

o = fl.counter(310, 215, 110, 25, "simple")
o:tooltip("Simple Counter")
o:type("simple")
o:labelsize(8)
o:callback(callback)

-------------------------------------------------------------------

o = fl.box(440, 140, 130, 120, "spinners")
o:box("engraved box")
o:labelfont(1)
o:align("top" , "inside")

o = fl.spinner(465, 176, 80, 24, "int")
o:labelsize(8)
o:minimum(-30)
o:maximum(30)
o:step(2)
o:value(5)
o:align("bottom")
o:callback(callback)

o = fl.spinner(465, 216, 80, 24, "float")
o:type("float")
o:labelsize(8)
o:minimum(0)
o:maximum(1)
o:step(0.01)
o:value(0.05)
o:align("bottom")
o:callback(callback)

-------------------------------------------------------------------

o = fl.box(300, 270, 270, 105, "dials")
o:box("engraved box")
o:labelfont(1)
o:align("top" , "inside")

o = fl.dial(320, 295, 65, 65, "normal")
o:tooltip("Standard Dial")
o:color(10)
o:selection_color(1)
o:labelsize(8)
o:value(0.5)
o:callback(callback)
o:angles(0,315)

o = fl.dial(400, 295, 65, 65, "line")
o:tooltip("Line Dial")
o:type("line")
o:color(10)
o:selection_color(1)
o:labelsize(8)
o:value(0.5)
o:callback(callback)

o = fl.dial(480, 295, 65, 65, "fill")
o:tooltip("Fill Dial")
o:type("fill")
o:color(10)
o:selection_color(1)
o:labelsize(8)
o:value(1)
o:callback(callback)
o:angles(0,360)

-----------------------------------------------------------

o = fl.box(300, 385, 150, 115, "rollers")
o:box("engraved box")
o:labelfont(1)
o:align("top" , "inside")

o = fl.roller(315, 390, 20, 95, "vertical")
o:tooltip("Vertical Roller")
o:labelsize(8)
o:callback(callback)

o = fl.roller(345, 430, 90, 20, "horizontal")
o:tooltip("Horizontal Roller")
o:type("horizontal")
o:labelsize(8)
o:callback(callback)

-----------------------------------------------------------

o = fl.box(460, 385, 110, 115, "Some widgets have color(fl.GREEN) and color2(fl.RED) to show the areas these effect.")
o:box("border frame")
o:color(fl.FOREGROUND_COLOR)
o:selection_color(fl.FOREGROUND_COLOR)
o:labelsize(11)
o:align("wrap")

w:done()
w:show(argc, argv)
return fl.run()
