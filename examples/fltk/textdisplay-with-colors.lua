#!/usr/bin/env lua
-- MoonFLTK example: textdisplay-with-colors.lua
--
-- Derived from the FLTK examples/textdisplay-with-colors.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

-- Style table
stable = {
       -- FONT COLOR      FONT FACE   FONT SIZE
       -- --------------- ----------- --------------
       {  fl.RED,         fl.COURIER, 18 }, -- A - Red
       {  fl.DARK_YELLOW, fl.COURIER, 18 }, -- B - Yellow
       {  fl.DARK_GREEN,  fl.COURIER, 18 }, -- C - Green
       {  fl.BLUE,        fl.COURIER, 18 }, -- D - Blue
   }

win = fl.window(640, 480, "Simple Text Display With Colors")
disp = fl.text_display(20, 20, 640-40, 480-40)

tbuff = fl.text_buffer()   -- text buffer
sbuff =  fl.text_buffer()  -- style buffer
disp:buffer(tbuff)

function unfinished_style_cb(tdisp, pos) 
   assert(tdisp == disp)
   print("callback", pos)
end

disp:highlight_data(sbuff, stable, 'A', unfinished_style_cb)

-- Text
tbuff:text("Red Line 1\nYel Line 2\nGrn Line 3\nBlu Line 4\n" ..
            "Red Line 5\nYel Line 6\nGrn Line 7\nBlu Line 8\n")
-- Style for text
sbuff:text("AAAAAAAAAA\nBBBBBBBBBB\nCCCCCCCCCC\nDDDDDDDDDD\n" ..
          "AAAAAAAAAA\nBBBBBBBBBB\nCCCCCCCCCC\nDDDDDDDDDD\n")

win:resizable(disp)
win:show()
return(fl.run())
