-- MoonFLTK example: unittest_schemes.lua
--
-- Derived from the FLTK test/unittest_schemes.cxx example (http://www.fltk.org)
--

local function SchemeChoice_CB(_, name, info)
   assert( _ == info.schemechoice )
   if name then
      fl.scheme(name)   -- change scheme
      info.widget:window():redraw() -- redraw window
   end
end

return function(x, y, w, h)
   local label = "schemes test"
   local self = fl.group_sub(x, y, w, h)
   local info = { widget = self, label = label }

   local schemechoice = fl.choice(x+125,y,140,25,"FLTK Scheme")
   info.schemechoice = schemechoice
   schemechoice:add("none", 0, SchemeChoice_CB, info)
   schemechoice:add("plastic", 0, SchemeChoice_CB, info)
   schemechoice:add("gtk+", 0, SchemeChoice_CB, info)
   schemechoice:add("gleam", 0, SchemeChoice_CB, info)
   schemechoice:value("none")
   schemechoice:labelfont(fl.HELVETICA_BOLD)
   local name = fl.scheme()
   if name then schemechoice:value(name) end
--@@ indagare perché non funziona: schemechoice:callback(SchemeChoice_CB, info)  

   local subwin = fl.window(x,y+30,w,h-30)
   subwin:begin()

   o = fl.button(10, 9, 90, 25, "button")
   o:box('up box')
   o:color(101)
   o:tooltip("selection_color() = default")
   o:labelfont(5)

   o = fl.button(10, 36, 90, 25, "button")
   o:box('up box')
   o:color(179)
   o:selection_color(o:color())
   o:tooltip("selection_color() = color()")
   o:labelfont(4)
   o:labelcolor(fl.BACKGROUND2_COLOR)
    
   o = fl.button(10, 63, 90, 25, "button")
   o:box('up box')
   o:color(91)
   o:selection_color(fl.lighter(o:color()))
   o:tooltip("selection_color() = fl.lighter(color())")
    
   o = fl.button(10, 90, 90, 25, "button")
   o:box('up box')
   o:color(fl.INACTIVE_COLOR)
   o:selection_color(fl.darker(o:color()))
   o:tooltip("selection_color() = fl.darker(color())")
   o:labelcolor(fl.BACKGROUND2_COLOR)
    
   o = fl.tabs(10, 120, 320, 215)
   o:color(fl.DARK1)
   o:selection_color(fl.DARK1)

   do local o = fl.group(14, 141, 310, 190, "tab1")
      --o:box('thin up box')
      o:color(fl.DARK1)
      o:selection_color(23)
      o:hide()
      do local o = fl.clock(24, 166, 130, 130)
         o:box('thin up box')
         o:color(12)
         o:selection_color(fl.BACKGROUND2_COLOR)
         o:labelcolor(fl.BACKGROUND2_COLOR)
         o:tooltip("fl.clock with thin up box")
      
         fl.progress(22, 306, 290, 20)
      
         o = fl.clock(179, 166, 130, 130)
         o:box('thin down box')
         o:color(26)
         o:tooltip("fl.clock with thin down box")
      end
      o:done()

      o = fl.group(15, 140, 310, 190, "tab2")
      --o:box('thin up box')
      o:color(fl.DARK1)
      do local o = fl.slider(20, 161, 25, 155)
         o:box('down box')
         o:tooltip("fl.slider with down box")
      
         o = fl.scrollbar(50, 161, 25, 155)
         o:value(0, 50, 1, 100)
         o:box('down box')
         o:tooltip("fl.scrollbar with down box")
      
         o = fl.value_slider(115, 161, 25, 155)
         o:box('down box')
      
         o = fl.value_output(240, 265, 75, 25)
         o:box('down box')
         o:tooltip("fl.value_output with down box")
      
         o = fl.adjuster(185, 210, 100, 25)
         o:tooltip("fl.adjuster")
      
         o = fl.counter(185, 180, 100, 25)
         o:box('down box')
         o:tooltip("fl.counter with down box")
      
         o = fl.roller(85, 161, 25, 155)
         o:box('up box')
         o:tooltip("fl.roller with up box")
      
         o = fl.value_input(155, 265, 75, 25)
         o:box('down box')
         o:tooltip("fl.value_input with down box")
      
      end
      o:done()
    
      o = fl.group(15, 140, 310, 190, "tab3")
      --o:box('thin up box')
      o:color(fl.DARK1)
      o:hide()
      do local o = fl.input(40, 230, 120, 25)
         o:box('down box')
         o:tooltip("fl.input with down box")
      
         o = fl.output(40, 260, 120, 25)
         o:box('down box')
         o:tooltip("fl.output with down box")
      
         o = fl.text_editor(220, 160, 90, 55)
         o:box('down frame')
         o:color(80)
         o:tooltip("fl.text_editor with down frame")
      
         o = fl.text_display(220, 230, 90, 55)
         o:box('down frame')
         o:color(12)
         o:tooltip("fl.text_display with down frame")
       
         o = fl.file_input(40, 290, 265, 30)
         o:box('down box')
         o:tooltip("fl.file_input with down box")
      end
      o:done()

   end 
   o:done()
      
   o = fl.box(341, 10, 80, 50, "thin box\ndown1")
   o:box('thin down box')
   o:color(20)
   o:labelsize(10)

   o = fl.box(430, 10, 80, 50, "thin box\nup1")
   o:box('thin up box')
   o:color(fl.SELECTION_COLOR)
   o:labelcolor(6)
   o:labelsize(10)
    
   o = fl.box(341, 71, 80, 44, "thin box\ndown2")
   o:box('thin down box')
   o:color(190)
   o:labelsize(10)
    
   o = fl.box(430, 71, 80, 44, "thin box\nup2")
   o:box('thin up box')
   o:color(96)
   o:labelcolor(fl.BACKGROUND2_COLOR)
   o:labelsize(10)
    
   o = fl.box(341, 127, 80, 50, "box down3")
   o:box('down box')
   o:color(3)
   o:labelsize(10)
    
   o = fl.box(430, 127, 80, 50, "box up3")
   o:box('up box')
   o:color(104)
   o:labelcolor(3)
   o:labelsize(10)
    
   o = fl.box(341, 189, 80, 50, "box down4")
   o:box('down box')
   o:color(42)
   o:labelcolor(fl.DARK_RED)
   o:labelsize(10)
   
   o = fl.box(430, 189, 80, 50, "box up4")
   o:box('up box')
   o:color(30)
   o:labelcolor(26)
   o:labelsize(10)
    
   o = fl.box(341, 251, 80, 82, "box down5")
   o:box('down box')
   o:color(19)
   o:labelcolor(4)
   o:labelsize(10)
    
   o = fl.box(430, 251, 80, 82, "box up5")
   o:box('up box')
   o:color(fl.FOREGROUND_COLOR)
   o:labelcolor(fl.BACKGROUND2_COLOR)
   o:labelsize(10)
    
   o = fl.light_button(110, 10, 105, 25, "light")
   o:box('down box')
   o:color(fl.BACKGROUND2_COLOR)
   o:selection_color(30)
   o:tooltip("fl.light_button with down box")
    
   o = fl.check_button(110, 37, 105, 25, "check")
   o:box('down frame')
   o:down_box('down box')
   o:color(fl.DARK1)
   o:tooltip("fl.check_button with down frame")
    
   o = fl.input(220, 10, 100, 25)
   o:box('down box')
   o:color(23)
   o:tooltip("fl.input with down box")
   
   o = fl.adjuster(110, 65, 80, 43)
   o:box('up box')
   o:color(fl.INACTIVE_COLOR)
   o:selection_color(fl.BACKGROUND2_COLOR)
   o:labelcolor(55)
   o:tooltip("fl.adjuster with up box")
    
   o = fl.text_editor(220, 53, 100, 29, "down frame")
   o:box('down frame')
   o:color(19)
   o:selection_color(fl.DARK1)
   o:tooltip("fl.adjuster with down frame")
    
   o = fl.text_editor(220, 99, 100, 38, "up frame")
   o:box('up frame')
   o:color(19)
   o:selection_color(fl.DARK1)
   o:tooltip("fl.text_editor with up frame")
   
   subwin:done()
   subwin:resizable(subwin)
--   subwin:show()
   return info
end
