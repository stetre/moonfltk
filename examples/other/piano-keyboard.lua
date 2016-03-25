#!/usr/bin/env lua
-- MoonFLTK example: piano-keyboard.lua

fl = require("moonfltk")

-- This example implements an 88-keys piano keyboard by subclassing
-- the fl.box widget.
-- The keyboard can be 'played' by clicking the keys with the mouse,
-- and the pressed or released keys are printed on stdout.
--  
--   bw
--   |-|
--  __________________________  ___ ___      (x0,y) __ (x0+ww,y)
-- | | | | | || | | | || || | |  |   |             |  |
-- | |2| | |5||7| | | || || | |  bh  wh            |  |
-- | | | | | || | | | || || | |  |   |             |  | <- white key
-- | |_| | |_||_| | |_||_||_| | _v_  |             |  |
-- |1 | 3|4 |6 |8 |  |  |  |  |      |             |  |
-- |__|__|__|__|__|__|__|__|__|     _v_            |__|
--  ww                                     (x0,y+wh)   (x0+ww, y+wh)
-- |--|        
-- 
-- 

function PianoKeyboard(x, y, w, h) 
   -- Note: w should be a multiple of 52 (=no of white keys) in order to 
   -- fill the box completely with the keyboard
   local self = fl.box_sub(x, y, w, h)
   local info = { self = self }
   local n = 52 -- no. of white keys
   local ww = math.floor(w/n) -- white key width
   local bw = math.floor(ww * 2/3)
   local wh = h -- white key height
   local bh = math.floor(wh*.6) -- black key height
   local white = {}  -- white[i] = x position of the i-th white key 
   local black = {}  -- black[i] = x position of the i-th black key 
   local keynumber = {} -- key numbers (keynumber.white1=1, keynumber.black1=2, .. up to 88)
   local pressed = {} -- key status (pressed.white1 = true if pressed, ...)
   
   -- init keynumber table
   local k = 1
   for i = 1,n do
      keynumber["white"..i] = k
      k = k + 1
      if i%7 ~= 2 and i%7 ~= 5 then -- no black keys between B-C or E-F
         keynumber["black"..i] = k
         k = k + 1
      end
   end

   local last_x, last_y = -1, -1

   self:override_draw(function(self)
      self:super_draw()
      local x,y,w,h = self:xywh()

      if x ~= last_x or y ~= last_y then
         -- the window was resized, let's recompute the positions of the keys
         for i = 1, n do
            white[i] = x+(i-1)*ww
         end
         for i = 1, n - 1 do
            if i%7 ~= 2 and i%7 ~= 5 then -- no black keys between B-C or E-F
               black[i] = x + (i-1)*ww + ww/2 + (ww-bw)/2
            end
         end
      end
      last_x, last_y = x, y

      -- Draw the keyboard (white key first, then black over them).
      for i = 1, n do
         fl.color(pressed["white"..i] and fl.GRAY or fl.WHITE)
         local x0 = white[i]  
         -- draw the white keys slighltly thinner so to outline them:
         fl.polygon(x0+1,y,x0+(ww-1),y,x0+(ww-1),y+wh,x0+1,y+wh)
      end
      for i = 1, n-1 do
         if black[i] then 
            fl.color(pressed["black"..i] and fl.GRAY or fl.BLACK)
            local x0 = black[i]  
            fl.polygon(x0, y, x0+bw, y, x0+bw, y+bh, x0, y+bh)
         end
      end
   end)

   local function find_key(self)
   -- Find which key is pressed/released, by checking the position of the
   -- last event against the area of the keys. We check black keys first,
   -- because they overlap white keys and the intersections are actually part
   -- of the black keys (real white piano keyboard are not rectangular...).
   -- This check is not very efficient, but it works just fine for a simple
   -- example.
      local y = self:y()
      for i = 1, n-1 do
         local x0 = black[i] 
         if x0 then
            if fl.event_inside(x0, y, bw, bh) then return "black" .. i end
         end
      end
      for i = 1, n do
         local x0 = white[i] 
         if x0 then
            if fl.event_inside(x0, y, ww, wh) then return "white" .. i end
         end
      end
      return "???" 
   end

   self:override_handle(function(self, ev)
      if ev == 'push' or ev == 'release' then -- mouse click on the keyboard
         local k = find_key(self)
         print(string.format("%-7s key %d (%s)", ev, keynumber[k], k))
         if ev == "push" then pressed[k] = true else pressed[k] = false end   
         self:redraw()
         if info.callback then info.callback(ev, keynumber[k]) end
         return true
      elseif ev == 'keydown' or ev == 'keyup' then
         print(ev, fl.event_key()) 
         -- we could map these to piano keys (like jack-keyboard does) so to
         -- turn the PC keyboard into a piano keyboard ...
         return true
      else
         return self:super_handle(ev)
      end
   end)

   collectgarbage()
   return self, info
end -- PianoKeyboard


-- main ---------------------------------------------

ww, wh = 15, 100 -- white key width and height

win = fl.double_window(52*ww+20,wh+20, arg[0])
keyboard, info = PianoKeyboard(10, 10, 52*ww, wh)

info.callback = function(ev, k) 
   -- Generate MIDI 'note on/off' event here.
   -- Other parameters (velocity, etc) may be controlled with other widgets
   -- (dials, sliders, etc).
   -- Using LuaJACK we can then send the generated events to a MIDI port,
   -- thus implementing a MIDI controller which can be used with any JACK-aware
   -- MIDI application.
end

win:done()
win:resizable(win)
win:show()
return fl.run()

