#!/usr/bin/env lua
-- MoonFLTK example: texteditor-simple.lua
--
-- Derived from the FLTK examples/texteditor-simple.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

win = fl.double_window(640, 480, "Simple Fl_Text_Editor")
buff = fl.text_buffer()
edit = fl.text_editor(20, 20, 640-40, 480-40)
edit:buffer(buff) -- attach the text buffer to our editor widget
win:resizable(edit)
win:show()
buff:text("line 0\nline 1\nline 2\nline 3\nline 4\nline 5\nline 6\nline 7\nline 8\n" ..
          "line 9\nline 10\nline 11\nline 12\nline 13\nline 14\nline 15\nline 16\nline 17\n"..
          "line 18\nline 19\nline 20\nline 21\nline 22\nline 23\n")
return fl.run()
