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

--=======================================================================
-- timer_.lua - Timers heap                                              
--=======================================================================

local printf = function(...) print(string.format(...)) end
--local printf = function(...) end

local function heap(cmp) -- priority queue
   local cmp = assert(cmp, "missing compare function")
   local self = {}
   return setmetatable(self,
      {
      __index = {
         first = function(self) return self[1] end,
         count = function(self) return #self end,
         insert = function(self, element)
            self[#self + 1] = element
            local i = #self
            local i2, parent
            while i > 1 do
               i2 = i/2
               parent = i2 - (i2)%1 -- floor(i/2)
               if cmp(self[parent],self[i]) then break end
               self[i], self[parent] = self[parent], self[i]
               i = parent
            end
         end,
         delete = function(self)
             -- remove root
            local len = #self
            if len == 0 then return end
            local root
            root, self[1] = self[1], self[len]
            self[len] = nil
            if len > 2 then
               self:heapify()
            end
            return root
         end,
         heapify = function(self, i)
            local cmp = cmp
            local i = i or 1
            local first = i
            local i2 = 2*i
            local left, right = i2 , i2+1
            if left <= #self and cmp(self[left],self[first]) then
               first = left
            end
            if right <= #self and cmp(self[right],self[first]) then
               first = right
            end
            if first ~= i then
               self[i], self[first] = self[first], self[i]
               self:heapify(first)
            end
         end,
      }, -- __index
   })
end

local timer = {} -- module table

local T = {} -- table of created timers (key=tid)
-- T[tid] = { exptime, timeout, callback }, 
-- exptime = 0 if inactive

local function compare(a, b) return a[1] < b[1] end
local active = heap(compare) -- priority queue of active timers
-- entries = { exptime, tid }

local gettime = function() error("gettime not initialized") end
local triggerat = function(t) error("triggerat not initialized") end

local next_tid = 1   -- nexttid to be used

local ADVANCE = 1e-3 
local advance = ADVANCE

function timer.init(triggeratfunc, gettimefunc, adv)
-- triggeratfunc(exptime): noyifies the next expiration time
-- gettimefunc(): must return the current time in seconds (monotonic)
-- adv (in seconds): advance time to check for expired timers 
   triggerat = triggeratfunc
   gettime = gettimefunc
   advance = adv or ADVANCE
end

function timer.deleteall()
   local tid
   for tid in pairs(T) do timer.delete(tid) end
end

function timer.trigger()
-- Must be called from the user module at the next expiration time.
-- Executes the callbacks of all active timers with exptime < tnow.
-- and returns the next exptime, or 0 if there are no active timers.
-- The next expiration time is set by triggerat().
   local tid, tmr, tnow
   local te = active:first()
   while te do
      tnow = gettime()
      if te[1] > tnow then 
         triggerat(te[1]-advance) 
         return
      end
      active:delete()
      tid = te[2]
      tmr = T[tid]
      if not tmr or tmr[1] ~= te[1] then 
         -- timer was deleted, stopped or its timeout changed
         goto continue 
      end
      --printf("%.9f: timer %u expired (exptime = %.9f)",tnow,tid, tmr[1])
      tmr[1] = 0
      tmr[3](tid) -- execute callback
      te = nil
      ::continue::
      te = active:first()
   end
end
      
function timer.create(timeout, callback)
-- creates a new timer and returns timer identifier (tid)
   if not callback or type(callback) ~= "function" then
      return nil, "missing or invalid callback"
   end
   local tid = next_tid
   next_tid = next_tid+1
   T[tid] = { 0 , timeout, callback }
   --printf("created timer %u timeout=%.9f",tid,timeout)
   return tid
end


local function search(tid)
-- search for timer identified by tid
   local tmr = T[tid]
   if tmr then return tmr end
   return nil, string.format("unknown tid=%u",tid)
end


function timer.delete(tid)
-- deletes timer identified by tid
   local tmr, errmsg = search(tid)
   if not tmr then return nil, errmsg end
   tmr[1] = 0 -- stop timer
   T[tid] = nil
end

function timer.start(tid, exptime)
-- Starts (or restarts) timer tid so to expire at exptime.
-- If exptime = nil, the expiration time is now+timeout passed at create()
   local tmr, errmsg = search(tid)
   if not tmr then return nil, errmsg end
   -- overwrites any previous exptime...
   local tstart = gettime()
   tmr[1] = exptime or tstart + tmr[2]
   active:insert({ tmr[1], tid }) -- te
   local te = active:first()
   if te then triggerat(te[1]-advance) end
   --printf("%.9f: starting timer %u exptime = %.9f",gettime(), tid, tmr[1])
   return tstart
end

function timer.stop(tid)
-- stops timer identified by tid
   local tmr, errmsg = search(tid)
   if not tmr then return nil, errmsg end
   tmr[1] = 0
   return true
end

function timer.timeout(tid, timeout)
-- set (opt.) and get timer's timeout
   local tmr, errmsg = search(tid)
   if not tmr then return nil, errmsg end
   if timeout then tmr[2] = timeout end
   return tmr[2]
end

function timer.callback(tid, callback)
-- set (opt.) and get timer's callback
   local tmr, errmsg = search(tid)
   if not tmr then return nil, errmsg end
   if callback then tmr[3] = callback end
   return tmr[3]
end

function timer.isrunning(tid)
-- returns true if timer is running, false otherwise , and the exptime
   local tmr, errmsg = search(tid)
   if not tmr then return nil, errmsg end
   if tmr[1] == 0 then
      return false, math.huge
   end
   return true, tmr[1]
end

return timer
