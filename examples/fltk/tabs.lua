#!/usr/bin/env lua
-- MoonFLTK example: tabs.lua
--
-- Derived from the FLTK test/tabs.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function cb_button1()
  fl.message("Test to see if this modal window prevents you from changing the tabs. It should.")
end

function cb_Test()
  fl.message("Make sure you cannot change the tabs while this modal window is up")
end

function cb_() wWizard:value(wWizard:child(1)) end
function cb_1() wWizard:prev() end
function cb_2() wWizard:next() end

function cb_3() 
   local last = wWizard:children() 
   wWizard:value(wWizard:child(last))
end

function cb_cancel() os.exit(false, true) end
function cb_OK() os.exit(true, true) end


foo_window = fl.double_window(642, 337, "Comparison of fl.tab (left) vs. fl.wizard (right)")

    do local o = fl.box(95, 0, 130, 35, "class fl.tabs")
      o:labeltype('engraved')
      o:labelfont(1)
    end -- fl.box o
    do local o = fl.tabs(10, 35, 300, 205)
      o:tooltip("the various index cards test different aspects of the fl.tabs widget")
      o:selection_color(4)
      o:labelcolor(fl.BACKGROUND2_COLOR)
      do local o = fl.group(10, 60, 300, 180, "Label&1")
        o:tooltip("this Tab tests correct keyboard navigation between text input fields")
        o:selection_color(1)
        do local o = fl.input(60, 80, 240, 40, "input:")
          o:tooltip("This is the first input field")
        end -- fl.input o
        do local o = fl.input(60, 120, 240, 30, "input2:")
          o:tooltip("")
        end -- fl.input o
        fl.input(60, 150, 240, 80, "input3:")
        o:done()
        fl.current_group():resizable(o)
      end -- fl.group o
      do local o = fl.group(10, 60, 300, 180, "tab&2")
        o:tooltip("tab2 tests among other things the cooperation of modal windows and tabs")
        o:selection_color(2)
        o:hide()
        do local o = fl.button(20, 90, 100, 30, "button1")
          o:callback(cb_button1)
        end -- fl.button o
        fl.input(140, 130, 100, 30, "input in box2")
        fl.button(30, 170, 260, 30, "This is stuff inside the fl.group \"tab2\"")
        do local o = fl.button(30, 200, 260, 30, "Test event blocking by modal window")
          o:callback(cb_Test)
        end -- fl.button o
        o:done()
      end -- fl.group o
      do local o = fl.group(10, 60, 300, 180, "tab&3")
        o:tooltip("tab3 checks for correct keyboard navigation")
        o:selection_color(3)
        o:hide()
        fl.button(20, 90, 60, 80, "button2")
        fl.button(80, 90, 60, 80, "button")
        fl.button(140, 90, 60, 80, "button")
        o:done()
      end -- fl.group o
      do local o = fl.group(10, 60, 300, 180, "&tab4")
        o:tooltip("this tab show the issue of indicating a selcted tab " ..
                     "if the tab layouts are very similar")
        o:selection_color(5)
        o:labeltype('engraved')
        o:labelfont(2)
        o:hide()
        fl.button(20, 80, 60, 110, "button2")
        fl.button(80, 80, 60, 110, "button")
        fl.button(140, 80, 60, 110, "button")
        o:done()
      end -- fl.group o
      do local o = fl.group(10, 60, 300, 180, "@fileprint  &print")
        o:tooltip("tab5 verifies if visibility requests are handled correctly")
        o:hide()
        do local o = fl.button(20, 75, 60, 80, "button2")
          o:tooltip("button2 has a different tooltp than tab5")
        end -- fl.button o
        fl.button(90, 75, 60, 80, "button")
        do local o = fl.clock(160, 75, 100, 100, 
                  "Make sure this clock does not use processor time " .. 
                  "when this tab is hidden or window is iconized")
          o:box('oshadow box')
          o:color(238)
          o:selection_color(fl.FOREGROUND_COLOR)
          o:labelfont(8)
          o:labelsize(10)
          o:align('bottom', 'wrap') -- 130
        end -- fl.clock o
        do local o = fl.window(20, 175, 40, 55)
          o:box('thin down box')
          o:color(173)
          o:selection_color(fl.BACKGROUND_COLOR)
          o:labeltype('normal')
          o:labelfont(0)
          o:labelsize(14)
          o:labelcolor(fl.FOREGROUND_COLOR)
          o:align('center', 'inside')
          o:when('release')
          o:done()
        end -- fl.window o
        do local o = fl.window(65, 175, 40, 55, "subwindows:")
          o:box('thin down box')
          o:color(167)
          o:selection_color(fl.BACKGROUND_COLOR)
          o:labeltype('normal')
          o:labelfont(0)
          o:labelsize(14)
          o:labelcolor(fl.FOREGROUND_COLOR)
          o:align('top')
          o:when('release')
          o:done()
        end -- fl.window o
        do local o = fl.window(110, 175, 40, 55)
          o:box('thin down box')
          o:color(239)
          o:selection_color(fl.BACKGROUND_COLOR)
          o:labeltype('normal')
          o:labelfont(0)
          o:labelsize(14)
          o:labelcolor(fl.FOREGROUND_COLOR)
          o:align('center', 'inside')
          o:when('release')
          o:done()
        end -- fl.window o
        o:done()
      end -- fl.group o
      o:done()
      fl.current_group():resizable(o)
    end -- fl.tabs o
    do local o = fl.box(410, 0, 130, 35, "class fl.wizard")
      o:labeltype('engraved')
      o:labelfont(1)
    end -- fl.box o
    do wWizard = fl.wizard(325, 60, 300, 180)
      do local o = fl.group(325, 60, 300, 180, "Label1")
        o:tooltip("this Tab tests correct keyboard navigation between text input fields")
        o:selection_color(1)
        do local o = fl.input(375, 80, 240, 40, "input:")
          o:tooltip("This is the first input field")
        end -- fl.input o
        do local o = fl.input(375, 120, 240, 30, "input2:")
          o:tooltip("")
        end -- fl.input o
        fl.input(375, 150, 240, 80, "input3:")
        o:done()
        fl.current_group():resizable(o)
      end -- fl.group o
      do local o = fl.group(325, 60, 300, 180, "tab2")
        o:tooltip("tab2 tests among other things the cooperation of modal windows and tabs")
        o:selection_color(2)
        o:hide()
        do local o = fl.button(335, 90, 100, 30, "button1")
          o:callback(cb_button1)
        end -- fl.button o
        fl.input(455, 130, 100, 30, "input in box2")
        fl.button(345, 170, 260, 30, "This is stuff inside the fl.group \"tab2\"")
        do local o = fl.button(345, 200, 260, 30, "Test event blocking by modal window")
          o:callback(cb_Test)
        end -- fl.button o
        o:done()
      end -- fl.group o
      do local o = fl.group(325, 60, 300, 180, "tab3")
        o:tooltip("tab3 checks for correct keyboard navigation")
        o:selection_color(3)
        o:hide()
        fl.button(335, 90, 60, 80, "button2")
        fl.button(395, 90, 60, 80, "button")
        fl.button(455, 90, 60, 80, "button")
        o:done()
      end -- fl.group o
      do local o = fl.group(325, 60, 300, 180, "tab4")
        o:tooltip("this tab show the issue of indicating a selcted tab " ..
                  "if the tab layouts are very similar")
        o:selection_color(5)
        o:labelfont(2)
        o:hide()
        fl.button(335, 80, 60, 110, "button2")
        fl.button(395, 80, 60, 110, "button")
        fl.button(455, 80, 60, 110, "button")
        o:done()
      end -- fl.group o
      do local o = fl.group(325, 60, 300, 180, "     tab5      ")
        o:tooltip("tab5 verifies if visibility requests are handled correctly")
        o:labeltype('engraved')
        o:hide()
        do local o = fl.button(335, 75, 60, 80, "button2")
          o:tooltip("button2 has a different tooltp than tab5")
        end -- fl.button o
        fl.button(405, 75, 60, 80, "button")
        do local o = fl.clock(475, 75, 100, 100, 
               "Make sure this clock does not use processor time when " .. 
               "this tab is hidden or window is iconized")
          o:box('oshadow box')
          o:color(238)
          o:selection_color(fl.FOREGROUND_COLOR)
          o:labelfont(8)
          o:labelsize(10)
          o:align('bottom', 'wrap') -- 130
        end -- fl.clock o
        do local o = fl.window(335, 175, 40, 55)
          o:box('thin down box')
          o:color(173)
          o:selection_color(fl.BACKGROUND_COLOR)
          o:labeltype('normal')
          o:labelfont(0)
          o:labelsize(14)
          o:labelcolor(fl.FOREGROUND_COLOR)
          o:align('center', 'inside')
          o:when('release')
          o:done()
        end -- fl.window o
        do local o = fl.window(380, 175, 40, 55, "subwindows:")
          o:box('thin down box')
          o:color(167)
          o:selection_color(fl.BACKGROUND_COLOR)
          o:labeltype('normal')
          o:labelfont(0)
          o:labelsize(14)
          o:labelcolor(fl.FOREGROUND_COLOR)
          o:align('top')
          o:when('release')
          o:done()
        end -- fl.window o
        do local o = fl.window(425, 175, 40, 55)
          o:box('thin down box')
          o:color(239)
          o:selection_color(fl.BACKGROUND_COLOR)
          o:labeltype('normal')
          o:labelfont(0)
          o:labelsize(14)
          o:labelcolor(fl.FOREGROUND_COLOR)
          o:align('center', 'inside')
          o:when('release')
          o:done()
        end -- fl.window o
        o:done()
      end -- fl.group o
      wWizard:done()
    end -- fl.wizard wWizard
    do local o = fl.group(410, 245, 130, 25)
      do local o = fl.button(410, 245, 30, 25, "@|<")
        o:tooltip("go to first page [Home]")
        o:shortcut(0xff50)
        o:callback(cb_)
      end -- fl.button o
      do local o = fl.button(440, 245, 30, 25, "@<")
        o:tooltip("go to previous page [left arrow]")
        o:shortcut(0xff51)
        o:callback(cb_1)
      end -- fl.button o
      do local o = fl.button(480, 245, 30, 25, "@>")
        o:tooltip("go to next page in wizard [right arrow]")
        o:shortcut(0xff53)
        o:callback(cb_2)
      end -- fl.button o
      do local o = fl.button(510, 245, 30, 25, "@>|")
        o:tooltip("go to last page [End]")
        o:shortcut(0xff57)
        o:callback(cb_3)
      end -- fl.button o
      o:done()
    end -- fl.group o
    fl.input(60, 255, 130, 25, "inputA:")
    fl.input(60, 285, 250, 25, "inputB:")
    do local o = fl.button(475, 295, 70, 30, "cancel")
      o:callback(cb_cancel)
    end -- fl.button o
    do local o = fl.return_button(555, 295, 70, 30, "OK")
      o:callback(cb_OK)
    end -- fl.return_button o
foo_window:done()
foo_window:show(argc, argv)
return fl.run()
