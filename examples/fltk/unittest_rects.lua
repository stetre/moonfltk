-- MoonFLTK example: unittest_rects.lua
--
-- Derived from the FLTK test/unittest_rects.cxx example (http://www.fltk.org)
--

------- test the rectangle drawing capabilities of this implementation ----------

return function(x, y, w, h)
   local label = "rectangles"
   local self = fl.box_sub(x, y, w, h)
   self:label("testing the fl.rect call\n" ..
     "No red pixels should be visible. " ..
     "If you see bright red lines, or if parts of the green frames are hidden, " ..
     "the rect drawing alignment is off.")
   self:align(fl.ALIGN_INSIDE|fl.ALIGN_BOTTOM|fl.ALIGN_LEFT|fl.ALIGN_WRAP)
   self:box('border box')
   self:override_draw(function()
      self:super_draw()
      local x, y = self:x(), self:y()
      local a, b = x+10, y+10
      fl.color(fl.BLACK) fl.rect(a, b, 100, 100)
      -- testing fl.rect() with positive size
      fl.color(fl.RED)   fl.loop(a+10, b+10, a+40, b+10, a+40, b+40, a+10, b+40)
      fl.color(fl.GREEN) fl.loop(a+ 9, b+ 9, a+41, b+ 9, a+41, b+41, a+ 9, b+41)
      fl.color(fl.GREEN) fl.loop(a+11, b+11, a+39, b+11, a+39, b+39, a+11, b+39)
      fl.color(fl.BLACK) fl.rect(a+10, b+10, 31, 31)
      -- testing fl.rect() with positive size
      fl.color(fl.RED)   fl.loop(a+60, b+60, a+90, b+60, a+90, b+90, a+60, b+90)
      fl.color(fl.GREEN) fl.loop(a+59, b+59, a+91, b+59, a+91, b+91, a+59, b+91)
      fl.color(fl.BLACK) fl.rectf(a+60, b+60, 31, 31)
   end)

   return { widget = self, label = label }
end

