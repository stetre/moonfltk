-- MoonFLTK example: unittest_scrollbarsize.lua
--
-- Derived from the FLTK test/unittest_scrollbarsize.cxx example (http://www.fltk.org)
--

-- Test new 1.3.x global vs. local scrollbar sizing

function MyTable(X,Y,W,H,L) 
   local self = fl.table_sub(X,Y,W,H,L)
   -- Make our data array, and initialize the table options.
   self:rows(13)                   -- how many rows
   self:row_height_all(10)         -- default height of rows
   -- Cols
   self:cols(13)                   -- how many columns
   self:col_width_all(10)          -- default width of columns
   self:done()       -- end the Fl_Table group

   self:override_draw_cell(function(_, context, ROW, COL, X, Y, W, H)
      -- Handle drawing table's cells
      --     Fl_Table calls this function to draw each visible cell in the table.
      --     It's up to us to use FLTK's drawing functions to draw the cells the way we want.
      if context == 'startpage' then -- before page is drawn..
         fl.font(fl.HELVETICA, 8)    -- set font for drawing operations
      elseif context == 'cell' then  -- Draw data in cells
         local s = string.format("%c", 63+ROW+COL) -- 63='A'-2 (ROW and COL are 1-based)
         fl.push_clip(X,Y,W,H)
         -- Draw cell bg
         fl.color(fl.WHITE) fl.rectf(X,Y,W,H)
         -- Draw cell data
         fl.color(fl.GRAY0) fl.draw(s, X,Y,W,H, fl.ALIGN_CENTER)
         -- Draw box border
         fl.color(self:color()) fl.rect(X,Y,W,H)
         fl.pop_clip()
      end
   end)

   return self
end


local function makebrowser(X, Y, W, H, L)
   local b = fl.browser(X,Y,W,H,L)
   b:type('multi')
   b:align(fl.ALIGN_TOP)
   b:add("Papa")     b:add("Delta") b:add("Hotel")
        b:add("Long entry will show h-bar")
   b:add("Charlie")  b:add("Echo")  b:add("Foxtrot")
   b:add("Golf")     b:add("Lima")  b:add("Victor")
   b:add("Alpha")    b:add("Xray")  b:add("Yankee")
   b:add("Oscar")    b:add("India") b:add("Juliet")
   b:add("Kilo")     b:add("Mike")  b:add("Sierra")
   b:add("November") b:add("Tango") b:add("Quebec")
   b:add("Bravo")    b:add("Romeo") b:add("Uniform")
   b:add("Whisky")   b:add("Zulu")
   b:add("Papa")     b:add("Delta") b:add("Hotel")
   b:add("Charlie")  b:add("Echo")  b:add("Foxtrot")
   b:add("Golf")     b:add("Lima")  b:add("Victor")
   b:add("Alpha")    b:add("Xray")  b:add("Yankee")
   b:add("Oscar")    b:add("India") b:add("Juliet")
   b:add("Kilo")     b:add("Mike")  b:add("Sierra")
   b:add("November") b:add("Tango") b:add("Quebec")
   b:add("Bravo")    b:add("Romeo") b:add("Uniform")
   b:add("Whisky")   b:add("Zulu")
   return b
end

local function maketree(X,Y,W,H,L)
   local b = fl.tree(X,Y,W,H,L)
   b:type('multi')
   b:align(fl.ALIGN_TOP)
   b:add("Papa")     b:add("Delta") b:add("Hotel")
        b:add("Long entry will show h-bar")
   b:add("Charlie")  b:add("Echo")  b:add("Foxtrot")
   b:add("Golf")     b:add("Lima")  b:add("Victor")
   b:add("Alpha")    b:add("Xray")  b:add("Yankee")
   b:add("Oscar")    b:add("India") b:add("Juliet")
   b:add("Kilo")     b:add("Mike")  b:add("Sierra")
   b:add("November") b:add("Tango") b:add("Quebec")
   b:add("Bravo")    b:add("Romeo") b:add("Uniform")
   b:add("Whisky")   b:add("Zulu")
   return b
end


local function maketable(X,Y,W,H,L)
   local mta = MyTable(X,Y,W,H,L)
   mta:align(fl.ALIGN_TOP)
   mta:done()
   return mta
end

ABI = fl._FLTK_ABI_VERSION >= 10301

local function slide_cb(slider, info)
   local label = slider:label()
   local val = math.floor(slider:value())
   if label == "A: Scroll Size" then
      info.brow_a:scrollbar_size(val)
      info.tree_a:scrollbar_size(val)
      if ABI then info.table_a:scrollbar_size(val) end
   else
      fl.scrollbar_size(val)
   end
   slider:window():redraw()
end


return function(X, Y, W, H)
   local label = "scrollbar size"
   local self = fl.group(X, Y, W, H)
   local info = { widget = self, label = label }

   self:begin()
   --      _____________    _______________
   --     |_____________|  |_______________|
   --                                                ---   -----  <-- tgrpy
   --       brow_a      brow_b      brow_c            | 14   | 
   --     ----------  ----------  ----------         ---     |    <-- browy
   --     |        |  |        |  |        |          |browh |
   --     |        |  |        |  |        |          |      |
   --     ----------  ----------  ----------         ---   tgrph 
   --                                                 |      |
   --       tree_a      tree_b      tree_c            | 20   | 
   --     ----------  ----------  ----------         ---     |    <-- treey
   --     |        |  |        |  |        |          |treeh |
   --     |        |  |        |  |        |          |      |
   --     ----------  ----------  ----------         ---     |
   --                                                 |      |
   --      table_a     table_b     table_c            | 20   | 
   --     ----------  ----------  ----------         ---     |    <-- tabley
   --     |        |  |        |  |        |          |tableh|
   --     |        |  |        |  |        |          |      |
   --     ----------  ----------  ----------         ---  ------
   --  etc..
   local tgrpy = Y+30
   local tgrph = H-130
   local browy = tgrpy+14
   local browh = math.floor(tgrph/3 - 20)
   local treey = browy + browh + 20
   local treeh = browh
   local tabley = treey + treeh + 20
   local tableh = browh
   info.brow_a = makebrowser(X+ 10,browy,100,browh,"Browser A")
   info.brow_b = makebrowser(X+120,browy,100,browh,"Browser B")
   info.brow_c = makebrowser(X+230,browy,100,browh,"Browser C")
   info.tree_a = maketree(X+ 10,treey,100,treeh,"Tree A")
   info.tree_b = maketree(X+120,treey,100,treeh,"Tree B")
   info.tree_c = maketree(X+230,treey,100,treeh,"Tree C")
   info.table_a = maketable(X+ 10,tabley,100,tableh,"Table A")
   info.table_b = maketable(X+120,tabley,100,tableh,"Table B")
   info.table_c = maketable(X+230,tabley,100,tableh,"Table C")
   local slide_glob = fl.value_slider(X+100,Y,100,18,"Global Scroll Size")
   info.slide_glob = slide_glob
   slide_glob:value(16)
   slide_glob:type('horizontal')
   slide_glob:align(fl.ALIGN_LEFT)
   slide_glob:range(0.0, 30.0)
   slide_glob:step(1.0)
   slide_glob:callback(slide_cb, info)
   slide_glob:labelsize(12)
   local slide_browa = fl.value_slider(X+350,Y,100,18,"A: Scroll Size")
   info.slide_browa = slide_browa 
   slide_browa:value(0)
   slide_browa:type('horizontal')
   slide_browa:align(fl.ALIGN_LEFT)
   slide_browa:range(0.0, 30.0)
   slide_browa:step(1.0)
   slide_browa:callback(slide_cb, info)
   slide_browa:labelsize(12)
   self:done()
   self:label("Verify global scroll sizing and per-widget scroll sizing.\n"..
       "Scrollbar's size should change interactively as size sliders are changed.\n"..
            "Changing 'Global Scroll Size' should affect all three browser's scrollbars UNLESS\n"..
       "the 'A: Scroll Size' slider is changed, in which case its value will take precedence\n" ..
       (ABI and "for the 'A' group of widgets."
         or "for the 'A' group of widgets. (NOTE: 'table_a' does not currently support this)"))
   self:labelsize(10)
   self:align(fl.ALIGN_INSIDE|fl.ALIGN_BOTTOM|fl.ALIGN_LEFT|fl.ALIGN_WRAP)
   return info
end

