-- MoonFLTK example: unittest_viewport.lua
--
-- Derived from the FLTK test/unittest_viewport.cxx example (http://www.fltk.org)
--

------- test viewport clipping ----------

return function(x, y, w, h, testalignment)
   local label = "viewport test"
   local self = fl.box_sub(x, y, w, h)
   self:label("Testing Viewport Alignment\n\n" ..
          "Only green lines should be visible.\n" ..
          "If red lines are visible in the corners of this window,\n" ..
          "your viewport alignment and clipping is off.\n" ..
          "If there is a space between the green lines and the window border,\n" ..
          "the viewport is off, but some clipping may be working.\n" ..
          "Also, your window size may be off to begin with.")
   self:align(fl.ALIGN_INSIDE|fl.ALIGN_CENTER|fl.ALIGN_WRAP)
   self:box('border box')
   self:override_show(function() 
      self:super_show()
      testalignment(true)
   end)
   self:override_hide(function() 
      self:super_hide()
      testalignment(false)
   end)

   return { widget = self, label = label }
end

