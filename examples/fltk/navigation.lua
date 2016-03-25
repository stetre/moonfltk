#!/usr/bin/env lua
-- MoonFLTK example: navigation.lua
--
-- Derived from the FLTK test/navigation.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

-- 
-- Navigation test program for the Fast Light Tool Kit (FLTK).
--
-- Silly test of navigation keys. This is not a recommended method of
-- laying out your panels!
--

WIDTH = 600
HEIGHT = 300
GRID = 25

function ToggleArrowFocus_CB(b)
  fl.option('arrow focus', b:value())
end

-- main --------------------------------------------------
if arg[1] then math.randomseed(tonumber(arg[1])) end

window = fl.window(WIDTH,HEIGHT+40,arg[0])

-- Include a toggle button to control arrow focus
arrowfocus_butt = fl.light_button(10,HEIGHT+10,130,20," Arrow Focus")
arrowfocus_butt:callback(ToggleArrowFocus_CB)
arrowfocus_butt:value(fl.option('arrow focus')) -- use default
arrowfocus_butt:tooltip("Control horizontal arrow key focus navigation behavior.\n" ..
                            "e.g. 'arrow focus'")
window:done() -- don't auto-add children

function rand() return math.random(0, math.maxinteger) end

for i = 1, 10000 do
   -- make up a random size of widget:
   local x = rand()%(WIDTH/GRID+1) * GRID
   local y = rand()%(HEIGHT/GRID+1) * GRID
   local w = rand()%(WIDTH/GRID+1) * GRID
   if w < x then w = x-w x=x-w else w = w-x end
   local h = rand()%(HEIGHT/GRID+1) * GRID
   if h < y then h = y-h y = y-h else h = h-y end
   if w < GRID or h < GRID or w < h then goto continue end
   -- find where to insert it and see if it intersects something:
   local j = nil
   local intersection = false
   for n=1,window:children() do
      local o = window:child(n)
      if x < (o:x()+o:w()) and (x+w) > o:x() and 
         y< (o:y()+o:h()) and (y+h)>o:y() then intersection = true break end
      if not j and ( y < o:y() or y==o:y() and x < o:x() ) then j = o end
   end
   -- skip if intersection:
   if intersection then goto continue end
   window:insert(fl.input(x,y,w,h),j)
   ::continue::
end

window:show(argc, argv)
return fl.run()

