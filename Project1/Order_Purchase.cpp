#include "Order_Purchase.h"

std::string string_to_symbol(std::istream& in, char symbol);	// scan a string upto the symbol
std::ostream& operator<<(std::ostream& out, Purchase a);
std::istream& operator>>(std::istream& in, Purchase& a);
std::ostream& operator<<(std::ostream& out, Order a);
std::istream& operator>>(std::istream& in, Order& a);

std::string string_to_symbol(std::istream& in, char symbol) {
	std::string ret;
	char temp;
	while (true)
	{
		temp = in.get();
		if (temp != symbol)
			ret.push_back(temp);
		else
		{
			while (ret.size() > 0 && (std::isspace(* ret.begin())))	// remove starting spaces, a necessary evil for scanning a string with spaces on a shared line
				ret.erase(ret.begin());
			return ret;
		}

		if (!in)
			return std::string();
	}
}
std::ostream& operator<<(std::ostream& out, Purchase a)
{
	out << Purchase::opener
	<< a.product_name << Purchase::delim << a.unit_price << Purchase::delim<< a.count
	<< Purchase::closer << std::endl;

return out;
}


std::istream& operator>>(std::istream& in, Purchase& a) {
	std::string product_name;
	double unit_price{ Purchase::invalid };
	int count{ Purchase::invalid };

	char temp;
	in >> temp;
	if (temp != Purchase::opener) {
		in.putback(temp);
		a = Purchase{};
		return in;
	}
	in >> temp;
	if (temp == Purchase::closer)
		return in;
	in.putback(temp);
	product_name = string_to_symbol(in, Purchase::delim);
	in >> unit_price; in.get();
	in >> count;
	string_to_symbol(in, Purchase::closer);
	a = Purchase{ product_name, unit_price, count };

	return in;
}

std::ostream& operator<<(std::ostream& out, Order a) {
	out << a.name << Order::delim << a.address << Order::delim << a.data << std::endl;

	if (a.purchases.size() == 0)	// print representation of empty purchases
		out << Purchase::opener << Purchase::closer << std::endl;
	for (auto elem : a.purchases)	// print purchases
		out << elem;

	return out;
}

std::istream& operator>>(std::istream& in, Order& a) {
	std::string name;
	std::string address;
	double data;
	std::vector<Purchase> purchases;

	name = string_to_symbol(in, Order::delim);
	address = string_to_symbol(in, Order::delim);

	in >> data; in.get();
	Purchase temp;
	in >> temp;
	while (temp) {
		purchases.push_back(temp);
		in >> temp;
	}

	a.name = name;
	a.address = address;
	a.data = data;
	a.purchases = purchases;

	return in;
}