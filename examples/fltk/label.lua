#!/usr/bin/env lua
-- MoonFLTK example: label.lua
--
-- Derived from the FLTK test/label.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function button_cb()
  local i = 0
  if leftb:value() then i = i | fl.ALIGN_LEFT end
  if rightb:value() then i = i | fl.ALIGN_RIGHT end
  if topb:value() then i = i | fl.ALIGN_TOP end
  if bottomb:value() then i = i | fl.ALIGN_BOTTOM end
  if insideb:value() then i = i | fl.ALIGN_INSIDE end
  if clipb:value() then i = i | fl.ALIGN_CLIP end
  if wrapb:value() then i = i | fl.ALIGN_WRAP end
  if imageovertextb:value() then i = i | fl.ALIGN_TEXT_OVER_IMAGE end
  if imagenexttotextb:value() then i = i | fl.ALIGN_IMAGE_NEXT_TO_TEXT end
  if imagebackdropb:value() then i = i | fl.ALIGN_IMAGE_BACKDROP end
  text:align(i)
  window:redraw()
end

function image_cb()
   if imageb:value() then
      text:image(img)
   else 
      text:image(nil)
   end
   window:redraw()
end

function font_cb()
  text:labelfont(fonts:value())
  window:redraw()
end

function size_cb()
  text:labelsize(sizes:value())
  window:redraw()
end

function input_cb()
  text:label(input:value())
  window:redraw()
end

function labeltype_cb(m_, pn_, labtype) 
   text:labeltype(labtype)
   window:redraw()
end

function symbol_cb()
   text:labeltype('normal') -- 'symbol'
   local v = input:value()
   if v and v:sub(1,1) ~= '@' then
      input:value("@->")
      text:label("@->")
   end
  window:redraw()
end

function add_choices(m)
   m:add("normal label", 0, labeltype_cb, 'normal')
   m:add("symbol label", 0 , symbol_cb)
   m:add("shadow label", 0, labeltype_cb, 'shadow') 
   m:add("engraved label", 0, labeltype_cb, 'engraved')
   m:add("embossed label", 0, labeltype_cb, 'embossed')
end

-- main
img = fl.xpm_image("pixmaps/blast.xpm")

window = fl.double_window(400,400, arg[0])

input = fl.input(50,375,350,25)
input:value("The quick brown fox jumped over the lazy dog.")
input:when(fl.WHEN_CHANGED)
input:callback(input_cb)

sizes= fl.hor_value_slider(50,350,350,25,"Size:")
sizes:align(fl.ALIGN_LEFT)
sizes:bounds(1,64)
sizes:step(1)
sizes:value(14)
sizes:callback(size_cb)

fonts=fl.hor_value_slider(50,325,350,25,"Font:")
fonts:align(fl.ALIGN_LEFT)
fonts:bounds(0,15)
fonts:step(1)
fonts:value(0)
fonts:callback(font_cb)

g = fl.group(50,275,350,50)
imageb = fl.toggle_button(50,275,50,25,"image")
imageb:callback(image_cb)
imageovertextb = fl.toggle_button(100,275,50,25,"I - T")
imageovertextb:callback(button_cb)
imagenexttotextb = fl.toggle_button(150,275,50,25,"I | T")
imagenexttotextb:callback(button_cb)
imagebackdropb = fl.toggle_button(200,275,50,25,"back")
imagebackdropb:callback(button_cb)
leftb = fl.toggle_button(50,300,50,25,"left")
leftb:callback(button_cb)
rightb = fl.toggle_button(100,300,50,25,"right")
rightb:callback(button_cb)
topb = fl.toggle_button(150,300,50,25,"top")
topb:callback(button_cb)
bottomb = fl.toggle_button(200,300,50,25,"bottom")
bottomb:callback(button_cb)
insideb = fl.toggle_button(250,300,50,25,"inside")
insideb:callback(button_cb)
wrapb = fl.toggle_button(300,300,50,25,"wrap")
wrapb:callback(button_cb)
clipb = fl.toggle_button(350,300,50,25,"clip")
clipb:callback(button_cb)
g:resizable(insideb)
g:done()

c = fl.choice(50,250,200,25)
add_choices(c)

text= fl.box('engraved box',100,75,200,100,input:value())
text:align(fl.ALIGN_CENTER)
window:resizable(text)
window:done()
window:show(arg[0],arg)
return fl.run()

