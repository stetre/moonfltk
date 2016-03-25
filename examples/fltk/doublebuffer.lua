#!/usr/bin/env lua
-- MoonFLTK example: doublebuffer.lua
--
-- Derived from the FLTK test/doublebuffer.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

PI = math.pi
sin = math.sin
cos = math.cos

-- this purposely draws each line 10 times to be slow:
function star(w, h, n)
   fl.push_matrix()
   fl.translate(w/2, h/2)
   fl.scale(w/2, h/2)
   for i = 0, n-1 do
      for j = i+1, n-1 do
         fl.begin_line()
         fl.vertex(cos(2*PI*i/n+.1), sin(2*PI*i/n+.1))
         fl.vertex(cos(2*PI*j/n+.1), sin(2*PI*j/n+.1))
         fl.end_line()
      end
   end
   fl.pop_matrix()
end

sides = {20,20}

function slider_cb(s, v)
   sides[v] = s:value()
   s:parent():redraw()
end

function bad_draw(w, h, which)
--   for (int i=0 i<10 i++) {
--     fl.color(7) fl.rectf(0,0,w,h) fl.color(0) star(w,h)
--     fl.color(0) fl.rectf(0,0,w,h) fl.color(7) star(w,h)
--   }
  fl.color(fl.BLACK) fl.rectf(0,0,w,h)
  fl.color(fl.WHITE) star(w,h,sides[which])
  --  for (int x=0 x<sides[which] x++) for (int y=0 y<sides[which] y++)
  --fl.draw_box(FL_UP_BOX, 10*x, 10*y, 25,25, FL_GRAY)
end

function single_blink_window(x,y,w,h,l)
   local self = fl.single_window_sub(x,y,w,h,l)
   self:resizable(self)
   self:override_draw(function()
      bad_draw(self:w(), self:h(),1)
      self:draw_child(self:child(1))
   end)
   return self
end

function double_blink_window(x,y,w,h,l)
   local self = fl.double_window_sub(x,y,w,h,l)
   self:resizable(self)
   self:override_draw(function()
      bad_draw(self:w(), self:h(),2)
      self:draw_child(self:child(1))
   end)
   return self
end

-- main ----------------------------------

if not fl.visual('double') then
   print("Xdbe not supported, faking double buffer with pixmaps.")
end

w01 = fl.window(420,420,"fl.single_window") w01:box('flat box')
w1 = single_blink_window(10,10,400,400,"fl.single_window")
w1:box('flat box') w1:color(fl.BLACK) --w1:position(100,200)
slider0 =fl.hor_slider(20,370,360,25)
slider0:range(2,30)
slider0:step(1)
slider0:value(sides[1])
slider0:callback(slider_cb, 1)
w1:done()
w01:done()
w02 = fl.window(420,420,"fl.double_window") w02:box('flat box')
w2 = double_blink_window(10,10,400,400,"fl.double_window")
w2:box('flat box') w2:color(fl.BLACK) --w2:position(600,200)
slider1 = fl.hor_slider(20,370,360,25)
slider1:range(2,30)
slider1:step(1)
slider1:value(sides[2])
slider1:callback(slider_cb, 2)
w2:done()
w02:done()
w01:show(argc, argv)
w1:show()
w02:show()
w2:show()
return fl.run()

