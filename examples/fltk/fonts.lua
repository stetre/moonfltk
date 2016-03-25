#!/usr/bin/env lua
-- MoonFLTK example: fonts.lua
--
-- Derived from the FLTK test/fonts.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function FontDisplay (B, X, Y, W, H, L)
   local t = {}
   t.widget = fl.widget_sub(X, Y, W, H, L)
   t.font = 0
   t.size = 14
   t.widget:box(B)
   t.widget:override_draw(function(wid) 
      assert(wid==t.widget)
      wid:draw_box()
      fl.font(t.font, t.size)
      fl.color(fl.BLACK)
      fl.draw(wid:label(), wid:x()+3, wid:y()+3, wid:w()-6, wid:h()-6, wid:align())
   end)
   return t
end

pickedsize = 14

function font_cb()
   local fn = fontobj:value()
   if fn == 0 then return end
   fn = fn - 1
   textobj.font = fn
   sizeobj:clear()
   local s = sizes[fn]
   local n = #s
   if n == 0 then -- no sizes
   elseif s[1] == 0 then -- many sizes
      for i = 1, 64 do
         sizeobj:add(tostring(i))
      end
      sizeobj:value(pickedsize)
  else -- some sizes
      local w = 0
      for i = 1, #s do
         if s[i] <= pickedsize then w = i end
         sizeobj:add(buf)
      end
      sizeobj:value(w+1)
  end
  textobj.widget:redraw()
end

function size_cb()
  local i = sizeobj:value()
  if i==0 then return end
  local c = sizeobj:text(i)
  textobj.size = tonumber(c)
  textobj.widget:redraw()
end

function create_the_forms()
   -- create the sample string
   local n = 0
   local label = "Hello world!\n"
   for c = 33,126 do
      n = n + 1
      if (n & 0x1f) == 0 then label = label ..'\n' end
      if c == 64 then
         label = label .. string.format("%c%c", c, c) -- '@'
      else
         label = label .. string.format("%c", c)
      end
   end
   label = label .. "\n"
   n = 0
   for c = 0xA1, 0x600-1, 9 do
      n = n + 1
      if (n & 0x1f) == 0 then label = label ..'\n' end
   label = label .. utf8.char(c) -- same as fl.utf8encode(c)
   end
   label = label .. '\0'

   -- create the basic layout
   form = fl.double_window(550,370, arg[0])
   tile = fl.tile(0, 0, 550, 370)
   local textgroup = fl.group(0, 0, 550, 185)
   textgroup:box("flat box")

   textobj = FontDisplay("engraved box",10,10,530,170,label)
   textobj.widget:align("top", "left", "inside", "clip")
   textobj.widget:color(9,47)
   textgroup:resizable(textobj.widget)
   textgroup:done()

   local fontgroup = fl.group(0, 185, 550, 185)
   fontgroup:box("flat box")
   
   fontobj = fl.hold_browser(10, 190, 390, 170)
   fontobj:box("engraved box")
   fontobj:color(53,3)
   fontobj:callback(font_cb)
  
   sizeobj = fl.hold_browser(410, 190, 130, 170)
   sizeobj:box("engraved box")
   sizeobj:color(53,3)
   sizeobj:callback(size_cb)

   fontgroup:resizable(fontobj)
   fontgroup:done()
   tile:done()

   form:resizable(tile)
   form:done()
end

-- main ----------------------------------------------------

fl.scheme()
--fl.args(argc, argv)
fl.get_system_colors()
create_the_forms()

-- For the Unicode test, get all fonts...
nfonts = fl.set_fonts("-*")
sizes = {}

for i = 0, nfonts-1 do
   local name, attr = fl.get_font_name(i)
   name = ((attr & fl.BOLD) == 0 and "" or "@b") ..
          ((attr & fl.ITALIC) == 0 and "" or "@i") ..
          "@." ..  -- suspend subsequent formatting (some MS fonts have '@' in their name)
          name
         
   fontobj:add(name)
   sizes[i] = { fl.get_font_sizes(i) }
end

fontobj:value(1)
font_cb(fontobj,0)
form:show(argc,argv)
return fl.run()
