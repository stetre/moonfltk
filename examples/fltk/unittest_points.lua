-- MoonFLTK example: unittest_points.lua
--
-- Derived from the FLTK test/unitest_points.cxx example (http://www.fltk.org)
--

------- test the point drawing capabilities of this implementation ----------

return function(x, y, w, h)
   local label = "drawing points"
   local self = fl.box_sub(x, y, w, h)
   self:label("testing the fl.point call\n" ..
     "You should see four pixels each in black, red, green and blue. " ..
     "Make sure that pixels are not anti-aliased (blurred across multiple pixels)!")
   self:align(fl.ALIGN_INSIDE|fl.ALIGN_BOTTOM|fl.ALIGN_LEFT|fl.ALIGN_WRAP)
   self:box('border box')

   self:override_draw(function ()
      self:super_draw()
      local x, y = self:x(), self:y()
      local a, b = x+10, y+10 
      fl.color(fl.WHITE) fl.rectf(a, b, 90, 90)
      fl.color(fl.BLACK) fl.rect(a, b, 90, 90)
      fl.point(a+10, b+10) fl.point(a+20, b+20)
      fl.point(a+10, b+20) fl.point(a+20, b+10)
      fl.color(fl.RED) a = x+70
      fl.point(a+10, b+10) fl.point(a+20, b+20)
      fl.point(a+10, b+20) fl.point(a+20, b+10)
      fl.color(fl.GREEN) a = x+10 b = y+70
      fl.point(a+10, b+10) fl.point(a+20, b+20)
      fl.point(a+10, b+20) fl.point(a+20, b+10)
      fl.color(fl.BLUE) a = x+70
      fl.point(a+10, b+10) fl.point(a+20, b+20)
      fl.point(a+10, b+20) fl.point(a+20, b+10)
   end)
   return { widget = self, label = label }
end

