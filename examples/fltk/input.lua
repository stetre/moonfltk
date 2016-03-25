#!/usr/bin/env lua
-- MoonFLTK example: input.lua
--
-- Derived from the FLTK test/input.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")


function cb(ob)
   print("Callback for " .. ob:label() .. " '" .. ob:value() .."'")
end

when = 0
input = {}

function toggle_cb(b, v)
  if b:value() then when = when | v else when = when & ~v end
  for i=1,5 do input[i]:when(when) end
end

function test(i)
  if i:changed() then
    i:clear_changed() print(i:label() .. " '" .. i:value() .."'")
--[[@@ ??
    char utf8buf[10]
    int last = fl_utf8encode(i:index(i:position()), utf8buf)
    utf8buf[last] = 0
    printf("Symbol at cursor position: %s\n", utf8buf)
--]]
  end
end

function button_cb()
   for i=1,5 do test(input[i]) end
end

function color_cb(button, c)
   local rgbi, r, g, b = fl.get_color(c)
   r, g, b = fl.color_chooser_b("", r, g, b)
   if r then
      fl.set_color(c,r,g,b) fl.redraw()
      button:labelcolor(fl.contrast(fl.BLACK,c))
      button:redraw()
   end
end

function tabnav_cb(w, fmi)
  fmi:tab_nav(b:value())
end

function arrownav_cb(b)
  fl.option('arrow focus', b:value())
end

-- main ----------------------------------
-- the following two lines set the correct color scheme, so that 
-- calling fl.contrast below will return good results
--fl.args(argc, argv)
fl.get_system_colors()
window = fl.window(400,420)

local y = 10
input[1] = fl.input(70,y,300,30,"Normal:") y = y + 35
input[1]:tooltip("Normal input field")
-- input[1]:cursor_color(FL_SELECTION_COLOR)
-- input[1]:maximum_size(20)
-- input[1]:static_value("this is a testgarbage")
input[2] = fl.float_input(70,y,300,30,"Float:") y = y + 35
input[2]:tooltip("input field for floating-point number (F1)")
input[2]:shortcut(fl.shortcut('f1'))
input[3] = fl.int_input(70,y,300,30,"Int:") y = y + 35
input[3]:tooltip("Input field for integer number (F2)")
input[3]:shortcut(fl.shortcut('f2'))
input[4] = fl.secret_input(70,y,300,30,"&Secret:") y = y + 35
input[4]:tooltip("Input field for password (Alt-S)")
input[5] = fl.multiline_input(70,y,300,100,"&Multiline:") y = y + 105
input[5]:tooltip("Input field for short text with newlines (Alt-M)")
input[5]:wrap(true)

for i = 1, 5 do
   input[i]:when('never') input[i]:callback(cb)
end
  
y1 = y

b = fl.toggle_button(10,y,200,25,"fl.WHEN_CHANGED")
b:callback(toggle_cb, fl.WHEN_CHANGED) y = y + 25
b:tooltip("Do callback each time the text changes")
b = fl.toggle_button(10,y,200,25,"fl.WHEN_RELEASE")
b:callback(toggle_cb, fl.WHEN_RELEASE) y = y + 25
b:tooltip("Do callback when widget loses focus")
b = fl.toggle_button(10,y,200,25,"fl.WHEN_ENTER_KEY")
b:callback(toggle_cb, fl.WHEN_ENTER_KEY) y = y + 25
b:tooltip("Do callback when user hits Enter key")
b = fl.toggle_button(10,y,200,25,"fl.WHEN_NOT_CHANGED")
b:callback(toggle_cb, fl.WHEN_NOT_CHANGED) y = y + 25
b:tooltip("Do callback even if the text is not changed") y = y + 5
b = fl.button(10,y,200,25,"&print changed()") y = y + 25
b:callback(button_cb)
b:tooltip("Print widgets that have changed() flag set")

b = fl.light_button(10,y,100,25," Tab Nav")
b:tooltip("Control tab navigation for the multiline input field")
b:callback(tabnav_cb, input[5])
b:value(input[5]:tab_nav())
b = fl.light_button(110,y,100,25," Arrow Nav") y = y + 25
b:tooltip("Control horizontal arrow key focus navigation behavior.\n" ..
             "e.g. 'arrow focus'")
b:callback(arrownav_cb)
b:value(input[5]:tab_nav())
b:value(fl.option('arrow focus'))

b = fl.button(220,y1,120,25,"color") y1 = y1 + 25
b:color(input[1]:color()) b:callback(color_cb, fl.BACKGROUND2_COLOR)
b:tooltip("Color behind the text")
b = fl.button(220,y1,120,25,"selection_color") y1 = y1 + 25
b:color(input[1]:selection_color()) b:callback(color_cb, fl.SELECTION_COLOR)
b:labelcolor(fl.contrast(fl.BLACK,b:color()))
b:tooltip("Color behind selected text")
b = fl.button(220,y1,120,25,"textcolor") y1 = y1 + 25
b:color(input[1]:textcolor()) b:callback(color_cb, fl.FOREGROUND_COLOR)
b:labelcolor(fl.contrast(fl.BLACK,b:color()))
b:tooltip("Color of the text")

window:done()
window:show(arg[0],argc)
return fl.run()
