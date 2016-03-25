#!/usr/bin/env lua
-- MoonFLTK example: scroll.lua
--
-- Derived from the FLTK test/scroll.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

cos = math.cos
sin = math.sin
floor = math.floor
PI = math.pi

function Drawing(X,Y,W,H,L)
   local wid = fl.widget_sub(X,Y,W,H,L)
   wid:align('top')
   wid:box('flat box')
   wid:color(fl.WHITE)

   wid:override_draw(function(_)
      assert( _ == wid)
      wid:draw_box()
      local x, y, w, h = wid:xywh()
      fl.push_matrix()
      fl.translate(floor(x+w/2), floor(y+h/2))
      fl.scale(floor(w/2), floor(h/2))
      fl.color(fl.BLACK)
      for i = 0,19 do
         for j = i+1,19 do
            fl.begin_line()
            fl.vertex(cos(PI*i/10+.1), sin(PI*i/10+.1))
            fl.vertex(cos(PI*j/10+.1), sin(PI*j/10+.1))
            fl.end_line()
         end
      end
      fl.pop_matrix()
   end)
   return wid
end


function box_cb(b)
  thescroll:box(b:value() and 'down frame' or 'no box')
  thescroll:redraw()
end

function type_cb(_menu, _pn, v)
  thescroll:type(v)
  thescroll:redraw()
end

scroll_types = { "no scrollbar", "horizontal", "vertical", "both", "always on",
               "horizontal always", "vertical always", "both always" }

function add_choices(m)
   for _, v in ipairs(scroll_types) do
      m:add(v, 0, type_cb, v)
   end
end

function align_cb(_menu, _pn, v)
  thescroll:scrollbar():align(v)
  thescroll:redraw()
end

function add_achoices(m)
   m:add("left+top", 0, align_cb, fl.ALIGN_LEFT+fl.ALIGN_TOP)
   m:add("left+bottom", 0, align_cb, fl.ALIGN_LEFT+fl.ALIGN_BOTTOM)
   m:add("right+top", 0, align_cb, fl.ALIGN_RIGHT+fl.ALIGN_TOP)
   m:add("right+bottom", 0, align_cb, fl.ALIGN_RIGHT+fl.ALIGN_BOTTOM)
end


-- main -------------------------------------------------
window = fl.window(5*75,400, arg[0])
window:box('no box')
scroll = fl.scroll(0,0,5*75,300)

n = 0
for y=0,15 do
   for x=0,4 do
      local b = fl.button(x*75,y*25+(y>=8 and 5*75 or 0),75,25)
      b:label(tostring(n))
      b:color(n)
      b:labelcolor(fl.WHITE)
      n = n + 1
   end
end

drawing = Drawing(0,8*25,5*75,5*75)
scroll:done()
window:resizable(scroll)

box = fl.box(0,300,5*75,window:h()-300) -- gray area below the scroll
box:box('flat box')

but1 = fl.light_button(150, 310, 200, 25, "box")
but1:callback(box_cb)
  
choice = fl.choice(150, 335, 200, 25, "type():")
add_choices(choice)
choice:value("both")

achoice = fl.choice (150, 360, 200, 25, "scrollbar.align():")
add_achoices(achoice)
achoice:value("right+bottom")

thescroll = scroll

--scroll:box('down box')
--scroll:type('vertical')
window:done()

window:show(arg[0],arg)
return fl.run()
