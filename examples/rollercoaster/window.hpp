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

  struct Block {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
    float m_inital_x{};
  };

  std::array<Block, 240> m_blocks;

  float m_angle{};

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  GLint m_colorLocation{};

  float m_FOV{30.0f};
  float m_lerp_ref{0.0f};
  float m_velocity;

  GLuint m_program{};

  void changeXDirection(Block &block, float deltaTime, float orientation);
  void changeYDirection(Block &block, float deltaTime, float orientation);
};

#endif