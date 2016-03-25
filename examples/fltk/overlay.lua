#!/usr/bin/env lua
-- MoonFLTK example: overlay.lua
--
-- Derived from the FLTK test/overlay.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

width=10
height=10

function overlay(w, h, label)
   -- create an overlay_window_sub object
   local ovl = fl.overlay_window_sub(w, h, label)
   -- override its draw_overlay() method:
   ovl:override_draw_overlay(function (ovl_)
      assert( ovl_ == ovl)
      fl.color(fl.RED)
      fl.rect((ovl:w() - width) / 2, (ovl:h() - height)/2, width, height)
   end)
   return ovl
end

function bcb1() width = width + 20, ovl:redraw_overlay() end
function bcb2() width = width - 20, ovl:redraw_overlay() end
function bcb3() height = height + 20, ovl:redraw_overlay() end
function bcb4() height = height - 20, ovl:redraw_overlay() end

ovl = overlay(400,400, arg[0])
b = fl.button(50,50,100,100,"wider\n(a)")
b:callback(bcb1) b:shortcut(fl.shortcut('a'))
b = fl.button(250,50,100,100,"narrower\n(b)")
b:callback(bcb2) b:shortcut(fl.shortcut('b'))
b = fl.button(50,250,100,100,"taller\n(c)")
b:callback(bcb3) b:shortcut(fl.shortcut('c'))
b = fl.button(250,250,100,100,"shorter\n(d)")
b:callback(bcb4) b:shortcut(fl.shortcut('d'))
ovl:resizable(ovl)
ovl:done()
ovl:show(arg[0],arg)
ovl:redraw_overlay()
return fl.run()

