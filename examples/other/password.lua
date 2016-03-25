#!/usr/bin/env lua
-- MoonFLTK example: password.lua

fl = require("moonfltk")

ret = fl.password("password:", "myfavouritepetname")
print("password:", ret)

