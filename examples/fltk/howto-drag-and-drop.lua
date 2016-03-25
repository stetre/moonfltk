#!/usr/bin/env lua
-- MoonFLTK example: howto-drag-and-drop.lua
--
-- Derived from the FLTK examples/how-to-drag-and-drop.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

-- SIMPLE SENDER CLASS
function Sender(x,y,w,h) 
   local self = fl.box_sub(x,y,w,h)
   self:box('flat box')
   self:color(9)
   self:label("Drag\nfrom\nhere..")
   -- Sender event handler
   self:override_handle(function(_, event)
      assert(_ == self)
      local ret = self:super_handle(event)
--    print("Sender", event, ret)
      if event == 'push' then  -- do 'copy/dnd' when someone clicks on box
         fl.copy("It works!")
         fl.dnd()
         return true
      end
      return ret     
   end)
   return self
end

-- SIMPLE RECEIVER CLASS
function Receiver(x,y,w,h) 
   local self = fl.box_sub(x,y,w,h)
   self:box('flat box')
   self:color(10)
   self:label("..to\nhere")
   -- Receiver event handler
   self:override_handle(function(_, event)
      assert(_ == self)
      local ret = self:super_handle(event)
--    print("Receiver", event, ret)
      if event == 'dnd enter' or event == 'dnd drag' 
            or event == 'dnd release' then
         return true  -- return true for these events to 'accept' dnd
      elseif event == 'paste' then -- handle actual drop (paste) operation
         local text = fl.event_text() 
         self:label(text)
         print("Pasted '" .. text .."'")
         return true
      end
      return ret
   end)
   return self
end
         
-- main -------------------------------
-- Create sender window and widget
win_a = fl.window(0,0,200,100,"Sender")
a = Sender(0,0,100,100)
win_a:done()
win_a:show()
-- Create receiver window and widget
win_b = fl.window(400,0,200,100,"Receiver")
b = Receiver(100,0,100,100)
win_b:done()
win_b:show()
return fl.run()

