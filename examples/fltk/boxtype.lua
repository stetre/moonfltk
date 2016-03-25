#!/usr/bin/env lua
-- MoonFLTK example: boxtype.lua
--
-- Derived from the FLTK test/boxtype.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

N = 0
W = 200
H = 50
ROWS = 14

function bt(name, square)
   local square = square or false
   local x = math.floor(N%4)
   local y = math.floor(N/4)
   N = N + 1
   x = x*W+10
   y = y*H+10
   b = fl.box(name, x, y, square and H-20 or W-20, H-20, name)
   b:labelsize(11)
  if square then b:align("right") end
end

-- main ------------------------------------------------
window = fl.double_window(4*W,ROWS*H, arg[0])
window:box("flat box")
--fl.args(arg[0], arg) @@
fl.get_system_colors()
window:color(12) -- light blue
bt("no box")
bt("flat box")
N = N + 2 -- go to start of next row to line up boxes & frames
bt("up box")
bt("down box")
bt("up frame")
bt("down frame")
bt("thin up box")
bt("thin down box")
bt("thin up frame")
bt("thin down frame")
bt("engraved box")
bt("embossed box")
bt("engraved frame")
bt("embossed frame")
bt("border box")
bt("shadow box")
bt("border frame")
bt("shadow frame")
bt("rounded box")
bt("rshadow box")
bt("rounded frame")
bt("rflat box")
bt("oval box")
bt("oshadow box")
bt("oval frame")
bt("oflat box")
bt("round up box")
bt("round down box")
bt("diamond up box")
bt("diamond down box")
bt("plastic up box")
bt("plastic down box")
bt("plastic up frame")
bt("plastic down frame")
bt("plastic thin up box")
bt("plastic thin down box")
N = N + 2
bt("plastic round up box")
bt("plastic round down box")
N = N + 2
bt("gtk up box")
bt("gtk down box")
bt("gtk up frame")
bt("gtk down frame")
bt("gtk thin up box")
bt("gtk thin down box")
bt("gtk thin up frame")
bt("gtk thin down frame")
bt("gtk round up box")
bt("gtk round down box")
window:resizable(window)
window:done()
window:show()
return fl.run()
