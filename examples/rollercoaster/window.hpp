#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "controller.hpp"
#include "model.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onEvent(SDL_Event const &event) override;

private:
  std::default_random_engine m_randomEngine;

  glm::ivec2 m_viewportSize{};

  Model m_model;
  Controller m_controller;

  struct Star {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
  };

  std::array<Star, 250> m_stars_left;
  std::array<Star, 250> m_stars_right;

  float m_angle{};

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  glm::vec3 m_eye{0.0f, 1.28f, 4.2f};
  glm::vec3 m_at{0.0f, 0.3f, -1.0f};
  glm::vec3 m_up{0.0f, 1.0f, -0.5f};

  float m_FOV{30.0f};

  GLuint m_program{};

  void randomizeStar(Star &star, float xPos);
};

#endif