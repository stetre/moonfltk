-- MoonFLTK example: unittest_lines.lua
--
-- Derived from the FLTK test/unittest_lines.cxx example (http://www.fltk.org)
--

------- test the line drawing capabilities of this implementation ----------

return function(x, y, w, h)
   local label = "drawing lines"
   local self = fl.box_sub(x, y, w, h)
   self:label("testing the integer based fl.line calls\n" ..
     "No red pixels should be visible.\n" ..
     "If you see bright red pixels, the line drawing alignment is off,\n" ..
     "or the last pixel in a line does not get drawn.\n" ..
     "If you see dark red pixels, anti-aliasing must be switched off.")
   self:align(fl.ALIGN_INSIDE|fl.ALIGN_BOTTOM|fl.ALIGN_LEFT|fl.ALIGN_WRAP)
   self:box('border box')
   self:override_draw(function()
      self:super_draw()
      local x, y = self:x(), self:y()
      local a, b = x+10, y+10 
      fl.color(fl.BLACK) fl.rect(a, b, 100, 100)
      -- testing fl.xyline(x, y, x1)
      fl.color(fl.RED) fl.point(a+10, b+10) fl.point(a+20, b+10)
      fl.color(fl.BLACK) fl.xyline(a+10, b+10, a+20)
      -- testing fl.xyline(x, y, x1, y2)
      fl.color(fl.RED) fl.point(a+10, b+20) fl.point(a+20, b+20)
      fl.point(a+20, b+30)
      fl.color(fl.BLACK) fl.xyline(a+10, b+20, a+20, b+30)
      -- testing fl.xyline(x, y, x1, y2, x3)
      fl.color(fl.RED) fl.point(a+10, b+40) fl.point(a+20, b+40)
      fl.point(a+20, b+50) fl.point(a+30, b+50)
      fl.color(fl.BLACK) fl.xyline(a+10, b+40, a+20, b+50, a+30)
      --+++ add testing for the fl.yxline commands!
      -- testing fl.loop(x,y, x,y, x,y, x, y)
      fl.color(fl.RED) fl.point(a+60, b+60) fl.point(a+90, b+60)
      fl.point(a+60, b+90) fl.point(a+90, b+90)
      fl.color(fl.BLACK)
      fl.loop(a+60, b+60, a+90, b+60, a+90, b+90, a+60, b+90)
   end)
   return { widget = self, label = label }
end
