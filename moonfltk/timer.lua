-- The MIT License (MIT)
--
-- Copyright (c) 2016 Stefano Trettel
--
-- Software repository: MoonFLTK, https://github.com/stetre/moonfltk
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.
-- 

--=================================================================================
-- timer.lua - MoonFLTK timer heap
--=================================================================================

local fl = require("moonfltk")
local timer_ = require("moonfltk.timer_")
local timer = {}

local function triggerat(exptime) -- specific for FLTK
   local seconds = exptime - fl.gettime()
   if seconds > 0 then -- set the FLTK timer so to expire at exptime
      fl.set_timeout(seconds, false, timer_.trigger)
   else -- exptime is in the past: trigger the timer module
      timer_.trigger()
   end
end

function timer.init()
-- Init the timer module for FLTK
   timer_.deleteall()
   timer_.init(triggerat, fl.gettime)  
end

timer.create = function(timeout, callback) 
   return timer_.create(timeout, callback) 
end
timer.delete = timer_.delete
timer.deletell = timer_.deleteall
timer.start = timer_.start
timer.stop = timer_.timeout
timer.callback = timer_.callback
timer.isrunning = timer_.isrunning

return timer
