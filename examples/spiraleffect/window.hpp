#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "model.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  std::default_random_engine m_randomEngine;

  glm::ivec2 m_viewportSize{};

  Model m_model;

  // cubos usados para gerar o espiral
  struct Cube {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
  };

  // array de cubos
  std::array<Cube, 800> m_objects;

  float m_angle{};

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  float m_FOV{30.0f};

  // referência da câmera look at
  float m_cameraAt_x{0.0f};
  float m_cameraAt_y{0.0f};
  float m_cameraAt_z{-1.0f};

  float m_spiralRadius{3.0f}; // RAIO DO ESPIRAL
  float m_spiralStep{
      m_spiralRadius /
      m_objects.size()};      // PASSO DO ESPIRAL COM BASE NA QTD DE CUBOS
  float m_TwoPI{M_PI * 2.0f}; // 2 * PI
  GLuint m_program{};

  void generateSpiral();
  void orientCamera(glm::vec3 const at);
};

#endif