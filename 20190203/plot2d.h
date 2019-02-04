#ifndef PLOT2D
#define PLOT2D
#include <Siv3D.hpp>

class plot2d {
 public:
  // constructor
  plot2d(GUI& g, const Color color);
  // main func
  void execute();
  // getter
  double get_x_min();
  double get_x_max();
  double get_y_min();
  double get_y_max();
  // setter
  static void set_x_min(double x);
  static void set_x_max(double x);
  static void set_y_min(double y);
  static void set_y_max(double y);

  static Vector2D<LineString> axis();
  static void draw_mous_pos(Font font, Color color);

 private:
  // for plot
  void plot(const ParsedExpression& f, LineString& line);

  GUI& gui_;
  const Color color_;

  static bool changed_all_;
  // data for plot
  LineString line_;
  // before window size
  double before_height_;
  double before_width_;
  // range
  double x_min_;
  double x_max_;
  double y_min_;
  double y_max_;
  // range @ grobal
  static double x_min_all_;
  static double x_max_all_;
  static double y_min_all_;
  static double y_max_all_;
  // default strings
  const String defaultExpression;
  const String default_x_min;
  const String default_x_max;
  const String default_y_min;
  const String default_y_max;
};

#endif PLOT2D