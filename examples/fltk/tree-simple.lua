#!/usr/bin/env lua
-- MoonFLTK example: tree-simple.lua
--
-- Derived from the FLTK examples/tree-simple.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

--fl.trace_objects(true)

-- Tree's callback
-- Invoked whenever an item's state changes.
--
function TreeCallback(_, item, reason, arg)
   assert(_ == tree)
   print(string.format("TreeCallback() item: '%s', reason: %s", item, reason))
   -- print(arg)
   if reason == 'selected' then
      -- ...
   elseif reason == 'deselected' then
      -- ...
   elseif reason == 'opened' then
      -- ...
   elseif reason == 'closed' then
      -- ...
   elseif reason == 'reselected' then
      -- ...
   end
end

-- main --------------------------------
fl.scheme("gtk+")
win = fl.double_window(250, 400, arg[0])
win:begin()

   -- Create the tree
   tree = fl.tree(10, 10, win:w()-20, win:h()-20)
   tree:root_label("mytree") -- otherwise defaults to "ROOT"

   tree:showroot(false)       -- don't show root of tree
   tree:callback(TreeCallback, "optional argument")-- setup a callback for the tree

   -- Add some items
   tree:add("Flintstones/Fred")
   tree:add("Flintstones/Wilma")
   tree:add("Flintstones/Pebbles")
   tree:add("Simpsons/Homer")
   tree:add("Simpsons/Marge")
   tree:add("Simpsons/Bart")
   tree:add("Simpsons/Lisa")
   tree:add("Pathnames/\\/bin")     -- front slashes
   tree:add("Pathnames/\\/usr\\/sbin")
   tree:add("Pathnames/C:\\\\Program Files") -- backslashes
   tree:add("Pathnames/C:\\\\Documents and Settings")
   tree:add("Widgets/Toggle")
   tree:item_widget("Widgets/Toggle", fl.toggle_button(10, 10, 100, 10))
   tree:item_widget("Widgets/Toggle"):label("push me")
   tree:add("Widgets/Clock")
   tree:item_widget("Widgets/Clock", fl.round_clock(0, 0, 50, 50))
   tree:item_labelsize("Widgets/Clock", 50)


   -- Start with some items closed
   tree:item_close("Simpsons")
   tree:item_close("Pathnames")

--[[ just to try some item methods:
print("root", tree:root())
print("item_is_root", tree:item_is_root(tree:root())) -- of course it is...
print("item_children", tree:item_children(tree:root()))
print("item_children", tree:item_children("Simpsons"))
print("item_depth", tree:item_depth("Simpsons"))
tree:item_label("Simpsons", "SIMPSONS")
print("item_label", tree:item_label("SIMPSONS/Lisa"))
print("item_label", tree:item_label("SIMPSONS/Lisa"))
tree:item_label("SIMPSONS/Lisa", "Lizzy")
print("item_label", tree:item_label("SIMPSONS/Lizzy"))
print("item_depth", tree:item_depth("SIMPSONS/Lizzy"))

print("find_child", tree:item_find_child("Flintstones", "Fred"))
print("find_child", tree:item_find_child("SIMPSONS", "Bart"))
print("find_child", tree:item_find_child("SIMPSONS", "Lizzy"))
print("find_child", tree:item_find_child("SIMPSONS", "Lisa"))
tree:insert("SIMPSONS", "Joe", 5)
print("item_child", tree:item_child("SIMPSONS", 5))
print("find_child", tree:item_find_child("SIMPSONS", "Lizzy"))
tree:show_self()

tree:item_swap_children("SIMPSONS", "Joe", "Bart")
for i = 1,tree:item_children("SIMPSONS") do
   print(i, tree:item_child("SIMPSONS", i))
end

--]]

win:done()
win:resizable(win)
win:show(arg[0], arg)
return fl:run()

