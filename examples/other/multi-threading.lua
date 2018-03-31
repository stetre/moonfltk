#!/usr/bin/env lua
-- MoonFLTK example: multi-threading.lua

-- Demonstrates fl.await and fl.thread_message together with
-- the LuaLanes multi-threading library (https://github.com/LuaLanes/lanes).

local lanes = require"lanes".configure()
local fl    = require"moonfltk"

fl.lock() -- "start" the FLTK lock mechanism
          -- this is needed for initializing fl.await and fl.thread_message

local START_NUMBER = 15
local DEBUG        = false
local TRACE        = DEBUG and print or function() end

local linda1 = lanes.linda()

local function background_code(threadName, initialSleepSeconds)
    -- This function together with simple upvalues is serialized and 
    -- transfered to another thread, see the LuaLanes documentation 
    -- for more details: http://lualanes.github.io/lanes/
    TRACE(threadName..": started.")
    local lanes = require"lanes"    -- module cannot be serialized
    local fl    = require"moonfltk" -- module cannot be serialized
    lanes.sleep(initialSleepSeconds)
    for i = START_NUMBER, 0, -1 do
        linda1:send(threadName, i)
        fl.awake("awake from " .. threadName)
        if i > 0 then
            lanes.sleep(1)
        end
    end
    TRACE(threadName..": sending quit")
    linda1:send("quit", threadName)
    fl.awake("awake from " .. threadName.." for quit")
end

local createThread = lanes.gen("*", background_code)

local thread1 = createThread("thread1", 2)
local thread2 = createThread("thread2", 2.5)

local win = fl.window(440, 180, arg[0])
local box = fl.box(20, 40, 400, 100, "Hello, World!");

box:box('up box')
box:labelfont(fl.BOLD)
box:labelsize(12)
box:align(fl.ALIGN_INSIDE | fl.ALIGN_LEFT | fl.ALIGN_TOP | fl.ALIGN_WRAP)
win:done()
win:show(arg[0], arg)

local m1 = ""
local m2 = ""
local finished = {}

TRACE("main: starting event loop.")
while fl.wait() do
    TRACE("main: ------------- event loop activated -------------")

    -- Read all thread messages.
    -- If fl.awake was called with an optional message string,
    -- tl.thread_message must be called to remove message
    -- strings from queue. This is not necessary, if
    -- fl.awake is called without argument.
    -- In this example the thread message is only used
    -- for demonstration.
    repeat
        local m = fl.thread_message()
        if m then
            TRACE("main: thread_message: " , m)
        end
    until m == nil

    -- Read all linda events.
    repeat    
        local k, v = linda1:receive(0, "thread1", "thread2", "quit")
        if k then
            TRACE("main: received from linda:", k, v)
            if k == "thread1" then
                m1 = v
            elseif k == "thread2" then
                m2 = v
            elseif k == "quit" then
                finished[v] = true
            end
            local label = "Hello, World!\n - Received from thread1: " .. m1 .. ","
                                    .. "\n - Received from thread2: " .. m2 .. "."
            if finished.thread1 and finished.thread2 then
                label = label .. "\nAll threads are finished!"
            end
            box:label(label)
        end
    until k == nil
end
TRACE("main: event loop finished.")
