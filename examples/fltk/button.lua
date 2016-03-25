#!/usr/bin/env lua
-- MoonFLTK example: button.lua
--
-- Derived from the FLTK test/button.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function beepcb()
   fl.beep('default')
--  fflush(stdout)
end

function exitcb() 
  os.exit(true, true) -- Note: we need to close the Lua state for proper cleanup
end

window = fl.window(320,65, arg[0])
b1 = fl.button(20, 20, 80, 25, "&Beep")
b1:callback(beepcb,0)
fl.button(120,20, 80, 25, "&no op") -- = b2
b3 = fl.button(220,20, 80, 25, "E&xit")
b3:callback(exitcb,0)
window:done()
window:show(arg[0],arg)
return fl.run()
