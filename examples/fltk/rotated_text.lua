#!/usr/bin/env lua
-- MoonFLTK example: rotated_text.lua
--
-- Derived from the FLTK test/rotated_text.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

PI = math.pi
sin = math.sin
cos = math.cos
floor = math.floor

function Rotated_Label_Box(X,Y,W,H,L) 
   local wid = fl.widget_sub(X,Y,W,H,L)
   local prop = { -- rotated text properties
            angle = 0, --int rt_angle
            text = input:value(), -- const char* rt_text
            align = 0, --  Fl_Align rt_align
         }
   -- Local functions that implement labeltypes on a rotated text
   local function innards(X, Y, data, n)
      for i = 1,n do
         fl.color(i < n and data[i][3] or wid:labelcolor())
         fl.draw(prop.angle,prop.text, X+data[i][1], Y+data[i][2])
      end
   end

   local function shadow_label(X, Y)
      --print("shadow_label", X, Y)
      local data = {{2,2,fl.DARK3},{0,0,0}}
      innards(X, Y, data, 2)
   end

   local function engraved_label(X, Y)
      --print("engraved_label", X, Y)
      local data = {
         {1,0,fl.LIGHT3},{1,1,fl.LIGHT3},{0,1,fl.LIGHT3},
         {-1,0,fl.DARK3},{-1,-1,fl.DARK3},{0,-1,fl.DARK3},{0,0,0}}
      innards(X, Y, data, 7)
   end

   local function embossed_label(X, Y)
      --print("embossed_label", X, Y)
      local data = {
         {-1,0,fl.LIGHT3},{-1,-1,fl.LIGHT3},{0,-1,fl.LIGHT3},
         {1,0,fl.DARK3},{1,1,fl.DARK3},{0,1,fl.DARK3},{0,0,0}}
      innards(X, Y, data, 7)
   end

   wid:override_draw(function(_)
      assert(_ == wid)
      wid:draw_box()
      --print(prop.angle, prop.align, prop.text)
      fl.font(wid:labelfont(), wid:labelsize())
      fl.color(wid:labelcolor())
      local x, y, w, h = wid:xywh()
      if (prop.align & fl.ALIGN_CLIP) ~= 0 then
         fl.push_clip(x,y,w,h)
      else 
         fl.push_no_clip()
      end
      local dx, dy = fl.measure(prop.text)
      
      --print(wid:labelfont(), wid:labelsize())
      --print(x,y,w,h, dx, dy)
      if (prop.align & fl.ALIGN_LEFT) ~= 0 then
         dx, dy = 0, 0
      elseif (prop.align & fl.ALIGN_RIGHT) ~= 0 then
         dy = floor(-sin(PI*(prop.angle+180)/180.)*dx)
         dx = floor(cos(PI*(prop.angle+180)/180.)*dx)
      else
         dy = floor((sin(PI*prop.angle/180.)*dx)/2)
         dx = floor((-cos(PI*prop.angle/180.)*dx)/2)
      end
      local lt = wid:labeltype()
      if lt == 'shadow' then
         shadow_label(x+w/2+dx,y+h/2+dy)
      elseif lt == 'engraved' then
         engraved_label(x+w/2+dx,y+h/2+dy)
      elseif lt == 'embossed' then
         embossed_label(x+w/2+dx,y+h/2+dy)
      else
         fl.draw(prop.angle,prop.text,x+w/2+dx,y+h/2+dy)
      end
      fl.pop_clip()
      wid:draw_label()
   end) -- override_draw()
   return wid, prop
end -- Rotated_Label_Box


function button_cb()
  local i = 0
  if leftb:value() then i = i | fl.ALIGN_LEFT end
  if rightb:value() then i = i | fl.ALIGN_RIGHT end
  if clipb:value() then i = i | fl.ALIGN_CLIP end
  text_prop.align= i
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

function angle_cb()
  text_prop.angle=angles:value()
  window:redraw()
end

function input_cb()
  text_prop.text=input:value()
  window:redraw()
end

function labeltype_cb(_menu, _choice, lt)
  text:labeltype(lt)
  window:redraw()
end

-- main ------------------------------------------------
window = fl.double_window(400,425, arg[0])

angles= fl.hor_value_slider(50,400,350,25,"Angle:")
angles:align(fl.ALIGN_LEFT)
angles:bounds(-360,360)
angles:step(1)
angles:value(0)
angles:callback(angle_cb)

input = fl.input(50,375,350,25)
input:value("Rotate Me!!!") -- there is no static_value() in MoonFLTK, use value() instead
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

g = fl.group(50,300,350,25)
leftb = fl.toggle_button(50,300,50,25,"left")
leftb:callback(button_cb)
rightb = fl.toggle_button(100,300,50,25,"right")
rightb:callback(button_cb)
clipb = fl.toggle_button(350,300,50,25,"clip")
clipb:callback(button_cb)
g:resizable(rightb)
g:done()

c = fl.choice(50,275,200,25)
c:add("normal label", 0, labeltype_cb, "normal")
c:add("shadow label", 0, labeltype_cb, "shadow")
c:add("engraved label", 0, labeltype_cb, "engraved")
c:add("embossed label", 0, labeltype_cb, "embossed")

text, text_prop = Rotated_Label_Box(100,75,200,100,"Widget with rotated text")
text:box('engraved box')
text:align(fl.ALIGN_BOTTOM)
window:resizable(text)
window:done()
window:show(arg[0],arg)
return fl.run()

