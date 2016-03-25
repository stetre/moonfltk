-- MoonFLTK example: unittest_symbol.lua
--
-- Derived from the FLTK test/unittest_symbol.cxx example (http://www.fltk.org)
--

-- Test symbol rendering
 
local function DrawTextAndBoxes(txt, X, Y)
   local wo, ho = fl.measure(txt, 0, true)
   -- Draw fl.measure() rect
   fl.color(fl.RED)
   fl.rect(X, Y, wo, ho)
   -- ----------------------------------------------------------------------
   -- NOTE: fl.text_extents() currently does not support multiline strings..
   --       until it does, let's leave this out, as we do multiline tests..
   -- -- draw fl.text_extents() glyph bounding box
   --[[
   local dx,dy
   dx, dy, wo, ho = fl.text_extents(txt)
   fl.color(fl.GREEN)
   fl.rect(X+dx, Y+dy, wo, ho)
   --]]

   -- Draw text with symbols enabled
   fl.color(fl.BLACK)
   
   fl.draw(txt, X, Y, 10, 10, fl.ALIGN_INSIDE|fl.ALIGN_TOP|fl.ALIGN_LEFT, nil, true)
end

return function(x, y, w, h)
   local label = "symbol text"
   local self = fl.widget_sub(x, y, w, h)
   self:override_draw(function()
      local x0, y0, w0, h0 = self:xywh() -- origin is current window position for Fl_Box
      fl.push_clip(x0, y0, w0, h0) -- reset local clipping
      -- set the background colour - slightly off-white to enhance the green bounding box
      fl.color(fl.gray_ramp(fl.NUM_GRAY - 3))
      fl.rectf(x0, y0, w0, h0)
      local fsize = 25
      fl.font(fl.HELVETICA, fsize)
      local xx = x0+10
      local yy = y0+10
      DrawTextAndBoxes("Text",xx,yy) yy = yy + fsize+10  -- check no symbols
      DrawTextAndBoxes("@->",xx,yy) yy = yy + fsize+10   -- check symbol alone
      DrawTextAndBoxes("@-> ",xx,yy) yy = yy + fsize+10  -- check symbol with trailing space
      DrawTextAndBoxes("@-> Rt Arrow",xx,yy) yy = yy + fsize+10   -- check symbol at left edge
      DrawTextAndBoxes("Lt Arrow @<-",xx,yy) yy = yy + fsize+10   -- check symbol at right edge
      DrawTextAndBoxes("@-> Rt/Lt @<-",xx,yy) yy = yy + fsize+10  -- check symbol at lt+rt edges
      DrawTextAndBoxes("@@ At/Lt @<-",xx,yy) yy = yy + fsize+10   -- check @@ at left, symbol at right
      DrawTextAndBoxes("@-> Lt/At @@",xx,yy) yy = yy + fsize+10   -- check symbol at left, @@ at right
      DrawTextAndBoxes("@@ At/At @@",xx,yy) yy = yy + fsize+10 -- check @@ at left+right
      xx = x0+200
      yy = y0+10
      DrawTextAndBoxes("Line1\nLine2",xx,yy) yy = yy + (fsize+10)*2 -- check 2 lines, no symbol
      DrawTextAndBoxes("@-> Line1\nLine2 @<-",xx,yy) yy = yy + (fsize+10)*2 -- check 2 lines, lt+rt symbols
      DrawTextAndBoxes("@-> Line1\nLine2\nLine3 @<-",xx,yy) yy = yy + (fsize+10)*3 -- check 3 lines, lt+rt symbols
      DrawTextAndBoxes("@@@@",xx,yy) yy = yy + (fsize+10)   -- check abutting @@'s
      DrawTextAndBoxes("@@ @@",xx,yy) yy = yy + (fsize+10)   -- check @@'s with space sep

      fl.font(fl.HELVETICA, 14)
      fl.color(fl.RED)
      fl.draw("fl.measure bounding box in RED", x0+10,y0+h0-20)
    
    fl.pop_clip() -- remove the local clip
   end)
   return { widget = self, label = label }
end

