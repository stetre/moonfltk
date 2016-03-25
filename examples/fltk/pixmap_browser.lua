#!/usr/bin/env lua
-- MoonFLTK example: pixmap_browser.lua
--
-- Derived from the FLTK test/pixmap_browser.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

function load_file(fname)
  if img then 
      img = nil -- release() is called by MoonFLTK at gc
   end

  if fl.filename_isdir(fname) then
      b:label("@fileopen") -- show a generic folder
      b:labelsize(64)
      b:labelcolor(fl.LIGHT2)
      b:image(nil) -- note that this is different that b:image(), which is a 'get'
      b:redraw()
      return
   end
   
   img = fl.shared_image_get(fname)
   if not img then
      b:label("@filenew") -- show an empty document
      b:labelsize(64)
      b:labelcolor(fl.LIGHT2)
      b:image(nil)
      b:redraw()
      return
   end

  if img:w() > b:w() or img:h() > b:h() then
      local temp
      if img:w() > img:h() then
         temp = img:copy(b:w(), math.floor(b:h() * img:h() / img:w()))
      else 
         temp = img:copy(math.floor(b:w() * img:w() / img:h()), b:h())
      end
      -- img:release() not needed
      img = temp
   end
  b:label(name)
  b:labelsize(14)
  b:labelcolor(fl.FOREGROUND_COLOR)
  b:image(img)
  b:redraw()
end

function file_cb(n)
   if n == name then return end
   load_file(n)
   name = n
   win:label(name)
end

function button_cb()
   fl.file_chooser_callback(file_cb)
   local n = fl.file_chooser("Image file?","*.{bm,bmp,gif,jpg,pbm,pgm,png,ppm,xbm,xpm}", name)
   print("choosed:", n)
   fl.file_chooser_callback()
end

--[[
int dvisual = 0
int arg(int, char **argv, int &i) {
  if (argv[i][1] == '8') {dvisual = 1 i++ return 1}
  return 0
--]]

-- main -------------------------------------------

fl.register_images()
--  Fl::args(argc,argv,i,arg)
win = fl.double_window(400,435, arg[0])
b = fl.box(10,45,380,380)
b:box('thin down box')
b:align(fl.ALIGN_INSIDE|fl.ALIGN_CENTER)
button = fl.button(150,5,100,30,"load")
button:callback(button_cb)
--if (!dvisual) Fl::visual(FL_RGB)
if arg[1] then load_file(arg[1]) end
win:resizable(win)
win:show(arg[0],arg)
return fl.run()


