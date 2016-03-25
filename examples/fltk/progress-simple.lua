#!/usr/bin/env lua
-- MoonFLTK example: progress-simple.lua
--
-- Derived from the FLTK examples/progress-simple.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

-- Button callback
function butt_cb(butt, w)
   -- Deactivate the button
   butt:deactivate() -- prevent button from being pressed again   
   fl.check()   -- give fltk some cpu to gray out button
   -- Make the progress bar (w is the parent window)
   w:begin()   -- add progress bar to it..
   local progress = fl.progress(10,50,200,30)
   progress:minimum(0) -- set progress range to be 0.0 ~ 1.0
   progress:maximum(1)
   progress:color(0x88888800)   -- background color
   progress:selection_color(0x4444ff00) -- progress bar color
   progress:labelcolor(fl.WHITE)-- percent text color
   w:done()  -- end adding to window
   -- Computation loop..
   for t=1,500 do
      progress:value(t/500.0)   -- update progress bar with 0.0 ~ 1.0 value
      local percent = string.format("%d%%", math.floor((t/500.0)*100.0))
      progress:label(percent) -- update progress bar's label
      fl.check()   -- give fltk some cpu to update the screen
      fl.sleep(1e-3)  -- 'your stuff' that's compute intensive
   end
   -- Cleanup
   w:remove(progress)   -- remove progress bar from window
   -- unreference the progress bar, so that it will be GC'ed
   fl.unreference(progress)
   -- let's force GC:
   progress = nil
   collectgarbage()

   butt:activate()  -- reactivate button
   w:redraw()   -- tell window to redraw now that progress removed
end

-- Main
win = fl.window(220,90, arg[0])
butt = fl.button(10,10,100,25,"Press")
butt:callback(butt_cb, win)
win:resizable(win)
win:show()

--[[ uncomment to test garbage collection of the win window.

-- first, remove all references to to the window:
fl.unreference(win) -- unreference it from the Lua registry
butt:argument(nil)  -- this is another reference, as argument to a callback
win = nil           -- the most obvious reference
-- if all the references have been removed, the GC should delete the window
-- and the process should exit:
collectgarbage()

--]]

return fl.run()

