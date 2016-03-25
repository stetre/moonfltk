#!/usr/bin/env lua
-- MoonFLTK example: table-spreadsheet.lua
--
-- Derived from the FLTK examples/table-spreadsheet.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

MAX_COLS = 10
MAX_ROWS = 10

function Spreadsheet(X,Y,W,H,L)
   local self = fl.table_sub(X,Y,W,H,L)
   local info = { self = self } -- 'class' data

   self:callback(event_callback, info)
   self:when(fl.WHEN_NOT_CHANGED | self:when())
    -- Create input widget that we'll use whenever user clicks on a cell
   info.input = fl.int_input(W/2,H/2,0,0)
   info.input:hide()
   info.input:callback(input_cb, info)
   info.input:when(fl.WHEN_ENTER_KEY_ALWAYS)    -- callback triggered when user hits Enter
   info.input:maximum_size(5)
   info.input:color(fl.YELLOW)

   info.values = {} -- array of data for cells
   for r = 1, MAX_ROWS do
      info.values[r] = {}
      for c = 1, MAX_COLS do
         info.values[r][c] =  c-1 + ((r-1)*MAX_COLS)  -- initialize cells
      end
   end
   info.row_edit, info.col_edit = 1, 1 -- row/col being modified
   self:done()
   self:set_selection(1,1,1,1)

   self:override_draw_cell(function (_, context, R, C, X, Y, W, H)
      draw_cell(info, context, R, C, X, Y, W, H)
   end)

   return self, info
end


function input_cb(_, info) -- input widget's callback
   set_value_hide(info)
end

function draw_cell(info, context, R, C, X, Y, W, H)
-- Handle drawing all cells in table
   local self, input = info.self, info.input

   if context == 'startpage' then -- table about to redraw
      return
   elseif context == 'col header' then -- table wants us to draw a column heading (C is column)
      fl.font(fl.HELVETICA | fl.BOLD, 14) -- set font for heading to bold
      fl.push_clip(X,Y,W,H)         -- clip region for text
      fl.draw_box('thin up box', X,Y,W,H, self:col_header_color())
      fl.color(fl.BLACK)
      if C == self:cols() then   -- Last column? show 'TOTAL'
         fl.draw("TOTAL", X,Y,W,H, fl.ALIGN_CENTER)
      else  -- Not last column? show column letter
         local s = tostring(64 + C) -- 1->'A', 2->'B' etc
         fl.draw(s, X,Y,W,H, fl.ALIGN_CENTER)
      end
      fl.pop_clip()
      return

   elseif context == 'row header' then -- table wants us to draw a row heading (R is row)
      fl.font(fl.HELVETICA | fl.BOLD, 14) -- set font for row heading to bold
      fl.push_clip(X,Y,W,H)
      fl.draw_box('thin up box', X,Y,W,H, self:row_header_color())
      fl.color(fl.BLACK)
      if R == self:rows() then   -- Last row? Show 'Total'
         fl.draw("TOTAL", X,Y,W,H, fl.ALIGN_CENTER)
      else  -- Not last row? show row#
         fl.draw(tostring(R), X,Y,W,H, fl.ALIGN_CENTER)
      end
      fl.pop_clip()
      return

   elseif context == 'cell' then -- table wants us to draw a cell
      if R == info.row_edit and C == info.col_edit and input:visible() then
         return -- dont draw for cell with input widget over it
      end
      -- Background
      if C < self:cols() and R < self:rows() then
         fl.draw_box('thin up box', X,Y,W,H, self:is_selected(R,C) and fl.YELLOW or fl.WHITE)
      else
         fl.draw_box('thin up box', X,Y,W,H, 0xbbddbb00) -- money green
      end
      -- Text
      fl.push_clip(X+3, Y+3, W-6, H-6)
      fl.color(fl.BLACK) 
      local lastcol, lastrow = C==self:cols(), R==self:rows()
      if lastcol or lastrow then -- Last row or col? Show total
         local s
         fl.font(fl.HELVETICA | fl.BOLD, 14) -- ..in bold font
         if lastcol and lastrow then   -- Last row+col? Total all cells
            s = tostring(sum_all(info))
         elseif lastcol then     -- Row subtotal
            s = tostring(sum_cols(info, R))
         else -- lastrow == true
            s = tostring(sum_rows(info, C))  -- Col subtotal
         end
         fl.draw(s, X+3,Y+3,W-6,H-6, fl.ALIGN_RIGHT)
      else  -- Not last row or col? Show cell contents
         fl.font(fl.HELVETICA, 14)     -- ..in regular font
         local s = tostring(info.values[R][C])
         fl.draw(s, X+3,Y+3,W-6,H-6, fl.ALIGN_RIGHT)
      end
      fl.pop_clip()
      return
   
   elseif context == 'rc resize' then  -- table resizing rows or columns
      if input:visible() then
         local X, Y, W, H = self:find_cell('table', info.row_edit, info.col_edit)
         input:resize(X,Y,W,H)
         self:init_sizes()
      end
      return
   end
end -- draw_cell



function sum_rows(info, C) -- Return the sum of all rows in this column
   local sum = 0
   for r = 1, info.self:rows()-1 do -- -1: don't include cell data in 'totals' column
      sum = sum + info.values[r][C]
   end
   return sum
end

function sum_cols(info, R) -- Return the sum of all cols in this row
   local sum = 0
   for c = 1, info.self:cols()-1 do       -- -1: don't include cell data in 'totals' column
      sum = sum + info.values[R][c]
   end
   return sum
end

function sum_all(info) -- Return the sum of all cells in table
    local sum = 0
    for c=1, info.self:cols()-1 do  -- -1: don't include cell data in 'totals' column
      for r=1, info.self:rows()-1 do   -- -1: ""
         sum = sum +info.values[r][c]
      end
   end
   return sum
end



function set_value_hide(info)
-- Apply value from input widget to values[row][col] array and hide (done editing)
   local self = info.self
   local input = info.input
   info.values[info.row_edit][info.col_edit] = tonumber(input:value())
   input:hide()
   self:window():cursor('default') -- XXX: if we don't do this, cursor can disappear!
end 


function start_editing(info, R, C)
-- Start editing a new cell: move the Fl_Int_Input widget to specified row/column
--    Preload the widget with the cell's current value,
--    and make the widget 'appear' at the cell's location.
--
   local self, input = info.self, info.input
   info.row_edit = R             -- Now editing this row/col
   info.col_edit = C
   self:set_selection(R,C,R,C)   -- Clear any previous multicell selection
   local X,Y,W,H = self:find_cell('cell', R,C)     -- Find X/Y/W/H of cell
   input:resize(X,Y,W,H)            -- Move Fl_Input widget there
   local s = tostring(info.values[R][C])     -- Load input widget with cell's current value
   input:value(s)
   input:position(0,#s)       -- Select entire input field
   input:show()               -- Show the input widget, now that we've positioned it
   input:take_focus()
end 

-- Tell the input widget it's done editing, and to 'hide'
function done_editing(info)
   if info.input:visible() then  -- input widget visible, ie. edit in progress?
      set_value_hide(info)       -- Transfer its current contents to cell and hide
   end
end


function event_callback(self, info) -- widget callback for table
-- Callback whenever someone clicks on different parts of the table
   assert(self == info.self)
   local R = self:callback_row()
   local C = self:callback_col()
   local context = self:callback_context() 
   print("event callback", context, R, C) -- R=C=0 if dead zone

   if context == 'cell' then -- A table event occurred on a cell
      local event = fl.event() -- see what FLTK event caused it
      print("event", event)
      if event == 'push' then -- mouse click?
         done_editing(info)            -- finish editing previous
         if R ~= self:rows() and C ~= self:cols() then -- only edit cells not in total's columns
            start_editing(info, R,C)   -- start new edit
         end
         return
      elseif event == 'keydown' then   -- key press in table?
         if fl.event_key() == 'escape' then os.exit(true, true) end  -- ESC closes app
         done_editing(info)            -- finish any previous editing
         if C == self:cols() or R==self:rows() then return end-- no editing of totals column
         local text = fl.event_text() or ""
         local t = text:sub(1, 1)

         -- there are smarter ways to do this check but we don't want to
         -- obfuscate the example:
         if t=='0' or t=='1' or t=='2' or t=='3' or t=='4' or
            t=='5' or t=='6' or t=='7' or t=='8' or t=='9' or
            t=='+' or t=='-' then-- any of these should start editing new cell
            start_editing(info, R,C)         -- start new edit
            info.input:handle(event)      -- pass typed char to input
         elseif t=='\r' or t=='\n' then -- let enter key edit the cell
            start_editing(info, R,C)         -- start new edit
         end
      return
      end
   elseif context == 'table' or context == 'row header' or context == 'col header' then
      -- A table event occurred on dead zone in table or on row/column header
         done_editing(info) -- done editing, hide
         return
   end 
end

-- main --------------------------------------------
win = fl.double_window(862, 322, "Fl_Table Spreadsheet")
tabl, info = Spreadsheet(10, 10, win:w()-20, win:h()-20)
--@@ tabl:tab_cell_nav(1) -- enable tab navigation of table cells (instead of fltk widgets)
tabl:tooltip("Use keyboard to navigate cells:\n" ..
                 "Arrow keys or Tab/Shift-Tab")
-- Table rows
tabl:row_header(true)
tabl:row_header_width(70)
tabl:row_resize(true)
tabl:rows(MAX_ROWS+1)      -- +1: leaves room for 'total row'
tabl:row_height_all(25)
-- Table cols
tabl:col_header(true)
tabl:col_header_height(25)
tabl:col_resize(true)
tabl:cols(MAX_COLS+1)      -- +1: leaves room for 'total column'
tabl:col_width_all(70)
-- Show window
win:done()
win:resizable(tabl)
win:show()
return fl.run()
