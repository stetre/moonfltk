#!/usr/bin/env lua
-- MoonFLTK example: menubar-add.lua
--
-- Derived from the FLTK examples/menubar-add.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")
--fl.trace_objects(true)

-- This callback is invoked whenever the user clicks an item in the menu bar
function MyMenuCallback(bar)
   local ipath = bar:mvalue() -- the pathname of the menu item that was picked
   local label = bar:item_label(ipath) -- its label
   local flags = bar:item_flags(ipath)
   print("You picked '".. label .."'")
   print("- item pathname is:", ipath)
   print("- item flags are:", flags, fl.menuitemflags(flags)) -- print also the strings
   if (flags & (fl.MENU_RADIO|fl.MENU_TOGGLE)) ~= 0 then -- Toggle or radio item?
      print("- item value is:", bar:item_value(ipath)) -- Print item's value
   end
   if label == "Google" then 
      fl.open_uri("http://google.com/")
   elseif label == "&Quit" then 
      os.exit(true, true) 
   end
end

-- main ---------------------
fl.scheme("gtk+")
win = fl.window(400,200, "menubar-simple")   -- Create window
menu = fl.menu_bar(0,0,400,25)      -- Create menubar, items..
--        pathname,      shortcut,                 callback,      argument, flags
menu:add("&File/&Open",  fl.shortcut("Ctrl", "o"), MyMenuCallback)
menu:add("&File/&Save",  fl.shortcut("Ctrl","s"), MyMenuCallback, nil, 'divider')
menu:add("&File/&Quit",  fl.shortcut("Ctrl","q"), MyMenuCallback)
menu:add("&Edit/&Copy",  fl.shortcut("Ctrl","c"), MyMenuCallback)
menu:add("&Edit/&Paste", fl.shortcut("Ctrl","v"), MyMenuCallback, nil, 'divider')
menu:add("&Edit/Radio 1",   0, MyMenuCallback, nil, 'radio')
menu:add("&Edit/Radio 2",   0, MyMenuCallback, nil, 'radio', 'divider')
menu:add("&Edit/Toggle 1",  0, MyMenuCallback, nil, 'toggle')        -- Default: off 
menu:add("&Edit/Toggle 2",  0, MyMenuCallback, nil, 'toggle')        -- Default: off
menu:add("&Edit/Toggle 3",  0, MyMenuCallback, nil, 'toggle', 'value')  -- Default: on
menu:add("&Help/Google",    0, MyMenuCallback)

-- Example: show how we can dynamically change the state of item Toggle #2 (turn it 'on')
menu:item_set("&Edit/Toggle 2") -- Turn it on

win:done()
win:show()
return fl.run()

