-- MoonFLTK example: unittest_text.lua
--
-- Derived from the FLTK test/unittest_text.cxx example (http://www.fltk.org)
--

--- fl.text_extents() tests -----------------------------------------------

local function DrawTextAndBoxes(text, X, Y)
   -- First, we draw the bounding boxes (fl.measure and fl.text_extents)
   -- draw fl.measure() typographical bounding box
   local wo, ho = fl.measure(text)
   local desc = fl.descent()
   fl.color(fl.RED)
   fl.rect(X, Y-ho+desc, wo, ho)
   -- draw fl.text_extents() glyph bounding box
   local dx, dy
   dx, dy, wo, ho  = fl.text_extents(text)
   fl.color(fl.GREEN)
   fl.rect(X+dx, Y+dy, wo, ho)
   -- Then we draw the text to show how it fits insode each of the two boxes
   fl.color(fl.BLACK)
   fl.draw(text, X, Y)
end

return function(x, y, w, h)
   local label = "rendering text"
   local self = fl.widget_sub(x, y, w, h)

   self:override_draw(function()
      local x0, y0, w0, h0 = self:xywh() -- origin is current window position for Fl_Box
      fl.push_clip(x0, y0, w0, h0) -- reset local clipping
      -- set the background colour - slightly off-white to enhance the green bounding box
      fl.color(fl.gray_ramp(fl.NUM_GRAY - 3))
      fl.rectf(x0, y0, w0, h0)

      fl.font(fl.HELVETICA, 30)
      local xx = x0+55
      local yy = y0+40
      DrawTextAndBoxes("!abcdeABCDE\"#A", xx, yy) yy = yy + 50 -- mixed string
      DrawTextAndBoxes("oacs",     xx, yy) xx = xx + 100    -- small glyphs
      DrawTextAndBoxes("qjgIPT",   xx, yy) yy = yy + 50 xx = xx - 100   -- glyphs with descenders
      DrawTextAndBoxes("````````", xx, yy) yy = yy + 50     -- high small glyphs
      DrawTextAndBoxes("--------", xx, yy) yy = yy + 50     -- mid small glyphs
      DrawTextAndBoxes("________", xx, yy) yy = yy + 50     -- low small glyphs

      fl.font(fl.HELVETICA, 14)
      fl.color(fl.RED)  fl.draw("fl.measure bounding box in RED", xx, yy) yy = yy + 20
      fl.color(fl.GREEN) fl.draw("fl.text_extents bounding box in GREEN", xx, yy)
      fl.color(fl.BLACK)
      xx = x0 + 10  yy = yy + 30
      fl.draw("NOTE: On systems with text anti-aliasing (e.g. OSX Quartz)", xx, yy)
      w0, h0 = fl.measure("NOTE: ")
      xx = xx + w0 yy = yy + h0
      fl.draw("text may leak slightly outside the fl.text_extents()", xx, yy)
      fl.pop_clip() -- remove the local clip
   end)
   return { widget = self, label = label }
end
