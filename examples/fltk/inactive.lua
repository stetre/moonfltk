#!/usr/bin/env lua
-- MoonFLTK example: inactive.lua
--
-- Derived from the FLTK test/inactive.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function add_items(menu)
   -- note: in MoonFLTK each menu item must have a unique 'pathname':
   menu:add("item1")
   menu:add("item2")
   menu:add("item3")
   menu:add("item4")
   menu:add("item5")
end

-- main -----------------------------------
w = fl.double_window(420, 369, arg[0])
   the_group = fl.group(25, 25, 375, 295, "activate()/deactivate() called on this group")
   the_group:box('engraved frame')
   the_group:align(fl.ALIGN_TOP|fl.ALIGN_INSIDE)
      fl.button(50, 50, 105, 25, "button")
       
      o = fl.light_button(50, 80, 105, 25, "light button")
      o:value(true)
      o:align(fl.ALIGN_CENTER|fl.ALIGN_INSIDE)
       
      o = fl.group(50, 130, 105, 125, "Child group")
      o:box('down frame')
        
      do local o = fl.check_button(50, 170, 105, 25, "red")
         o:type(102)
         o:down_box('diamond down box')
         o:selection_color(1)
         o:labelcolor(1)
         
         o = fl.check_button(50, 190, 105, 25, "green")
         o:type(102)
         o:down_box('diamond down box')
         o:selection_color(2)
         o:labelcolor(2)
         
         o = fl.check_button(50, 210, 105, 25, "blue")
         o:type(102)
         o:down_box('diamond down box')
         o:selection_color(4)
         o:labelcolor(4)
         
         o = fl.check_button(50, 230, 105, 25, "white")
         o:type(102)
         o:down_box('diamond down box')
         o:selection_color(55)
         o:labelcolor(55)
         
         o = fl.check_button(50, 130, 105, 25, "check")
         o:down_box('down box')
         
         o = fl.round_button(50, 150, 105, 25, "round")
         o:down_box('round down box')
      end 
         
      o:done()

      o = fl.slider(165, 50, 24, 205, "slider")
      o:value(0.5)
       
      o = fl.input(195, 50, 195, 30)
      o:value("fl.input")
       
      o = fl.menu_button(245, 90, 130, 30, "menu")
      add_items(o)
       
      o = fl.value_output(245, 130, 130, 30, "value:")
      o:maximum(10000)
      o:step(1)
      o:textfont(5)
      o:textsize(24)
      o:textcolor(4)
       
      o = fl.box(245, 170, 140, 50, "box")
      o:box('embossed frame')
      o:labeltype('shadow')
      o:labelfont(3)
      o:labelsize(38)
       
      o = fl.scrollbar(40, 274, 180, 20, "scrollbar")
      o:type(1)
       
      fl.roller(235, 230, 25, 65, "roller")
       
      fl.dial(275, 235, 50, 50, "dial")
       
      fl.clock(335, 235, 50, 50, "clock")
       
   the_group:done()
   fl.current_group():resizable(the_group)

   o = fl.button(25, 330, 185, 25, "active")
   o:type(102)
   o:value(true)
   o:callback(function() the_group:activate() end)
    
   o = fl.button(220, 330, 180, 25, "inactive")
   o:type(102)
   o:callback(function() the_group:deactivate() end)
    
w:done()
w:show(arg[0], arg)
return fl.run()
