#!/usr/bin/env lua
-- MoonFLTK example: piano-keyboard2.lua
--
-- An alternative implementation of a piano keyboard, using buttons.
--

fl = require("moonfltk")
timer = require("moonfltk.timer")


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
         self:do_callback()
      end
   end)

   self:override_handle(function(_, ev)
      if not button_event[ev] and not (key_event[ev] and fl.event_key() == key) then
         return self:super_handle(ev)
      end

      if timer.isrunning(T1) then
         timer.start(T1)
         return true
      end

      if ev == 'keyup' or ev == 'release' then
         if not self:value() then -- already up: ignore
            return true 
         end
         timer.start(T1)
         return true
      end

      -- ev == 'keydown' or 'shortcut' or 'push':
      if self:value() then -- already down: ignore
         return true 
      end
      self:value(true)
      self:do_callback()
      return true
   end)
   return self
end

-- Callback for piano keys:
function piano_cb(button, arg) 
   -- Generate MIDI 'note on/off' events here.
   print(arg .. " 'note " .. (button:value() and "on" or "off") .. "' event") 
   -- Redraw the window so that black keys remain over white ones: 
   win:redraw() 
end



-- KEY BINDINGS. 
-- You may want to change these to adapt them to your keyboard layout.
-- The spaces denote the missing black keys between E-F and B-C notes.
white_keys = "zxcvbnmqwertyu"
black_keys = "sd ghj 23 567"  

white_width = 25
white_height = 100
black_width = math.floor(white_width *2/3)
black_height = math.floor(white_height*3/5)


BORDER_X, BORDER_Y = 10, 10
WIN_X,WIN_Y = 200,200
WIN_W = white_width * #white_keys + 2*BORDER_X
WIN_H = white_height + 2*BORDER_Y

timer.init()
win = fl.window(WIN_X,WIN_Y,WIN_W,WIN_H, arg[0])

x, y = BORDER_X, BORDER_Y
for i=1,#white_keys do
   key = white_keys:sub(i,i)
   b = HoldDownButton(key,x + (i-1)*white_width,y,white_width,white_height)
   b:color(fl.WHITE)
   b:callback(piano_cb, "white"..i)
end

for i=1,#black_keys do
   key = black_keys:sub(i,i)
   if key ~= ' ' then
      b = HoldDownButton(key,x+i*white_width-black_width/2,y,black_width,black_height)
      b:color(fl.BLACK)
      b:callback(piano_cb, "black"..i)
   end
end

win:done()
win:resizable(win)
win:show(arg[0],arg)


while true do
   rv = fl.wait(0)
   if rv==nil or not win:shown() then os.exit() end
   if rv then -- an event occurred
      -- local ev = fl.event()
   end
end

