#!/usr/bin/env lua
-- MoonFLTK example: cursor.lua
--
-- Derived from the FLTK test/cursor.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

cursors = { -- list of available cursors
   "default", "arrow", "cross", "wait", "insert", "hand", "help", "move", "ns", 
   "we", "nwse", "nesw", "n", "ne", "e", "se", "s", "sw", "w", "nw", "none",
}

function choice_cb(menu, pathname, v)
   slider1:value(v)
   if cursors[v] then 
      fl.cursor(cursors[v])
   end
end

function add_choices(m)
-- adds the menu items to the choice menu:
   for i, c in ipairs(cursors) do
      m:add(c,0,choice_cb, i)
   end
end

function setcursor(slider)
   local v = slider:value()
   if cursors[v] then 
      fl.cursor(cursors[v])
      -- let's update the choice also (this was not done in the original example):
      choice:value(cursors[v]) 
   end
end

window = fl.double_window(400,300, arg[0])

choice = fl.choice(80,100,200,25,"Cursor:")
add_choices(choice)
choice:callback(choice_cb)
choice:when('release', 'not changed')
-- alt: choice:when(fl.WHEN_RELEASE + fl.WHEN_NOT_CHANGED)

slider1 = fl.hor_value_slider(80,180,310,30,"Cursor:")
cursor_slider = slider1
slider1:align('left')
slider1:step(1)
slider1:precision(0)
slider1:bounds(0,255)
slider1:value(1)
slider1:callback(setcursor)

window:resizable(window)
window:done()
window:show(argc,argv)
return fl.run()
