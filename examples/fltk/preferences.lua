#!/usr/bin/env lua
-- MoonFLTK example: preferences.lua
--
-- Derived from the FLTK test/preferences.cxx example (http://www.fltk.org)
--

-- NOTE: The Fl_Preferences class is not used in MoonFLTK, because Lua itself
--       is already a flexible configuration language.

fl = require("moonfltk")

filename = "./mypreferences.lua"

function closeWindowCB() 
   os.exit(true, true)
end

function saveAndCloseWindowCB() 
   writePrefs(filename, prefs)
   os.exit(true, true)
end

function bool(x, defval) if x == nil then return defval else return x end end

function readPrefs(fname)
   print("reading preferences")
   local f, err = loadfile(fname)
   if not f then
      fl.alert(err)
      return false
   end
   local prefs = f() -- this is unsafe, but it's only an example...

   local bed = prefs.Bed
   wAlarm:value( bed.alarm or "8:00" )
   wAmPm:value( bed.ampm or "a.m.")
   wWear:value( bed.wear or "bare foot" )
   local side = bed.side or 2
   if side == 1 then wLeft:value( true ) end
   if side == 2 then wRight:value( true ) end
   local tasks = bed.taskFlags or 0x05
   if (tasks & 0x01)~=0 then wShower:value( true ) end
   if (tasks & 0x02)~=0 then wShave:value( true ) end
   if (tasks & 0x04)~=0 then wBrush:value( true ) end
  
   local eat = prefs.Breakfast
   wDrink:value( eat.drink or "coffee" )
   wMilk:value( bool(eat.wMilk, false) )
   wBread:value( eat.bread or "wheat" )
   wButter:value( bool(eat.wButter, false) )
   wEggs:value( eat.nEggs or 2 )
   wMinutes:value( eat.minutes or 3.2 )
   wPaper:value( eat.newspaper or "NY Tymes" )
   return prefs
end


function writePrefs(fname, prefs)
   print("writing preferences")
   local f, err = io.open(fname, "w+")
   if not f then
      fl.alert(err)
      return false
   end

   f:write("return {");

   f:write("\nBed = {");
      f:write("\n\talarm = '" .. wAlarm:value() .."',")
      f:write("\n\tampm = '"  .. wAmPm:value() .."',")
      f:write("\n\twear = '"  .. wWear:value() .."',")
      local side = 0
      if wLeft:value() then side = 1 end
      if wRight:value() then side = 2 end
      f:write("\n\tside = "   .. side ..",")
  
      local tasks = 0
      if wShower:value() then tasks = tasks | 0x01 end
      if wShave:value() then tasks = tasks | 0x02 end
      if wBrush:value() then tasks = tasks | 0x04 end
      f:write("\n\ttaskFlags = " .. tasks ..",")
   f:write("\n}, -- Bed");
  
   f:write("\nBreakfast = {");
      f:write("\n\tdrink = '" .. wDrink:value() .."',")
      f:write("\n\twMilk = "  .. tostring(wMilk:value()) ..",")
      f:write("\n\tbread = '" .. wBread:value() .."',")
      f:write("\n\twButter = " .. tostring(wButter:value()) ..",")
      f:write("\n\tnEggs = "  .. wEggs:value() ..",")
      f:write("\n\tminutes = "   .. wMinutes:value() ..",")
      f:write("\n\tnewspaper = '"   .. wPaper:value() .."',")
   f:write("\n}, --Breakfast");

   f:write("\n}");
   f:close()
end



-- main --------------------------------------------------------

myWindow = fl.double_window(298, 311, "My Preferences")
myWindow:callback(closeWindowCB)

o = fl.button(210, 275, 75, 25, "Cancel")
o:callback(closeWindowCB)

o = fl.button(125, 275, 75, 25, "OK")
o:callback(saveAndCloseWindowCB)

do local o = fl.group(20, 30, 115, 225, "Get Up:")
   o:box('engraved frame')
   o:align(fl.ALIGN_TOP_LEFT)

   wAlarm = fl.input(25, 55, 45, 20, "Alarm at:")
   wAlarm:align(fl.ALIGN_TOP_LEFT)

   wAmPm = fl.choice(75, 55, 55, 20)
   wAmPm:down_box('border box')
   wAmPm:add("a.m.")
   wAmPm:add("p.m.")
   wAmPm:value("a.m.")

   wWear = fl.choice(25, 100, 105, 20, "Wear:")

   wWear:down_box('border box')
   wWear:align(fl.ALIGN_TOP_LEFT)
   wWear:add("shoes")
   wWear:add("sandals")
   wWear:add("flip flops")
   wWear:add("bare foot")
   wWear:value("shoes")
 
   do local o = fl.group(35, 120, 98, 60)
      wLeft = fl.round_button(35, 120, 95, 25, "left side")
      wLeft:type(102)
      wLeft:down_box('round down box')
      wRight = fl.round_button(35, 140, 95, 25, "right side")
      wRight:type(102)
      wRight:down_box('round down box')
      fl.box(38, 160, 95, 20, "of the bed")
      o:done()
   end 

   wShower = fl.check_button(25, 180, 105, 25, "shower")
   wShower:down_box('down box')
 
   wShave = fl.check_button(25, 200, 105, 25, "shave")
   wShave:down_box('down box')
 
   wBrush = fl.check_button(25, 220, 105, 25, "brush teeth")
   wBrush:down_box('down box')
 
   o:done()
end
 
do local o = fl.group(160, 30, 115, 225, "Breakfast::")
   o:box('engraved frame')
   o:align(fl.ALIGN_TOP_LEFT)

   wDrink = fl.choice(165, 50, 105, 20, "Drink:")
   wDrink:down_box('border box')
   wDrink:align(fl.ALIGN_TOP_LEFT)
   wDrink:add("coffee")
   wDrink:add("tea")
   wDrink:add("juice")

   wMilk = fl.check_button(170, 70, 100, 25, "with milk")
   wMilk:down_box('down box')

   wBread = fl.choice(165, 110, 105, 20, "Bread:")
   wBread:down_box('border box')
   wBread:align(fl.ALIGN_TOP_LEFT)
   wBread:add("wheat")
   wBread:add("white")
   wBread:add("rye")
   wBread:add("sour doh")

   wButter = fl.check_button(170, 130, 100, 25, "with butter")
   wButter:down_box('down box')

   wEggs = fl.int_input(165, 163, 30, 20, "eggs")
   wEggs:type(2)
   wEggs:align(fl.ALIGN_RIGHT)

   wMinutes = fl.value_slider(175, 185, 70, 20, "min.")
   wMinutes:type(1)
   wMinutes:minimum(2)
   wMinutes:maximum(6)
   wMinutes:value(3.1)
   wMinutes:align(fl.ALIGN_RIGHT)

   wPaper = fl.input(165, 225, 105, 20, "Newspaper:")
   wPaper:align(fl.ALIGN_TOP_LEFT)

   o:done()
end 

myWindow:done()
prefs = readPrefs(filename)
myWindow:show(arg[0], arg)
return fl.run()

