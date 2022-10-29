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
  void update(const Ball &ball, GameData &gameData, float deltaTime);

  struct Spike {
    GLuint m_VAO{}; // VAO
    GLuint m_VBO{}; // VBO

    glm::vec4 m_color{1};      // COR
    int m_polygonSides{3};     // LADOS DO POLÍGONO DO ESPINHO
    float m_rotation{1.6f};    // ROTAÇÃO
    float m_scale{0.06f};      // ESCALA
    glm::vec2 m_translation{}; // TRANSLAÇÃO
  };

  std::list<Spike> m_spikes; // LISTA DE ESPINHOS
  int maxQtdTopSpikes{0};    // QTD MÁXIMA DE ESPINHOS EM CIMA
  int qtdTopSpikes{0};       // QTD DE ESPINHOS EM CIMA

  Spike makeSpike();
  void randomizeTopSpikes(Spike &spike);

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