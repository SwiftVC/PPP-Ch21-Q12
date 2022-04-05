#ifndef _ORDER_PURCHASE_H_
#define _ORDER_PURCHASE_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <exception>

class Purchase {
public:
	std::string product_name;
	double unit_price;
	int count;

	Purchase(std::string prod_name, double unit_cost, int cnt)
		: product_name{ prod_name }, unit_price{ unit_cost }, count{ cnt } {}
	Purchase() : product_name{ "" }, unit_price{ invalid }, count{ invalid } {}
	friend std::ostream& operator<<(std::ostream& out, Purchase a);
	Purchase& operator=(Purchase a) { product_name = a.product_name; unit_price = a.unit_price; count = a.count; return *this; }

	const static char opener = '{';
	const static char closer = '}';
	const static char delim = ',';
	const static int invalid = -1;	// numerical value is uninitialzed
	operator bool() const { return (product_name == "" || unit_price == Purchase::invalid || count == Purchase::invalid) ? false : true; }
	double get_value() { return unit_price * count; }
};

class Order {
private:
	std::string name;
	std::string address;
	double data;
	std::vector<Purchase> purchases;
public:
	Order(std::string customer_name, std::string customer_address, double customer_cost, std::vector<Purchase> purchases)
		: name{ customer_name }, address{ customer_address }, data{ customer_cost }, purchases{ purchases } {}
	friend std::ostream& operator<<(std::ostream& out, Order a);
	friend std::istream& operator>>(std::istream& in, Order& a);
	Order() {}
	operator bool() const { return (name == "" || address == "") ? false : true; }
	const std::string get_name() const { return name; }
	const std::string get_address() const { return address; }
	double get_value() { double sum{ 0 }; for (auto temp : purchases) sum += temp.get_value(); return sum; }
public:
	const static char delim = ',';

};

#endif