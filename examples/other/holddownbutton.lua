#!/usr/bin/env lua
-- MoonFLTK example: holddownbutton.lua
--
-- This example implements a button that is bound to a keyboard key.
-- When the key or the button is pushed, the value changes to 'true'.
-- When the key or the button is released, the value changes to 'false'. 
-- In both events, the callback is executed.
-- As long as the key/button is held down, no other event is generated.
--
-- Note: a timer is used to ensure that holding down the key for a long 
-- interval do not generate a sequence of keydown/keyup events.
-- In principle, the timer should not be necessary because in the FLTK design
-- intentions, holding down a key should not generate repeated keydown/keyup
-- events. However, this design goal is not always achieved in every platform
-- (see: https://groups.google.com/forum/#!topic/fltkgeneral/uqoA2qxUjbk )
-- The implementation in this example is robust with regards to such bad
-- platform behaviours.
--

fl = require("moonfltk")
timer = require("moonfltk.timer")

DEBUG=false
TRACE = DEBUG and print or function() end

function HoldDownButton(key, X,Y,W,H,L)
   local self = fl.button_sub(X,Y,W,H,L)
   local key = key
   local timeout = 0.05 -- seconds
   -- events we are interested in:
   local key_event = { keydown=true, keyup=true, shortcut=true }
   local button_event = { push=true, release=true }


   local T1 = timer.create(timeout, function() 
      if self:value() then -- need an UP
         self:value(false)
         TRACE("RELEASE")
         self:do_callback()
      end
   end)

   self:override_handle(function(_, ev)
      if not button_event[ev] and not (key_event[ev] and fl.event_key() == key) then
         return self:super_handle(ev)
      end

      if timer.isrunning(T1) then
         timer.start(T1)
         TRACE("IGNORE", ev, self:value())
         return true
      end

      if ev == 'keyup' or ev == 'release' then
         if not self:value() then -- already up: ignore
            return true 
         end
         TRACE("START", ev)
         timer.start(T1)
         return true
      end

      -- ev == 'keydown' or 'shortcut' or 'push':

      if self:value() then -- already down: ignore
         return true 
      end

      self:value(true)
      self:do_callback()
      TRACE("PRESS", ev)
      return true
   end)
   return self
end

timer.init()
win = fl.window(200,200,190, 70, arg[0])

function cb(button) 
   print("Button '"..button:label().. "' ".. (button:value() and "pressed" or "released")) 
end

b = HoldDownButton('q', 10, 10, 50, 50, 'Q')
b:color(fl.RED)
b:callback(cb)

b = HoldDownButton('w', 70, 10, 50, 50, 'W')
b:color(fl.GREEN)
b:callback(cb)

b = HoldDownButton('e', 130, 10, 50, 50, 'E')
b:color(fl.BLUE)
b:callback(cb)

win:done()
win:resizable(win)
win:show(arg[0],arg)

return fl.run()

