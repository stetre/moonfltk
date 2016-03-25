#!/usr/bin/env lua
-- MoonFLTK example: tile.lua
--
-- Derived from the FLTK test/tile.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

window = fl.double_window(300,300, arg[0])
window:box('no box')
window:resizable(window)

dx = 20
dy = dx -- border width of resizable() - see below
tile = fl.tile(0,0,300,300)

box0 = fl.box(0,0,150,150,"0")
box0:box('down box')
box0:color(9)
box0:labelsize(36)
box0:align('clip')

w1 = fl.double_window(150,0,150,150,"1")
w1:box('no box')
box1 = fl.box(0,0,150,150,"1\nThis is a child window")
box1:box('down box')
box1:color(19)
box1:labelsize(18)
box1:align('clip', 'inside', 'wrap')
w1:resizable(box1)
w1:done()

box2a = fl.box(0,150,70,150,"2a")
box2a:box('down box')
box2a:color(12)
box2a:labelsize(36)
box2a:align('clip')

box2b = fl.box(70,150,80,150,"2b")
box2b:box('down box')
box2b:color(13)
box2b:labelsize(36)
box2b:align('clip')

box3a = fl.box(150,150,150,70,"3a")
box3a:box('down box')
box3a:color(12)
box3a:labelsize(36)
box3a:align('clip')

box3b = fl.box(150,150+70,150,80,"3b")
box3b:box('down box')
box3b:color(13)
box3b:labelsize(36)
box3b:align('clip')

-- create the symmetrical resize box with dx and dy pixels distance, resp.
-- from the borders of the fl.tile widget
r = fl.box(tile:x()+dx,tile:y()+dy,tile:w()-2*dx,tile:h()-2*dy)
tile:resizable(r)

tile:done()
window:done()

--w1:show()
window:show(arg[0],arg)
return fl.run()
