#!/usr/bin/env lua
-- MoonFLTK example: table.lua
--
-- Derived from the FLTK test/table.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

-- Exercise all aspects of the Fl_Table_Row widget

-- Simple demonstration class to derive from Fl_Table_Row
function DemoTable(x,y,w,h,l)
   local self = fl.table_row_sub(x,y,w,h,l)
   local info = { self = self }
   info.cell_bgcolor = fl.WHITE  -- color of cell's bg color
   info.cell_fgcolor = fl.BLACK  -- color of cell's fg color

   self:callback(event_callback, info)

   -- FLTK does not know how to draw a cell, so we have to override the draw_cell
   -- method with a function that does it.
   -- We override it with a function that calls my_draw_cell(), that does the job
   -- (notice that by doing so we can also add the info parameter).
   self:override_draw_cell(function (_, context, r, c, x, y, w, h)
      my_draw_cell(info, context, r, c, x, y, w, h) end)
   self:done()

   return self, info
end


function my_draw_cell(info, context, R, C, X, Y, W, H)
-- This function does the job for our draw_cell() method.
-- The draw_cell() method is called by FLTK whenever it has to draw cells, headers,
-- and in a few other situations. The context argument tells us what particular
-- 'situation' caused the call. For example, context='startpage' means that FLTK
-- is about to redraw the table, context='cell' means that FLTK wants us to draw a
-- cell, context='row header' means that it wants us to draw a cell of the row header,
-- and so on.
-- The other parameters tell us what cell we have to draw (R,C), where to draw it (X,Y)
-- and how big (W,H).
   local s = string.format("%d/%d", R, C)    -- text for each cell
   local self = info.self

   if context == 'startpage' then
      fl.font(fl.HELVETICA, 16)
      return
   elseif context == 'col header' then
      fl.push_clip(X, Y, W, H)
      fl.draw_box('thin up box', X, Y, W, H, self:col_header_color())
      fl.color(fl.BLACK)
      fl.draw(s, X, Y, W, H, fl.ALIGN_CENTER)
      fl.pop_clip()
      return
   elseif context == 'row header' then
      fl.push_clip(X, Y, W, H)
      fl.draw_box('thin up box', X, Y, W, H, self:row_header_color())
      fl.color(fl.BLACK)
      fl.draw(s, X, Y, W, H, fl.ALIGN_CENTER)
      fl.pop_clip()
      return
   elseif context == 'cell' then
      fl.push_clip(X, Y, W, H)
      -- BG COLOR
      fl.color(self:row_selected(R) and self:selection_color() or info.cell_bgcolor)
      fl.rectf(X, Y, W, H)
      -- TEXT
      fl.color(info.cell_fgcolor)
      fl.draw(s, X, Y, W, H, fl.ALIGN_CENTER)
      -- BORDER
      fl.color(self:color()) 
      fl.rect(X, Y, W, H)
      fl.pop_clip()
      return
   elseif context == 'table' then
      print("TABLE CONTEXT CALLED")
      return
   else -- if context == 'endpage' or context == 'rc resize' or context == 'none' then
      return
   end
end


function event_callback(self, info)
-- This is the event callback for our table widget. It is called by FLTK whenever
-- someone clicks on different parts of the table.
-- Logically, the callback should pass us detailed information about the event that
-- caused it (e.g. the user clicked on the cell R=21 C=15).
-- Since the generic callback for widgets has only standard arguments, we have to
-- retrieve those informations by ourselves using the table:callback_xxx() methods.
-- In this example we just retrieve the callback information and print it on stdout,
-- but in a real world application we can do anything we like, even control other
-- widgets or launch a shuttle to Mars (if we have a function that does it ;-).
   local R, C = self:callback_row(), self:callback_col()
   local context = self:callback_context()
   local label = self:label() or "?"
   print(string.format("'%s' callback: " ..
            "Row=%d Col=%d Context=%s Event=%s InteractiveResize? %s\n",
      label, R, C, context, fl.event(), tostring(self:is_interactive_resize())))
end

-- Now that our table widget is defined, we define a few other widgets to control its
-- properties. These widgets control the table widget by calling its methods from
-- within their own callback.
-- In the 'main' section below we'll create a window containing the table widget itself
-- and these control widgets.

function setrows_cb(_, inp)
   local rows = tonumber(inp:value())
   if rows < 0 then rows = 0 end
   G_table:rows(rows)
end

function setcols_cb(_, inp)
   local cols = tonumber(inp:value())
   if cols < 0 then cols = 0 end
   G_table:cols(cols)
end

function setrowheader_cb(_, check)
   G_table:row_header(check:value())
end

function setcolheader_cb(_, check)
   G_table:col_header(check:value())
end

function setrowresize_cb(_, check)
   G_table:row_resize(check:value())
end

function setcolresize_cb(_, check)
   G_table:col_resize(check:value())
end

function setpositionrow_cb(_, inp)
   local toprow = tonumber(inp:value())
   if toprow < 0 or toprow >= G_table:rows() then
      fl.alert("Must be in range 0 thru #rows")
   else
      G_table:row_position(toprow)
   end
end

function setpositioncol_cb(_, inp)
   local leftcol = tonumber(inp:value())
   if leftcol < 0 or leftcol >= G_table:cols() then
      fl.alert("Must be in range 0 thru #cols") 
   else
      G_table:col_position(leftcol)
   end
end

function setrowheaderwidth_cb(_, inp)
   local val = tonumber(inp:value())
   if val < 1 then val = 1 inp:value("1") end
   G_table:row_header_width(val)
end

function setcolheaderheight_cb(_, inp)
   local val = tonumber(inp:value())
   if val < 1 then val = 1 inp:value("1") end
   G_table:col_header_height(val)
end

function setrowheadercolor_cb(_, inp)
   local val = tonumber(inp:value())
   if val < 0 then fl.alert("Must be a color >0")
   else G_table:row_header_color(val) 
   end
end

function setcolheadercolor_cb(_, inp)
   local val = tonumber(inp:value())
   if val < 0 then fl.alert("Must be a color >0")
   else G_table:col_header_color(val)
   end
end

function setrowheightall_cb(_, inp)
   local val = tonumber(inp:value())
   if val < 0 then val = 0 inp:value("0") end
   G_table:row_height_all(val)
end

function setcolwidthall_cb(_, inp)
   local val = tonumber(inp:value())
   if val < 0 then val = 0 inp:value("0") end
   G_table:col_width_all(val)
end

function settablecolor_cb(_, inp)
   local val = tonumber(inp:value())
   if val < 0 then fl.alert("Must be a color >0") 
   else G_table:color(val)
   end
   G_table:redraw()
end

function setcellfgcolor_cb(_, inp)
   local val = tonumber(inp:value())
   if val < 0 then fl.alert("Must be a color >0")
   else G_info.cell_fgcolor = val
   end
   G_table:redraw()
end

function setcellbgcolor_cb(_, inp)
   local val = tonumber(inp:value())
   if val < 0 then fl.alert("Must be a color >0")
   else G_info.cell_bgcolor = val
   end
   G_table:redraw()
end

function tablebox_choice_cb(_, boxtype)
   G_table:table_box(boxtype)
   G_table:redraw()
end

function widgetbox_choice_cb(_, boxtype)
   G_table:box(boxtype)
   G_table:resize(G_table:x(), G_table:y(), G_table:w(), G_table:h())
end

function type_choice_cb(_, selectmode)
   G_table:type(selectmode)
end

function box_choices(m, cb)
   local boxtypes = { "no box", "flat box", "up box", "down box", "up frame", 
            "down frame", "thin up box", "thin down box", "thin up frame", 
            "thin down frame", "engraved box", "embossed box", "engraved frame", 
            "embossed frame", "border box", "shadow box", "border frame", }
   for _, bt in ipairs(boxtypes) do
      m:add(bt, 0, cb, bt)
   end
end   

function type_choices(m)
   m:add('none', 0, type_choice_cb, 'none')
   m:add('single', 0, type_choice_cb, 'single')
   m:add('multi', 0, type_choice_cb, 'multi')
end

-- main --------------------------------------------------------
win = fl.window(900, 730, arg[0])

-- GLOBAL TABLE WIDGET
G_table, G_info = DemoTable(20, 20, 860, 460, "Demo")
G_table:selection_color(fl.YELLOW)
G_table:when(fl.WHEN_RELEASE|fl.WHEN_CHANGED)
G_table:table_box('no box')
G_table:col_resize_min(4)
G_table:row_resize_min(4)

-- ROWS
G_table:row_header(true)
G_table:row_header_width(60)
G_table:row_resize(true)
G_table:rows(500)
G_table:row_height_all(20)

-- COLS
G_table:cols(500)
G_table:col_header(true)
G_table:col_header_height(25)
G_table:col_resize(true)
G_table:col_width_all(80)

-- Add children to window
win:begin()

-- ROW
setrows = fl.input(150, 500, 120, 25, "Rows")
setrows:labelsize(12)
setrows:value(tostring(G_table:rows()))
setrows:callback(setrows_cb, setrows)
setrows:when(fl.WHEN_RELEASE)

rowheightall = fl.input(400, 500, 120, 25, "Row Height")
rowheightall:labelsize(12)
rowheightall:value(tostring(G_table:row_height(1)))
rowheightall:callback(setrowheightall_cb, rowheightall)
rowheightall:when(fl.WHEN_RELEASE)

positionrow = fl.input(650, 500, 120, 25, "Row Position")
positionrow:labelsize(12)
positionrow:value("1")
positionrow:callback(setpositionrow_cb, positionrow)
positionrow:when(fl.WHEN_RELEASE)

-- COL
setcols = fl.input(150, 530, 120, 25, "Cols")
setcols:labelsize(12)
setcols:value(tostring(G_table:cols()))
setcols:callback(setcols_cb, setcols)
setcols:when(fl.WHEN_RELEASE)

colwidthall = fl.input(400, 530, 120, 25, "Col Width")
colwidthall:labelsize(12)
colwidthall:value(tostring(G_table:col_width(1)))
colwidthall:callback(setcolwidthall_cb, colwidthall)
colwidthall:when(fl.WHEN_RELEASE)

positioncol = fl.input(650, 530, 120, 25, "Col Position")
positioncol:labelsize(12)
positioncol:value("1")
positioncol:callback(setpositioncol_cb, positioncol)
positioncol:when(fl.WHEN_RELEASE)

-- ROW HEADER
rowheaderwidth = fl.input(150, 570, 120, 25, "Row Header Width")
rowheaderwidth:labelsize(12)
rowheaderwidth:value(tostring(G_table:row_header_width()))
rowheaderwidth:callback(setrowheaderwidth_cb, rowheaderwidth)
rowheaderwidth:when(fl.WHEN_RELEASE)

rowheadercolor = fl.input(400, 570, 120, 25, "Row Header Color")
rowheadercolor:labelsize(12)
rowheadercolor:value(tostring(G_table:row_header_color()))
rowheadercolor:callback(setrowheadercolor_cb, rowheadercolor)
rowheadercolor:when(fl.WHEN_RELEASE)

rowheader = fl.check_button(550, 570, 120, 25, "Row Headers?")
rowheader:labelsize(12)
rowheader:callback(setrowheader_cb, rowheader)
rowheader:value(G_table:row_header())

rowresize = fl.check_button(700, 570, 120, 25, "Row Resize?")
rowresize:labelsize(12)
rowresize:callback(setrowresize_cb, rowresize)
rowresize:value(G_table:row_resize())

-- COL HEADER
colheaderheight = fl.input(150, 600, 120, 25, "Col Header Height")
colheaderheight:labelsize(12)
colheaderheight:value(tostring(G_table:col_header_height()))
colheaderheight:callback(setcolheaderheight_cb, colheaderheight)
colheaderheight:when(fl.WHEN_RELEASE)

colheadercolor = fl.input(400, 600, 120, 25, "Col Header Color")
colheadercolor:labelsize(12)
colheadercolor:value(tostring(G_table:col_header_color()))
colheadercolor:callback(setcolheadercolor_cb, colheadercolor)
colheadercolor:when(fl.WHEN_RELEASE)

colheader = fl.check_button(550, 600, 120, 25, "Col Headers?")
colheader:labelsize(12)
colheader:callback(setcolheader_cb, colheader)
colheader:value(G_table:col_header())

colresize = fl.check_button(700, 600, 120, 25, "Col Resize?")
colresize:labelsize(12)
colresize:callback(setcolresize_cb, colresize)
colresize:value(G_table:col_resize())

tablebox = fl.choice(150, 640, 120, 25, "Table Box")
tablebox:labelsize(12)
tablebox:textsize(12)
box_choices(tablebox, tablebox_choice_cb)
tablebox:value('no box')

widgetbox = fl.choice(150, 670, 120, 25, "Widget Box")
widgetbox:labelsize(12)
widgetbox:textsize(12)
box_choices(widgetbox, widgetbox_choice_cb)
widgetbox:value('down frame')

tablecolor = fl.input(400, 640, 120, 25, "Table Color")
tablecolor:labelsize(12)
tablecolor:value(tostring(G_table:color()))
tablecolor:callback(settablecolor_cb, tablecolor)
tablecolor:when(fl.WHEN_RELEASE)

cellbgcolor = fl.input(400, 670, 120, 25, "Cell BG Color")
cellbgcolor:labelsize(12)
cellbgcolor:value(tostring(G_info.cell_bgcolor))
cellbgcolor:callback(setcellbgcolor_cb, cellbgcolor)
cellbgcolor:when(fl.WHEN_RELEASE)

cellfgcolor = fl.input(400, 700, 120, 25, "Cell FG Color")
cellfgcolor:labelsize(12)
cellfgcolor:value(tostring(G_info.cell_fgcolor))
cellfgcolor:callback(setcellfgcolor_cb, cellfgcolor)
cellfgcolor:when(fl.WHEN_RELEASE)

ttype = fl.choice(650, 640, 120, 25, "Type")
ttype:labelsize(12)
ttype:textsize(12)
type_choices(ttype)
ttype:value('multi')

win:done()
win:resizable(G_table)
win:show(arg[0], arg)

return fl.run()
