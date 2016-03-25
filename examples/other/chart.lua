#!/usr/bin/env lua
-- MoonFLTK example: chart.lua
--
fl = require("moonfltk")

-- A simple example with the chart widget.

win = fl.window(300, 400, arg[0])
c = fl.chart(20, 20, 260, 360, "mychart")
c:type('line')

for i=1,10 do
   c:add(math.random(), tostring(i), fl.RED)
end

win:done()
win:show()
return fl:run()
