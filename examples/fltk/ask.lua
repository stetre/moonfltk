#!/usr/bin/env lua
-- MoonFLTK example: ask.lua
--
-- Derived from the FLTK test/ask.cxx example (http://www.fltk.org)
--



fl = require("moonfltk")

function update_input_text(wid, text)
   if text ~= nil then
      wid:label(text) -- implemented with copy_label()
      wid:redraw()
   end
end

function rename_me(wid)
   local text = fl.input("Input:", wid:label())
   update_input_text(wid, text)
end

function rename_me_pwd(wid)
   local text = fl.password("Password:", wid:label())
   update_input_text(wid, text)
end

function window_callback()
   local hs = fl.message_hotspot()
   fl.message_hotspot(false)
   fl.message_title("note: no hotspot set for this dialog")
   local rep = fl.choice("Are you sure you want to quit?", "Cancel", "Quit", "Dunno")
   fl.message_hotspot(hs)
   if rep == 1 then
      os.exit(true, true) -- note: we need to close the Lua state for proper cleanup
   elseif rep == 2 then
      fl.message("Well, maybe you should know before we quit.")
   end
end

-- main ---------------------------------------------------------------
win = fl.double_window(200, 105)
b1 = fl.return_button(20, 10, 160, 35, "Test text")
b2 = fl.button(20, 50, 160, 35, "MyPassword")
b1:callback(rename_me)
b2:callback(rename_me_pwd)


win:done()
win:resizable(win)
win:show(arg[0], arg)

win:callback(window_callback)

return fl.run()
