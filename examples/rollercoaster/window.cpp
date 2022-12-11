#include "window.hpp"

#include <cmath>
#include <cstdio>
#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <math.h>
#include <strings.h>

#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {
  // capturando eventos de teclado
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_controller.m_input.set(gsl::narrow<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_controller.m_input.set(gsl::narrow<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_controller.m_input.set(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_controller.m_input.set(gsl::narrow<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_controller.m_input.reset(gsl::narrow<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_controller.m_input.reset(gsl::narrow<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_controller.m_input.reset(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_controller.m_input.reset(gsl::narrow<size_t>(Input::Right));
  }
}

void Window::onCreate() {
  // carregando assets
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  // criando um openglprogram e carregando os shaders
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "depth.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "depth.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_box_model.loadObj(assetsPath + "box.obj");
  m_box_model.setupVAO(m_program);

  // Câmera olhando para z negativo, localizada um pouco acima da origem e
  // levemente inclinada
  glm::vec3 const eye{0.0f, 1.28f, 4.2f};
  glm::vec3 const at{0.0f, 0.3f, -1.0f};
  glm::vec3 const up{0.0f, 1.0f, -0.5f};
  m_viewMatrix = glm::lookAt(eye, at, up);

  // Posicionando cubos dos trilhos
  float step = 100.0f / (m_blocks.size() / 2.0f); // PASSO
  int index{0};                                   // CUBO ATUAL
  float actual_z{step};                           // REFERÊNCIA DA POSIÇÃO Z

  for (auto &block : m_blocks) {
    // CUBO IMPAR -> LADO DIREITO
    if (index % 2 == 1) {
      block.m_inital_x = 1.5f;
      // CUBO PAR -> LADO ESQUERDO
    } else {
      block.m_inital_x = -1.5f;
      actual_z -= step;
    }

    // atualizando posição
    block.m_position = glm::vec3(block.m_inital_x, 0.0f, actual_z);
    block.m_rotationAxis = glm::sphericalRand(1.0f);
    index = index + 1;
  }
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  m_velocity = 10.0f;

  // atualizando blocos
  for (auto &block : m_blocks) {
    // Incrementando z de acordo com a velocidade
    block.m_position.z += deltaTime * m_velocity;

    if (m_controller.m_input[gsl::narrow<size_t>(Input::Right)]) {
      // simulando curva à direita
      changeXDirection(block, deltaTime, -1.0f);
    } else if (m_controller.m_input[gsl::narrow<size_t>(Input::Left)]) {
      // simulando curva à esquerda
      changeXDirection(block, deltaTime, 1.0f);
    } else if (m_controller.m_input[gsl::narrow<size_t>(Input::Down)]) {
      // simulando descida
      changeYDirection(block, deltaTime, 1.0f);
    } else if (m_controller.m_input[gsl::narrow<size_t>(Input::Up)]) {
      // simulando subida
      changeYDirection(block, deltaTime, -1.0f);
    } else {
      // posição original
      block.m_position.x = block.m_inital_x;
      block.m_position.y = 0.0f;
      m_lerp_ref = 0.0f;
    }

    // Quando o trilho passa da câmera, ele é resetado para a posição de origem
    if (block.m_position.z >= 0.0f) {
      block.m_position.z = -100.0f; // Back to -100
      block.m_position.y = 0.0f;
    }
  }
}

void Window::changeYDirection(Block &block, float deltaTime,
                              float orientation) {
  // calculando "suavizador da curva"
  m_lerp_ref += deltaTime / 200;
  m_lerp_ref = std::clamp(m_lerp_ref, 0.0f, 1.0f);
  float y_reta = 0.0f;

  // gerando semi-circulo da curva com a equação y = (1-(z/100 - 1)²)
  float y_curva = -sqrt(1 - pow((-block.m_position.z / 100) - 1, 2));
  // atualizando posição
  block.m_position.y =
      orientation * std ::lerp(0.0f, y_curva * 3 + y_reta, m_lerp_ref);

  // variando velocidade do "movimento" de acordo com a ação
  // se subida, diminui a velocidade
  // se descida, aumenta
  // para dar a sensação de simulação
  if (orientation > 0)
    m_velocity = 20.0f;
  else
    m_velocity = 5.0f;
}

void Window::changeXDirection(Block &block, float deltaTime,
                              float orientation) {
  // calculando "suavizador da curva"
  m_lerp_ref += deltaTime / 200;
  m_lerp_ref = std::clamp(m_lerp_ref, 0.0f, 1.0f);
  float x_reta = block.m_inital_x;

  // gerando semi-circulo da curva com a equação x = (1-(z/100 - 1)²)
  float x_curva = -sqrt(1 - pow((-block.m_position.z / 100) - 1, 2));

  // atualizando posição
  block.m_position.x =
      orientation *
      std ::lerp(block.m_inital_x, x_curva * 3 + x_reta, m_lerp_ref);
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
  abcg::glUseProgram(m_program);

  // Localização das variáveis uniformes
  auto const viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  auto const colorLoc{abcg::glGetUniformLocation(m_program, "color")};

  // setando variáveis uniformes
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

  // Renderizando blocos
  for (auto &block : m_blocks) {
    // Model matrix do block em questão
    // escala -> translação
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, block.m_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));

    // Setando variável uniforme
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    // Variando a coloração dos trilhos de acordo com a proximidade da câmera
    // a ideia é simular os trilhos 'esquentando' com o atrito
    if (block.m_position.z >= -4.0f) {
      float red_prop = (4 / -block.m_position.z);
      abcg::glUniform4f(m_colorLocation, 1.0f + red_prop, 1.0f - red_prop,
                        1.0f - red_prop, 0.0f + red_prop);
    } else
      abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);

    m_box_model.render();
  }

  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                    gsl::narrow<float>(m_viewportSize.y)};
  m_projMatrix = glm::perspective(glm::radians(m_FOV), aspect, 0.01f, 100.0f);
}

void Window::onResize(glm::ivec2 const &size) { m_viewportSize = size; }

void Window::onDestroy() {
  m_box_model.destroy();
  abcg::glDeleteProgram(m_program);
}