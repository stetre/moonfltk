#!/usr/bin/env lua
-- MoonFLTK example: symbols.lua
--
-- Derived from the FLTK test/symbols.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

N = 0
W = 70
H = 70
ROWS = 6
COLS = 6

function slider_cb() 
   local val = math.floor(orientation:value())
   local sze = math.floor(size:value())
   for i = window:children(), 1, -1 do -- all window children
      local wc = window:child(i)
      local l = wc:argument()
      if l and l:sub(1,1) == '@' then -- all children with '@'
         l = l:sub(2)
         -- ascii legend or box with symbol?
         local buf = wc:box() == 'no box' and "@@" or "@"
         if sze ~= 0 then buf = buf .. string.format("%+d", sze) end
         if val ~= 0 then buf = buf .. string.format("%d", val) end
         buf = buf .. l
         wc:label(buf)
      end
   end
   window:redraw()
end

function bt(name)
   local x = N%COLS
   local y = math.floor(N/COLS)
   N = N + 1
   x = x*W+10
   y = y*H+10
   local buf = "@" .. name
   local a = fl.box(x,y,W-20,H-20)
   a:box('no box')
   a:label(buf)
   a:align(fl.ALIGN_BOTTOM)
   a:labelsize(11)
   a:argument(name)
   b = fl.box(x,y,W-20,H-20)
   b:box('up box')
   b:label(name)
   b:labelcolor(fl.DARK3)
   b:argument(name)
end

-- main ----------------------------------------
window = fl.double_window(COLS*W,ROWS*H+60, arg[0])
bt("@->")
bt("@>")
bt("@>>")
bt("@>|")
bt("@>[]")
bt("@|>")
bt("@<-")
bt("@<")
bt("@<<")
bt("@|<")
bt("@[]<")
bt("@<|")
bt("@<->")
bt("@-->")
bt("@+")
bt("@->|")
bt("@||")
bt("@arrow")
bt("@returnarrow")
bt("@square")
bt("@circle")
bt("@line")
bt("@menu")
bt("@UpArrow")
bt("@DnArrow")
bt("@search")
bt("@FLTK")
bt("@filenew")
bt("@fileopen")
bt("@filesave")
bt("@filesaveas")
bt("@fileprint")
bt("@refresh")
bt("@reload")
bt("@undo")
bt("@redo")

orientation = fl.value_slider(
    math.floor(window:w()*.05+.5), window:h()-40,
    math.floor(window:w()*.42+.5), 16, "Orientation")
orientation:type('horizontal')
orientation:range(0.0, 9.0)
orientation:value(0.0)
orientation:step(1)
orientation:callback(slider_cb)

size = fl.value_slider(
    math.floor(window:w()*.53+.5), window:h()-40,
    math.floor(window:w()*.42+.5), 16, "Size")
size:type('horizontal')
size:range(-3.0, 9.0)
size:value(0.0)
size:step(1)
size:callback(slider_cb)

window:resizable(window)
window:show(arg[0],arg)
return fl.run()

