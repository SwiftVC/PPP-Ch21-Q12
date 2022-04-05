#ifndef _CS_TEXT_DISPLAY_H_
#define _CS_TEXT_DISPLAY_H_

#include "Fl/Fl_Text_Display.H"
#include "Gui.h"
#include "std_lib_facilities.h"
#include <sstream>

// objects of CS_Text_Display are
// 1. declared with the constructor
// 2. attached to the window via Window::attach(CS_Text_Display)
// 3. the text in the buffer is set by: CS_Text_Display[OBJ].set_buffer(std::string); AFTER the object has been attached

class CS_Text_Display : public Widget {
public:
	CS_Text_Display(Point xy, int ww, int hh, const std::string& s="", Callback cb = nullptr)
		: Widget(xy, ww, hh, s, cb), text_buffer{ nullptr } {}


	void set_buffer(std::string str);
	void attach(Graph_lib::Window& win);
private:
	Fl_Text_Buffer* text_buffer;
};



void CS_Text_Display::attach(Graph_lib::Window& win)
{
	text_buffer = new Fl_Text_Buffer();
	pw = new Fl_Text_Display(loc.x, loc.y, width, height, label.c_str());
	(static_cast<Fl_Text_Display*>(pw))->buffer(text_buffer);
	pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
	pw->align(FL_ALIGN_LEFT);	// hardcoded left align as seen in others

	own = &win;
}

void CS_Text_Display::set_buffer(std::string str) {
	if (text_buffer == nullptr)
		throw std::exception("CS_Text_Display must be attached to Window before setting buffer");
	text_buffer->text(str.c_str());   // replace buffer with str
}

#endif