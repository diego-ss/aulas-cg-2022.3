#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  static std::string firstNumber;
  static std::string secondNumber;
  static std::string calculationOperator;
  static std::string result;

  ImFont *m_font{};

  void calculate();
  void clear();
};

#endif