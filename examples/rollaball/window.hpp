#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"
#include "ball.hpp"
#include "spikes.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{}; // DIMENSÕES DA JANELA
  GLuint m_objectsProgram{}; // OBJECTS PROGRAM
  GameData m_gameData; // GAME DATA REF
  Ball m_ball; // BALL REF
  Spikes m_spikes; // SPIKES REF
  abcg::Timer m_restartWaitTimer; // TEMPORIZADOR
  ImFont *m_font{}; // FONTE
  std::default_random_engine m_randomEngine; // RANDOMIZADOR

  void restart();
  void checkCollisions();
};

#endif