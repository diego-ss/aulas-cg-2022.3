#include "window.hpp"

#include <cmath>
#include <cstdio>
#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <math.h>
#include <strings.h>

#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {
  // Keyboard events
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
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "depth.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "depth.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_model.loadObj(assetsPath + "box.obj");
  m_model.setupVAO(m_program);

  // Camera at (0,0,0) and looking towards the negative z
  glm::vec3 const eye{0.0f, 1.28f, 4.2f};
  glm::vec3 const at{0.0f, 0.3f, -1.0f};
  glm::vec3 const up{0.0f, 1.0f, -0.5f};
  m_viewMatrix = glm::lookAt(eye, at, up);

  // Posicionando cubos dos trilhos
  float step = 100.0f / (m_stars_left.size() / 2.0f); // PASSO
  int index{0};                                       // CUBO ATUAL
  float actual_z{step}; // REFERÊNCIA DA POSIÇÃO Z

  for (auto &star : m_stars_left) {
    // CUBO IMPAR -> LADO DIREITO
    if (index % 2 == 1) {
      star.m_inital_x = 1.5f;
      // CUBO PAR -> LADO ESQUERDO
    } else {
      star.m_inital_x = -1.5f;
      actual_z -= step;
    }

    star.m_position = glm::vec3(star.m_inital_x, 0.0f, actual_z);

    // Random rotation axis
    star.m_rotationAxis = glm::sphericalRand(1.0f);
    index = index + 1;
  }
}

void Window::onUpdate() {
  // Increase angle by 90 degrees per second
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f) * deltaTime);

  // Update stars
  for (auto &star : m_stars_left) {
    // Increase z by 10 units per second
    star.m_position.z += deltaTime * 10.0f;

    if (m_controller.m_input[gsl::narrow<size_t>(Input::Right)]) {

      m_lerp_ref += deltaTime / 10;
      m_lerp_ref = std::clamp(m_lerp_ref, 0.0f, 1.0f);
      // x_value = pow(star.m_position.z / -15.0f, 2.0f) - star.m_inital_x;
      float x_reta = star.m_inital_x;
      float x_curva = -sqrt(1 - pow((-star.m_position.z / 100) - 1, 2));
      // float x_curva = std::clamp(x_value, -50.0f, 50.0f);

      // star.m_position.x = std::clamp(x_value, -50.0f, 50.0f);
      star.m_position.x =
          std ::lerp(star.m_inital_x, x_curva * 3 + x_reta, m_lerp_ref);
    } else if (m_controller.m_input[gsl::narrow<size_t>(Input::Left)]) {

      m_lerp_ref += deltaTime / 10;
      m_lerp_ref = std::clamp(m_lerp_ref, 0.0f, 1.0f);
      // x_value = pow(star.m_position.z / -15.0f, 2.0f) - star.m_inital_x;
      float x_reta = star.m_inital_x;
      float x_curva = -sqrt(1 - pow((-star.m_position.z / 100) - 1, 2));
      // float x_curva = std::clamp(x_value, -50.0f, 50.0f);

      // star.m_position.x = std::clamp(x_value, -50.0f, 50.0f);
      star.m_position.x =
          -std ::lerp(star.m_inital_x, x_curva * 3 + x_reta, m_lerp_ref);
    } else {
      star.m_position.x = star.m_inital_x;
      m_lerp_ref = 0.0f;
    }

    // If this star is behind the camera, select a new random position &
    // orientation and move it back to -100
    if (star.m_position.z >= 0.0f)
      star.m_position.z = -100.0f; // Back to -100
  }
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables
  auto const viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  auto const colorLoc{abcg::glGetUniformLocation(m_program, "color")};

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // White

  // Render each star
  for (auto &star : m_stars_left) {
    // Compute model matrix of the current star
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, star.m_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    // modelMatrix = glm::rotate(modelMatrix, m_angle, star.m_rotationAxis);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    if (star.m_position.z > -10)
      abcg::glUniform4f(m_colorLocation, 1.0f, 0.0f, 0.0f, 0.8f);

    else
      abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);

    m_model.render();
  }

  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const widgetSize{ImVec2(218, 200)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);

    {
      ImGui::PushItemWidth(120);
      static std::size_t currentIndex{};
      std::vector<std::string> const comboItems{"Perspective", "Orthographic"};

      if (ImGui::BeginCombo("Projection",
                            comboItems.at(currentIndex).c_str())) {
        for (auto const index : iter::range(comboItems.size())) {
          auto const isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      ImGui::PushItemWidth(170);
      auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                        gsl::narrow<float>(m_viewportSize.y)};
      if (currentIndex == 0) {
        m_projMatrix =
            glm::perspective(glm::radians(m_FOV), aspect, 0.01f, 100.0f);

        ImGui::SliderFloat("FOV", &m_FOV, 5.0f, 179.0f, "%.0f degrees");
      } else {
        m_projMatrix = glm::ortho(-20.0f * aspect, 20.0f * aspect, -20.0f,
                                  20.0f, 0.01f, 100.0f);
      }
      ImGui::PopItemWidth();
    }

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("X_TEST", &m_lerp_test, 0.0f, 1.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // temporário para análise do comportamento da câmera
    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("at_x", &m_at.x, -1.0f, 1.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("at_y", &m_at.y, -1.0f, 1.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("at_z", &m_at.z, -1.0f, 1.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("up_x", &m_up.x, -1.0f, 1.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("up_y", &m_up.y, -10.0f, 10.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("up_z", &m_up.z, -10.0f, 10.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("eye_x", &m_eye.x, -1.0f, 1.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("eye_y", &m_eye.y, -10.0f, 10.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("eye_z", &m_eye.z, -10.0f, 10.0f, "%.2f degrees");
    // ImGui::PopItemWidth();

    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) { m_viewportSize = size; }

void Window::onDestroy() {
  m_model.destroy();
  abcg::glDeleteProgram(m_program);
}