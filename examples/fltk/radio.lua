#!/usr/bin/env lua
-- MoonFLTK example: radio.lua
--
-- Derived from the FLTK test/radio.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")


-- Buttons test callback
function button_cb(b)
  local msg = "Label: '" .. b:label() .. "'\nValue: " .. tostring(b:value())
  cb_info:value(msg)
  cb_info:redraw()
  print(msg)
end

w = fl.double_window(369, 214, arg[0])
do local o = fl.button(20, 10, 160, 30, "&fl.button A1")
   o:tooltip("Normal button (callback called only when released)")
   o:labelsize(13)
   o:callback(button_cb)
end 
    do local o = fl.button(20, 44, 160, 30, "fl.button &A2")
      o:tooltip("Normal button with callback called when changed (push and released)")
      o:labelsize(13)
      o:when('changed')
      o:callback(button_cb)
    end 
    do local o = fl.return_button(20, 78, 160, 30, "fl.return_button &B")
      o:tooltip("Button with Return key as default shortcut")
      o:labelsize(13)
      o:callback(button_cb)
    end
    do local o = fl.light_button(20, 113, 160, 30, "fl.light_button &C")
      o:tooltip("Button with toggle state and a visual indicator of the current state")
      o:labelsize(13)
      o:callback(button_cb)
    end
    do local o = fl.check_button(20, 148, 160, 30, "fl.check_button &D")
      o:tooltip("Check button with toggle state")
      o:down_box('down box')
      o:labelsize(13)
      o:callback(button_cb)
    end
    do local o = fl.round_button(20, 178, 160, 30, "fl.round_button &E")
      o:tooltip("Round Button with toggle state")
      o:down_box('round down box')
      o:labelsize(13)
      o:callback(button_cb)
    end
    do local o = fl.group(190, 10, 70, 120)
      o:box('thin up frame')
      do local o = fl.round_button(190, 10, 70, 30, "radio &1")
        o:tooltip("Radio button, only one button is set at a time, in the corresponding group.")
        o:type(102)
        o:down_box('round down box')
        o:callback(button_cb)
      end
      do local o = fl.round_button(190, 40, 70, 30, "radio &2")
        o:tooltip("Radio button, only one button is set at a time, in the corresponding group.")
        o:type(102)
        o:down_box('round down box')
        o:callback(button_cb)
      end
      do local o = fl.round_button(190, 70, 70, 30, "radio &3")
        o:tooltip("Radio button, only one button is set at a time, in the corresponding group.")
        o:type(102)
        o:down_box('round down box')
        o:callback(button_cb)
      end
      do local o = fl.round_button(190, 100, 70, 30, "radio &4")
        o:tooltip("Radio button, only one button is set at a time, in the corresponding group.")
        o:type(102)
        o:down_box('round down box')
        o:callback(button_cb)
      end
      o:done()
    end
    do local o = fl.group(270, 10, 90, 120)
      o:box('thin up box')
      do local o = fl.button(280, 20, 20, 20, "radio")
        o:tooltip("Custom look button, only one button is set at a time, in the corresponding group.")
        o:type(102)
        o:selection_color(1)
        o:align(fl.ALIGN_RIGHT)
      end 
      do local o = fl.button(280, 45, 20, 20, "radio")
        o:tooltip("Custom look button, only one button is set at a time, in the corresponding group.")
        o:type(102)
        o:selection_color(1)
        o:align(fl.ALIGN_RIGHT)
      end 
      do local o = fl.button(280, 70, 20, 20, "radio")
        o:tooltip("Custom look button, only one button is set at a time, in the corresponding group.")
        o:type(102)
        o:selection_color(1)
        o:align(fl.ALIGN_RIGHT)
      end 
      do local o = fl.button(280, 95, 20, 20, "radio")
        o:tooltip("Custom look button, only one button is set at a time, in the corresponding group.")
        o:type(102)
        o:selection_color(1)
        o:align(fl.ALIGN_RIGHT)
      end 
      o:done()
    end

cb_info = fl.output(190, 148, 170, 62, "callback:")
cb_info:type(12)
cb_info:textsize(12)
cb_info:align(133)

w:done()
w:show(argc, argv)
return fl.run()
