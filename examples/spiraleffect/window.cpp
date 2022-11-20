#include "window.hpp"

#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

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

  // orientando câmera para olhar para z = -1.0f
  orientCamera({0.0f, 0.0f, -1.0f});
  generateSpiral();
}

void Window::orientCamera(glm::vec3 const at) {
  // a câmera é fixa na posição 0,0,0 e a direção
  // em que ela olha varia de acordo com o vetor at
  // enviado como parâmetro
  glm::vec3 const eye{0.0f, 0.0f, 0.0f};
  glm::vec3 const up{0.0f, 1.0f, 0.0f};
  m_viewMatrix = glm::lookAt(eye, at, up);
}

// gera espiral de cubos
void Window::generateSpiral() {
  // variáveis auxiliaxes
  auto num_objects = m_objects.size();
  auto increment = m_TwoPI / num_objects;
  auto angle = 0.0f;
  auto x = 0.0f, y = 5.0f;

  // para cada cubo, é gerada uma posição com base na equação para geração do
  // espiral ao mesmo tempo em que z é decrementado para criar a sensação de
  // profundidade
  for (auto &objRef : m_objects) {
    objRef.m_position =
        glm::vec3(m_spiralRadius * cos(angle) + x,
                  m_spiralRadius * sin(angle) * y, -angle * 15.0f);

    // rotacionando cubos
    objRef.m_rotationAxis = glm::sphericalRand(1.0f);

    // incrementando angulo para geração do espiral
    angle += increment;
    // diminuindo raio do espiral
    m_spiralRadius -= m_spiralStep;
  }
}

void Window::onUpdate() {
  // incrementando angulo em 90 graus por segundo
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f) * deltaTime);

  int index{0};
  // atualizando objetos
  for (auto &objRef : m_objects) {
    // incrementando z em 15 unidades por segundo
    objRef.m_position.z += deltaTime * 15.0f;

    // resetando posição do cubo quando chega em z = -20
    if (objRef.m_position.z > -20.0f) {
      // orientando câmera para olhar para os cubos mais próximos
      orientCamera(
          {objRef.m_position.x / 80.0f, objRef.m_position.y / 80.0f, -1.0f});

      // voltando para o fundo da tela
      objRef.m_position.z = -100.0f;
    }
    index++;
  }
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // capturando as variáveis uniformes
  auto const viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  auto const colorLoc{abcg::glGetUniformLocation(m_program, "color")};

  // setando as variáveis uniformes
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // White

  // renderizando cubos
  for (auto &objRef : m_objects) {
    // aplicando as transformações lineares -> rotação -> escala -> translação
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, objRef.m_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, objRef.m_rotationAxis);

    // variável uniforme
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    m_model.render();
  }

  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const widgetSize{ImVec2(218, 60)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);

    {
      // a projeção foi mantida para que o efeito de espiral seja melhor
      // visualizado
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

    // temporário para análise do comportamento da câmera
    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("AT_x", &m_cameraAt_x, 0.0f, 0.9f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("AT_Y", &m_cameraAt_y, 0.0f, 0.9f, "%.2f degrees");
    // ImGui::PopItemWidth();

    // ImGui::PushItemWidth(170);
    // ImGui::SliderFloat("AT_Z", &m_cameraAt_z, -1.0f, 179.0f, "%.0f degrees");
    // ImGui::PopItemWidth();

    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) { m_viewportSize = size; }

void Window::onDestroy() {
  m_model.destroy();
  abcg::glDeleteProgram(m_program);
}