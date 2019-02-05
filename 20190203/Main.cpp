#include <Siv3D.hpp>
#include <algorithm>
#include <cmath>
#include <vector>
#include "plot2d.h"

void Main() {
  // set window
  Window::SetTitle(L"nyan_plot");
  Window::SetStyle(WindowStyle::Sizeable);
  Graphics::SetBackground(Color(0, 0, 0));

  const Font font(10);
  Color mous_pos_color = Palette::White;

  // set gui
  std::vector<std::shared_ptr<GUI>> gui;
  std::vector<std::shared_ptr<plot2d>> plot;
  auto add_graph = [&gui, &plot]() {
    const Color color = RandomColor();
    gui.push_back(std::make_shared<GUI>(GUIStyle::Default));
    (*(gui.end() - 1))->setPos(0, 150 * (gui.size() - 1));
    (*(gui.end() - 1))->style.borderColor = color;
    plot.push_back(std::make_shared<plot2d>(&**(gui.end() - 1), color));
  };
  for (int i = 0; i < 3; i++) add_graph();

  // main loop
  while (System::Update()) {
    // key commands
    {
      if (Input::KeyControl.pressed && Input::KeyH.clicked)
        for (auto g : gui) g->hide();
      if (Input::KeyControl.pressed && Input::KeyS.clicked)
        for (auto g : gui) g->show();

      if (Input::KeyControl.pressed && Input::KeyW.clicked) {
        Graphics::SetBackground(Color(255, 255, 255));
        mous_pos_color = Palette::Black;
      }
      if (Input::KeyControl.pressed && Input::KeyB.clicked) {
        Graphics::SetBackground(Color(0, 0, 0));
        mous_pos_color = Palette::White;
      }
    }

    // set range
    {
      std::vector<double> x_mins;
      std::vector<double> x_maxs;
      std::vector<double> y_mins;
      std::vector<double> y_maxs;
      for (auto p : plot) {
        x_mins.push_back(p->get_x_min());
        x_maxs.push_back(p->get_x_max());
        y_mins.push_back(p->get_y_min());
        y_maxs.push_back(p->get_y_max());
      }
      plot2d::set_x_min(*(std::min_element(x_mins.cbegin(), x_mins.cend())));
      plot2d::set_x_max(*(std::max_element(x_maxs.cbegin(), x_maxs.cend())));
      plot2d::set_y_min(*(std::min_element(y_mins.cbegin(), y_mins.cend())));
      plot2d::set_y_max(*(std::max_element(y_maxs.cbegin(), y_maxs.cend())));
    }

    // plot graph
    for (auto p : plot) p->execute();
    // draw axis
    plot2d::axis().x.draw(Palette::Gray);
    plot2d::axis().y.draw(Palette::Gray);
    // draw mous position
    plot2d::draw_mous_pos(font, mous_pos_color);
  }
}