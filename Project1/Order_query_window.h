#ifndef ORDER_QUERY_WINDOW_H_
#define ORDER_QUERY_WINDOW_H_

#include "CS_Shapes.h"
#include "fltk.h"
#include "Gui.h"
#include "Window.h"
#include "Order_pad.h"
#include "CS_Text_Display.h"

class Order_query_window : public Window {
private:
	int display_box_w = 250;
	int display_box_h = 100;

	Point tl_offset{ 50, 50 };
	Point pt_file_name{ tl_offset.x + display_box_w, tl_offset.y + 0 * display_box_h };
	Point pt_orders_from_name{ tl_offset.x + display_box_w, tl_offset.y + 1 * display_box_h };
	Point pt_total_value_of_orders{ tl_offset.x + display_box_w, tl_offset.y + 2 * display_box_h };
	Point pt_list_all_orders_output{ tl_offset.x + display_box_w, tl_offset.y + 3 * display_box_h };
	std::string _filename;
	std::string _customername;

public:
	// shape/widgets listed in representation of GUI
	In_box file_name_input{ pt_file_name, display_box_w, display_box_h, "File name:" };													Button submit_file_name{ Point{pt_file_name.x + display_box_w, pt_file_name.y}, display_box_w, display_box_h, "SUBMIT", cb_get_filename };
	In_box orders_from_name_input{ pt_orders_from_name, display_box_w, display_box_h,"Orders from customer:" };							Button submit_cust_name{ Point{pt_orders_from_name.x + display_box_w, pt_orders_from_name.y}, display_box_w, display_box_h, "SUBMIT", cb_get_custname };	 CS_Text_Display orders_from_name_output{ Point{pt_orders_from_name.x + 2 * display_box_w, pt_orders_from_name.y}, display_box_w, display_box_h };
	Out_box total_value_of_orders_output{ pt_total_value_of_orders, display_box_w, display_box_h, "Find the total value of orders" };
	CS_Text_Display list_all_orders_output{ pt_list_all_orders_output, 2*display_box_w, 2*display_box_h, "List all orders" };

	Button next_button;
	bool button_pushed;
	vector<Order> orders;
	Order_query_window(Point xy, int w, int h, const string& title)
		: Window(xy, w, h, title),
		button_pushed(false),
		next_button(Point(x_max() - 70, 0), 70, 20, "Exit", cb_next)
	{
		attach(next_button);

		attach(file_name_input);	attach(submit_file_name);
		attach(orders_from_name_input); attach(submit_cust_name); attach(orders_from_name_output);
		attach(total_value_of_orders_output);
		attach(list_all_orders_output);
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

	void next() { button_pushed = true; }
	static void cb_next(Address, Address addr) // callback for next_button
	//	{ reference_to<Simple_window>(addr).next(); }
	{
		static_cast<Order_query_window*>(addr)->next();
	}

	static void cb_get_filename(Address, Address addr) // callback for next_button
	//	{ reference_to<Simple_window>(addr).next(); }
	{
		std::string filename = static_cast<Order_query_window*>(addr)->file_name_input.get_string();
		static_cast<Order_query_window*>(addr)->_filename = filename;
		
		static_cast<Order_query_window*>(addr)->file_name_given(addr);
	}

	static void cb_get_custname(Address, Address addr) // callback for next_button
	//	{ reference_to<Simple_window>(addr).next(); }
	{
		std::string name = static_cast<Order_query_window*>(addr)->orders_from_name_input.get_string();
		static_cast<Order_query_window*>(addr)->_customername = name;

		static_cast<Order_query_window*>(addr)->customer_name_given(addr);
	}
	
	static void file_name_given(Address addr) {
		// scan orders into vector
		static_cast<Order_query_window*>(addr)->orders.clear();
		static_cast<Order_query_window*>(addr)->scan_orders_into_vector(addr);

		if (static_cast<Order_query_window*>(addr)->orders.size() > 0)
		{
			// fill total value of orders
			static_cast<Order_query_window*>(addr)->fill_total_value(addr);
			// fill list all orders
			static_cast<Order_query_window*>(addr)->fill_all_orders(addr);
		}
	}

	static void customer_name_given(Address addr) {
		if (static_cast<Order_query_window*>(addr)->orders.size() > 0)
		{
			// fill total value of orders
			static_cast<Order_query_window*>(addr)->fill_individuals_orders(addr);
			
		}
	}

	static void scan_orders_into_vector(Address addr) {
		std::ifstream in{ static_cast<Order_query_window*>(addr)->_filename };
		Order temp;
		while (in)
		{
			in >> temp;
			static_cast<Order_query_window*>(addr)->orders.push_back(temp);
		}
	}

	static void fill_total_value(Address addr) {
		double sum{ 0 };
		for (auto temp : static_cast<Order_query_window*>(addr)->orders)
			sum += temp.get_value();
		static_cast<Order_query_window*>(addr)->total_value_of_orders_output.put(std::to_string(sum));
	}

	static void fill_all_orders(Address addr){
		std::string str;
		vector<Order> orders = static_cast<Order_query_window*>(addr)->orders;
		std::stringstream stringstr;
		for (auto temp : orders){
			stringstr << temp << '\n';
		}
		while (stringstr)
		{
			str += stringstr.get();
		}
		if(str.size()>0)
			str.pop_back();

		static_cast<Order_query_window*>(addr)->list_all_orders_output.set_buffer(str);
	}

	static void fill_individuals_orders(Address addr) {
		std::string name = static_cast<Order_query_window*>(addr)->_customername;

		std::stringstream stringstr;
		for (int i = 0; i < static_cast<Order_query_window*>(addr)->orders.size(); i++)
			if (static_cast<Order_query_window*>(addr)->orders[i].get_name() == name)
				stringstr << static_cast<Order_query_window*>(addr)->orders[i] << '\n';

		std::string str;
		while (stringstr)
		{
			str += stringstr.get();
		}
		if (str.size() > 0)
			str.pop_back();

		static_cast<Order_query_window*>(addr)->orders_from_name_output.set_buffer(str);
	}
};


#endif