#!/usr/bin/env lua
-- MoonFLTK example: resizebox.lua
--
-- Derived from the FLTK test/resizebox.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

big = false
W1 = 40 --big and 60 or 40
B = 0
W3 = 5*W1 + 6*B

function b_cb(_, w) 
   if (window:w() ~= W3 or window:h() ~= W3) then
      fl.message("Put window back to minimum size before changing")
      return
   end
   window:init_sizes()
   if w == 0 then
      box:hide() window:box('flat box') window:resizable(nil) return
   elseif w == 8 then
      box:resize(W1+B,W1,2*W1,B)
   elseif w == 2 then
      box:resize(W1+B,W1+B+2*W1,2*W1,B)
   elseif w == 4 then
      box:resize(W1+B,W1,B,2*W1)
   elseif w == 6 then
      box:resize(W1+B+2*W1,W1+B,B,2*W1)
   end
   window:box('no box')
   if w == 6 or w == 4 then
      box:label("re\nsiz\nab\nle")
   else 
      box:label("resizable")
   end
   box:show()
   window:resizable(box)
   window:redraw()
end


window = fl.double_window(W3,W3)
window:box('no box')
for x = 0,3 do
   for y = 0, 3 do
      if not ((x==1 or x==2) and (y==1 or y==2)) then
          n = fl.box('engraved box',x*(B+W1)+B,y*(B+W1)+B,W1,W1)
         n:color(x+y+8)
      end
   end
end

n = fl.box('engraved box',B,4*W1+5*B,4*W1+3*B,W1)
n:color(12)
n = fl.box('engraved box',4*W1+5*B,B,W1,5*W1+4*B)
n:color(13)
n = fl.box('engraved box',W1+B+B,W1+B+B,2*W1+B,2*W1+B)
n:color(8)

b = fl.radio_button(W1+B+50,W1+B+30,20,20,"@6>")
b:callback(b_cb,6)
fl.radio_button(W1+B+30,W1+B+10,20,20,"@8>"):callback(b_cb,8)
fl.radio_button(W1+B+10,W1+B+30,20,20,"@4>"):callback(b_cb,4)
fl.radio_button(W1+B+30,W1+B+50,20,20,"@2>"):callback(b_cb,2)
fl.radio_button(W1+B+30,W1+B+30,20,20,"off"):callback(b_cb,0)

box = fl.box('flat box',0,0,0,0,"resizable")
box:color(fl.DARK2)
b:set()
b:do_callback()
window:done()

window:size_range(W3,W3)
window:show(arg[0],arg)
return fl.run()
