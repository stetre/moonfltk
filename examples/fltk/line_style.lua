#!/usr/bin/env lua
-- MoonFLTK example: line_style.lua
--
-- Derived from the FLTK test/line_style.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

sliders = {}
choice = {}

function test_box(x, y, w, h, l)
   local wid = fl.double_window_sub(x, y, w, h, l) 
   wid:override_draw(function (wid_)
      assert(wid_ == wid)
      wid:super_draw() -- this executes Fl_Double_Window::draw()
      fl.color(sliders[1]:value(), sliders[2]:value(), sliders[3]:value())
--[[
      print(choice[1]:mvalue(), choice[1]:item_argument(choice[1]:mvalue()))
      print(choice[2]:mvalue(), choice[2]:item_argument(choice[2]:mvalue()))
      print(choice[3]:mvalue(), choice[3]:item_argument(choice[3]:mvalue()))
--]]

---[[ 
      fl.line_style(
            choice[1]:item_argument(choice[1]:mvalue()) +
            choice[2]:item_argument(choice[2]:mvalue()) +
            choice[3]:item_argument(choice[3]:mvalue()), -- style
            sliders[4]:value(),     -- width
            sliders[6]:value(),     -- dashes
            sliders[7]:value(),
            sliders[8]:value(),
            sliders[9]:value()
      )
      -- draw the defined fl_rect and fl_vertex first and then
      -- the additional one-pixel line, if enabled
      -- sliders[4] = x/y coordinate translation (default = 10)

      local n = draw_line:value() and 2 or  1
      for i=1, n do
         local move = sliders[5]:value()
         local w, h = wid:w(), wid:h()
         fl.rect(move,move,w-20,h-20)
         fl.begin_line()
         fl.vertex(move+25, move+25)
         fl.vertex(w-45+move, h-45+move)
         fl.vertex(w-50+move, move+25)
         fl.vertex(move+25, h/2-10+move)
         fl.end_line()
         -- you must reset the line type when done:
         fl.line_style(fl.SOLID)
         fl.color(fl.BLACK)
      end
--]]
   end)
   return wid
end


function do_redraw() test:redraw() end

function style_menu(m)
   m:add("solid", 0, do_redraw, fl.SOLID)
   m:add("dash", 0, do_redraw, fl.DASH)
   m:add("dot", 0, do_redraw, fl.DOT)
   m:add("dashdot", 0, do_redraw, fl.DASHDOT)
   m:add("dashdotdot", 0, do_redraw, fl.DASHDOTDOT)
   m:callback(do_redraw)
end

function cap_menu(m)
   m:add("default", 0, do_redraw, 0)
   m:add("flat", 0, do_redraw, fl.CAP_FLAT)
   m:add("round", 0, do_redraw, fl.CAP_ROUND)
   m:add("square", 0, do_redraw, fl.CAP_SQUARE)
   m:callback(do_redraw)
end

function join_menu(m)
   m:add("default", 0, do_redraw, 0)
   m:add("miter", 0, do_redraw, fl.JOIN_MITER)
   m:add("round", 0, do_redraw, fl.JOIN_ROUND)
   m:add("bevel", 0, do_redraw, fl.JOIN_BEVEL)
   m:callback(do_redraw)
end




function makeform()
  form = fl.double_window(500,250,"fl.line_style() test")
  sliders[1]= fl.value_slider(280,10,180,20,"R")
  sliders[1]:bounds(0,255)
  sliders[2]= fl.value_slider(280,30,180,20,"G")
  sliders[2]:bounds(0,255)
  sliders[3]= fl.value_slider(280,50,180,20,"B")
  sliders[3]:bounds(0,255)
  choice[1]= fl.choice(280,70,180,20,"Style")
  style_menu(choice[1]) -- instead of choice[1]:menu(style_menu)
  choice[2]= fl.choice(280,90,180,20,"Cap")
  cap_menu(choice[2])
  choice[3]= fl.choice(280,110,180,20,"Join")
  join_menu(choice[3])
  sliders[4]= fl.value_slider(280,130,180,20,"Width")
  sliders[4]:bounds(0,20)
  sliders[5]= fl.value_slider(280,150,180,20,"Move")
  sliders[5]:bounds(-10,20)
  draw_line = fl.check_button(280,170,20,20,"&Line")
  draw_line:align('left')
  fl.box(305,170,160,20,"add a 1-pixel black line")
  sliders[6] = fl.slider(200,210,70,20,"Dash")
  sliders[6]:align('top', 'left')
  sliders[6]:bounds(0,40)
  sliders[7] = fl.slider(270,210,70,20)
  sliders[7]:bounds(0,40)
  sliders[8] = fl.slider(340,210,70,20)
  sliders[8]:bounds(0,40)
  sliders[9] = fl.slider(410,210,70,20)
  sliders[9]:bounds(0,40)
  for i=1,9 do
    sliders[i]:type('horizontal')
    if (i<6) then sliders[i]:align('left') end
    sliders[i]:callback(do_redraw)
    sliders[i]:step(1)
  end
  sliders[1]:value(255) -- R
  sliders[2]:value(100) -- G
  sliders[3]:value(100) -- B
  sliders[5]:value(10)  -- move line coordinates
  draw_line:value(false)
  draw_line:callback(do_redraw)
--  for i=1,3 do
--    choice[i]:value(0)
--    choice[i]:callback(do_redraw)
--  end
  test = test_box(0,0,200,200)
  test:done()
  form:resizable(test)
  form:done()
end

makeform()
form:show(arg[0],arg)
return fl.run()
