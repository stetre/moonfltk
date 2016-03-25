#!/usr/bin/env lua
-- MoonFLTK example: input_choice.lua
--
-- Derived from the FLTK test/input_choice.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

flag = true

function buttcb(_, ic)
   flag = not flag
   if flag then ic:activate() else ic:deactivate() end
   if ic:changed() then
      print("Callback: changed() is set\n")
      ic:clear_changed()
   end
end

function input_choice_cb(_, ic)
    print("Value = '" .. ic:value() .."'")
end

-- main ---------------------
win = fl.double_window(300, 200, arg[0])

ic = fl.input_choice(40,40,100,28,"Test")
ic:callback(input_choice_cb, ic)
ic:add("one")
ic:add("two")
ic:add("three")
ic:value(1) -- alt. ic:value("two")

onoff = fl.button(40,150,200,28,"Activate/Deactivate")
onoff:callback(buttcb, ic)

win:done()
win:resizable(win)
win:show(arg[0], arg)
return fl.run()

