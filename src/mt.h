/* The MIT License (MIT)
 *
 * Copyright (c) 2016 Stefano Trettel
 *
 * Software repository: MoonFLTK, https://github.com/stetre/moonfltk
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef mtDEFINED
#define mtDEFINED

#ifdef BEGIN_FLTK_INCLUDE
    BEGIN_FLTK_INCLUDE // {
#endif
#include <FL/Fl_Adjuster.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Browser_.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Cairo.H>
#include <FL/Fl_Cairo_Window.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Check_Browser.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Clock.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Copy_Surface.H>
#include <FL/Fl_Counter.H>
//#include <FL/Fl_Device.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Double_Window.H>
//#include <FL/Fl_Export.H>
#include <FL/Fl_File_Browser.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_File_Icon.H>
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Fill_Dial.H>
#include <FL/Fl_Fill_Slider.H>
#include <FL/Fl_Float_Input.H>
//#include <FL/Fl_FormsBitmap.H>
//#include <FL/Fl_FormsPixmap.H>
//#include <FL/Fl_Free.H>
#include <FL/Fl_GIF_Image.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Help_Dialog.H>
#include <FL/Fl_Help_View.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Hor_Fill_Slider.H>
#include <FL/Fl_Hor_Nice_Slider.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Input_.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Line_Dial.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Menu.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Multi_Browser.H>
//#include <FL/Fl_Multi_Label.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Nice_Slider.H>
//#include <FL/Fl_Object.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Overlay_Window.H>
#include <FL/Fl_Pack.H>
//#include <FL/Fl_Paged_Device.H>
#include <FL/Fl_Pixmap.H>
//#include <FL/Fl_Plugin.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_PNM_Image.H>
//#include <FL/Fl_Positioner.H>
#include <FL/Fl_PostScript.H>
//#include <FL/Fl_Preferences.H>
#include <FL/Fl_Printer.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Radio_Light_Button.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Repeat_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Roller.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Round_Clock.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/Fl_Single_Window.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Spinner.H>
//#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Table_Row.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Tiled_Image.H>
#include <FL/Fl_Tile.H>
//#include <FL/Fl_Timer.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Toggle_Light_Button.H>
#include <FL/Fl_Toggle_Round_Button.H>
#include <FL/Fl_Tooltip.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Tree_Item_Array.H>
#include <FL/Fl_Tree_Item.H>
#include <FL/Fl_Tree_Prefs.H>
#include <FL/Fl_Valuator.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Wizard.H>
#include <FL/Fl_XBM_Image.H>
#include <FL/Fl_XPM_Image.H>
//#include <FL/glut.H>
//#include <FL/forms.H>

#ifdef END_FLTK_INCLUDE
    END_FLTK_INCLUDE   // }
#endif

/* metatables names in the Lua registry */
#define MT_Adjuster "moonfltk_Adjuster"
#define MT_Bitmap "moonfltk_Bitmap"
#define MT_BMP_Image "moonfltk_BMP_Image"
#define MT_Box "moonfltk_Box"
#define MT_Browser_ "moonfltk_Browser_"
#define MT_Browser "moonfltk_Browser"
#define MT_Button "moonfltk_Button"
//#define MT_Cairo "moonfltk_Cairo"
//#define MT_Cairo_Window "moonfltk_Cairo_Window"
#define MT_Chart "moonfltk_Chart"
#define MT_Check_Browser "moonfltk_Check_Browser"
#define MT_Check_Button "moonfltk_Check_Button"
#define MT_Choice "moonfltk_Choice"
#define MT_Clock "moonfltk_Clock"
#define MT_Clock_Output "moonfltk_Clock_Output"
#define MT_Color_Chooser "moonfltk_Color_Chooser"
#define MT_Copy_Surface "moonfltk_Copy_Surface"
#define MT_Counter "moonfltk_Counter"
//#define MT_Device "moonfltk_Device"
#define MT_Dial "moonfltk_Dial"
#define MT_Double_Window "moonfltk_Double_Window"
//#define MT_Export "moonfltk_Export"
#define MT_File_Browser "moonfltk_File_Browser"
#define MT_File_Chooser "moonfltk_File_Chooser"
#define MT_File_Icon "moonfltk_File_Icon"
#define MT_File_Input "moonfltk_File_Input"
#define MT_Fill_Dial "moonfltk_Fill_Dial"
#define MT_Fill_Slider "moonfltk_Fill_Slider"
#define MT_Float_Input "moonfltk_Float_Input"
//#define MT_FormsBitmap "moonfltk_FormsBitmap"
//#define MT_FormsPixmap "moonfltk_FormsPixmap"
//#define MT_Free "moonfltk_Free"
#define MT_GIF_Image "moonfltk_GIF_Image"
#define MT_Gl_Window "moonfltk_Gl_Window"
#define MT_Group "moonfltk_Group"
#define MT_Help_Dialog "moonfltk_Help_Dialog"
#define MT_Help_View "moonfltk_Help_View"
#define MT_Hold_Browser "moonfltk_Hold_Browser"
#define MT_Hor_Fill_Slider "moonfltk_Hor_Fill_Slider"
#define MT_Hor_Nice_Slider "moonfltk_Hor_Nice_Slider"
#define MT_Hor_Slider "moonfltk_Hor_Slider"
#define MT_Hor_Value_Slider "moonfltk_Hor_Value_Slider"
#define MT_Image "moonfltk_Image"
#define MT_Image_Surface "moonfltk_Image_Surface"
#define MT_Input_Choice "moonfltk_Input_Choice"
#define MT_Input_ "moonfltk_Input_"
#define MT_Input "moonfltk_Input"
#define MT_Int_Input "moonfltk_Int_Input"
#define MT_JPEG_Image "moonfltk_JPEG_Image"
#define MT_Light_Button "moonfltk_Light_Button"
#define MT_Line_Dial "moonfltk_Line_Dial"
#define MT_Menu_Bar "moonfltk_Menu_Bar"
#define MT_Menu_Button "moonfltk_Menu_Button"
#define MT_Menu_ "moonfltk_Menu_"
#define MT_Menu "moonfltk_Menu"
//#define MT_Menu_Item "moonfltk_Menu_Item"
#define MT_Menu_Window "moonfltk_Menu_Window"
#define MT_Multi_Browser "moonfltk_Multi_Browser"
//#define MT_Multi_Label "moonfltk_Multi_Label"
#define MT_Multiline_Input "moonfltk_Multiline_Input"
#define MT_Multiline_Output "moonfltk_Multiline_Output"
#define MT_Native_File_Chooser "moonfltk_Native_File_Chooser"
#define MT_Nice_Slider "moonfltk_Nice_Slider"
//#define MT_Object "moonfltk_Object"
#define MT_Output "moonfltk_Output"
#define MT_Overlay_Window "moonfltk_Overlay_Window"
#define MT_Pack "moonfltk_Pack"
//#define MT_Paged_Device "moonfltk_Paged_Device"
#define MT_Pixmap "moonfltk_Pixmap"
//#define MT_Plugin "moonfltk_Plugin"
#define MT_PNG_Image "moonfltk_PNG_Image"
#define MT_PNM_Image "moonfltk_PNM_Image"
//#define MT_Positioner "moonfltk_Positioner"
#define MT_PostScript "moonfltk_PostScript"
//#define MT_Preferences "moonfltk_Preferences"
#define MT_Printer "moonfltk_Printer"
#define MT_Progress "moonfltk_Progress"
#define MT_Radio_Button "moonfltk_Radio_Button"
#define MT_Radio_Light_Button "moonfltk_Radio_Light_Button"
#define MT_Radio_Round_Button "moonfltk_Radio_Round_Button"
#define MT_Repeat_Button "moonfltk_Repeat_Button"
#define MT_Return_Button "moonfltk_Return_Button"
#define MT_RGB_Image "moonfltk_RGB_Image"
#define MT_Roller "moonfltk_Roller"
#define MT_Round_Button "moonfltk_Round_Button"
#define MT_Round_Clock "moonfltk_Round_Clock"
#define MT_Scrollbar "moonfltk_Scrollbar"
#define MT_Scroll "moonfltk_Scroll"
#define MT_Secret_Input "moonfltk_Secret_Input"
#define MT_Select_Browser "moonfltk_Select_Browser"
#define MT_Shared_Image "moonfltk_Shared_Image"
#define MT_Simple_Counter "moonfltk_Simple_Counter"
#define MT_Single_Window "moonfltk_Single_Window"
#define MT_Slider "moonfltk_Slider"
#define MT_Spinner "moonfltk_Spinner"
//#define MT_Sys_Menu_Bar "moonfltk_Sys_Menu_Bar"
#define MT_Table "moonfltk_Table"
#define MT_Table_Row "moonfltk_Table_Row"
#define MT_Tabs "moonfltk_Tabs"
#define MT_Text_Buffer "moonfltk_Text_Buffer"
#define MT_Text_Display "moonfltk_Text_Display"
#define MT_Text_Editor "moonfltk_Text_Editor"
#define MT_Tiled_Image "moonfltk_Tiled_Image"
#define MT_Tile "moonfltk_Tile"
//#define MT_Timer "moonfltk_Timer"
#define MT_Toggle_Button "moonfltk_Toggle_Button"
#define MT_Toggle_Light_Button "moonfltk_Toggle_Light_Button"
#define MT_Toggle_Round_Button "moonfltk_Toggle_Round_Button"
#define MT_Tooltip "moonfltk_Tooltip"
#define MT_Tree "moonfltk_Tree"
#define MT_Tree_Item_Array "moonfltk_Tree_Item_Array"
#define MT_Tree_Item "moonfltk_Tree_Item"
#define MT_Tree_Prefs "moonfltk_Tree_Prefs"
#define MT_Valuator "moonfltk_Valuator"
#define MT_Value_Input "moonfltk_Value_Input"
#define MT_Value_Output "moonfltk_Value_Output"
#define MT_Value_Slider "moonfltk_Value_Slider"
#define MT_Widget "moonfltk_Widget"
#define MT_Window "moonfltk_Window"
#define MT_Wizard "moonfltk_Wizard"
#define MT_XBM_Image "moonfltk_XBM_Image"
#define MT_XPM_Image "moonfltk_XPM_Image"
//#define MT_Glut_Window "moonfltk_Glut_Window"
//#define MT_FormsText "moonfltk_FormsText "


/* TEST/CHECK/PUSH FUNCTIONS
 * ----------------------------------------------------------------
 *
 * For every type Fl_Xxx, the following functions are defined:
 * 
 * Fl_Xxx* test_Xxx(lua_State *L, int arg)
 *      Similar to luaL_testudata(L, arg, MT_Xxx), but succeeds
 *      also if the type of the object is a subclass of Fl_Xxx. 
 *
 * Fl_Xxx* check_Xxx(lua_State *L, int arg)
 *      Similar to luaL_testudata(L, arg, MT_Xxx), but succeeds
 *      also if the type of the object is a subclass of Fl_Xxx. 
 *
 * ('check' and 'test' differ in that if the test fails, 'check'
 * raises an error, while 'test' returns NULL).
 *
 * int push_Xxx(lua_State *L, Fl_Xxx *p)
 *      Pushes on the stack the userdatum associated with p and
 *      returns 1 (=no of pushed values). 
 *      If no userdatum is associated with p, raises an error.
 *
 * (Associations between userdata and object pointers are made 
 * using udata_new() instead of lua_newuserdata()).
 */


#define TESTCHECKPUSH(T)                                \
inline Fl_##T* test_##T(lua_State *L, int arg)          \
    {                                                   \
    ud_t *ud = (ud_t*)udata_test(L, arg, MT_##T);       \
    return (Fl_##T*)(ud ? ud->obj : NULL);              \
    }                                                   \
                                                        \
inline Fl_##T *check_##T(lua_State *L, int arg)         \
    {                                                   \
    Fl_##T *p = test_##T(L, arg);                       \
    if(p) return p;                                     \
    luaL_argerror(L, arg, "arg is not a "#T);           \
    return NULL;                                        \
    }                                                   \
                                                        \
inline int push_##T(lua_State *L, Fl_##T *p)            \
    { return udata_push(L, p); }                        \
                                                        \
inline int push_##T##IfValid(lua_State *L, Fl_##T *p)   \
    { return udata_push_ifvalid(L, p); }


TESTCHECKPUSH(Adjuster)
TESTCHECKPUSH(Bitmap)
TESTCHECKPUSH(BMP_Image)
TESTCHECKPUSH(Box)
TESTCHECKPUSH(Browser_)
TESTCHECKPUSH(Browser)
TESTCHECKPUSH(Button)
//TESTCHECKPUSH(Cairo)
//TESTCHECKPUSH(Cairo_Window)
TESTCHECKPUSH(Chart)
TESTCHECKPUSH(Check_Browser)
TESTCHECKPUSH(Check_Button)
TESTCHECKPUSH(Choice)
TESTCHECKPUSH(Clock)
TESTCHECKPUSH(Clock_Output)
TESTCHECKPUSH(Color_Chooser)
TESTCHECKPUSH(Copy_Surface)
TESTCHECKPUSH(Counter)
//TESTCHECKPUSH(Device)
TESTCHECKPUSH(Dial)
TESTCHECKPUSH(Double_Window)
//TESTCHECKPUSH(Export)
TESTCHECKPUSH(File_Browser)
TESTCHECKPUSH(File_Chooser)
TESTCHECKPUSH(File_Icon)
TESTCHECKPUSH(File_Input)
TESTCHECKPUSH(Fill_Dial)
TESTCHECKPUSH(Fill_Slider)
TESTCHECKPUSH(Float_Input)
//TESTCHECKPUSH(FormsBitmap)
//TESTCHECKPUSH(FormsPixmap)
//TESTCHECKPUSH(Free)
TESTCHECKPUSH(GIF_Image)
TESTCHECKPUSH(Gl_Window)
TESTCHECKPUSH(Group)
TESTCHECKPUSH(Help_Dialog)
TESTCHECKPUSH(Help_View)
TESTCHECKPUSH(Hold_Browser)
TESTCHECKPUSH(Hor_Fill_Slider)
TESTCHECKPUSH(Hor_Nice_Slider)
TESTCHECKPUSH(Hor_Slider)
TESTCHECKPUSH(Hor_Value_Slider)
TESTCHECKPUSH(Image)
TESTCHECKPUSH(Image_Surface)
TESTCHECKPUSH(Input_Choice)
TESTCHECKPUSH(Input_)
TESTCHECKPUSH(Input)
TESTCHECKPUSH(Int_Input)
TESTCHECKPUSH(JPEG_Image)
TESTCHECKPUSH(Light_Button)
TESTCHECKPUSH(Line_Dial)
TESTCHECKPUSH(Menu_Bar)
TESTCHECKPUSH(Menu_Button)
TESTCHECKPUSH(Menu_)
TESTCHECKPUSH(Menu)
//TESTCHECKPUSH(Menu_Item) see menu.cc
TESTCHECKPUSH(Menu_Window)
TESTCHECKPUSH(Multi_Browser)
//TESTCHECKPUSH(Multi_Label)
TESTCHECKPUSH(Multiline_Input)
TESTCHECKPUSH(Multiline_Output)
TESTCHECKPUSH(Native_File_Chooser)
TESTCHECKPUSH(Nice_Slider)
//TESTCHECKPUSH(Object)
TESTCHECKPUSH(Output)
TESTCHECKPUSH(Overlay_Window)
TESTCHECKPUSH(Pack)
//TESTCHECKPUSH(Paged_Device)
TESTCHECKPUSH(Pixmap)
//TESTCHECKPUSH(Plugin)
TESTCHECKPUSH(PNG_Image)
TESTCHECKPUSH(PNM_Image)
//TESTCHECKPUSH(Positioner)
//TESTCHECKPUSH(PostScript)
//TESTCHECKPUSH(Preferences)
TESTCHECKPUSH(Printer)
TESTCHECKPUSH(Progress)
TESTCHECKPUSH(Radio_Button)
TESTCHECKPUSH(Radio_Light_Button)
TESTCHECKPUSH(Radio_Round_Button)
TESTCHECKPUSH(Repeat_Button)
TESTCHECKPUSH(Return_Button)
TESTCHECKPUSH(RGB_Image)
TESTCHECKPUSH(Roller)
TESTCHECKPUSH(Round_Button)
TESTCHECKPUSH(Round_Clock)
TESTCHECKPUSH(Scrollbar)
TESTCHECKPUSH(Scroll)
TESTCHECKPUSH(Secret_Input)
TESTCHECKPUSH(Select_Browser)
TESTCHECKPUSH(Shared_Image)
TESTCHECKPUSH(Simple_Counter)
TESTCHECKPUSH(Single_Window)
TESTCHECKPUSH(Slider)
TESTCHECKPUSH(Spinner)
//TESTCHECKPUSH(Sys_Menu_Bar)
TESTCHECKPUSH(Table)
TESTCHECKPUSH(Table_Row)
TESTCHECKPUSH(Tabs)
TESTCHECKPUSH(Text_Buffer)
TESTCHECKPUSH(Text_Display)
TESTCHECKPUSH(Text_Editor)
TESTCHECKPUSH(Tiled_Image)
TESTCHECKPUSH(Tile)
//TESTCHECKPUSH(Timer)
TESTCHECKPUSH(Toggle_Button)
TESTCHECKPUSH(Toggle_Light_Button)
TESTCHECKPUSH(Toggle_Round_Button)
TESTCHECKPUSH(Tooltip)
TESTCHECKPUSH(Tree)
//TESTCHECKPUSH(Tree_Item_Array)
//TESTCHECKPUSH(Tree_Item)
TESTCHECKPUSH(Tree_Prefs)
TESTCHECKPUSH(Valuator)
TESTCHECKPUSH(Value_Input)
TESTCHECKPUSH(Value_Output)
TESTCHECKPUSH(Value_Slider)
TESTCHECKPUSH(Widget)
TESTCHECKPUSH(Window)
TESTCHECKPUSH(Wizard)
TESTCHECKPUSH(XBM_Image)
TESTCHECKPUSH(XPM_Image)
//TESTCHECKPUSH(Glut_Window)
//TESTCHECKPUSH(FormsText)

/*------------------------------------------------------------------------------*
 |  Subclasses                                                                  |
 *------------------------------------------------------------------------------*/

#define DRAWETC                     \
    void draw();                    \
    void hide();                    \
    void show();                    \
    void resize(int, int, int, int);\
    int handle(int);

#define SUPER_DRAWETC(T)                                                        \
/* Each of these methods executes the corresponding method of the base class */ \
    void super_draw() { Fl_##T::draw(); }                                       \
    void super_hide() { Fl_##T::hide(); }                                       \
    void super_show() { Fl_##T::show(); }                                       \
    void super_resize(int x, int y, int w, int h) { Fl_##T::resize(x, y, w, h); }   \
    int super_handle(int e) { return Fl_##T::handle(e); }

//--------------------------------------------------------------------------------

class Fl_WidgetSUB : public Fl_Widget {
    DRAWETC
public : 
    Fl_WidgetSUB(int x, int y, int w, int h, const char* l) : Fl_Widget(x,y,w,h,l) { }
    SUPER_DRAWETC(Widget)
};
#define MT_WidgetSUB "moonfltk_WidgetSUB"
TESTCHECKPUSH(WidgetSUB)

//--------------------------------------------------------------------------------

class Fl_BoxSUB : public Fl_Box {
    DRAWETC
public : 
    Fl_BoxSUB(int x, int y, int w, int h, const char* l) : Fl_Box(x,y,w,h,l) { }
    Fl_BoxSUB(Fl_Boxtype b, int x, int y, int w, int h, const char *l) : Fl_Box(b,x,y,w,h,l) { }
    SUPER_DRAWETC(Box)
};
#define MT_BoxSUB "moonfltk_BoxSUB"
TESTCHECKPUSH(BoxSUB)

//--------------------------------------------------------------------------------

class Fl_ButtonSUB : public Fl_Button {
    DRAWETC
public : 
    Fl_ButtonSUB(int x, int y, int w, int h, const char* l) : Fl_Button(x,y,w,h,l) { }
    SUPER_DRAWETC(Button)
};
#define MT_ButtonSUB "moonfltk_ButtonSUB"
TESTCHECKPUSH(ButtonSUB)


//--------------------------------------------------------------------------------

class Fl_GroupSUB : public Fl_Group {
    DRAWETC
public : 
    Fl_GroupSUB(int x, int y, int w, int h, const char* l) : Fl_Group(x,y,w,h,l) { }
    SUPER_DRAWETC(Group)
};
#define MT_GroupSUB "moonfltk_GroupSUB"
TESTCHECKPUSH(GroupSUB)

//--------------------------------------------------------------------------------

class Fl_BrowserSUB : public Fl_Browser {
    DRAWETC
public : 
    Fl_BrowserSUB(int x, int y, int w, int h, const char* l) : Fl_Browser(x,y,w,h,l) { }
    SUPER_DRAWETC(Browser)
};
#define MT_BrowserSUB "moonfltk_BrowserSUB"
TESTCHECKPUSH(BrowserSUB)

#if 0
//--------------------------------------------------------------------------------

class Fl_File_BrowserSUB : public Fl_File_Browser {
    DRAWETC
public : 
    Fl_File_BrowserSUB(int x, int y, int w, int h, const char* l) : Fl_File_Browser(x,y,w,h,l) { }
    SUPER_DRAWETC(File_Browser)
};
#define MT_File_BrowserSUB "moonfltk_File_BrowserSUB"
TESTCHECKPUSH(File_BrowserSUB)

//--------------------------------------------------------------------------------

class Fl_Hold_BrowserSUB : public Fl_Hold_Browser {
    DRAWETC
public : 
    Fl_Hold_BrowserSUB(int x, int y, int w, int h, const char* l) : Fl_Hold_Browser(x,y,w,h,l) { }
    SUPER_DRAWETC(Hold_Browser)
};
#define MT_Hold_BrowserSUB "moonfltk_Hold_BrowserSUB"
TESTCHECKPUSH(Hold_BrowserSUB)

//--------------------------------------------------------------------------------

class Fl_Multi_BrowserSUB : public Fl_Multi_Browser {
    DRAWETC
public : 
    Fl_Multi_BrowserSUB(int x, int y, int w, int h, const char* l) : Fl_Multi_Browser(x,y,w,h,l) { }
    SUPER_DRAWETC(Multi_Browser)
};
#define MT_Multi_BrowserSUB "moonfltk_Multi_BrowserSUB"
TESTCHECKPUSH(Multi_BrowserSUB)


//--------------------------------------------------------------------------------

class Fl_Select_BrowserSUB : public Fl_Select_Browser {
    DRAWETC
public : 
    Fl_Select_BrowserSUB(int x, int y, int w, int h, const char* l) : Fl_Select_Browser(x,y,w,h,l) { }
    SUPER_DRAWETC(Select_Browser)
};
#define MT_Select_BrowserSUB "moonfltk_Select_BrowserSUB"
TESTCHECKPUSH(Select_BrowserSUB)

//--------------------------------------------------------------------------------

class Fl_Check_BrowserSUB : public Fl_Check_Browser {
    DRAWETC
public : 
    Fl_Check_BrowserSUB(int x, int y, int w, int h, const char* l) : Fl_Check_Browser(x,y,w,h,l) { }
    SUPER_DRAWETC(Check_Browser)
};
#define MT_Check_BrowserSUB "moonfltk_Check_BrowserSUB"
TESTCHECKPUSH(Check_BrowserSUB)

#endif


//--------------------------------------------------------------------------------

class Fl_TableSUB : public Fl_Table {
    DRAWETC
    void draw_cell(TableContext, int, int, int, int, int, int); 
public : 
    Fl_TableSUB(int x, int y, int w, int h, const char* l) : Fl_Table(x,y,w,h,l) { }
    SUPER_DRAWETC(Table)
};
#define MT_TableSUB "moonfltk_TableSUB"
TESTCHECKPUSH(TableSUB)

//--------------------------------------------------------------------------------

class Fl_Table_RowSUB : public Fl_Table_Row {
    DRAWETC
    void draw_cell(TableContext, int, int, int, int, int, int); 
public : 
    Fl_Table_RowSUB(int x, int y, int w, int h, const char* l) : Fl_Table_Row(x,y,w,h,l) { }
    SUPER_DRAWETC(Table_Row)
};
#define MT_Table_RowSUB "moonfltk_Table_RowSUB"
TESTCHECKPUSH(Table_RowSUB)



//--------------------------------------------------------------------------------

class Fl_TabsSUB : public Fl_Tabs {
    DRAWETC
public : 
    Fl_TabsSUB(int x, int y, int w, int h, const char* l) : Fl_Tabs(x,y,w,h,l) { }
    SUPER_DRAWETC(Tabs)
};
#define MT_TabsSUB "moonfltk_TabsSUB"
TESTCHECKPUSH(TabsSUB)


//--------------------------------------------------------------------------------

class Fl_WindowSUB : public Fl_Window {
    DRAWETC
public : 
    Fl_WindowSUB(int x, int y, int w, int h, const char* l) : Fl_Window(x,y,w,h,l) { }
    Fl_WindowSUB(int w, int h, const char* l) : Fl_Window(w,h,l) { }
    SUPER_DRAWETC(Window)
};

#define MT_WindowSUB "moonfltk_WindowSUB"
TESTCHECKPUSH(WindowSUB)

//--------------------------------------------------------------------------------

class Fl_Double_WindowSUB : public Fl_Double_Window {
    DRAWETC
public : 
    Fl_Double_WindowSUB(int x, int y, int w, int h, const char* l) : Fl_Double_Window(x,y,w,h,l) { }
    Fl_Double_WindowSUB(int w, int h, const char* l) : Fl_Double_Window(w,h,l) { }
    SUPER_DRAWETC(Double_Window)
};

#define MT_Double_WindowSUB "moonfltk_Double_WindowSUB"
TESTCHECKPUSH(Double_WindowSUB)

//--------------------------------------------------------------------------------

class Fl_Overlay_WindowSUB : public Fl_Overlay_Window {
    DRAWETC
    void draw_overlay();
public : 
    Fl_Overlay_WindowSUB(int x, int y, int w, int h, const char* l) : Fl_Overlay_Window(x,y,w,h,l) { }
    Fl_Overlay_WindowSUB(int w, int h, const char* l) : Fl_Overlay_Window(w,h,l) { }
    SUPER_DRAWETC(Overlay_Window)
};

#define MT_Overlay_WindowSUB "moonfltk_Overlay_WindowSUB"
TESTCHECKPUSH(Overlay_WindowSUB)

//--------------------------------------------------------------------------------

#if USE_GL

class Fl_Gl_WindowSUB : public Fl_Gl_Window {
    DRAWETC
public : 
    Fl_Gl_WindowSUB(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x,y,w,h,l) { }
    Fl_Gl_WindowSUB(int w, int h, const char* l) : Fl_Gl_Window(w,h,l) { }
    SUPER_DRAWETC(Gl_Window)
};

#define MT_Gl_WindowSUB "moonfltk_Gl_WindowSUB"
TESTCHECKPUSH(Gl_WindowSUB)

#endif

//--------------------------------------------------------------------------------

class Fl_Single_WindowSUB : public Fl_Single_Window {
    DRAWETC
public : 
    Fl_Single_WindowSUB(int x, int y, int w, int h, const char* l) : Fl_Single_Window(x,y,w,h,l) { }
    Fl_Single_WindowSUB(int w, int h, const char* l) : Fl_Single_Window(w,h,l) { }
    SUPER_DRAWETC(Single_Window)
};

#define MT_Single_WindowSUB "moonfltk_Single_WindowSUB"
TESTCHECKPUSH(Single_WindowSUB)

//--------------------------------------------------------------------------------

class Fl_Menu_WindowSUB : public Fl_Menu_Window {
    DRAWETC
public : 
    Fl_Menu_WindowSUB(int x, int y, int w, int h, const char* l) : Fl_Menu_Window(x,y,w,h,l) { }
    Fl_Menu_WindowSUB(int w, int h, const char* l) : Fl_Menu_Window(w,h,l) { }
    SUPER_DRAWETC(Menu_Window)
};

#define MT_Menu_WindowSUB "moonfltk_Menu_WindowSUB"
TESTCHECKPUSH(Menu_WindowSUB)

//--------------------------------------------------------------------------------

class Fl_RollerSUB : public Fl_Roller {
    DRAWETC
public : 
    Fl_RollerSUB(int x, int y, int w, int h, const char* l) : Fl_Roller(x,y,w,h,l) { }
    SUPER_DRAWETC(Roller)
};
#define MT_RollerSUB "moonfltk_RollerSUB"
TESTCHECKPUSH(RollerSUB)


//--------------------------------------------------------------------------------

class Fl_SliderSUB : public Fl_Slider {
    DRAWETC
public : 
    Fl_SliderSUB(int x, int y, int w, int h, const char* l) : Fl_Slider(x,y,w,h,l) { }
    SUPER_DRAWETC(Slider)
};
#define MT_SliderSUB "moonfltk_SliderSUB"
TESTCHECKPUSH(SliderSUB)



#undef DRAWETC
#undef SUPER_DRAWETC
#undef TESTCHECKPUSH

#endif /* mtDEFINED */

