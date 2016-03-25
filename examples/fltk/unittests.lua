#!/usr/bin/env lua
-- MoonFLTK example: unittests.lua
--
-- Derived from the FLTK test/unittests.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

-- WINDOW/WIDGET SIZES
MAINWIN_W = 700   -- main window w()
MAINWIN_H = 400   -- main window h()
BROWSER_X = 10    -- browser x()
BROWSER_Y = 25    -- browser y()
BROWSER_W = 150   -- browser w()
BROWSER_H = MAINWIN_H-35      -- browser h()
TESTAREA_X = (BROWSER_W + 20) -- test area x()
TESTAREA_Y = 25   -- test area y()
TESTAREA_W = (MAINWIN_W - BROWSER_W - 30) -- test area w()
TESTAREA_H = BROWSER_H        -- test area h()


function MainWindow(w,h,l)
   local self = fl.double_window_sub(w, h, l)
   local info = { self = self, fTestAlignment = false }

   -- this code is used by the viewport alignment test
   local function drawAlignmentIndicators()
      local w, h = self:w(), self:h()
      local sze = 16
      -- top left corner
      fl.color(fl.GREEN) fl.yxline(0, sze, 0, sze)
      fl.color(fl.RED)   fl.yxline(-1, sze, -1, sze)
      fl.color(fl.WHITE) fl.rectf(3, 3, sze-2, sze-2)
      fl.color(fl.BLACK) fl.rect(3, 3, sze-2, sze-2)
      -- bottom left corner
      fl.color(fl.GREEN) fl.yxline(0, h-sze-1, h-1, sze)
      fl.color(fl.RED)   fl.yxline(-1, h-sze-1, h, sze)
      fl.color(fl.WHITE) fl.rectf(3, h-sze-1, sze-2, sze-2)
      fl.color(fl.BLACK) fl.rect(3, h-sze-1, sze-2, sze-2)
      -- bottom right corner
      fl.color(fl.GREEN) fl.yxline(w-1, h-sze-1, h-1, w-sze-1)
      fl.color(fl.RED)   fl.yxline(w, h-sze-1, h, w-sze-1)
      fl.color(fl.WHITE) fl.rectf(w-sze-1, h-sze-1, sze-2, sze-2)
      fl.color(fl.BLACK) fl.rect(w-sze-1, h-sze-1, sze-2, sze-2)
      -- top right corner
      fl.color(fl.GREEN) fl.yxline(w-1, sze, 0, w-sze-1)
      fl.color(fl.RED)   fl.yxline(w, sze, -1, w-sze-1)
      fl.color(fl.WHITE) fl.rectf(w-sze-1, 3, sze-2, sze-2)
      fl.color(fl.BLACK) fl.rect(w-sze-1, 3, sze-2, sze-2)
   end

   function info.testAlignment(v)
      -- print("testAlignment", v)
      info.fTestAlignment = v
      self:redraw()
   end

   self:override_draw(function()
      self:super_draw()
      if info.fTestAlignment then
         drawAlignmentIndicators()
      end
   end)
   return self, info
end


-- The unit tests:
unittests = { 
   "unittest_about", 
   "unittest_points",
   "unittest_lines",
   "unittest_rects",
   "unittest_circles",
   "unittest_text",
   "unittest_symbol",
   "unittest_images",
   "unittest_viewport",
   "unittest_scrollbarsize",
   "unittest_schemes",
}


Test = {} -- holds the test info (indexed by browser:data(line))

-- callback whenever the browser value changes
function Browser_CB()
   for line=1, browser:size() do
      local t = Test[browser:data(line)]
      if browser:selected(line) then t.widget:show() else t.widget:hide() end
   end
end

-- this is the main call. It creates the window and adds all previously
-- registered tests to the browser widget.
--@@ fl.args(argc,argv)
fl.visual('rgb')
mainwin, mainwininfo = MainWindow(MAINWIN_W, MAINWIN_H, "Fltk Unit Tests")
browser = fl.hold_browser(BROWSER_X, BROWSER_Y, BROWSER_W, BROWSER_H, "Unit Tests")
browser:align('top', 'left')
browser:when('changed')
browser:callback(Browser_CB)


-- associate the various unit tests with browser lines
for i, test in ipairs(unittests) do
   local create = require(test)
   mainwin:begin()
   local t = create(TESTAREA_X, TESTAREA_Y, TESTAREA_W, TESTAREA_H, mainwininfo.testAlignment)
   mainwin:done()
   Test[#Test+1] = t
   browser:add(t.label, #Test)
end

----
mainwin:resizable(mainwin)
mainwin:show(argc,argv)
-- Select first test in browser, and show that test.
browser:select(1)
Browser_CB(browser,0)
return fl.run()

