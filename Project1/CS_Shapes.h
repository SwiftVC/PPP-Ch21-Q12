//inline void fl_arc(int x, int y, int w, int h, double a1, double a2) { fl_graphics_driver->arc(x, y, w, h, a1, a2); }
#include "Graph.h"
#include <map>
#include "Simple_window.h"

#ifndef CS_ARC_GUARD
#define CS_ARC_GUARD

namespace Graph_lib {

	const double pi = 3.14159265359;
	class Box;
	class Rectangle;
	class Mark;

	struct Regular_triangle : Polygon {
		Regular_triangle(Point center, double distance_from_center_to_corner) : c{ center }, distance_center_to_corner{ distance_from_center_to_corner }{}
		void draw_lines() const;

		double to_corner() const { return distance_center_to_corner; }
		Point center() const { return c; }
	private:
		Point c;
		int n_sides = 3;
		double distance_center_to_corner;
	};

	Point  n(const Regular_triangle& t);
	Point se(const Regular_triangle& t);
	Point sw(const Regular_triangle& t);

	class Binary_tree : public Shape {
	public:
		Binary_tree(Point center, int level, double radius,
			bool arrow, bool inheritance, double arrowhead_length, double arrowhead_width, Color arrow_color, Color arrow_fill_color)
			: l{ level }, from_center{ radius }, arr{ arrow }, inheritance{ inheritance }, arr_color{ arrow_color }, arr_fill_color{ arrow_fill_color },
			arrhead_l{ arrowhead_length }, arrhead_w{ arrowhead_width }, nodetext(l+2)
		{
			add(center);
			
			// Reserve representation of a vector for handling calls to 'void add_text(string content, string location);'
			// This vector compliments vect in void draw_lines() const, storing text for the equivalent points.
			nodetext[1].push_back("");		// seed level == 1 with base address
			for (int i = 2; i < l + 2; i++)	// reserve array, each parent array entry corresponds to a value of l
			{
				for (int j = 0; j < nodetext[i - 1].size() * 2; j++)
				{
					nodetext[i].push_back("");
				}
			}
		}
		void draw_lines() const;
		Point virtual node_base() const;		// returns the center coordinate and draws the base shape
		void virtual node_extension(Point c_base_shape, Point& lhs_top_tri, Point& rhs_top_tri, int line_delta_x, int line_delta_y) const;

		void add_text(string content, string location);	// populates nodetext function by parsing content to location(s). 
	protected:
		double get_angle() const { return line_angle; }
		double get_from_center() const { return from_center; }
		double get_line_angle() const { return line_angle; }
		bool get_arr() const { return arr; }
		bool get_inheritance() const { return inheritance; }
		Color get_arr_color() const { return arr_color; }
		Color get_fill_color() const { return arr_fill_color; }
		double get_arrhead_l() const { return arrhead_l; }
		double get_arrhead_w() const { return arrhead_w; }
	private:
		int l;	// level
		double from_center;	// to corner OR radius
		double line_angle = pi / 16.0;
		bool arr;
		bool inheritance;
		Color arr_color;
		Color arr_fill_color;
		double arrhead_l;
		double arrhead_w;
		Vector<Vector<string>> nodetext;
	};

	class Binary_triangle_tree : public Binary_tree {
	public:
		Binary_triangle_tree(Point center, int level, double radius,
			bool arrow, bool inheritance, double arrowhead_length, double arrowhead_width, Color arrow_color, Color arrow_fill_color)
			: Binary_tree(center, level, radius,
				 arrow, inheritance, arrowhead_length, arrowhead_width, arrow_color, arrow_fill_color) {}
		Point node_base() const;
		void node_extension(Point tl_base_shape, Point& lhs_circle_tl, Point& rhs_circle_tl, int line_delta_x, int line_delta_y) const;
	};

	class Pseudo_window {
	public:
		Pseudo_window(Point p);
		void activate(Simple_window& win) const;
	private:
		Point tl;
		Rectangle* background;
		Rectangle* foreground;
		Rectangle* redsquare;
		Mark* corner_cross;
		int width = 600;
		int height = 300;
	};

	class Group {
	public:
		void up(int index, int distance) { vect[index].move(0, -distance); }
		void down(int index, int distance) { vect[index].move(0, distance); }
		void left(int index, int distance) { vect[index].move(-distance, 0); }
		void right(int index, int distance) { vect[index].move(distance, 0); }
		int size() { return vect.size(); }
		
		void put(Shape* s)
		{
			vect.push_back(s);
		}

		Shape& get(int index) { return vect[index]; }

	private:
		Vector_ref<Shape> vect;
	};



	struct Octagon : public Shape {
		Octagon(Point centre, int center_to_corner) : distance_from_center_to_corner { center_to_corner }
		{
			add(centre);
		}
		void draw_lines() const;
	private:
		int distance_from_center_to_corner;
	};

	struct Striped_closed_polyline : Closed_polyline {
		using Closed_polyline::Closed_polyline;
		void draw_lines() const;
	};

	struct Striped_circle : Circle {
		Striped_circle(Point xy, int r) : Circle(xy, r) {}
		void draw_lines() const;
	};

	struct Striped_rectangle : Rectangle {
		Striped_rectangle(Point xy, int ww, int hh) : Rectangle(xy, ww, hh) {}
		Striped_rectangle(Point x, Point y) : Rectangle(x, y) {}
		void draw_lines() const;
	};

	struct Smiley : Circle {
		Smiley(Point p, int r) : Circle(p, r) {}
		void draw_lines() const;
	};

	struct Smileyhat : Smiley {
		Smileyhat(Point p, int r) : Smiley(p, r) {}
		void draw_lines() const;
	};

	struct Frowny : Circle {
		Frowny(Point p, int r) : Circle(p, r) {}
		void draw_lines() const;
	};

	struct Frownyhat : Frowny {
		Frownyhat(Point p, int r) : Frowny(p, r) {}
		void draw_lines() const;
	};


	struct Star : Shape {
		Star(Point center, int points, double center_to_corner, int sharpness) : c{ center }, n_points{ points }, c_to_corner{ center_to_corner }, sharp{ sharpness } {}
		void draw_lines() const;
	private:
		Point c;
		int n_points;
		double c_to_corner;
		int sharp;				// a weighting towards the center for each inner point
	};

	struct Poly : Shape {
		Poly(vector<Point> vect) {
			for(int i = 0; i < vect.size(); i++)
				add(vect[i]);
			Polygon p;
			for (int i = 0; i < vect.size(); i++)		// creates a temporary polygon type to use Stroustrup's intersection checks
				p.add(vect[i]);

		}
		void draw_lines() const;
	};


	struct Right_triangle : Shape {
		Right_triangle(Point corner, string version, double sidelength) : vers{ version }, side{sidelength} {		// uses the 90 degree corner as the point
			add(Point{ corner.x, corner.y });
			if (version != "/_|" && version != "|_\\" && version != "<|" && version != "|>" && version != "\-|" && version != "|-/")
				error("Invalid Right_triangle version, please use: /_|, |_\\, <|, |>, \\-| or |-/");
		}

		void draw_lines() const;
	
	private:
		string vers;
		double side;
	};
	//	All triangles centered in right-angle's corner


	struct Regular_polygon : Polygon {
		Regular_polygon(Point center, int number_of_sides, double distance_from_center_to_corner) : c{ center }, n_sides{ number_of_sides }, distance_center_to_corner{distance_from_center_to_corner}{}
		void draw_lines() const;

	private:
		Point c;
		int n_sides;
		double distance_center_to_corner;
	};



	struct Regular_hexagon : Shape {
	public:
		Regular_hexagon(Point centre, int distance_to_corner_from_center) : distance_from_center{ distance_to_corner_from_center } {
			add(Point{ centre.x, centre.y});
		}

		void draw_lines() const;

	private:
		int distance_from_center;
	};

	struct Arc : Shape {
	
	public:
		Arc(Point p, int ww, int hh, double startAngle, double endAngle) : w{ww}, h{hh}, sAngle{startAngle}, eAngle{endAngle}
		{
			add(Point{ p.x - ww,p.y - hh });
		}

	void draw_lines() const;

	private:
		int w;
		int h;
		double sAngle;
		double eAngle;
	};

	struct Box : Rectangle {
	public:
		Box(Point p, int width, int height, int curvedpercent)
			: w{ width }, h{ height }, percentage_curved{curvedpercent}, Graph_lib::Rectangle{ p, width, height }, l{p, ""}
		{
			Shape::add(Point{ p.x - width,p.y - height });
		}

		Box(Point p, int width, int height, int curvedpercent, string label)
			: w{ width }, h{ height }, percentage_curved{ curvedpercent }, l{ Point{p.x, p.y + h}, label }, Graph_lib::Rectangle{ p, width, height }
		{
			Shape::add(Point{ p.x - width,p.y - height });
		}


		static const int textspacing = 10;
		Box(Point p, int height, int curvedpercent, string label)				// width scales with label size
			: w{ int(label.size()* textspacing)}, h{int(height)}, percentage_curved{curvedpercent}, Graph_lib::Rectangle{p, label.size()* textspacing, int(height)}, l{Point{p.x + textspacing/2, int(p.y + h-4)}, label}
		{
			Shape::add(Point{ p.x - int(w),p.y - int(height) });
		}

		void draw_lines() const;
	private:
		int w;
		int h;
		int percentage_curved;
		Text l;
	};

	struct Arrow : Shape {
	public:
		Arrow(Point p1, Point p2, double arrowhead_length, double arrowhead_width) : arrowhead_l{ arrowhead_length }, arrowhead_w(arrowhead_width)
		{
			add(p1);		// p1 is Shape::points[0]
			add(p2);		// p2 is Shape::points[1]
		}

		void draw_lines() const;
	private:
		double arrowhead_l = 20;
		double arrowhead_w = 10;
	};

#define boxsize 40
	struct ColorBox : Rectangle {
	public:
		ColorBox(Point tl, unsigned char red, unsigned char green, unsigned char blue) : Rectangle(tl, boxsize, boxsize), c{ Point{tl.x, tl.y + boxsize}, "" },
			r{ red }, g{ green }, b{ blue }, str{""} {
			std::stringstream stream;
			stream << std::hex << int(r);
			string temp = "";
			stream >> temp;
			str += temp + ',';

			stream.flush();
			stream.clear();
			stream << std::hex << int(g);
			stream >> temp;
			str += temp + ',';

			stream.flush();
			stream.clear();
			stream << std::hex << int(b);
			stream >> temp;
			str += temp;

			c.set_label(str);
			c.set_font_size(boxsize/5);
		}

		void draw_lines() const;	// draws box with predefined size and color in hex labelled

	private:
		Text c;						// number code for color;
		int w = boxsize;
		int h = boxsize;
		unsigned char r;
		unsigned char g;
		unsigned char b;
		string str;
	};

	double angle_between_points(Point a, Point b);			// returns the angle between points in degrees. See implementation for comments UNUSED
	double angle_between_points_2(Point start, Point end);	// returns the angle between points in degrees with alterations, see comments	UNUSED
	double angle_conversion(double angle);					// converts the angle to angle_between_points_2 format							UNUSED
	double to_rad(double degrees);
	void quadratic_formula(double& x1, double& x2, double a, double b, double c);

	Point n(Rectangle& r);
	Point s(Rectangle& r);
	Point e(Rectangle& r);
	Point w(Rectangle& r);
	Point center(Rectangle& r);
	Point ne(Rectangle& r);
	Point se(Rectangle& r);
	Point nw(Rectangle& r);
	Point sw(Rectangle& r);

	Point n(Circle& c);
	Point s(Circle& c);
	Point e(Circle& c);
	Point w(Circle& c);
	Point center(Circle& c);
	Point ne(Circle& c);
	Point se(Circle& c);
	Point nw(Circle& c);
	Point sw(Circle& c);

	Point n(Ellipse& e);
	Point s(Ellipse& e);
	Point e(Ellipse& e);
	Point w(Ellipse& e);
	Point center(Ellipse& e);
	Point ne(Ellipse& e);
	Point se(Ellipse& e);
	Point nw(Ellipse& e);
	Point sw(Ellipse& e);
}
#endif

