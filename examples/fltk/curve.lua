#!/usr/bin/env lua
-- MoonFLTK example: curve.lua
--
-- Derived from the FLTK test/curve.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

args = { 20,20, 50,200, 100,20, 200,200, 0 }
name = { "X0", "Y0", "X1", "Y1", "X2", "Y2", "X3", "Y3", "rotate"}

function Drawing(X, Y, W, H) 
   local self = fl.widget_sub(X,Y,W,H)

   self:override_draw(function( )
      local x, y, w, h = self:xywh()
      fl.push_clip(x,y,w,h)
      fl.color(fl.DARK3)
      fl.rectf(x,y,w,h)
      fl.push_matrix()
      if args[9] ~= 0 then
         fl.translate(x+w/2.0, y+h/2.0)
         fl.rotate(args[9])
         fl.translate(-(x+w/2.0), -(y+h/2.0))
      end
      fl.translate(x,y)
      if not points then
         fl.color(fl.WHITE)
         fl.begin_complex_polygon()
         fl.curve(args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8])
         fl.end_complex_polygon()
      end
      fl.color(fl.BLACK)
      fl.begin_line()
      fl.vertex(args[1],args[2])
      fl.vertex(args[3],args[4])
      fl.vertex(args[5],args[6])
      fl.vertex(args[7],args[8])
      fl.end_line()
      fl.color(points and fl.WHITE or fl.RED)
      if points then fl.begin_points() else fl.begin_line() end
      fl.curve(args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8])
      if points then fl.end_points() else fl.end_line() end
      fl.pop_matrix()
      fl.pop_clip()
   end)

   return self
end

function points_cb(b)
   points = b:value()
   drawing:redraw()
end

function slider_cb(s, n)
  args[n] = s:value()
  drawing:redraw()
end

-- main --------------------------------
window = fl.double_window(300,555, arg[0])
drawing = Drawing(10,10,280,280)

local y = 300
for n = 1,9 do
   s = fl.hor_value_slider(50,y,240,25,name[n]) y = y + 25
   s:minimum(0) s:maximum(280)
   if n == 9 then s:maximum(360) end
   s:step(1)
   s:value(args[n])
   s:align(fl.ALIGN_LEFT)
   s:callback(slider_cb, n)
end

but = fl.toggle_button(50,y,50,25,"points")
but:callback(points_cb)

window:done()
window:show(argc,argv)
return fl.run()

