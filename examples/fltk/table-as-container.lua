#!/usr/bin/env lua
-- MoonFLTK example: table-as-container.lua
--
-- Derived from the FLTK examples/table-as-container.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

--
-- Simple demonstration class deriving from Fl_Table
--
function WidgetTable(x, y, w, h, l)
   local self = fl.table_sub(x,y,w,h,l)
   self:col_header(true)
   self:col_resize(true)
   self:col_header_height(25)
   self:row_header(true)
   self:row_resize(true)
   self:row_header_width(80)
   self:done()

   local function SetSize(newrows, newcols)
      self:clear()      -- clear any previous widgets, if any
      self:rows(newrows)
      self:cols(newcols)
      self:begin()      -- start adding widgets to group
      
      for r = 1, newrows do
         for c = 1, newcols do
            local x,y,w,h = self:find_cell('table', r, c)
            local s = string.format("%s.%s", r-1, c-1)
            if c & 1 ~= 0 then -- Create the input widgets
               local inp = fl.input(x,y,w,h)
               inp:value(s)
            else -- Create the light buttons
               local butt = fl.light_button(x,y,w,h,s)
               butt:align('center', 'inside')
               butt:callback(button_cb)
               butt:value( (((r-1)+(c-1)*2) & 4) ~= 0 )
            end
         end
      end   
      self:done()
   end -- SetSize

   self:override_draw_cell(function(_, context, R, C, X, Y, W, H)
      -- Handle drawing all cells in table
      if context == 'startpage' then 
         fl.font(fl.HELVETICA, 12)     -- font used by all headers

      elseif context == 'rc resize' then
         local index = 1
         for r = 1, self:rows() do
            for c = 1, self:cols() do
               if ( index <= self:children()) then
                  --print("index", index)
                  local x, y, w, h = self:find_cell('table', r, c)
                  local child = self:child(index)
                  child:resize(x,y,w,h)
                  --print("child", index, child:visible(), x, y, w, h)
                  index = index + 1
               end
            end
         end
         self:init_sizes() -- tell group children resized
         return

      elseif context == 'row header' then
         fl.push_clip(X, Y, W, H)
         fl.draw_box('thin up box', X, Y, W, H, self:row_header_color())
         fl.color(fl.BLACK)
         local s = "Row " .. tostring(R-1)
         fl.draw(s, X, Y, W, H, fl.ALIGN_CENTER)
         fl.pop_clip()
         return

      elseif context == 'col header' then
         fl.push_clip(X, Y, W, H)
         local s = "Column " .. tostring(C-1)
         fl.draw_box('thin up box', X, Y, W, H, self:col_header_color())
         fl.color(fl.BLACK)
         fl.draw(s, X, Y, W, H, fl.ALIGN_CENTER)
         fl.pop_clip()
         return

      elseif context == 'cell' then
--       print('cell', R, C)
         return -- fltk handles drawing the widgets
      end
   end) -- draw_cell()

   return { self=self, SetSize=SetSize }
end

function button_cb(w) print("BUTTON: ".. w:label()) end

-- main ---------------
win = fl.double_window(940, 500, "table as container")
tabl = WidgetTable(20, 20, win:w()-40, win:h()-40, "FLTK widget table")
tabl.SetSize(50, 50)
collectgarbage() -- just to check that labels are not GC'ed
win:done()
--print("N WIDGETS", tabl.self:children(), win:children())
assert(win:child(1) == tabl.self)
win:resizable(tabl.self)
win:show()
--print(win:visible(), tabl.self:visible(), (tabl.self:child(1)):visible())
return fl.run()

