#include "CS_Shapes.h"
#include <cmath>
namespace Graph_lib {
	vector<Point> get_polygon_coordinates(int number_of_sides, double side_length, Point origin);
	
	void Binary_tree::add_text(string content, string location) {
		// determine the nodetext index from location

		int outer_index = 1;
		int inner_index = 0;
		for (int i = 0; i < location.size(); i++) {
			char ch = location[i];
			switch (ch) {
			case 'r':
				inner_index = inner_index * 2 + 1;
				outer_index++;
				break;
			case 'l':
				inner_index *= 2;
				outer_index++;
				break;
			default:
				error("Invalid character in add_text's second argument");

			}
		}

		nodetext[outer_index][inner_index] = content;

	}

	void Regular_triangle::draw_lines() const {
		Regular_polygon poly{ c, 3, distance_center_to_corner };
		poly.set_color(color());	poly.set_fill_color(fill_color());	poly.set_style(style());
		poly.draw_lines();
	}

	Point  n(const Regular_triangle& t) {
		return Point{ t.center().x, t.center().y - int(t.to_corner())};
	}
	Point se(const Regular_triangle& t) {
		return Point{ t.center().x + int(t.to_corner() * cos(pi/6)), t.center().y + int(t.to_corner() * sin(pi / 6))};
	}
	Point sw(const Regular_triangle& t) {
		return Point{ t.center().x - int(t.to_corner() * cos(pi / 6)), t.center().y + int(t.to_corner() * sin(pi / 6)) };
	}

	Point center(const Regular_triangle& t){
		return t.center();
	}

	Point Binary_triangle_tree::node_base() const {
		Regular_triangle tri{ Point{point(0).x, point(0).y}, double(get_from_center()) };
		tri.set_color(color());	tri.set_fill_color(fill_color());	tri.set_style(style());
		tri.draw_lines();

		return center(tri);
	}

	void Binary_triangle_tree::node_extension(Point c_base_shape, Point& lhs_tri_c, Point& ths_tri_c, int line_delta_x, int line_delta_y) const {	// expects and returns centre coordinates
		fl_line_style(FL_SOLID, 1);
		Fl_Color oldc = fl_color();
		
		Regular_triangle basetri{ Point{c_base_shape.x , c_base_shape.y }, double(get_from_center()) };
		Point start_of_rhs_line{ se(basetri).x, se(basetri).y};
		Point start_of_lhs_line{ sw(basetri).x, sw(basetri).y };

		Point end_of_rhs_line{ start_of_rhs_line.x + int(double(line_delta_x) * cos(get_angle())), start_of_rhs_line.y + line_delta_y };
		Point end_of_lhs_line{ start_of_lhs_line.x - int(double(line_delta_x) * cos(get_angle())), start_of_lhs_line.y + line_delta_y };

		Point top_rhs_tri = end_of_rhs_line;
		Point top_lhs_tri = end_of_lhs_line;

		if (!get_arr()) {
		Line rhs_line{ start_of_rhs_line, end_of_rhs_line };
		rhs_line.set_color(color());	rhs_line.set_fill_color(fill_color());	rhs_line.set_style(style());
		rhs_line.draw_lines();
		Line lhs_line{ start_of_lhs_line, end_of_lhs_line };
		lhs_line.set_color(color());	lhs_line.set_fill_color(fill_color());	lhs_line.set_style(style());
		lhs_line.draw_lines();
		}
		else {
			if (get_inheritance()) {
				Arrow rhs_arr{ end_of_rhs_line , start_of_rhs_line, get_arrhead_l(), get_arrhead_w()}; rhs_arr.set_color(get_arr_color()); rhs_arr.set_fill_color(get_fill_color());
				rhs_arr.draw_lines();
				Arrow lhs_arr{ end_of_lhs_line , start_of_lhs_line, get_arrhead_l(), get_arrhead_w()}; lhs_arr.set_color(get_arr_color()); lhs_arr.set_fill_color(get_fill_color());
				lhs_arr.draw_lines();
			}
			else {
				Arrow rhs_arr{ start_of_rhs_line , end_of_rhs_line, get_arrhead_l(), get_arrhead_w() }; rhs_arr.set_color(get_arr_color()); rhs_arr.set_fill_color(get_fill_color());
				rhs_arr.draw_lines();
				Arrow lhs_arr{ start_of_lhs_line , end_of_lhs_line, get_arrhead_l(), get_arrhead_w() }; lhs_arr.set_color(get_arr_color()); lhs_arr.set_fill_color(get_fill_color());
				lhs_arr.draw_lines();
			}
		}

		Regular_triangle rhs{ Point{top_rhs_tri.x, top_rhs_tri.y + int(get_from_center())}, get_from_center() };
		rhs.set_color(color());	rhs.set_fill_color(fill_color());	rhs.set_style(style());
		rhs.draw_lines();

		Regular_triangle lhs{ Point{top_lhs_tri.x, top_lhs_tri.y + int(get_from_center())}, get_from_center() };
		lhs.set_color(color());	lhs.set_fill_color(fill_color());	lhs.set_style(style());
		lhs.draw_lines();


		ths_tri_c = center(rhs);
		lhs_tri_c = center(lhs);

		return;
	}

	void Binary_tree::node_extension(Point c_base_circle, Point& lhs_circle_tl, Point& rhs_circle_tl, int line_delta_x, int line_delta_y) const {	// expects and returns centre coordinates
		Point start_of_rhs_line{c_base_circle.x + int(double(from_center) * cos(pi * 0.25)), c_base_circle.y + int(double(from_center) * cos(pi * 0.25)) };
		Point start_of_lhs_line{c_base_circle.x - int(double(from_center) * cos(pi * 0.25)), c_base_circle.y + int(double(from_center) * cos(pi * 0.25)) };

		Point end_of_rhs_line{ start_of_rhs_line.x + int(double(line_delta_x) * cos(line_angle)), start_of_rhs_line.y + line_delta_y };
		Point end_of_lhs_line{ start_of_lhs_line.x - int(double(line_delta_x) * cos(line_angle)), start_of_lhs_line.y + line_delta_y };

		Point top_left_of_rhs_circle{ end_of_rhs_line.x - int(from_center), end_of_rhs_line.y};
		Point top_left_of_lhs_circle{ end_of_lhs_line.x - int(from_center), end_of_lhs_line.y};

		fl_line_style(FL_SOLID, 1);
		Fl_Color oldc = fl_color();

		if (fill_color().visibility()) {
			fl_color(fill_color().as_int());
			fl_pie(top_left_of_rhs_circle.x, top_left_of_rhs_circle.y, from_center + from_center - 1, from_center + from_center - 1, 0, 360);
			fl_pie(top_left_of_lhs_circle.x, top_left_of_lhs_circle.y, from_center + from_center - 1, from_center + from_center - 1, 0, 360);
		}
		if (color().visibility()) {
			fl_color(color().as_int());
			if (!arr) {
				fl_line(start_of_rhs_line.x, start_of_rhs_line.y, end_of_rhs_line.x, end_of_rhs_line.y);
				fl_line(start_of_lhs_line.x, start_of_lhs_line.y, end_of_lhs_line.x, end_of_lhs_line.y);
			}
			else {
				if (inheritance) {
					Arrow rhs_arr{ end_of_rhs_line , start_of_rhs_line, arrhead_l, arrhead_w }; rhs_arr.set_color(arr_color); rhs_arr.set_fill_color(arr_fill_color);
					rhs_arr.draw_lines();
					Arrow lhs_arr{ end_of_lhs_line , start_of_lhs_line, arrhead_l, arrhead_w }; lhs_arr.set_color(arr_color); lhs_arr.set_fill_color(arr_fill_color);
					lhs_arr.draw_lines();
				}
				else {
					Arrow rhs_arr{ start_of_rhs_line , end_of_rhs_line, arrhead_l, arrhead_w }; rhs_arr.set_color(arr_color); rhs_arr.set_fill_color(arr_fill_color);
					rhs_arr.draw_lines();
					Arrow lhs_arr{ start_of_lhs_line , end_of_lhs_line, arrhead_l, arrhead_w }; lhs_arr.set_color(arr_color); lhs_arr.set_fill_color(arr_fill_color);
					lhs_arr.draw_lines();
				}
			}
			fl_arc(top_left_of_rhs_circle.x, top_left_of_rhs_circle.y, from_center + from_center, from_center + from_center, 0, 360);
			fl_arc(top_left_of_lhs_circle.x, top_left_of_lhs_circle.y, from_center + from_center, from_center + from_center, 0, 360);
		}

		fl_color(oldc);	// reset color (to pevious) and style (to default)
		fl_line_style(0);

		lhs_circle_tl = { top_left_of_lhs_circle.x + int(from_center), top_left_of_lhs_circle.y + int(from_center) };
		rhs_circle_tl = { top_left_of_rhs_circle.x + int(from_center), top_left_of_rhs_circle.y + int(from_center) };

		return;
	}

	Point Binary_tree::node_base() const {
		Circle circ{ Point{point(0).x, point(0).y}, int(from_center) };
		circ.set_color(color());	circ.set_fill_color(fill_color());	circ.set_style(style());
		circ.draw_lines();

		return center(circ);
	}

	void Binary_tree::draw_lines() const {
		
		if (l == 0)
			return;
		Point base_circle_centre;
		if (l >= 1) {
			base_circle_centre = node_base();
		}

		Vector<Vector<Point>> vect(l + 2);	// l starts at 0 AND needs an additional row to store the last layer's addresses

		// seed level == 1 with base address
		vect[1].push_back(base_circle_centre);
		// reserve array, each parent array entry corresponds to a value of l
		for (int i = 2; i < l + 2; i++)
		{
			for(int j = 0; j < vect[i-1].size() * 2; j++)
				vect[i].push_back(Point{ 0,0 });
		}

		// populate array, starting from the seeded level, associated with two addresses from the next level
		for (int i = 1; i < l; i++)
		{
			for (int j = 0; j < vect[i].size(); j++)
			{
				int delta_x = 200 / (i*i);				// varies change of y between nodes
				int delta_y = i * 5;					// varies change in x between nodes
				node_extension(vect[i][j], vect[i + 1][j * 2], vect[i + 1][j * 2 + 1], delta_x, delta_y);
			}
		}

		// draw text annotations
		for(int i = 0; i < nodetext.size(); i++) {
			for(int j = 0; j < nodetext[i].size(); j++)
				if (nodetext[i][j] != "")
				{
					Point target = vect[i][j];
					string str = nodetext[i][j];
					Text text{ target, str };
					text.set_color(Color::magenta);
					text.draw_lines();
				}
		}
	}

	void Pseudo_window::activate(Simple_window& win) const {
		win.attach(*background);
		win.attach(*foreground);
		win.attach(*redsquare);
		win.attach(*corner_cross);
	}

	Pseudo_window::Pseudo_window(Point p) : tl{ p } {
		background = new Rectangle{ Point{ tl.x, tl.y }, width, height };
		background->set_color(Color::black);
		background->set_fill_color(Color::cyan);
		int title_height = 20;
		foreground = new Rectangle{ Point{ tl.x, tl.y + title_height }, width, height-title_height};
		foreground->set_color(Color::black);
		foreground->set_fill_color(Color::white);

		int square_side = 15;
		int side_offset = 3;
		redsquare = new Rectangle{ Point{ tl.x + width - square_side - side_offset, tl.y + side_offset}, square_side, square_side };
		redsquare->set_color(Color::black);
		redsquare->set_fill_color(Color::dark_red);

		corner_cross = new Mark{ Point{ tl.x + width - square_side - side_offset + square_side/2, tl.y + side_offset + square_side / 2} , 'X' };
		corner_cross->set_color(Color::white);
	}

	void Octagon::draw_lines() const {
		Regular_polygon poly(Point(point(0).x, point(0).y), 8, distance_from_center_to_corner);
		poly.set_color(this->color());
		poly.set_fill_color(this->fill_color());
		poly.set_style(this->style());
		poly.draw_lines();
	}

	// does two lines (p1,p2) and (p3,p4) intersect?
// if se return the distance of the intersect point as distances from p1
	inline pair<double, double> line_intersect_ripped(Point p1, Point p2, Point p3, Point p4, bool& parallel)
	{
		double x1 = p1.x;
		double x2 = p2.x;
		double x3 = p3.x;
		double x4 = p4.x;
		double y1 = p1.y;
		double y2 = p2.y;
		double y3 = p3.y;
		double y4 = p4.y;

		double denom = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
		if (denom == 0) {
			parallel = true;
			return pair<double, double>(0, 0);
		}
		parallel = false;
		return pair<double, double>(((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom,
			((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom);
	}


	//intersection between two line segments
	//Returns true if the two segments intersect,
	//in which case intersection is set to the point of intersection
	bool line_segment_intersect_ripped(Point p1, Point p2, Point p3, Point p4, Point& intersection) {
		bool parallel;
		pair<double, double> u = line_intersect_ripped(p1, p2, p3, p4, parallel);
		if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1) return false;
		intersection.x = p1.x + u.first * (p2.x - p1.x);
		intersection.y = p1.y + u.first * (p2.y - p1.y);
		return true;
	}
	
	void Striped_closed_polyline::draw_lines() const
	{
		Closed_polyline::draw_lines();

		// Find Bounds
		int largest_y = - INT32_MAX;	int largest_x = - INT32_MAX;
		int smallest_x = INT32_MAX;		int smallest_y = INT32_MAX;
		for (int i = 0; i < number_of_points(); i++)		// determine bounding box of screen coordinates for intersection scan
		{
			if (point(i).x > largest_x)		largest_x = point(i).x;
			if (point(i).y > largest_y)		largest_y = point(i).y;
			if (point(i).x < smallest_x)	smallest_x = point(i).x;
			if (point(i).y < smallest_y)	smallest_y = point(i).y;
		}

		// Draw a horizontal line across the x-bounds at each y, checking for intersections and filling a vector within a vector representing each height (y)
		// If an intersection is found, another line is drawn from the intersection, to the right to store further intersections until no more are found
		int vertical_increment = 10;
		int vertical_entries = (largest_y - smallest_y) / vertical_increment;
		vector<vector<Point>> vect(vertical_entries);	// eg: (vect[j])[i] may store several doublets of points

		for (int y = smallest_y; y < largest_y; y += vertical_increment) {	// generate pairs of points with each vertical increment
			Point lhs{smallest_x, y};
			Point rhs{largest_x + 1, y};	// largest_x + 1 to prevent situation where rhs is stuck at x = intersection

			// check generated horizontal line against point(0) and point(number_of_points()-1) of polygon
			Point intersection;
			while (line_segment_intersect_ripped(point(0), point(number_of_points() - 1), lhs, rhs, intersection))
			{
				vect[(y - smallest_y) / vertical_increment].push_back(intersection);
				lhs.x = intersection.x + 1;
			}
			lhs.x = smallest_x;

			for (int k = 0; k < number_of_points() - 1; k++)	// check all lines in polygon apart from point(0) and point(number_of_points()-1)
			{
				Point intersection;
				while(line_segment_intersect_ripped(point(k), point(k + 1), lhs, rhs, intersection))
				{
					vect[(y - smallest_y) / vertical_increment].push_back(intersection);
					lhs.x = intersection.x+1;
				}
				lhs.x = smallest_x;
			}
		}
		// Sort X coordinates into series, this method is easily broken regarding polygon geometries
		for (int i = 0; i < vect.size(); i++) {
			sort(vect[i].begin(), vect[i].end(), [](Point a, Point b) {
				return a.x < b.x;});
			unique(vect[i].begin(), vect[i].end());			// Delete Duplicates
		}


		// Connect the horizontal intersections in pairs
		int line_width = vertical_increment / 2;
		fl_line_style(FL_SOLID, line_width);
		Fl_Color oldc = fl_color();
		fl_color(color().as_int());
		if (color().visibility())
		{
			for (int i = 0; i < vect.size(); i++) {
				for (int j = 0; j < vect[i].size() - 1; j+=2) {
					fl_line(vect[i][j].x, vect[i][j].y, vect[i][j + 1].x, vect[i][j + 1].y);
				}
			}
		}
		fl_color(oldc);	// reset color (to pevious) and style (to default)
		fl_line_style(0);
	}

	void Striped_circle::draw_lines() const {

		Circle::draw_lines();

		if (color().visibility())
		{
			Fl_Color oldc = fl_color();
			fl_color(color().as_int());
			Point centre_top = { point(0).x + radius(), point(0).y};

			int line_width = 5;
			fl_line_style(FL_SOLID, line_width);

			for (int y = radius(); y > -radius(); y--)		// assume a circle is centered at 0,0. Work from y = radius to y = -radius
			{
				int x1 = sqrt(radius() * radius() - y * y);
				cout << sqrt(radius() * radius() - y * y) << endl;
				cout << y << endl;
				int x2 = -x1;

				if (y % (2 * line_width) == 0 && y != radius() && y!= -radius())
				{
					fl_line(centre_top.x + x1, y + centre_top.y * 2, centre_top.x + x2, y + centre_top.y*2);
				}
			}
			fl_color(oldc);	// reset color (to pevious) and style (to default)
			fl_line_style(0);
		}

	}

	void Striped_rectangle::draw_lines() const {

		Rectangle::draw_lines();

		if (color().visibility())
		{
			Fl_Color oldc = fl_color();
			fl_color(color().as_int());
			Point p = { point(0).x, point(0).y };
			int line_width = 5;
			fl_line_style(FL_SOLID, line_width);
			for (int i = 0; i < height(); i++)
			{
				if (i % (2*line_width) == 0 && i!=0)
				{
					p.y = point(0).y + i;
					fl_line(p.x, p.y, p.x + width(), p.y);
				}
			}
			fl_color(oldc);	// reset color (to pevious) and style (to default)
			fl_line_style(0);
		}
	}

	void Smiley::draw_lines() const {
		Circle::draw_lines();

		Point center = { point(0).x + radius(), point(0).y + radius() };

		int eye_r = radius() / 6;
		Point lhs_ep{ center.x - int(0.4 * double(radius())) - eye_r, center.y - int(0.3 * double(radius())) - eye_r };

		Point rhs_ep{ center.x + int(0.4 * double(radius())) - eye_r, center.y - int(0.3 * double(radius())) - eye_r };

		fl_color(Color::white);
		fl_pie(lhs_ep.x, lhs_ep.y, eye_r + eye_r - 1, eye_r + eye_r - 1, 0, 360);
		fl_pie(rhs_ep.x, rhs_ep.y, eye_r + eye_r - 1, eye_r + eye_r - 1, 0, 360);

		Point below_center{ center.x - eye_r, center.y - eye_r + int(0.3 * double(radius())) };
		int smile_r = radius() / 3;
		fl_pie(below_center.x - smile_r/2, below_center.y, smile_r + smile_r - 1, smile_r, 180, 360);
	}

	void Smileyhat::draw_lines() const {
		int hat_width = radius()/2;
		int hat_length = 2*hat_width;
		Smiley::draw_lines();
		Point top = { point(0).x + radius(), point(0).y};
		Point hat_rhs_ridge = { top.x + hat_width/2, top.y };
		Point hat_rhs_top_ridge = { hat_rhs_ridge.x, hat_rhs_ridge.y - hat_width / 8 };
		Point hat_rhs_protrusion_base = { hat_rhs_top_ridge.x - hat_width / 8, hat_rhs_top_ridge.y };
		Point hat_rhs_protrusion_top = { hat_rhs_protrusion_base.x, hat_rhs_protrusion_base.y - hat_length };

		Point hat_lhs_ridge = { top.x - hat_width / 2, top.y };
		Point hat_lhs_top_ridge = { hat_lhs_ridge.x, hat_lhs_ridge.y - hat_width / 8 };
		Point hat_lhs_protrusion_base = { hat_lhs_top_ridge.x + hat_width / 8, hat_lhs_top_ridge.y };
		Point hat_lhs_protrusion_top = { hat_lhs_protrusion_base.x, hat_lhs_protrusion_base.y - hat_length };
		fl_color(Color::black);
		fl_polygon(hat_rhs_ridge.x, hat_rhs_ridge.y, hat_rhs_top_ridge.x, hat_rhs_top_ridge.y, hat_lhs_top_ridge.x, hat_lhs_top_ridge.y, hat_lhs_ridge.x, hat_lhs_ridge.y);
		fl_polygon(hat_rhs_protrusion_base.x, hat_rhs_protrusion_base.y, hat_rhs_protrusion_top.x, hat_rhs_protrusion_top.y, hat_lhs_protrusion_top.x, hat_lhs_protrusion_top.y, hat_lhs_protrusion_base.x, hat_lhs_protrusion_base.y);
	}

	void Frownyhat::draw_lines() const {
		int hat_width = radius() / 2;
		int hat_length = 2 * hat_width;
		Frowny::draw_lines();
		Point top = { point(0).x + radius(), point(0).y };
		Point hat_rhs_ridge = { top.x + hat_width / 2, top.y };
		Point hat_rhs_top_ridge = { hat_rhs_ridge.x, hat_rhs_ridge.y - hat_width / 8 };
		Point hat_rhs_protrusion_base = { hat_rhs_top_ridge.x - hat_width / 8, hat_rhs_top_ridge.y };
		Point hat_rhs_protrusion_top = { hat_rhs_protrusion_base.x, hat_rhs_protrusion_base.y - hat_length };

		Point hat_lhs_ridge = { top.x - hat_width / 2, top.y };
		Point hat_lhs_top_ridge = { hat_lhs_ridge.x, hat_lhs_ridge.y - hat_width / 8 };
		Point hat_lhs_protrusion_base = { hat_lhs_top_ridge.x + hat_width / 8, hat_lhs_top_ridge.y };
		Point hat_lhs_protrusion_top = { hat_lhs_protrusion_base.x, hat_lhs_protrusion_base.y - hat_length };
		fl_color(Color::black);
		fl_polygon(hat_rhs_ridge.x, hat_rhs_ridge.y, hat_rhs_top_ridge.x, hat_rhs_top_ridge.y, hat_lhs_top_ridge.x, hat_lhs_top_ridge.y, hat_lhs_ridge.x, hat_lhs_ridge.y);
		fl_polygon(hat_rhs_protrusion_base.x, hat_rhs_protrusion_base.y, hat_rhs_protrusion_top.x, hat_rhs_protrusion_top.y, hat_lhs_protrusion_top.x, hat_lhs_protrusion_top.y, hat_lhs_protrusion_base.x, hat_lhs_protrusion_base.y);
	}

	void Frowny::draw_lines() const {
		Circle::draw_lines();

		Point center = { point(0).x + radius(), point(0).y + radius() };

		int eye_r = radius() / 6;
		Point lhs_ep{ center.x - int(0.4 * double(radius())) - eye_r, center.y - int(0.3 * double(radius())) - eye_r };

		Point rhs_ep{ center.x + int(0.4 * double(radius())) - eye_r, center.y - int(0.3 * double(radius())) - eye_r };

		fl_color(Color::white);
		fl_pie(lhs_ep.x, lhs_ep.y, eye_r + eye_r - 1, eye_r + eye_r - 1, 135, 360);
		fl_pie(rhs_ep.x, rhs_ep.y, eye_r + eye_r - 1, eye_r + eye_r - 1, 180, 45);

		Point below_center{ center.x - eye_r, center.y - eye_r + int(0.3 * double(radius())) };
		int aggressive_r = radius() / 3;
		fl_pie(below_center.x - aggressive_r / 2, below_center.y + aggressive_r/2, aggressive_r + aggressive_r - 1, aggressive_r, 0, 180);
	}


	
	void Star::draw_lines() const {

			// Use polygon drawing program to get outer_points matrix of star extremities
			double half_of_degrees_in_each_vertice = 0.5 * (180.0 * n_points - 360.0) / n_points; // converges to 0.5*180

			int side_length = int(2 * c_to_corner * cos(half_of_degrees_in_each_vertice * pi / 180.0));
			int deltay_to_corner = int(c_to_corner * sin(half_of_degrees_in_each_vertice * pi / 180.0));
			int deltax_to_corner = -int(c_to_corner * cos(half_of_degrees_in_each_vertice * pi / 180.0));

			Point bottom_lhs{ c.x + deltax_to_corner, c.y + deltay_to_corner };
			vector<Point> outer_points = get_polygon_coordinates(n_points, double(side_length), bottom_lhs);


			// matrix of star extremities and averaged points between each
			vector<Point> inner_and_outer;

			for (int i = 0; i < outer_points.size(); i++){
				inner_and_outer.push_back(outer_points[i]);
				if(i+1<outer_points.size())
					inner_and_outer.push_back(Point{ (outer_points[i].x + outer_points[i + 1].x + sharp * c.x) / (sharp + 2), (outer_points[i].y + outer_points[i + 1].y + sharp * c.y) / (sharp + 2) });
			}
			inner_and_outer.push_back(Point{ (outer_points[0].x + outer_points[outer_points.size() - 1].x + sharp * c.x) / (sharp + 2), (outer_points[0].y + outer_points[outer_points.size() - 1].y + sharp * c.y) / (sharp + 2) });
			
			if (fill_color().visibility()) {	// fill
				fl_color(fill_color().as_int());
				for (int i = 0; i < inner_and_outer.size() - 1; i++) {
					fl_polygon(c.x, c.y, inner_and_outer[i].x, inner_and_outer[i].y, inner_and_outer[i + 1].x, inner_and_outer[i + 1].y);
				}
				fl_polygon(c.x, c.y, inner_and_outer[0].x, inner_and_outer[0].y, inner_and_outer[inner_and_outer.size() - 1].x, inner_and_outer[inner_and_outer.size() - 1].y);
			}
			

			// close polygon
			if (color().visibility()) {
				fl_color(color().as_int());
				fl_line(inner_and_outer[inner_and_outer.size() - 1].x, inner_and_outer[inner_and_outer.size() - 1].y, inner_and_outer[0].x, inner_and_outer[0].y);
				for (int i = 0; i < inner_and_outer.size(); i++)
				{
					Point(inner_and_outer[i].x, inner_and_outer[i].y);
					if (i + 1 < inner_and_outer.size()) {
						fl_color(color().as_int());
						fl_line(inner_and_outer[i].x, inner_and_outer[i].y, inner_and_outer[i + 1].x, inner_and_outer[i + 1].y);
					}
				}
				for (int i = 0; i < inner_and_outer.size(); i++) {
					fl_line(inner_and_outer[i].x, inner_and_outer[i].y, c.x, c.y);
				}
			}

	}
	

		/*
		Point c;
		int n_points;
		double c_to_corner;
		*/
}

namespace Graph_lib {
	void Poly::draw_lines() const {
		if (fill_color().visibility()) {
			fl_color(fill_color().as_int());
			fl_begin_complex_polygon();
			for (int i = 0; i < number_of_points(); ++i) {
				fl_vertex(point(i).x, point(i).y);
			}
			fl_end_complex_polygon();
			fl_color(color().as_int());	// reset color
		}

		if (color().visibility())
			Shape::draw_lines();
	}
}
namespace Graph_lib {
	void Right_triangle::draw_lines() const {
		if (fill_color().visibility()) {	// fill
			fl_color(fill_color().as_int());
			if (vers == "/_|"){
				fl_polygon(point(0).x, point(0).y, point(0).x  - side, point(0).y, point(0).x, point(0).y - side);
			}
			if (vers == "|_\\"){
				fl_polygon(point(0).x, point(0).y, point(0).x + side, point(0).y, point(0).x, point(0).y - side);
			}
			if (vers == "<|"){
				fl_polygon(point(0).x, point(0).y, point(0).x + int(side*cos(pi/4)), point(0).y + int(side * sin(pi / 4)), point(0).x + int(side * cos(pi / 4)), point(0).y - int(side * sin(pi / 4)));
			}
			if (vers == "|>") {
				fl_polygon(point(0).x, point(0).y, point(0).x - int(side * cos(pi / 4)), point(0).y + int(side * sin(pi / 4)), point(0).x - int(side * cos(pi / 4)), point(0).y - int(side * sin(pi / 4)));
			}
			if (vers == "\-|") {
				fl_polygon(point(0).x, point(0).y, point(0).x - side, point(0).y, point(0).x, point(0).y + side);
			}
			if (vers == "|-/") {
				fl_polygon(point(0).x, point(0).y, point(0).x + side, point(0).y, point(0).x, point(0).y + side);
			}

		}


		if (color().visibility())
		{
			fl_color(color().as_int());
			if (vers == "/_|"){															// polygon set's:
				fl_line(point(0).x, point(0).y, point(0).x - side, point(0).y);			// start and middle
				fl_line(point(0).x, point(0).y, point(0).x, point(0).y - side);			// start and end
				fl_line(point(0).x - side, point(0).y, point(0).x, point(0).y - side);	// middle and end
			}
			if (vers == "|_\\"){
				fl_line(point(0).x, point(0).y, point(0).x + side, point(0).y);
				fl_line(point(0).x, point(0).y, point(0).x, point(0).y - side);
				fl_line(point(0).x + side, point(0).y, point(0).x, point(0).y - side);
			}
			if (vers == "<|") {
				fl_line(point(0).x, point(0).y, point(0).x + int(side * cos(pi / 4)), point(0).y + int(side * sin(pi / 4)));
				fl_line(point(0).x, point(0).y, point(0).x + int(side * cos(pi / 4)), point(0).y - int(side * sin(pi / 4)));
				fl_line(point(0).x + int(side * cos(pi / 4)), point(0).y + int(side * sin(pi / 4)), point(0).x + int(side * cos(pi / 4)), point(0).y - int(side * sin(pi / 4)));
			}
			if (vers == "|>") {
				fl_line(point(0).x, point(0).y, point(0).x - int(side * cos(pi / 4)), point(0).y + int(side * sin(pi / 4)));
				fl_line(point(0).x, point(0).y, point(0).x - int(side * cos(pi / 4)), point(0).y - int(side * sin(pi / 4)));
				fl_line(point(0).x - int(side * cos(pi / 4)), point(0).y + int(side * sin(pi / 4)), point(0).x - int(side * cos(pi / 4)), point(0).y - int(side * sin(pi / 4)));
			}
			if (vers == "\-|") {
				fl_line(point(0).x, point(0).y, point(0).x - side, point(0).y);
				fl_line(point(0).x, point(0).y, point(0).x, point(0).y + side);
				fl_line(point(0).x - side, point(0).y, point(0).x, point(0).y + side);
			}
			if (vers == "|-/") {
				fl_line(point(0).x, point(0).y, point(0).x + side, point(0).y);
				fl_line(point(0).x, point(0).y, point(0).x, point(0).y + side);
				fl_line(point(0).x + side, point(0).y, point(0).x, point(0).y + side);
			}

		}
	}
}


namespace Graph_lib {

	void Regular_polygon::draw_lines() const {	// uses Frankenstein approach to adapt get_polygon_coordinates from Ch12Q11
		Point tl{ 50, 50 };

		double half_of_degrees_in_each_vertice = 0.5*(180.0 * n_sides - 360.0) / n_sides; // converges to 0.5*180

		int side_length = int(2*distance_center_to_corner*cos(half_of_degrees_in_each_vertice * pi / 180.0));	//	Side length is derived from distance to corner
		int deltay_to_corner = int(distance_center_to_corner * sin(half_of_degrees_in_each_vertice * pi / 180.0));
		int deltax_to_corner = -int(distance_center_to_corner * cos(half_of_degrees_in_each_vertice * pi / 180.0));


		Point bottom_lhs{ c.x+deltax_to_corner, c.y+deltay_to_corner };
		vector<Point> coordinates = get_polygon_coordinates(n_sides, double(side_length), bottom_lhs);
		
		
		if (fill_color().visibility()) {	// fill
			fl_color(fill_color().as_int());
			for (int i = 0; i < coordinates.size() - 1; i++) {
				fl_polygon(c.x, c.y, coordinates[i].x, coordinates[i].y, coordinates[i + 1].x, coordinates[i + 1].y);
			}
			fl_polygon(c.x, c.y, coordinates[0].x, coordinates[0].y, coordinates[coordinates.size() - 1].x, coordinates[coordinates.size() - 1].y);
		}

		// close polygon
		if (color().visibility()) {
			fl_color(color().as_int());
			fl_line(coordinates[coordinates.size() - 1].x, coordinates[coordinates.size() - 1].y, coordinates[0].x, coordinates[0].y);
			for (int i = 0; i < coordinates.size(); i++)
			{
				Point(coordinates[i].x, coordinates[i].y);
				if (i + 1 < coordinates.size()) {
					fl_color(color().as_int());
					fl_line(coordinates[i].x, coordinates[i].y, coordinates[i + 1].x, coordinates[i + 1].y);
				}
			}
		}

	}
	int which_quadrant(double deg_from_x) {
		double remainder_from_360 = fmod(deg_from_x, 360.0);
		int quadrant = 0;
		if (remainder_from_360 > 270)
			return 4;
		if (remainder_from_360 > 180)
			return 3;
		if (remainder_from_360 > 90)
			return 2;
		if (remainder_from_360 >= 0)
			return 1;
		else
			return 0;
	}

	vector<Point> get_polygon_coordinates(int number_of_sides, double side_length, Point origin) {

		vector<Point> ans;

		Point point_buffer = origin;
		double degrees_in_each_vertice = (180.0 * number_of_sides - 360.0) / number_of_sides; // converges to 180
		double deg_from_parallel = -(180 - degrees_in_each_vertice);

		ans.push_back(origin);
		for (int i = 0; i < number_of_sides - 1; i++)
		{
			deg_from_parallel += 180 - degrees_in_each_vertice;
			double deg_from_x = fmod(deg_from_parallel, 360.0);

			int quadrant = which_quadrant(deg_from_parallel);

			double theta_deg = deg_from_x - 90 * (quadrant - 1);

			double delta_x = 0.0;
			double delta_y = 0.0;
			if (quadrant == 1) {
				delta_x = +side_length * cos((theta_deg / 180) * pi);
				delta_y = -side_length * sin((theta_deg / 180) * pi);		// minus delta y due to the screen coordinate system
			}
			else if (quadrant == 2) {
				delta_x = -side_length * sin((theta_deg / 180) * pi);
				delta_y = -side_length * cos((theta_deg / 180) * pi);
			}
			else if (quadrant == 3) {
				delta_x = -side_length * cos((theta_deg / 180) * pi);
				delta_y = +side_length * sin((theta_deg / 180) * pi);
			}
			else {
				delta_x = +side_length * sin((theta_deg / 180) * pi);
				delta_y = +side_length * cos((theta_deg / 180) * pi);
			}

			point_buffer.x = point_buffer.x + delta_x;
			point_buffer.y = point_buffer.y + delta_y;

			ans.push_back(Point(point_buffer.x, point_buffer.y));
		}

		return ans;
	}
}

namespace Graph_lib {
	void Regular_hexagon::draw_lines() const {
		// top point relative to center
		Point top_center{ point(0).x, point(0).y - int(distance_from_center * (cos((60.0 / 2.0) * pi / 180.0))) };
		Point _11_oclock{ top_center.x - distance_from_center / 2, top_center.y };
		Point _01_oclock{ top_center.x + distance_from_center / 2, top_center.y };
		Point _03_oclock{ point(0).x + distance_from_center, point(0).y };
		Point bot_center{ point(0).x, point(0).y + int(distance_from_center * (cos((60.0 / 2.0) * pi / 180.0))) };
		Point _05_oclock{ bot_center.x + distance_from_center / 2, bot_center.y };
		Point _07_oclock{ bot_center.x - distance_from_center / 2, bot_center.y };
		Point _09_oclock{ point(0).x - distance_from_center, point(0).y };

		if (fill_color().visibility()) {	// fill
			fl_color(fill_color().as_int());
		}

		fl_polygon(_11_oclock.x, _11_oclock.y, _01_oclock.x, _01_oclock.y, _03_oclock.x, _03_oclock.y, _05_oclock.x, _05_oclock.y);
		fl_polygon(_05_oclock.x, _05_oclock.y, _07_oclock.x, _07_oclock.y, _09_oclock.x, _09_oclock.y, _11_oclock.x, _11_oclock.y);

		if (color().visibility()) {
				fl_color(color().as_int());
				fl_line(_11_oclock.x, _11_oclock.y, _01_oclock.x, _01_oclock.y);
				fl_line(_03_oclock.x, _03_oclock.y, _01_oclock.x, _01_oclock.y);
				fl_line(_03_oclock.x, _03_oclock.y, _05_oclock.x, _05_oclock.y);
				fl_line(_05_oclock.x, _05_oclock.y, _07_oclock.x, _07_oclock.y);
				fl_line(_09_oclock.x, _09_oclock.y, _07_oclock.x, _07_oclock.y);
				fl_line(_09_oclock.x, _09_oclock.y, _11_oclock.x, _11_oclock.y);
		}
	}

}
namespace Graph_lib {
	void ColorBox::draw_lines() const {
			fl_color(r, g, b);
			//fl_color(fill_color().as_int());
			fl_rectf(point(0).x, point(0).y, w, h);
			//fl_color(color().as_int());	// reset color
			fl_color(r, g, b);

			fl_color(color().as_int());
			fl_rect(point(0).x, point(0).y, w, h);
		// text drawing
		fl_color(Color::white);
		c.draw_lines();
	}
}
/* Ellipse Connection Points */
namespace Graph_lib {
	Point n(Ellipse& e) {
		return Point{ e.point(0).x + e.major(), e.point(0).y};
	}
	Point s(Ellipse& e) {
		return Point{ e.point(0).x + e.major(), e.point(0).y + 2*e.minor()};
	}
	Point e(Ellipse& e) {
		return Point{ e.point(0).x + 2*e.major(), e.point(0).y + e.minor()};
	}
	Point w(Ellipse& e) {
		return Point{ e.point(0).x, e.point(0).y + e.minor() };
	}
	Point center(Ellipse& e) {
		return Point{ e.point(0).x + e.major(), e.point(0).y + e.minor()};
	}
	Point ne(Ellipse& e) {
		return Point{ e.point(0).x + e.major() + int(double(e.major())*cos(pi/4.0)), e.point(0).y + e.minor() - int(double(e.minor()) * cos(pi / 4.0)) };		// 45 degrees : pi/4.0
	}
	Point se(Ellipse& e) {
		return Point{ e.point(0).x + e.major() + int(double(e.major()) * cos(pi / 4.0)), e.point(0).y + e.minor() + int(double(e.minor()) * cos(pi / 4.0)) };
	}
	Point nw(Ellipse& e) {
		return Point{ e.point(0).x + e.major() - int(double(e.major()) * cos(pi / 4.0)), e.point(0).y + e.minor() - int(double(e.minor()) * cos(pi / 4.0)) };
	}
	Point sw(Ellipse& e) {
		return Point{ e.point(0).x + e.major() - int(double(e.major()) * cos(pi / 4.0)), e.point(0).y + e.minor() + int(double(e.minor()) * cos(pi / 4.0)) };
	}
}
/* Circle Connection Points */
namespace Graph_lib {
	Point n(Circle& c) {
		return Point{ c.point(0).x + c.radius(), c.point(0).y };
	}
	Point s(Circle& c) {
		return Point{ c.point(0).x + c.radius(), c.point(0).y + 2*c.radius() };
	}
	Point e(Circle& c) {
		return Point{ c.point(0).x + 2*c.radius(), c.point(0).y + c.radius() };
	}
	Point w(Circle& c) {
		return Point{ c.point(0).x, c.point(0).y + c.radius() };
	}
	Point center(Circle& c) {
		return Point{ c.point(0).x + c.radius(), c.point(0).y + c.radius() };
	}
	Point ne(Circle& c) {
		return Point{ c.point(0).x + c.radius() + int(double(c.radius()) * sin(pi / 4.0)), c.point(0).y + c.radius() - int(double(c.radius()) * cos(pi / 4.0)) };	// calculates center offset
	}
	Point se(Circle& c) {
		return Point{ c.point(0).x + c.radius() + int(double(c.radius()) * sin(pi / 4.0)), c.point(0).y + c.radius() + int(double(c.radius()) * cos(pi / 4.0)) };
	}
	Point nw(Circle& c) {
		return Point{ c.point(0).x + c.radius() - int(double(c.radius()) * sin(pi / 4.0)), c.point(0).y + c.radius() - int(double(c.radius()) * cos(pi / 4.0)) };
	}
	Point sw(Circle& c) {
		return Point{ c.point(0).x + c.radius() - int(double(c.radius()) * sin(pi / 4.0)), c.point(0).y + c.radius() + int(double(c.radius()) * cos(pi / 4.0)) };
	}
}
/* Rectangle Connection Points */
namespace Graph_lib {
	Point n(Rectangle& r){
		return Point{ r.point(0).x + r.width()/2, r.point(0).y};
	}
	Point s(Rectangle& r) {
		return Point{ r.point(0).x + r.width()/2, r.point(0).y + r.height() };
	}
	Point e(Rectangle& r) {
		return Point{ r.point(0).x + r.width(), r.point(0).y + r.height()/2};
	}
	Point w(Rectangle& r) {
		return Point{ r.point(0).x, r.point(0).y + r.height()/2 };
	}
	Point center(Rectangle& r) {
		return Point{ r.point(0).x + r.width()/2, r.point(0).y + r.height()/2 };
	}
	Point ne(Rectangle& r) {
		return Point{ r.point(0).x + r.width(), r.point(0).y };
	}
	Point se(Rectangle& r) {
		return Point{ r.point(0).x + r.width(), r.point(0).y + r.height()};
	}
	Point nw(Rectangle& r) {
		return Point{ r.point(0).x, r.point(0).y };
	}
	Point sw(Rectangle& r) {
		return Point{ r.point(0).x, r.point(0).y + r.height() };
	}
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~ */

void Graph_lib::quadratic_formula(double& x1, double& x2, double a, double b, double c) {
	if (b * b - 4.0 * a * c < 0.0)
		error("No solutions to quadratic formula");
	x1 = (-b + sqrt(b * b - 4.0 * a * c)) / (2 * a);
	x2 = (-b - sqrt(b * b - 4.0 * a * c)) / (2 * a);

	return;
}

double Graph_lib::to_rad(double degrees) {
	return 180.0 * degrees / Graph_lib::pi;
}

double Graph_lib::angle_between_points(Point start, Point end) {
	if(start.x == end.x && start.y == end.y){
		error("angle_between_points() called with the same points");
	}
	return Graph_lib::to_rad(atan2(end.y - start.y, end.x - start.x));
}

/*	angle_between_points(Point start, Point end) degrees
* 

		   90
		   |
	135	   |    45
		   |
180 -------|-------- 0
		   |
	-135   |   -45
		   |
		  -90

*
*/

/*	SCREEN COORDINATES TRANSLATION angle_between_points(Point start, Point end) degrees
*

		  -90
		   |
	-135   |    -45
		   |
180 -------|-------- 0
		   |
	 135   |    45
		   |
		   90

*
*/

double Graph_lib::angle_between_points_2(Point start, Point end) {
	double angle = Graph_lib::angle_between_points(start, end);
	if (angle < 0)
		return abs(180.0 + angle) + 180.0;
	else
		return angle;
}

/*	angle_between_points_2(Point start, Point end) degrees
*

		   90
		   |
	135	   |    45
		   |
180 -------|-------- 0
		   |
	225    |   315
		   |
		  270

*
*/

double Graph_lib::angle_conversion(double angle) {
	if (angle >= 360)
		error("angle_conversion() greater than 360 attempted");
	if (angle > 0)
		return angle;
	else
		return 360.0 - abs(angle);
}

void Graph_lib::Arc::draw_lines() const	// convertd from ellipse
{
	if (fill_color().visibility()) {	// fill
		fl_color(fill_color().as_int());
		fl_arc(point(0).x, point(0).y, w + w - 1, h + h - 1, sAngle, eAngle);
		fl_color(color().as_int());	// reset color
	}

	if (color().visibility()) {
		fl_color(color().as_int());
		fl_arc(point(0).x, point(0).y, w + w, h + h, sAngle, eAngle);			// using fl_arc() [2/2]
	}
}


void Graph_lib::Arrow::draw_lines() const
{
	fl_line_style(FL_SOLID, 1);
	Fl_Color oldc = fl_color();

	if (color().visibility())
	{
		/*
			add(p1);		// p1 is Shape::points[0]
			add(p2);		// p2 is Shape::points[1]
		*/
		fl_color(Shape::color().as_int());
		fl_line(Shape::point(0).x, Shape::point(0).y, Shape::point(1).x, Shape::point(1).y);

		double changeiny = Shape::point(1).y - Shape::point(0).y;
		double changeinx = Shape::point(1).x - Shape::point(0).x;
		double m = changeiny / changeinx;								// equation of shaft y = mx + c
		double c = Shape::point(1).y - m * Shape::point(1).x;			// equation of shaft y = mx + c


		// find coordinate of arrowhead_l protrusion from line by solving simultaneous equations:
		// [arrowhead_l = SQRT(DELTAY^2 + DELTAX^2)] & [y = mx + c]
		// DELTAY = P2.y - P1.y; P0 is the arrow nock, P1 is the tip, P2 is the arrowhead_l along the shaft, relative to the arrow tip Shape::point(1)
		// y = mx + c ==> P2.y = mP2.x + c
		double P2x_a = m * m + 1.0;	// Solving the quadratic formula for P2.x
		double P2x_b = 2.0 * m * c - 2.0 * Shape::point(1).y * m - 2.0 * Shape::point(1).x;
		double P2x_c = c * c + Shape::point(1).y * Shape::point(1).y - 2.0 * Shape::point(1).y * c + Shape::point(1).x * Shape::point(1).x - arrowhead_l * arrowhead_l;

		double x1 = 0;
		double x2 = 0;
		Graph_lib::quadratic_formula(x1, x2, P2x_a, P2x_b, P2x_c);
		double y1 = m * x1 + c;
		double y2 = m * x2 + c;
		Point arrowhead_base{ 0,0 };

		// the x solution of the arrowhead base's position's returned two sets of coordinates, the set between the inital arrow points is the correct arrowhead_base
		if ((Shape::point(0).x > x1 && x1 > Shape::point(1).x) || (Shape::point(0).x < x1 && x1 < Shape::point(1).x))
		{
			arrowhead_base.x = x1;
			arrowhead_base.y = y1;
		}
		else
		{
			arrowhead_base.x = x2;
			arrowhead_base.y = y2;
		}	
		
		// now that we have our point, we find the coordinates of a perpendicular line with -1/m as the gradient.
		// The coordinate of either arrow head corner will be on the point, this time solving
		// [arrowhead_w/2] = SQRT(DELTAY^2 + DELTAX^2)] & [y = mx + c]
		// DELTAY = P2.y - P1.y; P0 is the arrow nock, P1 is the tip, P2 is the arrowhead_l along the shaft, relative to the arrow tip Shape::point(1)
		// y = mx + c ==> P2.y = mP2.x + c (where m and c are for the perpendicular line)
		double perp_m = -1.0 / m;
		double perp_c = arrowhead_base.y - perp_m * arrowhead_base.x;

		double perp_P2x_a = perp_m*perp_m + 1.0;
		double perp_P2x_b = 2.0 * perp_m * perp_c - 2.0 * arrowhead_base.y * perp_m - 2.0 * arrowhead_base.x;
		double perp_P2x_c = perp_c * perp_c + arrowhead_base.y * arrowhead_base.y - 2.0 * arrowhead_base.y * perp_c + arrowhead_base.x * arrowhead_base.x - (arrowhead_w/2.0) * (arrowhead_w/2.0);
		
		
		double perp_x1 = 0;
		double perp_x2 = 0;
		Graph_lib::quadratic_formula(perp_x1, perp_x2, perp_P2x_a, perp_P2x_b, perp_P2x_c);
		double perp_y1 = perp_m * perp_x1 + perp_c;
		double perp_y2 = perp_m * perp_x2 + perp_c;
		Point arrowhead_corner1{ int(perp_x1), int(perp_y1) };
		Point arrowhead_corner2{ int(perp_x2), int(perp_y2) };
		fl_line(arrowhead_base.x, arrowhead_base.y, arrowhead_corner1.x, arrowhead_corner1.y);
		fl_line(arrowhead_base.x, arrowhead_base.y, arrowhead_corner2.x, arrowhead_corner2.y);
		fl_line(Shape::point(1).x, Shape::point(1).y, arrowhead_corner1.x, arrowhead_corner1.y);
		fl_line(Shape::point(1).x, Shape::point(1).y, arrowhead_corner2.x, arrowhead_corner2.y);

		fl_color(Shape::color().as_int());
		//fl_rectf(arrowhead_corner1.x, arrowhead_corner2.y, 50, 100);
		fl_line(arrowhead_corner1.x, arrowhead_corner1.y, arrowhead_corner2.x, arrowhead_corner2.y);
		fl_line(arrowhead_corner2.x, arrowhead_corner2.y, Shape::point(1).x, Shape::point(1).y);
		fl_line(Shape::point(1).x, Shape::point(1).y, arrowhead_corner1.x, arrowhead_corner1.y);

		if (fill_color().visibility()) {	// fill
			fl_color(fill_color().as_int());
			//fl_rect(arrowhead_corner1.x, arrowhead_corner2.y, 50, 100);
			fl_polygon(arrowhead_corner1.x, arrowhead_corner1.y, arrowhead_corner2.x, arrowhead_corner2.y, Shape::point(1).x, Shape::point(1).y);
		}
		
	}
	fl_color(oldc);	// reset color (to pevious) and style (to default)
	fl_line_style(0);
}




void Graph_lib::Box::draw_lines() const
{
	vector<Point> corners;
	corners.push_back(Point{point(0).x, point(0).y});
	corners.push_back(Point{point(0).x + w,point(0).y});		
	corners.push_back(Point{point(0).x + w,point(0).y + h});
	corners.push_back(Point{point(0).x,point(0).y + h});

	double deduction = (100.0 - percentage_curved)/100.0;
	Point topline_a{ corners[0].x + int(deduction * w), corners[0].y };
	Point topline_b{ corners[1].x - int(deduction * w), corners[0].y };

	Point rihline_a{ corners[1].x, corners[1].y + int(deduction * h) };
	Point rihline_b{ corners[2].x, corners[2].y - int(deduction * h) };

	Point lefline_a{ corners[0].x, corners[0].y + int(deduction * h) };
	Point lefline_b{ corners[3].x, corners[3].y - int(deduction * h) };

	Point botline_a{ corners[3].x + int(deduction * w), corners[3].y };
	Point botline_b{ corners[2].x - int(deduction * w), corners[2].y };

	if (fill_color().visibility())
	{
		fl_color(fill_color().as_int());
		// top right pie
		fl_pie(topline_a.x - (rihline_b.x - topline_a.x), topline_a.y, 2.0 * (rihline_b.x - topline_a.x), 2.0 * (rihline_b.y - topline_a.y), 0, 90);
		// top left pie
		fl_pie(lefline_b.x, topline_b.y, 2.0 * (topline_b.x - lefline_b.x), 2.0 * (lefline_b.y - topline_b.y), 90, 180);
		// bot left pie
		fl_pie(lefline_b.x, lefline_a.y - (botline_b.y - lefline_a.y), 2.0 * (topline_b.x - lefline_b.x), 2.0 * (lefline_b.y - topline_b.y), 180, 270);
		// bot right arc
		fl_pie(botline_a.x - (rihline_a.x - botline_a.x), lefline_a.y - (botline_b.y - lefline_a.y), 2.0 * (topline_b.x - lefline_b.x), 2.0 * (lefline_b.y - topline_b.y), 270, 360);

		// rectangle between top left and top right pies
		Rectangle toprect( topline_b, Point(topline_a.x, rihline_b.y)); toprect.set_fill_color(fill_color()); toprect.set_color(fill_color()); toprect.draw_lines();
		// rectangle between top right and bottom right pies
		Rectangle midrightrect(Point(topline_a.x, rihline_b.y), rihline_a); midrightrect.set_fill_color(fill_color()); midrightrect.set_color(fill_color()); midrightrect.draw_lines();
		// rectangle between bottom left and bottom right pies
		Rectangle botrect(Point(botline_b.x, lefline_a.y), botline_a); botrect.set_fill_color(fill_color()); botrect.set_color(fill_color()); botrect.draw_lines();
		// rectangle between top left and bottom left pies
		Rectangle leftrect(lefline_b, Point(botline_b.x, lefline_a.y)); leftrect.set_fill_color(fill_color()); leftrect.set_color(fill_color()); leftrect.draw_lines();

		// Center background rectangle
		Rectangle centerbackg(Point(topline_b.x, lefline_b.y), Point(botline_a.x, rihline_a.y));	centerbackg.set_fill_color(fill_color()); centerbackg.set_color(fill_color()); centerbackg.draw_lines();
	}

	if (color().visibility())
	{
		fl_color(color().as_int());
		fl_line(topline_a.x, topline_a.y, topline_b.x, topline_b.y);	// draw top line, with % deduction on either side
		fl_line(rihline_a.x, rihline_a.y, rihline_b.x, rihline_b.y);

		fl_line(botline_a.x, botline_a.y, botline_b.x, botline_b.y);
		fl_line(lefline_a.x, lefline_a.y, lefline_b.x, lefline_b.y);



/*							~~~ Arc Drawing ~~~
				topline_b---------------------------topline_a


	lefline_b													rihline_b
	|																|
	|																|
	|																|
	|																|
	|																|
	|																|
	lefline_a													rihline_a


				botline_b---------------------------botline_a
*/

		// top right arc
		fl_arc(topline_a.x - (rihline_b.x - topline_a.x), topline_a.y, 2.0*(rihline_b.x - topline_a.x), 2.0*(rihline_b.y - topline_a.y), 0, 90);	// using fl_arc() [2/2]
		// top left arc
		fl_arc(lefline_b.x, topline_b.y, 2.0*(topline_b.x - lefline_b.x), 2.0*(lefline_b.y - topline_b.y), 90, 180);
		// bot left arc
		fl_arc(lefline_b.x, lefline_a.y - (botline_b.y - lefline_a.y), 2.0 * (topline_b.x - lefline_b.x), 2.0 * (lefline_b.y - topline_b.y), 180, 270);
		// bot right arc
		fl_arc(botline_a.x - (rihline_a.x - botline_a.x), lefline_a.y - (botline_b.y - lefline_a.y), 2.0 * (topline_b.x - lefline_b.x), 2.0 * (lefline_b.y - topline_b.y), 270, 360);


		l.draw_lines();
	}
}


