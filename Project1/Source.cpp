/*
*	Author:	CS
*	Date:	2022_03_21	YYYY_MM_DD
* 
*	GUI interface for querying a file of orders.
*	file1.txt is provided, with "Zulu Lima" having multiple orders
*
*	New header files:
*	CS_Text_Display.h	 -	interface for text display widget in style (and hierarchy) of PP&P GUI headers- required for large amount of text output
*	Order_query_window.h -	GUI class for window object
*
*	Improvements:
*	Input filtering is not used for 'valid' purchases and orders,
*	empty fields add junk contents.
* 
*	Correction:
*	file scanning for vector<Orders> was missing the last entry, correct scanning in this question. (error in question 10)
* 
*/

#include <iostream>
#include "Order_Purchase.h"
#include <iterator>	// for std::istream_iterator
#include "Order_query_window.h"

int main() {
	try {
		Point tl{ 50,50 };
		constexpr int width = 1200;
		constexpr int height = 600;
		Order_query_window win{ tl, width, height, "Enter file1.txt" };	
		win.wait_for_button();
		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}