#!/usr/bin/env lua
-- MoonFLTK example: pitchbend.lua
--
-- This example implements a slider that can be used as pitch bend 
-- wheel in a MIDI controller.
-- When the slider is released, it returns to the central position.

fl = require("moonfltk")

function PitchBend(X,Y,W,H,L)
   local self = fl.slider_sub(X,Y,W,H,L)
   local min, max = -8192, 8191 -- range in MIDI 'pitch wheel' message
   local step = 1 -- steps in MIDI 'pitch wheel' message
   self:type("vertical nice")
   self:range(max, min) -- reversed, so we have the max on top
   self:step(step)
   self:color(10)
   self:selection_color(1)

   self:override_handle(function(_, ev)
      if ev == 'release' then
         if self:value() ~= 0 then
            self:value(0)
            self:do_callback()
         end
         return true
      end
      return self:super_handle(ev)
   end)
   return self
end


win = fl.window(200,200,250, 190, arg[0])

pitchbend = PitchBend(100, 10, 50, 170)
pitchbend:callback(function () 
   -- generate MIDI pitch wheel message here
   print("pitch bend value = " .. pitchbend:value()) 
end)

win:done()
win:resizable(win)
win:show(arg[0],arg)

return fl.run()

