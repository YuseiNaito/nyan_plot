#include <cmath>
#include "plot2d.h"

bool plot2d::changed_all_;
double plot2d::x_min_all_;
double plot2d::x_max_all_;
double plot2d::y_min_all_;
double plot2d::y_max_all_;

plot2d::plot2d(GUI& g, const Color color)
    : gui_(g),
      color_(color),
      defaultExpression(L"sin(x)"),
      default_x_min(L"-7.0"),
      default_x_max(L"7.0"),
      default_y_min(L"-1.0"),
      default_y_max(L"1.0") {
  gui_.addln(L"exp", GUITextArea::Create(1, 17));
  gui_.add(GUIText::Create(L"x min", 45));
  gui_.add(L"x_min", GUITextArea::Create(1, 5));
  gui_.add(GUIText::Create(L"max", 25));
  gui_.addln(L"x_max", GUITextArea::Create(1, 5));
  gui_.add(GUIText::Create(L"y min", 45));
  gui_.add(L"y_min", GUITextArea::Create(1, 5));
  gui_.add(GUIText::Create(L"max", 25));
  gui_.add(L"y_max", GUITextArea::Create(1, 5));
  gui_.textArea(L"exp").setText(defaultExpression);
  gui_.textArea(L"x_min").setText(default_x_min);
  gui_.textArea(L"x_max").setText(default_x_max);
  gui_.textArea(L"y_min").setText(default_y_min);
  gui_.textArea(L"y_max").setText(default_y_max);
  gui_.setTitle(L"");
  gui_.style.movable = true;
  x_min_all_ = 0;
  x_max_all_ = 0;
  y_min_all_ = 0;
  y_max_all_ = 0;
}

void plot2d::execute() {
  if (changed_all_ || gui_.textArea(L"exp").hasChanged ||
      gui_.textArea(L"x_min").hasChanged ||
      gui_.textArea(L"x_max").hasChanged ||
      gui_.textArea(L"y_min").hasChanged ||
      gui_.textArea(L"y_max").hasChanged ||
      Window::Height() != before_height_ || Window::Width() != before_width_) {
    if (const ParsedExpression exp{gui_.textArea(L"exp").text}) {
      const double x_min{Parse<double>(gui_.textArea(L"x_min").text)};
      const double x_max{Parse<double>(gui_.textArea(L"x_max").text)};
      const double y_min{Parse<double>(gui_.textArea(L"y_min").text)};
      const double y_max{Parse<double>(gui_.textArea(L"y_max").text)};
      gui_.textArea(L"exp").style.color = Palette::Black;
      x_min_ = x_min;
      x_max_ = x_max;
      y_min_ = y_min;
      y_max_ = y_max;
      plot(exp, line_);
    } else {
      gui_.textArea(L"exp").style.color = Palette::Red;
    }
    before_height_ = Window::Height();
    before_width_ = Window::Width();
  }
  line_.draw(4, color_);
}

void plot2d::plot(const ParsedExpression& f, LineString& line) {
  line.clear();
  const double coef_x = std::abs(x_max_all_) > std::abs(x_min_all_)
                            ? (Window::Width() / 2.0) / std::abs(x_max_all_)
                            : (Window::Width() / 2.0) / std::abs(x_min_all_);
  const double coef_y = std::abs(y_max_all_) > std::abs(y_min_all_)
                            ? (Window::Height() / 2.0) / std::abs(y_max_all_)
                            : (Window::Height() / 2.0) / std::abs(y_min_all_);
  for (double tmp_x = x_min_; tmp_x <= x_max_; tmp_x += 0.05) {
    double tmp_y{f.evaluateOpt({{L"x", tmp_x}}).value_or(0)};
    if (y_min_ > tmp_y)
      tmp_y = y_min_;
    else if (y_max_ < tmp_y)
      tmp_y = y_max_;
    const double x = coef_x * tmp_x + Window::Width() / 2.0;
    const double y = Window::Height() / 2.0 - coef_y * tmp_y;
    line.push_back(Vec2{x, y});
  }
}

double plot2d::get_x_min() { return x_min_; }
double plot2d::get_x_max() { return x_max_; }
double plot2d::get_y_min() { return y_min_; }
double plot2d::get_y_max() { return y_max_; }

void plot2d::set_x_min(double x) {
  changed_all_ = x != x_min_all_;
  plot2d::x_min_all_ = x;
}
void plot2d::set_x_max(double x) {
  changed_all_ = x != x_max_all_;
  plot2d::x_max_all_ = x;
}
void plot2d::set_y_min(double y) {
  changed_all_ = y != y_min_all_;
  plot2d::y_min_all_ = y;
}
void plot2d::set_y_max(double y) {
  changed_all_ = y != y_max_all_;
  plot2d::y_max_all_ = y;
}

Vector2D<LineString> plot2d::axis() {
  const double coef_x = std::abs(x_max_all_) > std::abs(x_min_all_)
                            ? (Window::Width() / 2.0) / std::abs(x_max_all_)
                            : (Window::Width() / 2.0) / std::abs(x_min_all_);
  const double coef_y = std::abs(y_max_all_) > std::abs(y_min_all_)
                            ? (Window::Height() / 2.0) / std::abs(y_max_all_)
                            : (Window::Height() / 2.0) / std::abs(y_min_all_);
  LineString x{{0, Window::Height() / 2.0},
               {Window::Width(), Window::Height() / 2.0}};
  LineString y{{Window::Width() / 2.0, 0},
               {Window::Width() / 2.0, Window::Height()}};
  return Vector2D<LineString>(x, y);
}

void plot2d::draw_mous_pos(Font font, Color color) {
  const double coef_x = std::abs(x_max_all_) > std::abs(x_min_all_)
                            ? (Window::Width() / 2.0) / std::abs(x_max_all_)
                            : (Window::Width() / 2.0) / std::abs(x_min_all_);
  const double coef_y = std::abs(y_max_all_) > std::abs(y_min_all_)
                            ? (Window::Height() / 2.0) / std::abs(y_max_all_)
                            : (Window::Height() / 2.0) / std::abs(y_min_all_);
  const auto pos_x = (Mouse::Pos().x - Window::Width() / 2.0) / coef_x;
  const auto pos_y = (Window::Height() / 2.0 - Mouse::Pos().y) / coef_y;
  font(pos_x, L" ", pos_y)
      .draw(Vec2(Mouse::Pos().x -
                     100 * Sign(Mouse::Pos().x - Window::Width() / 2.0) - 50,
                 Mouse::Pos().y -
                     10 * Sign(Mouse::Pos().y - Window::Height() / 2.0)),
            color);
}
