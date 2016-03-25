#!/usr/bin/env lua
-- MoonFLTK example: windowfocus.lua
--
-- Derived from the FLTK test/windowfocus.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function popup() 
   win2:position(win1:x() + win1:w(), win1:y())
   win2:show()
   win2:wait_for_expose()
   inp:take_focus()
end

win1 = fl.double_window(300, 200, arg[0])
win1:label("show() focus test")

b = fl.box(10, 10, 280, 130)
b:label("Type something to pop the subwindow up." .. 
" The focus should stay on the input, and you should be able to continue typing.")
b:align("wrap", "left", "inside")

inp = fl.input(10, 150, 150, 25)
inp:when("changed")
inp:callback(popup)

win1:done()

win2 = fl.double_window(300, 200)
win2:label("window2")
win2:done()

win1:show(arg[0],arg)

return fl.run()
