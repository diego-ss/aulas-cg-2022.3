#ifndef BALL_HPP_
#define BALL_HPP_

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"

class Ball {
public:
  void create(GLuint program);
  void paint(GameData const &gameData);
  void destroy();
  void update(GameData const &gameData, float deltaTime);

  glm::vec4 m_color{1};
  float m_rotation{};
  float m_scale{0.125f};
  glm::vec2 m_translation{};
  glm::vec2 m_velocity{1.0f, 0.0f};

  bool isJumping{false};     // INDICADOR DE PULO
  bool isFallingDown{false}; /// INDICADOR DE QUEDA

  float const m_maxTranslationY{0.6f}; // LIMITE SUPERIOR DO PULO
  float const m_minTranslationY{0.0f}; // CHÃO
  float const m_jumpForce{1.0f};       // FORÇA DE PULO
  float const m_fallForce{-1.1f};      // FORÇA DE QUEDA (CAI MAIS RÁPIDO)

private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
};
#endif