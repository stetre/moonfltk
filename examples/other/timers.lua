#!/usr/bin/env lua
-- MoonFLTK example: timers.lua

fl = require("moonfltk")
timer = require("moonfltk.timer")

-- Init the timer module
timer.init()

tname = {}  -- tname[tid] = timer name
tstart = {} -- tstart[tid] = starting time 

-- Callback for timers' expiration:
function texpired(tid)
   local now = fl.gettime()
   print(string.format("timer %s expired (elapsed time: %g seconds)", 
         tname[tid], now - tstart[tid]))
   -- restart timer:
   tstart[tid] = timer.start(tid)
end

-- Create a few timers:
t1 = timer.create(2, texpired)
tname[t1] = "T1"
t2 = timer.create(3.1, texpired)
tname[t2] = "T2"
t3 = timer.create(4.3, texpired)
tname[t3] = "T3"

-- Start them:
tstart[t1], errmsg = timer.start(t1)
tstart[t2], errmsg = timer.start(t2)
tstart[t3], errmsg = timer.start(t3)

-- Enter the FLTK main loop:
while true do
   fl.wait(0)
end

