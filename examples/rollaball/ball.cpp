#include "ball.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Ball::create(GLuint program) {
  destroy();

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Reset ball attributes
  m_rotation = 0.0f;
  m_translation = glm::vec2(-0.7f, 0.0f);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create geometry data
  auto const sides{10};
  std::vector<glm::vec2> positions{{0, 0}};
  auto const step{M_PI * 2 / sides};
  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Ball::paint(const GameData &gameData) {
  if (gameData.m_state != State::Playing)
    return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);
  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, 12);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Ball::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Ball::update(GameData const &gameData, float deltaTime) {
  // Rotação constante da bola
  m_rotation = glm::wrapAngle(m_rotation - 4.0f * deltaTime * 2);

  // Detecção de pulo
  if (gameData.m_input[gsl::narrow<size_t>(Input::Up)] &&
      gameData.m_state == State::Playing && !isJumping && !isFallingDown) {

    isJumping = true;
  }

  // Detecção de boost
  if (gameData.m_input[gsl::narrow<size_t>(Input::Space)] && !isBoosted) {
    m_velocity += m_velocityBoost;
    isBoosted = true;
  } else if (m_velocity.x > 1.0f) {
    m_velocity -= m_velocityBoost;
    isBoosted = false;
  }

  // Aplicando pulo
  if (isJumping && m_translation.y < m_maxTranslationY) {
    auto const jumpVector = glm::vec2{0.0f, m_jumpForce};
    m_translation.y =
        std::clamp((m_translation + jumpVector * deltaTime * 2.0f).y,
                   m_translation.y, m_maxTranslationY);
  }

  // Detectando queda
  if (m_translation.y == m_maxTranslationY) {
    isFallingDown = true;
    isJumping = false;
  } else if (m_translation.y == 0) {
    isFallingDown = false;
  }

  // Aplicando queda
  if (isFallingDown && m_translation.y > m_minTranslationY) {
    auto const fallVector = glm::vec2{0.0f, m_fallForce};
    m_translation.y =
        std::clamp((m_translation + fallVector * deltaTime * 2.0f).y,
                   m_minTranslationY, m_translation.y);
  }
}