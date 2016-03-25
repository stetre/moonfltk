#!/usr/bin/env lua
-- MoonFLTK example: pixmap-simple.lua

fl = require("moonfltk")

xpm = {
    "11 11 2 1",
    ".  c None",
    "@  c #000000",
    "...@.......",
    "...@@......",
    "...@@@.....",
    "...@@@@....",
    "...@@@@@...",
    "...@@@@@@..",
    "...@@@@@...",
    "...@@@@....",
    "...@@@.....",
    "...@@......",
    "...@......."
    }

im = fl.pixmap(xpm)

w = fl.double_window(200, 200, arg[0])
b = fl.toggle_button(80, 80, 40, 40)
b:image(im)
w:resizable(w)
w:done()
w:show(arg[0], arg)
return fl.run()

