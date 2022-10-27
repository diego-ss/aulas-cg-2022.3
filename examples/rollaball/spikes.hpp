#ifndef SPIKES_HPP_
#define SPIKES_HPP_

#include <list>
#include <random>

#include "abcgOpenGL.hpp"

#include "ball.hpp"
#include "gamedata.hpp"

class Spikes {
public:
  void create(GLuint program, int quantity);
  void paint();
  void destroy();
  void update(const Ball &ball, GameData const &gameData, float deltaTime);

  struct Spike {
    GLuint m_VAO{};
    GLuint m_VBO{};

    float m_angularVelocity{};
    glm::vec4 m_color{1};
    bool m_hit{};
    int m_polygonSides{};
    float m_rotation{};
    float m_scale{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{1.0f, 0.0f};
  };

  std::list<Spike> m_spikes;

  Spike makeSpike(glm::vec2 translation = {}, float scale = 0.06f);

private:
  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
};

#endif