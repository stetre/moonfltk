#!/usr/bin/env lua
-- MoonFLTK example: clipboard.lua
--
-- Derived from the FLTK examples/clipboard.cxx example (http://www.fltk.org)
--

fl = require("moonfltk")

-- Displays and follows the content of the clipboard with either image or text data

function chess(x, y, h, w) -- a box with a chess-like pattern below its image
   local self = fl.box_sub('flat box', x, y, h, w)
   self:align(fl.ALIGN_CENTER | fl.ALIGN_CLIP)
   self:override_draw(function(_)
      assert(_ == self)
      self:draw_box()
      local im = self:image()
      if im then -- draw the chess pattern below the box centered image
         local x, y, w, h = self:xywh()
         local X, Y, W, H = x+(w-im:w())/2, y+(h-im:h())/2, im:w(), im:h()
         fl.push_clip(X,Y,W,H)
         fl.push_clip(x,y,w,h)
         fl.color(fl.WHITE) 
         fl.rectf(X,Y,W,H)
         fl.color(fl.LIGHT2)
         local side = 4
         local side2 = 2*side
         for j = Y, Y+H+1, side do
            for i = X + (j-Y)%side2, X+W+1, side2 do
               fl.rectf(i,j,side,side)
            end
         end
         fl.pop_clip()
         fl.pop_clip()
      end
      self:draw_label() -- draw the box image
   end)
   return self
end

TAB_COLOR = fl.DARK3

function clipboard_viewer(x, y, w, h)
-- use tabs to display as appropriate the image or textual content of the clipboard
   local self = fl.tabs_sub(x,y,w,h)
   self:override_handle(function (_, event)
      if event ~= 'paste' then return self:super_handle(event) end
 --   print(event, fl.event_clipboard_type())
      if fl.event_clipboard_type() == 'image' then -- an image is being pasted
         local im = fl.event_clipboard()
         if not im then return true end
         title = string.format("%dx%d",im:w(), im:h()) -- display the image original size
         local scale_x = im:w() / image_box:w() -- rescale the image if larger than the display box
         local scale_y =  im:h() / image_box:h()
         local scale = scale_x
         if scale_y > scale then scale = scale_y end
         if scale > 1 then
            im = im:copy(math.floor(im:w()/scale), math.floor(im:h()/scale))
         end
         local oldim = image_box:image()
--       if oldim then delete oldim end @@ fl.unreference()?
         image_box:image(im) -- show the scaled image
         image_size:label(title)
         self:value(image_box:parent())
         self:window():redraw()
    else  -- text is being pasted
         display:buffer():text(fl.event_text())
         self:value(display)
         display:redraw()
    end
    return true
  end)
   return self
end


function cb(_, tabs)
   -- try to find image or text in the clipboard
-- print("image?", fl.clipboard_contains('image'))
-- print("text/plain?", fl.clipboard_contains('text/plain'))
   if fl.clipboard_contains('image') then
      fl.paste(tabs, 'clipboard', 'image')
   elseif fl.clipboard_contains('text/plain') then
      fl.paste(tabs, 'clipboard', 'text/plain')
   end
end

function clip_callback(source) 
-- called after clipboard was changed or at application activation
   if source == 'clipboard' then cb(nil, tabs) end
end

-- main ----------------------------------
fl.register_images() -- required to allow pasting of images
win = fl.window(500, 550, "clipboard viewer")
tabs = clipboard_viewer(0, 0, 500, 500)
g = fl.group( 5, 30, 490, 460, 'image') -- g will display the image form
g:box('flat box')
image_box = chess(5, 30, 490, 450)
image_size = fl.box('no box', 5, 485, 490, 10)
g:done()
g:selection_color(TAB_COLOR)

buffer = fl.text_buffer()
display = fl.text_display(5,30,490, 460, 'text/plain') -- display will display the text form
display:buffer(buffer)
display:selection_color(TAB_COLOR)
tabs:done()
tabs:resizable(display)

g2 = fl.group( 10,510,200,25)
refresh = fl.button(10,510,200,25, "Refresh from clipboard")
refresh:callback(cb, tabs)
g2:done()
g2:resizable(nil)
win:done()
win:resizable(tabs)
win:show(argc,argv)
-- will update with new clipboard content immediately or at application activation:
fl.add_clipboard_notify(clip_callback) --, tabs) 
fl.rgb_scaling('bilinear') -- set bilinear image scaling method
return fl.run()


