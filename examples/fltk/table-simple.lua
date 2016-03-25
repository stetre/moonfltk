#!/usr/bin/env lua
-- MoonFLTK example: table-simple.lua
--
-- Derived from the FLTK examples/table-simple.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

MAX_ROWS = 30
MAX_COLS = 26     -- A-Z

-- Derive a class from Fl_Table
function MyTable(X, Y, W, H, L)
   local self = fl.table_sub(X,Y,W,H,L)
   --  Make our data array, and initialize the table options.
   local data = {}
   for r = 1, MAX_ROWS do
      data[r] = {}
      for c=1, MAX_COLS do
         data[r][c] = (r*1000)+c-1
      end
   end
   -- Rows
   self:rows(MAX_ROWS)     -- how many rows
   self:row_header(true)   -- enable row headers (along left)
   self:row_height_all(20) -- default height of rows
   self:row_resize(false)  -- disable row resizing
   -- Cols
   self:cols(MAX_COLS)     -- how many columns
   self:col_header(true)   -- enable column headers (along top)
   self:col_width_all(80)  -- default width of columns
   self:col_resize(true)   -- enable column resizing

   self:done()       -- end the Fl_Table group

   -- Draw the row/col headings
   --    Make this a dark thin upbox with the text inside.
   --
   local function DrawHeader(s, X, Y, W, H)
      fl.push_clip(X,Y,W,H)
      fl.draw_box('thin up box', X,Y,W,H, self:row_header_color())
      fl.color(fl.BLACK)
      fl.draw(s, X,Y,W,H, fl.ALIGN_CENTER)
      fl.pop_clip()
   end
   
   -- Draw the cell data
   --    Dark gray text on white background with subtle border
   --
   local function DrawData(s, X, Y, W, H)
      fl.push_clip(X,Y,W,H)
      -- Draw cell bg
      fl.color(fl.WHITE) fl.rectf(X,Y,W,H)
      -- Draw cell data
      fl.color(fl.GRAY0) fl.draw(s, X,Y,W,H, fl.ALIGN_CENTER)
      -- Draw box border
      fl.color(self:color()) fl.rect(X,Y,W,H)
      fl.pop_clip()
   end

   -- Handle drawing table's cells
   --     Fl_Table calls this function to draw each visible cell in the table.
   --     It's up to us to use FLTK's drawing functions to draw the cells the way we want.
   --
   self:override_draw_cell(function (_, context, ROW, COL, X, Y, W, H) 
      print(table.concat(
         {"draw_cell: ", context, " r=", ROW, " c=", COL, " x=", X, " y=", Y, " w=", W, " h=", H }))
   print("find cell", self:find_cell('table', 1, 1))

      if context == 'startpage' then -- before page is drawn..
         fl.font(fl.HELVETICA, 16)              -- set the font for our drawing operations
         return 
      end

      if context == 'col header' then -- Draw column headers
        local s = string.format("%c",65+COL-1) -- "A", "B", "C", etc.
        DrawHeader(s,X,Y,W,H)
        return 
      end

      if context == 'row header' then -- Draw row headers
        local s = string.format("%03d:",ROW-1)  -- "001:", "002:", etc
        DrawHeader(s,X,Y,W,H)
        return 
      end
      
      if context == 'cell' then -- Draw data in cells
        local s = string.format("%d",data[ROW][COL])
        DrawData(s,X,Y,W,H)
        return
      end

   end)
   return { self=self, data=data }
end


-- main
win =fl.double_window(900, 400, arg[0])
tab = MyTable(10,10,880,380) -- it is not a good idea to call a variable 'table' in Lua...
win:done()
win:resizable(tab.self)
win:show(arg[0],arg)
return fl.run()

