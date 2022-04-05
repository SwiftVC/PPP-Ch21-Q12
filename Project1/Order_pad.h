#include "GUI.h"	// for Simple_window only (doesn't really belong in Window.h)
#include "Order_Purchase.h"
#ifndef Order_pad_h_
#define Order_pad_h_
using namespace Graph_lib;

// widgets: must be attached via Window::attach, every widget belongs to a window
class Purchase_box;
class Order_window;
class Purchase_box : public Shape {
	double height_factor = 0.2;
public:	
	Purchase_box(Point p, int width, int height, Order_window* win, void cb_add_purchase(Address, Address)) :
		owned_by{ win },
		title{ p, "Purchase" },
		name{ p, width,int(height*height_factor),"Product name:" },
		price{ Point{p.x, p.y + int(0.25*height)}, width, int(height*height_factor), "Unit Price:" },
		count{ Point{p.x, p.y + int(0.50*height)}, width, int(height * height_factor), "Count:" },
		add_purchase{ Point{p.x, p.y + int(0.75*height)}, width, int(height * height_factor), "Add purchase", cb_add_purchase },
		frame{ p, Point(p.x + width, p.y + height) }
	{
		add(p);
		add(Point(p.x + width, p.y + height));
		set_color(Graph_lib::Color::black);
	}

	void draw_lines() const {
		title.draw_lines();
		frame.draw_lines();
	}

	void attach_widgets(Graph_lib::Window& win) {
		name.attach(win);	win.attach(name);
		price.attach(win);	win.attach(price);
		count.attach(win);	win.attach(count);
		add_purchase.attach(win);	win.attach(add_purchase);
		name.show();
		price.show();
		count.show();
		add_purchase.show();
	}


public:
	Text title;
	In_box name;
	In_box price;
	In_box count;
	Button add_purchase;
	Graph_lib::Rectangle frame;
	Order_window* owned_by;
};

class Order_box : public Shape {
	int y_spacing = 20;
public:
	Order_box(Point p, int width, int height, Order_window* win) :
		owned_by{win},
		title{ p, "Order" },
		name{p, width,y_spacing,"Name:"},
		address{Point{p.x, p.y + y_spacing}, width, y_spacing, "Address:"},
		data{ Point{p.x, p.y + 2*y_spacing}, width, y_spacing, "Data:"},
		add_order{Point{p.x, p.y + height}, width, y_spacing, "Add order", cb_add_order},
		frame{ p, Point(p.x + width, p.y + height) }
	{
		add(p);
		add(Point(p.x + width, p.y + height));
		set_color(Graph_lib::Color::black);
	}

	void draw_lines() const {
		title.draw_lines();
		frame.draw_lines();
	}

	void attach_widgets(Graph_lib::Window& win) {
		name.attach(win);	win.attach(name);
		address.attach(win);	win.attach(address);
		data.attach(win);	win.attach(data);
		add_order.attach(win);	win.attach(add_order);
		name.show();
		address.show();
		data.show();
		add_order.show();
	}

	// pw is the address of the window which the button is attached
	static void cb_add_order(Address, Address pw);

	// Had to be defined below Order_window, which it calls
	static void set_order_member_args(Address& pw);	// sets s_name, s_address and s_data to order_box data



public:
	Text title;
	In_box name;
	In_box address;
	In_box data;
	Button add_order;
	Graph_lib::Rectangle frame;
	Order_window* owned_by;
	std::string s_name;
	std::string s_address;
	std::string s_data;
	
};


class Order_window : public Window {
public:
	const int p_box_spacing = 150;
	const int purch_box_w = 150;
	int p_box_horiz_spacing = purch_box_w*2;
	const int purch_box_h = 100;
	const int purch_box_shift = 350;

	Button next_button;
	bool button_pushed;
	Order_box order_box;
	std::array<Purchase_box*, 4> purchase_boxes;
	vector<Order> orders;
	std::array<Purchase, 4> data_purch;

	void write_orders(std::string output) {
		std::ofstream outfile{ output };
		for (auto elem : orders)
			outfile << elem << std::endl;
	}

	// add information from purchase 1-4 to array element 0-3
	static void cb_add_purchase1(Address, Address addr)
	{auto win = static_cast<Order_window*>(addr);	win->set_purchase_index_i_args(addr, 0);}

	static void cb_add_purchase2(Address, Address addr)
	{auto win = static_cast<Order_window*>(addr);	win->set_purchase_index_i_args(addr, 1);}

	static void cb_add_purchase3(Address, Address addr)
	{auto win = static_cast<Order_window*>(addr);	win->set_purchase_index_i_args(addr, 2);}

	static void cb_add_purchase4(Address, Address addr)
	{auto win = static_cast<Order_window*>(addr);	win->set_purchase_index_i_args(addr, 3);}

	void set_purchase_index_i_args(Address& pw, int i) {
		auto win = static_cast<Order_window*>(pw);
		auto box = win->purchase_boxes[i];

		std::string name = box->name.get_string();
		std::string price = box->name.get_string();
		std::stringstream ss;
		ss << price;
		double dbl_price;
		ss >> dbl_price;
		int count = box->count.get_int();
		win->data_purch[i] = Purchase{ name, dbl_price, count };
		return;
	}

public:
	Order_window(Point xy, int w, int h, const string& title)
		: Window(xy, w, h, title),
		button_pushed(false),
		next_button(Point(x_max() - 70, 0), 70, 20, "Next", cb_next),
		order_box{ Point(80, 50), 200, 100, this },
		purchase_boxes{
		new Purchase_box(Point{xy.x + purch_box_shift, xy.y}, purch_box_w, purch_box_h, this, cb_add_purchase1),
		new Purchase_box(Point{xy.x + purch_box_shift, xy.y + p_box_spacing}, purch_box_w, purch_box_h, this, cb_add_purchase2),
		new Purchase_box(Point{xy.x + purch_box_shift + p_box_horiz_spacing, xy.y}, purch_box_w, purch_box_h, this, cb_add_purchase3),
		new Purchase_box(Point{xy.x + purch_box_shift + p_box_horiz_spacing, xy.y + p_box_spacing}, purch_box_w, purch_box_h, this, cb_add_purchase4) }
	{
		attach(next_button);
		attach(order_box);
		order_box.attach_widgets(*this);
		for (auto ptr : purchase_boxes) {
			attach(*ptr);
			ptr->attach_widgets(*this);
		}
	}

	void wait_for_button()
		// modified event loop
		// handle all events (as per default), but quit when button_pushed becomes true
		// this allows graphics without control inversion
	{
		while (!button_pushed) Fl::wait();
		button_pushed = false;
		Fl::redraw();
	}


	static void cb_next(Address, Address addr) // callback for next_button
	//	{ reference_to<Simple_window>(addr).next(); }
	{
		static_cast<Order_window*>(addr)->next();
	}
	
	void next() { button_pushed = true; }
};

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void Order_box::set_order_member_args(Address& pw) {
	auto win = static_cast<Order_window*>(pw);
	win->order_box.s_name = win->order_box.name.get_string();
	win->order_box.s_address = win->order_box.address.get_string();
	win->order_box.s_data = win->order_box.data.get_string();
	return;
}

void Order_box::cb_add_order(Address, Address pw) {
	set_order_member_args(pw);
	// call window and take convert any purchases in the array of four, with a title to an order for the vector
	auto win = static_cast<Order_window*>(pw);
	double dbl_data;
	std::string data = win->order_box.s_data;
	std::stringstream ss;
	ss << data;
	ss >> dbl_data;


	// populate vector of purchases
	vector<Purchase> vect;
	for (int i = 0; i < win->data_purch.size(); i++)
		if (win->data_purch[i].product_name != "")
			vect.push_back(win->data_purch[i]);
	Order neworder{ win->order_box.s_name, win->order_box.s_address,dbl_data, vect };
	win->orders.push_back(neworder);
}
/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#endif