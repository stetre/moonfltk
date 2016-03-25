#!/usr/bin/env lua
-- MoonFLTK example: colbrowser.lua
--
-- Derived from the FLTK test/colbrowser.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

FREE_COL4 = fl.FREE_COLOR+3
INDIANRED = 164

rgdb = {}   

function set_entry(i)
   local c = rgdb[i]
   fl.set_color(FREE_COL4, c[2], c[3], c[4])
   rs:value(c[2])
   gs:value(c[3])
   bs:value(c[4])
   rescol:redraw()
end

function br_cb(ob)
   local r = ob:value()
   if r <= 0 then return end
   set_entry(r)
end

function load_browser(fname)
   local f, err = loadfile(fname)
   if not f then
      fl_alert(err)
      return false
   end
   local db = f() -- this is unsafe, but it's only an example...
   local lr, lg, lb = -1, -1, -1
   for i, color in ipairs(db) do
      local n, r, g, b = table.unpack(color)
      if r ~= lr or g ~= lg or b ~= lb then
         lr, lg, lb = r, g, b
         colbr:add(string.format("(%3d %3d %3d) %s", r, g, b, n))
         rgdb[#rgdb+1] = color 
      end
   end
   colbr:topline(1)
   colbr:select(1, true)
   set_entry(1)
   return true
end


-- entry = { name, r, g, b }
function search_entry(r, g, b)
   local j = nil
   local mindiff = math.huge
   for i, rgb in ipairs(rgdb) do
      local diffr = r - rgb[2]
      local diffg = g - rgb[3]
      local diffb = b - rgb[4]
      
      local diff = (3.0 * (diffr*diffr) + 5.9 * (diffg *diffg) + 1.1 * (diffb*diffb))
      
      if mindiff > diff then
         mindiff = diff
         j = i
      end
   end
   return j
end


function search_rgb()
   local top = colbr:topline()
   
   local r = rs:value()
   local g = gs:value()
   local b = bs:value()
   
   fl.set_color(FREE_COL4, r, g, b)
   rescol:redraw()
   local i = search_entry(r, g, b)
   -- change topline only if necessary 
   if i < top or i > (top+15) then
      colbr:topline(i-8)
   end
   colbr:select(i+1, true)
end

-- change database
function db_cb(ob)
   local name = fl.input("Enter New Database Name", dbname)
   
   if not name or name == dbname then return end
   
   if load_browser(name) then
      dbname = name
   else
      ob:label(dbname)
   end
end


function create_form_cl()
   cl = fl.double_window(400,385)
   cl:box('up box')
   cl:color(INDIANRED, fl.GRAY)
   
   local title = fl.box(40, 10, 300, 30, "Color Browser")
   title:box('no box')
   title:labelcolor(fl.RED)
   title:labelsize(32)
   title:labelfont(fl.HELVETICA_BOLD)
   title:labeltype('shadow')
   
   dbobj = fl.button(40, 50, 300, 25, "")
   dbobj:type('normal')
   dbobj:box('border box')
   dbobj:color(INDIANRED,INDIANRED)
   dbobj:callback(db_cb, 0)
   
   colbr = fl.hold_browser(10, 90, 280, 240, "")
   colbr:textfont(fl.COURIER) 
   colbr:callback(br_cb, 0)
   colbr:box('down box')
   
   rescol = fl.box(300, 90, 90, 35, "")
   rescol:color(FREE_COL4, FREE_COL4)
   rescol:box('border box')
   
   rs = fl.value_slider(300, 130, 30, 200, "")
   rs:type('vertical fill')
   rs:color(INDIANRED, fl.RED)
   rs:bounds(0, 255)
   rs:precision(0)
   rs:callback(search_rgb, 0)
   rs:when(fl.WHEN_RELEASE)
   
   gs = fl.value_slider(330, 130, 30, 200, "")
   gs:type('vertical fill')
   gs:color(INDIANRED, fl.GREEN)
   gs:bounds(0, 255)
   gs:precision(0)
   gs:callback(search_rgb, 1)
   gs:when(fl.WHEN_RELEASE)
   
   bs = fl.value_slider(360, 130, 30, 200, "")
   bs:type('vertical fill')
   bs:color(INDIANRED, fl.BLUE)
   bs:bounds(0, 255)
   bs:precision(0)
   bs:callback(search_rgb, 2)
   bs:when(fl.WHEN_RELEASE)
   
   local done = fl.button(160, 345, 80, 30, "Done")
   done:type('normal')
   done:callback(function() os.exit(true, true) end)
   
   cl:done()
   cl:resizable(cl)

end

-- main --------------------------------------------

create_form_cl()
dbname = "rgb.lua"
   
if load_browser(dbname) then
   dbobj:label(dbname)
else
    dbobj:label("None")
dbobj:redraw()
end
   
cl:size_range(cl:w(),cl:h(),2*cl:w(),2*cl:h())
   
cl:label(arg[0]) --"RGB Browser"
cl:show(arg[0],arg)
   
return fl.run()

