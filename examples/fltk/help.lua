#!/usr/bin/env lua
-- MoonFLTK example: help.lua
--
-- Derived from the FLTK test/help.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

fl.trace_objects(true)

help = fl.help_dialog()

if arg[1] then 
   help:load(arg[1])
else
   help:load("help/help-test.html")
end
  
help:show()
return fl.run()
