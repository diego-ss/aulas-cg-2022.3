#include "spikes.hpp"

#include <glm/gtx/fast_trigonometry.hpp>

void Spikes::create(GLuint program, int quantity) {
  destroy();

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;

  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create spikes
  m_spikes.clear();
  m_spikes.resize(quantity);

  float i{0.0f};
  for (auto &spike : m_spikes) {
    spike = makeSpike();
    spike.m_translation = {1.0f + i, -0.01f};
  }
}

void Spikes::paint() {
  abcg::glUseProgram(m_program);

  for (auto const &spike : m_spikes) {
    abcg::glBindVertexArray(spike.m_VAO);

    abcg::glUniform4fv(m_colorLoc, 1, &spike.m_color.r);
    abcg::glUniform1f(m_scaleLoc, spike.m_scale);
    abcg::glUniform1f(m_rotationLoc, spike.m_rotation);

    for (auto i : {-2, 0, 2}) {
      for (auto j : {-2, 0, 2}) {
        abcg::glUniform2f(m_translationLoc, spike.m_translation.x + j,
                          spike.m_translation.y + i);

        abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, spike.m_polygonSides + 2);
      }
    }

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Spikes::destroy() {
  for (auto &spike : m_spikes) {
    abcg::glDeleteBuffers(1, &spike.m_VBO);
    abcg::glDeleteVertexArrays(1, &spike.m_VAO);
  }
}

void Spikes::update(const Ball &ball, float deltaTime) {
  for (auto &spike : m_spikes) {
    // spike.m_translation.x -= ball.m_velocity.x * deltaTime * 10.0f;
    //      glm::wrapAngle(spike.m_rotation + spike.m_angularVelocity *
    //      deltaTime);
    spike.m_translation.x -= ball.m_velocity.x * deltaTime;

    // Destruir quando sair da tela
    //  Wrap-around

    if (spike.m_translation.x < -1.0f) {
      std::uniform_real_distribution<float> randomDists(0.0f, 1.0f);
      float randomDist = randomDists(m_randomEngine) + 1.0f;
      spike.m_translation.x += randomDist;
    }
  }
}

Spikes::Spike Spikes::makeSpike(glm::vec2 translation, float scale) {
  Spike spike;

  auto &re{m_randomEngine}; // Shortcut

  // Randomly pick the number of sides
  std::uniform_int_distribution randomSides(3, 3);
  spike.m_polygonSides = randomSides(re);

  // Get a random color (actually, a grayscale)
  std::uniform_real_distribution randomIntensity(0.5f, 1.0f);
  spike.m_color = glm::vec4(randomIntensity(re));

  spike.m_color.a = 1.0f;
  spike.m_rotation = 1.6f;
  spike.m_scale = scale;
  spike.m_translation = translation;

  // Get a random angular velocity
  spike.m_angularVelocity = m_randomDist(re);

  // Get a random direction
  glm::vec2 const direction{m_randomDist(re), m_randomDist(re)};
  // spike.m_velocity = glm::normalize(direction) / 7.0f;

  // Create geometry data
  std::vector<glm::vec2> positions{{0, 0}};
  auto const step{M_PI * 2 / spike.m_polygonSides};
  std::uniform_real_distribution randomRadius(0.8f, 1.0f);
  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    auto const radius{randomRadius(re)};
    positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO
  abcg::glGenBuffers(1, &spike.m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, spike.m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &spike.m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(spike.m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, spike.m_VBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return spike;
}