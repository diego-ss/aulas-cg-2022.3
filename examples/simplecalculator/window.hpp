#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  float operator1{0.0f};
  float operator2{0.0f};
  float result{0.0f};
  std::string calcOperator{"+"};

  ImFont *m_font{};

  void calculate();
  void clear();
};

#endif