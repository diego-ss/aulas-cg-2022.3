#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {
  // Eventos do teclado
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Space));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Up));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Space));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Up));
  }

  // Eventos do mouse
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Space));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Up));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Space));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Up));
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Carregando fonte
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  // Criando programa para renderizar objetos
  m_objectsProgram =
      abcg::createOpenGLProgram({{.source = assetsPath + "objects.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "objects.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // limpando tela com a cor preta
  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void Window::restart() {
  // estado inicial do jogo
  m_gameData.m_state = State::Playing;
  m_gameData.m_score = 0;

  // criando bola e espinhos
  m_ball.create(m_objectsProgram);
  m_spikes.create(m_objectsProgram, 4);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Aguardando 5 minutos para reiniciar o jogo
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  // atualizações da bola e dos espinhos
  m_ball.update(m_gameData, deltaTime);
  m_spikes.update(m_ball, m_gameData, deltaTime);

  // verificando colisões
  if (m_gameData.m_state == State::Playing)
    checkCollisions();
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // método paint dos objetos
  m_ball.paint(m_gameData);
  m_spikes.paint();
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground |
                               ImGuiWindowFlags_NoTitleBar |
                               ImGuiWindowFlags_NoInputs};

  // Printando Score enquanto o jogo está rodando
  if (m_gameData.m_state == State::Playing) {
    {
      ImGui::SetNextWindowPos(ImVec2(5, 10));
      ImGui::Begin(" ", nullptr, flags);
      ImGui::PushFont(m_font);

      ImGui::Text("Score: %i", m_gameData.m_score);

      ImGui::PopFont();
      ImGui::End();
    }
  }

  // printando Game Over + Score ao finalizar
  {
    auto const size{ImVec2(300, 200)};
    auto const position{ImVec2((m_viewportSize.x - size.x) / 2.0f,
                               (m_viewportSize.y - size.y - 100) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);

    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!\nScore: %i", m_gameData.m_score);
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_spikes.destroy();
  m_ball.destroy();
}

void Window::checkCollisions() {
  // Verificando colisão entre a bola e os espinhos
  for (auto const &spike : m_spikes.m_spikes) {
    auto const spikeTranslation{spike.m_translation};
    auto const distance{glm::distance(m_ball.m_translation, spikeTranslation)};

    if (distance < m_ball.m_scale * 0.9f + spike.m_scale * 0.17f) {
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
    }
  }
}
