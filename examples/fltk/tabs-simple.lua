#!/usr/bin/env lua
-- MoonFLTK example: tabs-simple.lua
--
-- Derived from the FLTK examples/table-simple.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

--
-- Simple tabs example
--      _____  _____
--   __/ Aaa \/ Bbb \______________________
--  |    _______                           |
--  |   |_______|                          |
--  |    _______                           |
--  |   |_______|                          |
--  |    _______                           |
--  |   |_______|                          |
--  |______________________________________|
--
fl.scheme("gtk+")
win = fl.window(500,200,"Tabs Example")

-- Create the tab widget
tabs = fl.tabs(10,10,500-20,200-20)

-- ADD THE "Aaa" TAB
--   We do this by adding a child group to the tab widget.
--   The child group's label defined the label of the tab.
--
aaa = fl.group(10,35,500-20,200-45,"Aaa")

-- Put some different buttons into the group, which will be shown
-- when the tab is selected.
b1 = fl.button(50, 60,90,25,"Button A1") b1:color(88+1)
b2 = fl.button(50, 90,90,25,"Button A2") b2:color(88+2)
b3 = fl.button(50,120,90,25,"Button A3") b3:color(88+3)

aaa:done()

-- ADD THE "Bbb" TAB
--   Same details as above.
bbb = fl.group(10,35,500-10,200-35,"Bbb")

-- Put some different buttons into the group, which will be shown
-- when the tab is selected.
b1 = fl.button( 50,60,90,25,"Button B1") b1:color(88+1)
b2 = fl.button(150,60,90,25,"Button B2") b2:color(88+3)
b3 = fl.button(250,60,90,25,"Button B3") b3:color(88+5)
b4 = fl.button( 50,90,90,25,"Button B4") b4:color(88+2)
b5 = fl.button(150,90,90,25,"Button B5") b5:color(88+4)
b6 = fl.button(250,90,90,25,"Button B6") b6:color(88+6)

bbb:done()

tabs:done()

win:done()
win:show(arg[0], arg)
return fl.run()

