#!/usr/bin/env lua
-- MoonFLTK example: arc.lua
--
-- Derived from the FLTK test/arc.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

y = 300
name = { "X", "Y", "R", "start", "end", "rotate" }
args = { 140, 140, 50, 0, 360, 0 }

function drawing(x, y, w, h)
   local wid = fl.widget_sub(x, y, w, h)
   wid:override_draw(function (wid_)
      assert(wid_ == wid)
      --local x, y, w, h = wid:x(), wid:y(), wid:w(), wid:h()
      local x, y, w, h = wid:xywh()
      fl.push_clip(x, y, w, h)
      fl.color(fl.DARK3)
      fl.rectf(x, y, w, h)
      fl.push_matrix()
      if args[6] ~= 0 then
         fl.translate(x+w/2, y+h/2)
         fl.rotate(args[6])
         fl.translate(-(x+w/2), -(y+h/2))
      end
      fl.color(fl.WHITE)
      fl.translate(x, y)
      fl.begin_complex_polygon()
      fl.arc(args[1], args[2], args[3], args[4], args[5])
      fl.gap()
      fl.arc(140, 140, 20, 0, -360)
      fl.end_complex_polygon()
      fl.color(fl.RED)
      fl.begin_line()
      fl.arc(args[1], args[2], args[3], args[4], args[5])
      fl.end_line()
      fl.pop_matrix()
      fl.pop_clip()
   end)
   return wid
end 

function slider_cb(slider, n)
   args[n] = slider:value()
   drw:redraw()
end

-- MAIN ---------------------------------------------------

win = fl.double_window(300, 500, arg[0])
drw = drawing(10, 10, 280, 280)

for n=1,6 do
   local s = fl.hor_value_slider(50, y, 240, 25, name[n]) 
   y = y + 25
   if n < 4 then
      s:minimum(0) s:maximum(300)
   elseif n == 6 then
      s:minimum(0) s:maximum(360)
   else
      s:minimum(-360) s:maximum(360)
   end
   s:step(1)
   s:value(args[n])
   s:align('left')
   s:callback(slider_cb, n)
end

win:done()
win:show(arg[0], arg)
return fl.run()

