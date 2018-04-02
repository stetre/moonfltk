#!/usr/bin/env lua
-- MoonFLTK example: editor.lua
--
-- Derived from the FLTK test/editor.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")


function printf(...) print(string.format(...)) end
function isalnum(c) return string.match(c, "[%w]") == c end
function islower(c) return string.match(c, "[%l]") == c end

-- Syntax highlighting stuff...
TS = 14 -- default editor textsize
-- stylebuf = nil
styletable = { -- Style table
   { fl.BLACK,       fl.COURIER,           TS }, -- A - Plain
   { fl.BLUE,        fl.HELVETICA_ITALIC,  TS }, -- B - Line comments
   { fl.BLUE,        fl.HELVETICA_ITALIC,  TS }, -- C - Block comments
   { fl.DARK_RED,    fl.COURIER,           TS }, -- D - Strings
   { fl.MAGENTA,     fl.COURIER,           TS }, -- E - Directives
   { fl.DARK_GREEN,  fl.COURIER_BOLD,      TS }, -- F - Types
   { fl.DARK_YELLOW, fl.COURIER_BOLD,      TS }, -- G - Keywords
}

-- Style letters:
PLAIN = 'A'
LINECOMMENT = 'B'
BLOCKCOMMENT = 'C'
STRING = 'D'
DIRECTIVE = 'E'
TYPE = 'F'
KEYWORD = 'G'

code_keywords = { -- List of known C/C++ keywords...
   "and", "and_eq", "asm", "bitand", "bitor", "break", "case", "catch", "compl", 
   "continue", "default", "delete", "do", "else", "false", "for", "goto", "if",
   "new", "not", "not_eq", "operator", "or", "or_eq", "return", "switch", "template",
   "this", "throw", "true", "try", "while", "xor", "xor_eq" }

code_types = { -- List of known C/C++ types...
   "auto", "bool", "char", "class", "const", "const_cast", "double", "dynamic_cast",
   "enum", "explicit", "extern", "float", "friend", "inline", "int", "long", "mutable",
   "namespace", "private", "protected", "public", "register", "short", "signed",
   "sizeof", "static", "static_cast", "struct", "template", "typedef", "typename",
   "union", "unsigned", "virtual", "void", "volatile" }

-- reverse tables:
keywords = {}
for _, v in ipairs(code_keywords) do keywords[v] = true end
types = {}
for _, v in ipairs(code_types) do types[v] = true end

function iskeyword(s) return keywords[s]==true end
function istype(s) return types[s]==true end

-- Parse text and produce style data.
function style_parse(text, style) 
   -- printf("style_parse() text:'%s', style:'%s'", text, style)

   local col = 1
   local last = false
   local newstyle = { }

   local current = style:sub(1,1)

   while #newstyle < #text do

      local i = #newstyle + 1
      
      if current == LINECOMMENT or current == PLAIN or current == KEYWORD or current == TYPE then 
         current = PLAIN 
      end

      if current == PLAIN then
         -- Check for directives, comments, strings, and keywords
         local c = text:sub(i,i)

         if col == 1 and c == '#' then -- directive
            current = DIRECTIVE 

         elseif text:sub(i, i+1) == "//" then -- line comment
            current = LINECOMMENT
            repeat
               newstyle[#newstyle+1] = LINECOMMENT
               nextchar = text:sub(i,i) 
               i = i + 1
            until nextchar == '' or nextchar == '\n'
            goto continue

         elseif text:sub(i, i+1) == "/*" then -- block comment
            current = BLOCKCOMMENT

         elseif text:sub(i, i+1) == '\\"' then -- quoted quote
            newstyle[#newstyle+1] = current
            newstyle[#newstyle+1] = current
            col = col + 2
            goto continue

         elseif c == '"' then
            current = STRING

         elseif not last and (islower(c) or c == '_') then 
            -- might be a keyword or a type
            -- get the candidate string:
            local j = i
            repeat
               local nextchar = text:sub(j,j)
               j = j + 1
            until not (islower(nextchar) or nextchar == '_')
            local s = text:sub(i, j-2)
            if istype(s) then
               --print("type:", s)  
               for j = 1, #s do 
                  newstyle[#newstyle+1] = TYPE 
                  col = col  + 1
               end
               last = true
               goto continue
            elseif iskeyword(s) then   
               --print("keyword:", s)  
               for j = 1, #s do 
                  newstyle[#newstyle+1] = KEYWORD 
                  col = col  + 1
               end
               last = true
               goto continue
            end
         end

      elseif current == BLOCKCOMMENT and text:sub(i,i+1) == "*/" then -- close a block comment
         newstyle[#newstyle+1] = current
         newstyle[#newstyle+1] = current
         col = col + 2
         current = PLAIN
         goto continue

      elseif current == STRING then
         -- continuing a string ...
         if text:sub(i, i+1) == '\\"' then -- quoted end quote
            newstyle[#newstyle+1] = current
            newstyle[#newstyle+1] = current
            col = col + 2
            goto continue
         elseif text:sub(i, i) == '"' then -- end quote
            newstyle[#newstyle+1] = current
            col = col + 1
            current = PLAIN
            goto continue
         end

      end
   
      -- Copy style info 
      local c = text:sub(i,i)
      if current == PLAIN and (c == '{' or c == '}') then
         newstyle[#newstyle+1] = KEYWORD
      else
         newstyle[#newstyle+1] = current
      end

      col = col + 1
      last = ( isalnum(c) or c == '_' or c == '.' )

      if c == '\n' then
         -- reset column and possibly the style
         col = 0
         if current == LINECOMMENT or current == DIRECTIVE then current = PLAIN end
      end
      ::continue::

   end
      
   return table.concat(newstyle)
end


-- Initialize the style buffer...
function style_init() 
   if not stylebuf then stylebuf = fl.text_buffer(textbuf:length()) end
   local text = textbuf:text()
   local style = style_parse(text, "")
   stylebuf:text(style)
   --printf("style_init() text:'%s', style:'%s'", text, style)
end


-- 'style_unfinished_cb()' - Update unfinished styles.
function style_unfinished_cb(disp, pos) 
end


-- Update the style buffer...
function style_update(buf, pos, nins, ndel, nrest, deltext, info)
-- buf: the text_buffer
-- pos: position of update
-- nins: number of inserted characters
-- ndel: number of deleted characters
-- nrest: number of restyled characters
-- deltext: text that was deleted
-- info: callback user data

-- printf("style_update() pos=%d, nins=%d, ndel=%d, nrest=%d deltext='%s'", pos, nins, ndel, nrest, deltext) --, info))
   -- If this is just a selection change, just unselect the style buffer...
   if nins == 0 and ndel == 0 then 
      stylebuf:unselect()
      return
   end

  -- Track changes in the text buffer...
   if nins > 0 then 
      -- Insert characters into the style buffer...
      local style = string.rep(PLAIN, nins)
      stylebuf:replace(pos, pos + ndel - 1, style)
   else -- nins=0, ndel > 0
      -- Just delete characters in the style buffer...
      stylebuf:remove(pos, pos + ndel - 1)
   end


   -- Select the area that was just updated to avoid unnecessary callbacks...
   local upd_end = pos + nins - ndel - 1 -- updated text = pos ... upd_end
   -- if characters were deleted (upd_end < pos) there is no need to 
   -- select() an area that is not there...
   if upd_end >= pos then stylebuf:select(pos, upd_end) end

   -- Re-parse the changed region; we do this by parsing from the beginning of 
   -- the previous line of the changed region to the end of the line of the 
   -- changed region...  Then we check the last style character and keep updating 
   -- if we have a multi-line comment character...
   local start = textbuf:line_start(pos)
   local end_  = textbuf:line_end(upd_end > pos and upd_end or pos)
   local text  = textbuf:text_range(start, end_)
   local style = stylebuf:text_range(start, end_)
   local len = end_ - start + 1 -- length of changed region (= #text = #style)
   -- last style character in the changed region:
   local last = start > end_ and '' or style:sub(len, len)

   local style = style_parse(text, style)
   stylebuf:replace(start, end_, style)
   info.editor:redisplay_range(start, end_)

   if start > end_ or last ~= style:sub(len, len) then
      -- Recalculate the rest of the buffer style
      -- Either the user deleted some text, or the last character on the line changed
      -- styles, so reparse the remainder of the buffer...
      end_ = textbuf:length()
      text = textbuf:text_range(start, end_)
      style = stylebuf:text_range(start, end_)
      style = style_parse(text, style)
      stylebuf:replace(start, end_, style)
      info.editor:redisplay_range(start, end_);
   end
end


-- Editor window class...

function EditorWindow(w, h, t)
   local self = fl.double_window(w, h, t)
   local info = { } -- this will hold the 'class' members...
   info.self = self -- so we can retrieve the window itself from info
   info.replace_dlg = fl.window(300, 105, "Replace")
   info.replace_find = fl.input(80, 10, 210, 25, "Find:")
   info.replace_find:align(fl.ALIGN_LEFT)
   info.replace_with = fl.input(80, 40, 210, 25, "Replace:")
   info.replace_with:align(fl.ALIGN_LEFT);
   info.replace_all = fl.button(10, 70, 90, 25, "Replace All")
   info.replace_all:callback(replall_cb, info)
   info.replace_next = fl.return_button(105, 70, 120, 25, "Replace Next")
   info.replace_next:callback(wreplace2_cb, info)
   info.replace_cancel = fl.button(230, 70, 60, 25, "Cancel")
   info.replace_cancel:callback(replcan_cb, info)
   info.replace_dlg:done()
   info.replace_dlg:set_non_modal()
   info.editor = nil -- fl.text_editor
   info.search = nil -- string being searched for
   return self, info
end

--[[
EditorWindow::~EditorWindow() {
  delete replace_dlg;
}
--]]

function check_save()
   if not changed then return true end
   local r = fl.choice("The current file has not been saved.\n" ..
                    "Would you like to save it now?",
                    "Cancel", "Save", "Don't Save")
   if r == 0 then
      return false
   elseif r == 1 then 
      save_cb() -- Save the file...
      return false
   else  
      return true
   end
end

loading = false

function load_file(newfile, ipos)
   loading = true
   local insert = (ipos ~= -1)
   changed = insert;
   if not insert then filename = nil end
   local ok, err
   if not insert then 
      ok, err = textbuf:loadfile(newfile) 
   else 
      ok, err = textbuf:insertfile(newfile, ipos)
   end
   changed = changed  or textbuf:input_file_was_transcoded()
   if not ok then
      fl.alert(string.format("Error reading from file '%s':\n%s.", newfile, err))
   else
      if not insert then filename = newfile end
   end
   loading = false
   textbuf:call_modify_callback()
end


function save_file(newfile) 
   local ok, err = textbuf:savefile(newfile)
   if not ok then
      fl.alert(string.format("Error writing to file '%s':\n%s.", newfile, err));
   else
      filename = newfile
   end
   changed = false
   textbuf:call_modify_callback()
end

-- MENU ITEM CALLBACKS ------------------------------------------------------
-- Args: 
-- m = the menu itself, 
-- pn = the item's pathname, 
-- info = the EditorWindow info table

function copy_cb(m, pn, info)
   fl.kf_copy(nil, info.editor)
end

function cut_cb(m, pn, info)
   fl.kf_cut(nil, info.editor)
end

function delete_cb(m, pn, info)
   textbuf:remove_selection()
end

function linenumbers_cb(m, pn, info)
   local editor = info.editor
   if m:item_value(pn) then
      editor:linenumber_width(50) -- enable
      editor:linenumber_size(editor:textsize())
   else
      editor:linenumber_width(0) -- disable
   end
   info.self:redraw()
end

function wordwrap_cb(m, pn, info)
   local editor = info.editor
   if m:item_value(pn) then
      editor:wrap_mode('at bounds', 0)
   else
      editor:wrap_mode('none', 0)
   end
   info.self:redraw()
end


function find_cb(m, pn, info)
   local val = fl.input("Search String:", info.search)
   if val then
      -- User entered a string - go find it!
      info.search = val
      find2_cb(m, pn, info)
   end
end

function find2_cb(m, pn, info)
   if not info.search then
      -- Search string is blank; get a new one...
      return find_cb(m, pn, info)
   end
   local editor = info.editor
   local pos = editor:insert_position()
   pos = textbuf:search_forward(pos, info.search)
   if pos then
   -- Found a match; select and update the position...
      textbuf:select(pos, pos + info.search:len())
      editor:insert_position(pos + info.search:len())
      editor:show_insert_position()
   else 
      fl.alert("No occurrences of '".. info.search .."' found!")
   end
end


function stripdir(s)
   if not s then return end
   local index = string.find(s, "/[^/]*$") -- find the index of the last '/'
   if not index then return s end  -- "foo"            --> foo
   local fname = s:sub(index+1)    -- "/home/joe/foo"  --> foo
   if fname == "" then return end  -- "/home/joe/"     --> nil
   return fname
end

function set_title(w)
   title = stripdir(filename) or "Untitled"
   if changed then title = title .. " (modified)" end
   w:label(title)
end

function modify_cb(buf, pos, nins, ndel, nrest, deltext, info)
-- Note: there is only one modify_callback in MoonFLTK.
   style_update(buf, pos, nins, ndel, nrest, deltext, info)
   changed_cb(buf, pos, nins, ndel, nrest, deltext, info)
end

function changed_cb(buf, pos, nins, ndel, nrest, deltext, info)
   if (nins ~= 0  or ndel ~= 0) and (not loading) then changed = true end
   set_title(info.self)
   if loading then info.editor:show_insert_position() end
end

function new_cb(m, pn, info)
  if not check_save() then return end
  filename = nil
  textbuf:select(1, textbuf:length()+1)
  textbuf:remove_selection()
  changed = false
  textbuf:call_modify_callback()
end

function open_cb(m, pn, info)
   if not check_save() then return end
   local fnfc = fl.native_file_chooser()
   fnfc:title("Open file")
   fnfc:type('file')
   local r = fnfc:show()
   if r == "cancel" then return end
   if r == "ok" then load_file(fnfc:filename(), -1) end
   print(r) -- an error occurred
end

function insert_cb(m, pn, info)
   local fnfc = fl.native_file_chooser()
   fnfc:title("Insert file")
   fnfc:type('file')
   local r = fnfc:show()
   if r == "cancel" then return end
   if r == "ok" then
      load_file(fnfc:filename(), info.editor:insert_position())
   else
      print(r) -- an error occurred
   end
end

function paste_cb(m, pn, info) 
   fl.kf_paste(nil, info.editor)
end

function close_window(info)
   local w = info.self
   local editor = info.editor

   if num_windows == 1 then
      if not check_save() then return end
   end

   w:hide()
   editor:buffer(nil)
   textbuf:modify_callback(nil)
   --@@ fl.unreference?        Fl::delete_widget(w);
   num_windows = num_windows - 1
   if num_windows == 0 then os.exit(true, true) end
end

function close_cb(m, pn, info) return close_window(info) end -- menu item callback
function wclose_cb(w, info) return close_window(info) end  -- widget callback


function quit_cb(m, pn, info)
   if changed and not check_save() then return end
   os.exit(true, true)
end

function replace_cb(m, pn, info)
  info.replace_dlg:show();
end

function wreplace2_cb(w, info) replace2_cb(nil, nil, info) end -- wrapper for widgets

function replace2_cb(m, pn, info)
   local find = info.replace_find:value()
   local replace = info.replace_with:value()

   if not find then
      -- Search string is blank; get a new one...
      info.replace_dlg:show()
      return
   end

   info.replace_dlg:hide()

   local pos = info.editor:insert_position()
   local pos = textbuf:search_forward(pos, find)
   if pos then
      -- Found a match; update the position and replace text...
      textbuf:select(pos, pos + find:len())
      textbuf:remove_selection()
      textbuf:insert(pos, replace)
      textbuf:select(pos, pos + find:len())
      info.editor:insert_position(pos + find:len())
      info.editor:show_insert_position()
   else 
      fl.alert("No occurrences of '" .. find .. "' found!")
   end
end


function replall_cb(w, info)
   local find = info.replace_find:value();
   local replace = info.replace_with:value();

   if not find then
      -- Search string is blank; get a new one...
      info.replace_dlg:show();
      return
   end

   info.replace_dlg:hide()

   info.editor:insert_position(0)
   local times = 0

   -- Loop through the whole string
   repeat 
      local pos = info.editor:insert_position()
      pos = textbuf:search_forward(pos, find)
      if pos then
         -- Found a match; update the position and replace text...
         found = true
         textbuf:select(pos, pos + find:len())
         textbuf:remove_selection()
         textbuf:insert(pos, replace)
         info.editor:insert_position(pos + replace:len())
         info.editor:show_insert_position()
         times = times + 1
      end
   until not pos

   if times > 0 then 
      fl.message("Replaced " .. times .. "occurrences.")
   else 
      fl.alert("No occurrences of '" .. find .. "' found!") 
   end
end


function replcan_cb(_, info) 
  info.replace_dlg:hide()
end

function save_cb() 
   if not filename then
      -- No filename - get one!
      saveas_cb()
   else 
      save_file(filename)
   end
end


function saveas_cb()
   local fnfc = fl.native_file_chooser()
   fnfc:title("Save File As?")
   fnfc:type('save file')
   local r = fnfc:show()
   if r == "cancel" then return end
   if r == "ok" then save_file(fnfc:filename()) end
   print(r) -- an error occurred
end

function view_cb() 
   local w = new_view()
   w:show()
end


function addmenuitems(m, info)
   local sc = fl.shortcut
   m:add("&File/&New File", 0, new_cb, info)
   m:add("&File/&Open File...", sc("Ctrl", 'o'), open_cb , info)
   m:add("&File/&Insert File...",  sc("Ctrl", 'i'), insert_cb, info, fl.MENU_DIVIDER)
   m:add("&File/&Save File", sc("Ctrl", 's'), save_cb, info)
   m:add("&File/Save File &As...", sc("Ctrl", "Shift", 's'), saveas_cb, info, fl.MENU_DIVIDER)
   m:add("&File/New &View", sc("Alt", 'v'), view_cb, info)
   m:add("&File/&Close View", sc("Ctrl", 'w'), close_cb, info, fl.MENU_DIVIDER)
   m:add("&File/E&xit", sc("Ctrl", 'q'), quit_cb, info)
   m:add("&Edit/Cu&t", sc("Ctrl", 'x'), cut_cb, info)
   m:add("&Edit/&Copy", sc("Ctrl", 'c'), copy_cb, info)
   m:add("&Edit/&Paste", sc("Ctrl", 'v'), paste_cb, info)
   m:add("&Edit/&Delete", 0, delete_cb, info)
   m:add("&Edit/Preferences/Line Numbers", sc("Ctrl", 'l'), linenumbers_cb, info, fl.MENU_TOGGLE)
   m:add("&Edit/Preferences/Word Wrap", 0, wordwrap_cb, info, fl.MENU_TOGGLE)
   m:add("&Search/&Find...", sc("Ctrl", 'f'), find_cb, info)
   m:add("&Search/F&ind Again", sc("Ctrl", 'g'), find2_cb, info)
   m:add("&Search/&Replace...", sc("Ctrl", 'r'), replace_cb, info)
   m:add("&Search/Re&place Again", sc("Ctrl", 't'), replace2_cb, info)
end
   

function new_view()
   local w, info = EditorWindow(660, 400, title)
-- instead of passing w to the callbacks, we pass info
-- (if w is needed, one can retrieve it as w = info.self)
   w:begin()
   local m = fl.menu_bar(0, 0, 660, 30)
   addmenuitems(m, info)
   info.editor = fl.text_editor(0, 30, 660, 370)
   info.editor:textfont(fl.COURIER)
   info.editor:textsize(TS)
 --info.editor:wrap_mode('at bounds', 250)
   info.editor:buffer(textbuf)
   info.editor:highlight_data(stylebuf, styletable, PLAIN, style_unfinished_cb)
   w:done()
   w:resizable(info.editor)
   w:callback(wclose_cb, info)
   textbuf:modify_callback(modify_cb, info)
   textbuf:call_modify_callback()
   num_windows = num_windows + 1
   return w
end

function cb(fname)
  load_file(fname, -1);
end

num_windows = 0;

-- main -----------------------------
textbuf = fl.text_buffer()
changed = false
-- filename = nil
-- title = nil
--textbuf:transcoding_warning_action = NULL; @@
style_init()
-- fl.open_callback(cb) @@??

window = new_view()
window:show(arg[0])
return fl.run()
