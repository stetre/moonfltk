-- MoonFLTK example: unittest_circles.lua
--
-- Derived from the FLTK test/unittest_circles.cxx example (http://www.fltk.org)
--

------- test the circle drawing capabilities of this implementation ----------

return function(x, y, w, h)
   local label = "circles and arcs"
   local self = fl.box_sub(x, y, w, h)
   self:label("testing int drawing of circles and ovals (fl.arc, fl.pie)\n" ..
          "No red lines should be visible. " ..
          "If you see bright red pixels, the circle drawing alignment is off. " ..
          "If you see dark red pixels, your system supports anti-aliasing " ..
          "which should be of no concern. " ..
          "The green rectangles should not be overwritten by circle drawings.")
   self:align(fl.ALIGN_INSIDE|fl.ALIGN_BOTTOM|fl.ALIGN_LEFT|fl.ALIGN_WRAP)
   self:box('border box')
   self:override_draw(function()
      self:super_draw()
      local x, y = self:x(), self:y()
      local a, b = x+10, y+10 
      fl.color(fl.BLACK) fl.rect(a, b, 100, 100)
      -- test fl.arc for full circles
      fl.color(fl.GREEN) fl.rect(a+ 9, b+ 9, 33, 33)
      fl.color(fl.RED) fl.xyline(a+24, b+10, a+27) fl.xyline(a+24, b+40, a+27)
      fl.yxline(a+10, b+24, b+27) fl.yxline(a+40, b+24, b+27)
      fl.color(fl.BLACK) fl.arc(a+10, b+10, 31, 31, 0.0, 360.0)
      -- test fl.arc segmet 1
      fl.color(fl.GREEN) fl.rect(a+54, b+ 4, 43, 43)
      fl.rect(a+54, b+4, 18, 18) fl.rect(a+79, b+29, 18, 18)
      fl.color(fl.RED) fl.point(a+55, b+30) fl.point(a+70, b+45)
      fl.point(a+80, b+5) fl.point(a+95, b+20)
      fl.color(fl.BLACK) fl.arc(a+65, b+ 5, 31, 31, -35.0, 125.0)
      -- test fl.arc segmet 2
      fl.color(fl.BLACK) fl.arc(a+55, b+15, 31, 31, 145.0, 305.0)
      -- test fl.pie for full circles
      fl.color(fl.RED) fl.xyline(a+24, b+60, a+27) fl.xyline(a+24, b+90, a+27)
      fl.yxline(a+10, b+74, b+77) fl.yxline(a+40, b+74, b+77)
      fl.color(fl.GREEN) fl.rect(a+ 9, b+59, 33, 33)
      fl.color(fl.BLACK) fl.pie(a+10, b+60, 31, 31, 0.0, 360.0)
      -- test fl.pie segmet 1
      fl.color(fl.GREEN) fl.rect(a+54, b+54, 43, 43)
      fl.rect(a+54, b+54, 18, 18) fl.rect(a+79, b+79, 18, 18)
      fl.point(a+79, b+71) fl.point(a+71, b+79)
      fl.color(fl.RED) fl.point(a+55, b+80) fl.point(a+70, b+95)
      fl.point(a+80, b+55) fl.point(a+95, b+70)
      fl.point(a+81, b+69) fl.point(a+69, b+81)
      fl.color(fl.BLACK) fl.pie(a+65, b+55, 31, 31, -30.0, 120.0)
      -- test fl.pie segmet 2
      fl.color(fl.BLACK) fl.pie(a+55, b+65, 31, 31, 150.0, 300.0)
      ------ oval testing (horizontal squish)
      a = a+120 b = b+0 fl.color(fl.BLACK) fl.rect(a, b, 100, 100)
      fl.color(fl.GREEN)
      fl.rect(a+19, b+9, 63, 33) fl.rect(a+19, b+59, 63, 33)
      fl.color(fl.BLACK)
      fl.arc(a+20, b+10, 61, 31, 0, 360) fl.pie(a+20, b+60, 61, 31, 0, 360)
      ------ oval testing (horizontal squish)
      a = a+120 b = b+0 fl.color(fl.BLACK) fl.rect(a, b, 100, 100)
      fl.color(fl.GREEN)
      fl.rect(a+9, b+19, 33, 63) fl.rect(a+59, b+19, 33, 63)
      fl.color(fl.BLACK)
      fl.arc(a+10, b+20, 31, 61, 0, 360) fl.pie(a+60, b+20, 31, 61, 0, 360)
   end)
   return { widget = self, label = label }
end
