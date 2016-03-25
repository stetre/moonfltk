#!/usr/bin/env lua
-- MoonFLTK example: output.lua
--
-- Derived from the FLTK test/output.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

fl.trace_objects(true)

function font_cb()
  text:textfont(fonts:value())
  text:redraw()
  text2:textfont(fonts:value())
  text2:redraw()
end

function size_cb()
  text:textsize(sizes:value())
  text:redraw()
  text2:textsize(sizes:value())
  text2:redraw()
end

function input_cb()
  text:value(input:value())
  text2:value(input:value())
end

-- main ---------------------------
window = fl.double_window(400,400, arg[0])

input = fl.input(50,375,350,25)
input:value([[
Come, sweet slumber
Enshroud me in thy purple cloak
Hmm, doesn't even rhyme]])
input:when(fl.WHEN_CHANGED)
input:callback(input_cb)

sizes = fl.hor_value_slider(50,350,350,25,"Size")
sizes:align(fl.ALIGN_LEFT)
sizes:bounds(1,64)
sizes:step(1)
sizes:value(14)
sizes:callback(size_cb)

fonts = fl.hor_value_slider(50,325,350,25,"Font")
fonts:align(fl.ALIGN_LEFT)
fonts:bounds(0,15)
fonts:step(1)
fonts:value(0)
fonts:callback(font_cb)

text2 = fl.multiline_output(100,150,200,100,"fl.multiline_output")
text2:value(input:value())
text2:align(fl.ALIGN_BOTTOM)
text2:tooltip("This is an fl.multiline_output widget.")
window:resizable(text2)

text = fl.output(100,90,200,30,"fl.output")
text:value(input:value())
text:align(fl.ALIGN_BOTTOM)
text:tooltip("This is an fl.output widget.")

window:done()
window:show(arg[0],arg)
return fl.run()
