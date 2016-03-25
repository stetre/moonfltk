#!/usr/bin/env lua
-- MoonFLTK example: wizard-simple.lua
--
-- Derived from the FLTK examples/wizard-simple.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function back_cb() G_wiz:prev() end
function next_cb() G_wiz:next() end

function done_cb() os.exit(true, true) end

G_win = fl.window(400,300,"Example Wizard")
G_wiz = fl.wizard(0,0,400,300)

-- Wizard: page 1
g = fl.group(0,0,400,300)
bnext = fl.button(290,265,100,25,"Next @->") 
bnext:callback(next_cb)
out = fl.multiline_output(10,30,400-20,300-80,"Welcome")
out:labelsize(20)
out:align(fl.ALIGN_TOP|fl.ALIGN_LEFT)
out:value("This is First page")
g:done()

-- Wizard: page 2
g = fl.group(0,0,400,300)
bnext = fl.button(290,265,100,25,"Next @->") 
bnext:callback(next_cb)
back = fl.button(180,265,100,25,"@<- Back") 
back:callback(back_cb)
out = fl.multiline_output(10,30,400-20,300-80,"Terms And Conditions")
out:labelsize(20)
out:align(fl.ALIGN_TOP|fl.ALIGN_LEFT)
out:value("This is the Second page")
g:done()

-- Wizard: page 3
g = fl.group(0,0,400,300)
done = fl.button(290,265,100,25,"Finish") done:callback(done_cb)
back = fl.button(180,265,100,25,"@<- Back") 
back:callback(back_cb)
out = fl.multiline_output(10,30,400-20,300-80,"Finish")
out:labelsize(20)
out:align(fl.ALIGN_TOP|fl.ALIGN_LEFT)
out:value("This is the Last page")
g:done()

G_wiz:done()
G_win:done()
G_win:show(arg[0], arg)
return fl.run()

