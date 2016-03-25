#!/usr/bin/env lua
-- MoonFLTK example: subclass.lua

-- This example shows how to sublass a widget (in this case a window).

fl = require("moonfltk")

-- Constructor function:
function MyWindow(w, h, label)
   -- Create a 'window sub' object:
   local win = fl.window_sub(w, h, label)
   print("created window", win)
   -- Override the basic window methods:
   win:override_draw(mydraw)
   win:override_handle(myhandle)
   win:override_hide(myhide)
   win:override_resize(myresize)
   return win
end

function mydraw(win)
   assert(win == mywin)
   print("draw()") 
   win:super_draw() -- this executes window:draw()
end

function myhandle(win, event)
   assert(win == mywin)
   print("handle('" .. event .. "')")
   -- this must return true or false (see Fl_Widget::handle())
   return win:super_handle(event) 
end

function myhide(win) 
   assert(win == mywin)
   print("hide()") 
   win:super_hide()
end

function myresize(win, x, y, w, h) 
   assert(win == win)
   print(string.format("resize(%d, %d, %d, %d)", x, y, w, h))
   win:super_resize(x, y, w, h) 
end

mywin = MyWindow(340, 180, arg[0])
mywin:resizable(mywin) -- so that we can try myresize()
b = fl.box(10, 10, 320, 160)
b:label("If you provoke some events, by moving or resizing the window, " ..
"moving the mouse or clicking its buttons, pressing keys, and so on, " ..
"you should see the events traced on stdout.")
b:align('wrap')
mywin:done()
mywin:show()
return fl.run()

