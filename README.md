<h1 align="center">Diego Sousa Santos - 11044616</h1>

<p align="center">
  <img  src="https://user-images.githubusercontent.com/55899445/194708500-d6d7288c-7e47-44f4-b2e0-af67640d99d9.png" width="150" height="150"/>
</p>

# ABCg

![linux workflow](https://github.com/hbatagelo/abcg/actions/workflows/linux.yml/badge.svg)
![macOS workflow](https://github.com/hbatagelo/abcg/actions/workflows/macos.yml/badge.svg)
![Windows workflow](https://github.com/hbatagelo/abcg/actions/workflows/windows.yml/badge.svg)
![WASM workflow](https://github.com/hbatagelo/abcg/actions/workflows/wasm.yml/badge.svg)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/hbatagelo/abcg)](https://github.com/hbatagelo/abcg/releases/latest)

Development framework accompanying the course [MCTA008-17 Computer Graphics](http://professor.ufabc.edu.br/~harlen.batagelo/cg/) at [UFABC](https://www.ufabc.edu.br/).

[Documentation](https://hbatagelo.github.io/abcg/abcg/doc/html/) \| [Release notes](CHANGELOG.md) 

ABCg is a lightweight C++ framework that simplifies the development of 3D graphics applications based on [OpenGL](https://www.opengl.org), [OpenGL ES](https://www.khronos.org), [WebGL](https://www.khronos.org/webgl/), and [Vulkan](https://www.vulkan.org). It is designed for the tutorials and assignments of the course "MCTA008-17 Computer Graphics" taught at Federal University of ABC (UFABC).

* * *

### Atividade 01 - SimpleCalculator
<details>
  <summary>EXPANDIR PARA VISUALIZAR</summary>

<p align="center">
    <img width="456" alt="simpleCalculatorIMG" src="https://user-images.githubusercontent.com/55899445/194708704-9e639cb6-2a07-48f9-84ef-48868aa4f470.PNG">
</p>

-   Link GitHub Pages: [SimpleCalculator](https://diego-ss.github.io/cg-2022.3-UFABC/simpleCalculator/)

### Descrição geral

-   O propósito da aplicação é simular o funcionamento de uma calculadora simples (com as operações básicas) com as técnicas apresentadas até então durante as aulas da disciplina, visando atender os requisitos propostos na atividade 1.
-   Para isso, existe uma tela com dois inputs que representam os números envolvidos na operação, com um combobox indicando as operações possíveis, dois botões para calcular e limpar os parâmetros e um label para exibir o resultado.
-   Os operadores e resultado são armazenados em variáveis da classe Window.

### Detalhes da implementação

#### Assets
-   Como assets auxiliares, foi utilizada a fonte Inconsolata-Medium como no projeto TicTacToe.
#### main.cpp
-   No arquivo main foi utilizada a implementação padrão que vimos em aula.
#### window.hpp
-   Para a definição da classe Window, foram sobrescritos dois métodos da classe OpenGLWindow (da qual Window tem herança): onCreate e onPaintUI.
```cpp
protected:
  void onCreate() override;
  void onPaintUI() override;
```
-   Além disso, foram definidas as seguintes variáveis e métodos auxiliares para o processamento:
```cpp
private:
  float operator1{0.0f}; // REFERE-SE AO VALOR NUMÉRICO DO PRIMEIRO OPERADOR
  float operator2{0.0f}; // REFERE-SE AO VALOR NUMÉRICO DO SEGUNDO OPERADOR
  float result{0.0f}; // ARMAZENA O RESULTADO DA OPERAÇÃO MATEMÁTICA
  std::string calcOperator{"+"}; // INDICADOR DE QUAL OPERAÇÃO SERÁ REALIZADA

  ImFont *m_font{}; // FONTE DE TEXTO

  void calculate(); // MÉTODO AUXILIAR PARA REALIZAR O CÁLCULO COM BASE NOS PARÂMETROS INFORMADOS
  void clear(); // MÉTODO AUXILIAR PARA LIMPAR OS DADOS DA CALCULADORA
```
#### window.cpp
-   Neste arquivo foram implementados os métodos sobrescritos e os novos definidos na classe Window, do arquivo window.hpp.
-   A ideia da composição da janela foi considerar uma tabela com seis linhas, todas com uma coluna apenas: 
    -  a primeira contém um input para que o usuário digite o primeiro número da operação.
    -  a segunda contém um combobox com as operações disponíveis. 
    -  a terceira contém um input para que o usuário digite o segunda número da operação.
    -  a quarta contém um botão que confirma a realização da operação.
    -  a quinta contém uma label que exibe o resultado da operação matemática.
    -  a sexta contém um botão responsável por limpar os operadores e o resultado.
-   No método onCreate é ralizado o import da fonte de texto e zerados os operadores.
```cpp
void Window::onCreate() {
  // Load font with bigger size for the X's and O's
  auto const filename{abcg::Application::getAssetsPath() +
                      "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 72.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }

  clear();
}
```
-   No método clear, é realizada a limpeza dos operadores e do resultado.
```cpp
void Window::clear() {
  operator1 = 0.0f;
  operator2 = 0.0f;
  result = 0.0f;
}
```
-   No método calculate, é realizada a operação com base nos parâmetros informados pelo usuário.
```cpp
void Window::calculate() {
  // verificar o operador
  if (calcOperator == "+")
    result = operator1 + operator2;
  else if (calcOperator == "-")
    result = operator1 - operator2;
  else if (calcOperator == "*")
    result = operator1 * operator2;
  else if (calcOperator == "/")
    result = operator1 / operator2;
}
```
-   No método onPaintUI é onde está sendo realizada toda definição dos elementos de UI da tela da calculadora.
-   Para isso, dentro dele foram definidas as seguintes variáveis:
```cpp
      auto const buttonHeight{70}; // altura fixa dos botões
      static std::vector comboItems{"+", "-", "*", "/"}; // vetor de strings com as operações possíveis
      static std::size_t currentIndex{}; // índice da operação selecionada pelo usuário
```
-   Após isso é iniciada a tabela e definida cada uma de suas linhas. 
```cpp
        // primeira linha da tabela
        ImGui::TableNextRow();
        {
          // primeiro número da operação
          ImGui::TableSetColumnIndex(0);
          // ajustando largura do input
          ImGui::PushItemWidth(appWindowWidth);
          // input de float
          ImGui::InputFloat("op1", &operator1);
        }
```
```cpp
        // segunda linha da tabela
        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          // combo do operador
          if (ImGui::BeginCombo("Combo Operador",
                                comboItems.at(currentIndex))) {
            for (auto index{0U}; index < comboItems.size(); ++index) {
              bool const isSelected{currentIndex == index};
              if (ImGui::Selectable(comboItems.at(index), isSelected)) {
                currentIndex = index;
                calcOperator = comboItems.at(currentIndex); // SETANDO A VARIÁVEL DO OPERADOR QUANDO ALGUM ITEM É SELECIONADO
              }

              if (isSelected)
                ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
          }
        }
```
```cpp
        // terceira linha da tabela
        ImGui::TableNextRow();
        {
          // segundo número da operação
          ImGui::TableSetColumnIndex(0);
          // ajustando largura do input
          ImGui::PushItemWidth(appWindowWidth);
          // input de float
          ImGui::InputFloat("op2", &operator2);
        }
```
```cpp
        // quarta linha da tabela
        ImGui::TableNextRow();
        {
          // botão de realizar operação
          ImGui::TableSetColumnIndex(0);
          if (ImGui::Button("=", ImVec2(-1, buttonHeight))) {
            calculate(); // REALIZANDO O CÁLCULO QUANDO O BOTÃO É ACIONADO
          }
        }
```
```cpp
      // quinta linha da tabela
        ImGui::TableNextRow();
        {
          // resultado da operação
          ImGui::TableSetColumnIndex(0);
          ImGui::SetCursorPosX(
              (appWindowWidth -
               ImGui::CalcTextSize(std::to_string(result).c_str()).x) /
              2);
          ImGui::Text("%s", std::to_string(result).c_str()); // EXIBINDO O RESULTADO NA LABEL
        }
```
```cpp
        // sexta linha da tabela
        ImGui::TableNextRow();
        {
          // botão de limpar números
          ImGui::TableSetColumnIndex(0);
          if (ImGui::Button("clear", ImVec2(-1, buttonHeight))) {
            clear(); // LIMPANDO PARÂMETROS E RESULTADO
          }
        }
```
-   Para compilação web, foi utilizado o arquivo html do tictactoe adapatado para esta aplicação.

### Resultado Final
<p align="center">
    <img src="https://user-images.githubusercontent.com/55899445/194710962-51332d27-a383-4e27-aa58-99d1d52cf3b8.gif"/>
</p>


* * *
</details>

### Atividade 02 - Roll a Ball
<details>
  <summary>EXPANDIR PARA VISUALIZAR</summary>

<p align="center">
    <img width="456" alt="rollaballImage" src="https://user-images.githubusercontent.com/55899445/198695644-ec553618-34cb-4b7b-8207-4572cc3d21d8.png">
</p>

-   Link GitHub Pages: [RollABall](https://diego-ss.github.io/cg-2022.3-UFABC/rollaball/)

### Descrição geral

-   O propósito da aplicação é ser um jogo 2D cujo desenvolvimento e funcionalidades utilizem dos conceitos vistos em aula relacionados à gráficos 2D com primitivas do OpenGL, utilizando a biblioteca ABCg e renderização dos gráficos usando shaders.
-   As primitivas utilizadas resumem-se em triângulos, utilizados de forma singular para formar os espinhos e em conjunto para formar a bola.
-   O objetivo do jogo é conseguir o máximo de pontos, desviando dos espinhos ao decorrer do tempo. 
-   A bola se movimenta sozinha, e é possível utilizar a <b>seta para cima</b> para pular e a <b>barra de espaço</b> para acelerar.
-   Se a bola tocar em um dos espinhos, a partida termina e o jogo é reiniciado.

### Detalhes da implementação

#### Assets
-   Como assets auxiliares, foi utilizada a fonte Inconsolata-Medium como no projeto TicTacToe e no SimpleCalculator.
###  objects.vert
-   Shader utilizado na renderização da bola e dos espinhos. Os vértices possuem o atributo inPosition que represente a posição x,y do vértice.
```cpp
#version 300 es

layout(location = 0) in vec2 inPosition;

uniform vec4 color; // COR RGBA UNIFORME
uniform float rotation; // ROTAÇÃO UNIFORME
uniform float scale; // ESCALA UNIFORME
uniform vec2 translation; // TRANSLAÇÃO UNIFORME

out vec4 fragColor;

void main() {
  float sinAngle = sin(rotation);
  float cosAngle = cos(rotation);
  vec2 rotated = vec2(inPosition.x * cosAngle - inPosition.y * sinAngle,
                      inPosition.x * sinAngle + inPosition.y * cosAngle);

  vec2 newPosition = rotated * scale + translation; // NOVA POSIÇÃO ROTATED
  gl_Position = vec4(newPosition, 0, 1);
  fragColor = color;
}
```
###  objects.frag
-   Fragment shader responsável pelo processamento de pixels na GPU. Foi definido como abaixo, apenas redirecionando as cores de saída.
```cpp
#version 300 es

precision mediump float;

in vec4 fragColor;

out vec4 outColor;

void main() { outColor = fragColor; }
```

#### main.cpp
-   No arquivo main foi utilizada a implementação padrão que vimos em aula, removendo os  FPS, botão de tela cheia e utilizando 4 samples.
#### gamedata.hpp
-   Classe responsável por armazenar status e informações relevantes para as regras e funcionalidades do jogo.
-   Para sua definição, foram definidas as seguintes propriedades:
```cpp
  enum class Input { Up, Space }; // definição de enum para classificar os inputs possíveis
  enum class State { Playing, GameOver }; // definição de enum para classificar os status de jogo possíveis
  
  // struct para armazenar os dados do jogo
  struct GameData {
    State m_state{State::Playing}; // ESTADO DO JOGO
    std::bitset<2> m_input;        // ARRAAY DOS INPUTS
    int m_score;                   // PONTUAÇÃO
  };
```
#### window.hpp
-   Para a definição da classe Window, foram sobrescritos sete métodos da classe OpenGLWindow (da qual Window tem herança):
```cpp
  void onEvent(SDL_Event const &event) override; // PARA CAPTURAR EVENTOS DO TECLADO
  void onCreate() override; // AÇÕES AO CRIAR A APLICAÇÃO
  void onUpdate() override; // AÇÕES AO ATUALIZAR A JANELA
  void onPaint() override; // AÇÕES AO RENDERIZAR A JANELA
  void onPaintUI() override; // AÇÕES DE UI
  void onResize(glm::ivec2 const &size) override; // AÇÕES AO REDIMENSIONAR A JANELA
  void onDestroy() override; // AÇÕES AO DESTRUIR A JANELA
```
-   Além disso, foram definidas as seguintes variáveis e métodos auxiliares para o processamento:
```cpp
  glm::ivec2 m_viewportSize{}; // DIMENSÕES DA JANELA
  GLuint m_objectsProgram{}; // OBJECTS PROGRAM
  GameData m_gameData; // GAME DATA REF
  Ball m_ball; // BALL REF
  Spikes m_spikes; // SPIKES REF
  abcg::Timer m_restartWaitTimer; // TEMPORIZADOR
  ImFont *m_font{}; // FONTE
  std::default_random_engine m_randomEngine; // RANDOMIZADOR

  void restart(); // PARA REINICIAR AS CONDIÇÕES DE JOGO
  void checkCollisions(); // PARA VERIFICAR COLISÕES
```
#### window.cpp
-   Neste arquivo foram implementados os métodos sobrescritos e os novos definidos na classe Window, do arquivo window.hpp.
-   A ideia da composição da janela foi considerar o jogo e seus elementos renderizando e também dois elementos de UI: 
    -  um texto para contabilizar o Score.
    -  um texto que é exibido quando o jogo é finalizado. 
-   Os eventos capiturados são para as teclas SPACE, UP, W e para os botões do mouse.

- No método <b>OnEvent</b> são capturadas as interações do usuário com a interface:
```cpp
// Eventos do teclado
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE) // Capturando DOWN da barra de espaço
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Space)); // Setando o vetor de inputs do GameData
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) // capturando DOWN do w e seta 
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Up)); // Setando o vetor de inputs do GameData
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE) // Capturando UP barra de espaço
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Space)); // Setando o vetor de inputs do GameData
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) // capturando DOWN do w e seta 
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Up)); // Setando o vetor de inputs do GameData
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
```
- No método <b>OnCreate</b> são realizadas as operações necessárias ao inicializar o ambiente, como inicialização de variáveis e carregamento da fonte externa:
```cpp
// Carregando fonte
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  // Criando programa para renderizar objetos
  m_objectsProgram =
      abcg::createOpenGLProgram(//Suprimido pois o README não suporta);

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
```
- No método <b>restart</b>, são reinicializadas as variáveis e os objetos renderizados na tela.
```cpp
  // estado inicial do jogo
  m_gameData.m_state = State::Playing;
  m_gameData.m_score = 0;

  // criando bola e espinhos
  m_ball.create(m_objectsProgram);
  m_spikes.create(m_objectsProgram, 4);
```
- No método <b>onUpdate</b> é onde são atualizadas constantemente as formas renderizadas, a interação entre elas e os elementos de UI.
```cpp
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
```
 - No método <b>onPaint</b> são desenhados o viewport e os objetos da aplicação. 
```cpp
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // método paint dos objetos
  m_ball.paint(m_gameData);
  m_spikes.paint();
```
  - No método <b>onPaintUI</b> são desenhados os elementos de UI da cena.
```cpp
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
```
  - No método <b>checkCollisions</b> são verificadas colisões entre os espinhos e a bola. Caso haja colisão, o jogo acaba.
```cpp
 // Verificando colisão entre a bola e os espinhos
  for (auto const &spike : m_spikes.m_spikes) {
    auto const spikeTranslation{spike.m_translation};
    auto const distance{glm::distance(m_ball.m_translation, spikeTranslation)};

    if (distance < m_ball.m_scale * 0.9f + spike.m_scale * 0.17f) {
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
    }
  }
```
                                 
#### ball.hpp
- Para a definição da classe ball, foram utilizadas as seguintes propriedades e métodos:
```cpp
  void create(GLuint program);
  void paint(GameData const &gameData);
  void destroy();
  void update(GameData const &gameData, float deltaTime);

  glm::vec4 m_color{1};             // COR
  float m_rotation{};               // ROTAÇÃO
  float m_scale{0.125f};            // ESCALA
  glm::vec2 m_translation{};        // TRANSLAÇÃO
  glm::vec2 m_velocity{1.0f, 0.0f}; // VELOCIDADE

  bool isJumping{false};       // INDICADOR DE PULO
  bool isFallingDown{false};   // INDICADOR DE QUEDA
  bool isBoosted{false};       // INDICADOR DE BOOST
  float m_velocityBoost{3.0f}; // BOOST DE VELOCIDADE

  float const m_maxTranslationY{0.6f}; // LIMITE SUPERIOR DO PULO
  float const m_minTranslationY{0.0f}; // CHÃO
  float const m_jumpForce{1.0f};       // FORÇA DE PULO
  float const m_fallForce{-1.1f};      // FORÇA DE QUEDA (CAI MAIS RÁPIDO)
                                 
  GLuint m_program{};
  GLint m_translationLoc{}; // VARIÁVEIS UNIFORMES DO VERTEX SHADER
  GLint m_colorLoc{}; // VARIÁVEIS UNIFORMES DO VERTEX SHADER
  GLint m_scaleLoc{}; // VARIÁVEIS UNIFORMES DO VERTEX SHADER
  GLint m_rotationLoc{}; // VARIÁVEIS UNIFORMES DO VERTEX SHADER

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
```
#### ball.cpp
- No método <b>create</b> são inicializadas as variáveis uniformes, as propriedades, a geometria e são criados o VAO, VBO e EBO do objeto.
```cpp
  destroy();

  m_program = program;

  // LOcalização dos uniformes do programa
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Resetando bola
  m_rotation = 0.0f;
  m_translation = glm::vec2(-0.7f, 0.0f);

  // Localização dos atributos no programa
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Criando a geometria
  auto const sides{10};
  std::vector<glm::vec2> positions{{0, 0}};
  auto const step{M_PI * 2 / sides};
  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // VBO das posições
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Criando VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind dos vertices ao VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindVertexArray(0);
```
- No método <b>paint</b> são realizados os binds do vertex shader e a definição da variáveis uniformes do objeto, caso o jogo esteja ativo.
```cpp
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
```
- No método <b>update</b> são realizadas as atualizações de estado do objeto, como rotação, boost de velocidade, pulo e queda da bola.
```cpp
  / Rotação constante da bola
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
```
- No método <b>destroy</b> são destruídos o VBO, EBO e VAO do objeto
```cpp
  // destruindo elementos
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
```
#### spikes.hpp
- Para a definição da classe spikes, foram utilizadas as seguintes propriedades e métodos:
```cpp
public:
  void create(GLuint program, int quantity);
  void paint();
  void destroy();
  void update(const Ball &ball, GameData &gameData, float deltaTime);
  
  // estrutura de um único spike
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
  GLint m_colorLoc{}; // VARIÁVEIS UNIFORMES DO VERTEX SHADER
  GLint m_rotationLoc{}; // VARIÁVEIS UNIFORMES DO VERTEX SHADER
  GLint m_translationLoc{}; // VARIÁVEIS UNIFORMES DO VERTEX SHADER
  GLint m_scaleLoc{}; // VARIÁVEIS UNIFORMES DO VERTEX SHADER
  
  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};
```
#### spikes.cpp
- No método <b>create</b> são inicializadas as variáveis uniformes, objetos spike com suas geometrias, VBO e VAO.
```cpp
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

  float i{0.0f}; // ACUMULADOR PARA SEPARAR OS ESPINHOS NO COMEÇO
  maxQtdTopSpikes = (quantity / 2) - 1; // LIMITANDO ESPINHOS NO TOPO
  qtdTopSpikes = 0;                     // CONTANDO ESPINHOS NO TOPO

  for (auto &spike : m_spikes) {
    spike = makeSpike(); // CRIANDO ESPINHO (geometria, VAO, VBO e bind dos vértices)
    // iniciando o espinho no canto direito e um pouco abaixo
    spike.m_translation = {1.0f + i, -0.05f};
    randomizeTopSpikes(spike); // MÉTODO PARA RANDOMIZAR QUAL OU QUAIS SPIKE FICARÁ EM CIMA
    i += 0.2f;
  }  
```
- No método <b>paint</b> são realizados os binds dos VAOs dos objetos spike e de suas variáveis uniformes, bem como realizado o draw dos pontos da geometria.
```cpp
abcg::glUseProgram(m_program);

  for (auto const &spike : m_spikes) {
    abcg::glBindVertexArray(spike.m_VAO); // BIND DOS ATRIBUTOS DO VERTEX AO VAO
    abcg::glUniform4fv(m_colorLoc, 1, &spike.m_color.r); // VARIÁVEL UNIFORME
    abcg::glUniform1f(m_scaleLoc, spike.m_scale);        // VARIÁVEL UNIFORME
    abcg::glUniform1f(m_rotationLoc, spike.m_rotation);  // VARIÁVEL UNIFORME
    abcg::glUniform2f(m_translationLoc, spike.m_translation.x,
                      spike.m_translation.y); // VARIÁVEL UNIFORME
    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0,
                       spike.m_polygonSides + 2); // DESENHANDO OS PONTOS
    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);  
```
- No método <b>update</b> são realizadas as atualizações de translação do objeto. Quando o spike sai da tela, é adicionado 1 no score.
```cpp
for (auto &spike : m_spikes) {
    // condicionando a translação à velocidade da bola
    spike.m_translation.x -= ball.m_velocity.x * deltaTime;

    // reposicionando quando chega no fim da tela
    if (spike.m_translation.x < -1.0f) {
      // calculando uma distância randômica do ponto de origem para separar os
      // espinhos
      std::uniform_real_distribution<float> randomDists(0.0f, 0.9f);
      auto &re{m_randomEngine};
      float randomDist = randomDists(re) + 2.0f;
      spike.m_translation.x += randomDist;

      // aumentando Score
      if (gameData.m_state == State::Playing)
        gameData.m_score += 1;
    }
  }  
```
-   Para compilação web, foi utilizado o arquivo html da simpleCalculator adapatado para esta aplicação.

### Resultado Final
<p align="center">
    <img alt="gif roll a ball" src="https://user-images.githubusercontent.com/55899445/198696680-c425a09f-5b33-4359-84a7-f06c1d6af755.gif"/>
</p>


* * *
</details>

### Atividade 03 - Spiral Effect
<details>
  <summary>EXPANDIR PARA VISUALIZAR</summary>

<p align="center">
    <img width="456" alt="SpiralEffectScreenshot" src="https://user-images.githubusercontent.com/55899445/202917321-6f1ab8a8-37ed-4da2-9caa-589d830e81cd.PNG">
</p>

-   Link GitHub Pages: [SpiralEffect](https://diego-ss.github.io/cg-2022.3-UFABC/spiraleffect/)

### Descrição geral

-   O propósito do projeto é ser uma aplicação 3D cujo desenvolvimento e funcionalidades utilizem dos conceitos vistos em aula relacionados à matrizes e transformações geométricas e formas primitivas do openGL, utilizando a biblioteca ABCg e renderização dos gráficos usando shaders.
-   As primitivas utilizadas resumem-se em triângulos, utilizados de forma singular para formar os espinhos e em conjunto para formar os cubos.
-   A aplicação simula a geração de um pequeno espiral e a movimentação da câmera em sincronia com a trilha gerada, o que resulta em uma movimentação similar â uma montanha russa, por exemplo.
-   <b>OBSERVAÇÃO:</b> existe um bug em que em algum momento o espiral cria algumas descontinuidades entre os cubos. Está sendo investigado o motivo disso.

### Detalhes da implementação

#### Assets
-   Como assets auxiliares, foi utilzado o objeto vetoriado box.obj, para gerar os cubos da aplicação.
###  depth.vert
-   Shader utilizado na renderização dos cubos que compoem o espiral. A lógica relacionada é que a coloração dos cubos ficará mais branca de forma proporcional à altura e à proximidade do objeto à câmera, simulando uma espécie de iluminação nos objetos.
```cpp
#version 300 es

layout(location = 0) in vec3 inPosition;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  // o tom da cor varia de acordo com a distância e a altura 
  float i = 1.0 - (-posEyeSpace.z / 50.0) + (posEyeSpace.y / 50.0);
  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projMatrix * posEyeSpace;
}
```
###  depth.frag
-   Fragment shader responsável pelo processamento de pixels na GPU. Foi definido como abaixo, apenas redirecionando as cores de saída.
```cpp
#version 300 es

precision mediump float;

in vec4 fragColor;
out vec4 outColor;

void main() { outColor = fragColor; }
```
#### main.cpp
-   No arquivo main foi utilizada a implementação padrão que vimos em aula, removendo os  FPS, botão de tela cheia e utilizando 4 samples.

#### window.hpp
-   Para a definição da classe Window, foram sobrescritos seis métodos da classe OpenGLWindow (da qual Window tem herança):
```cpp
  void onCreate() override; // AÇÕES AO CRIAR A APLICAÇÃO
  void onUpdate() override; // AÇÕES AO ATUALIZAR A JANELA
  void onPaint() override; // AÇÕES AO RENDERIZAR A JANELA
  void onPaintUI() override; // AÇÕES DE UI
  void onResize(glm::ivec2 const &size) override; // AÇÕES AO REDIMENSIONAR A JANELA
  void onDestroy() override; // AÇÕES AO DESTRUIR A JANELA
```
-   Além disso, foram definidas as seguintes variáveis e métodos auxiliares para o processamento:
```cpp
 // cubos usados para gerar o espiral
  struct Cube {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
  };

  // array de cubos
  std::array<Cube, 800> m_objects;

  float m_angle{};

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  float m_FOV{30.0f};

  float m_spiralRadius{3.0f}; // RAIO DO ESPIRAL
  float m_spiralStep{m_spiralRadius/m_objects.size()};      // PASSO DO ESPIRAL COM BASE NA QTD DE CUBOS
  float m_TwoPI{M_PI * 2.0f}; // 2 * PI
  GLuint m_program{};

  void generateSpiral();
  void orientCamera(glm::vec3 const at);
```

#### window.cpp
-   Neste arquivo foram implementados os métodos sobrescritos e os novos definidos na classe Window, do arquivo window.hpp.
-   A ideia da composição da janela foi considerar apenas a janela da aplicação e uma janela pequena para selecionar o modo de projeção.
-   Para a projeção ortogonal, podemos visualizar com clareza a formação do espiral. Para o modo de projeção em perspectiva, temos a visualização do efeito de montanha russa com a câmera look at.

- No método <b>OnCreate</b> são realizadas as operações necessárias ao inicializar o ambiente, como inicialização de variáveis e carregamento dos shaders:
```cpp
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);
  
  // caracteres { duplos suprimidos por erro de build
  m_program =
      abcg::createOpenGLProgram(.source = assetsPath + "depth.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "depth.frag",
                                  .stage = abcg::ShaderStage::Fragment);

  m_model.loadObj(assetsPath + "box.obj");
  m_model.setupVAO(m_program);

  // orientando câmera para olhar para z = -1.0f
  orientCamera({0.0f, 0.0f, -1.0f});
  generateSpiral();
}
```
- No método <b>orientCamera</b>, a câmera é instruida a olhar para um determino ponto <b>at</b>.
```cpp
// a câmera é fixa na posição 0,0,0 e a direção
  // em que ela olha varia de acordo com o vetor at
  // enviado como parâmetro
  glm::vec3 const eye{0.0f, 0.0f, 0.0f};
  glm::vec3 const up{0.0f, 1.0f, 0.0f};
  m_viewMatrix = glm::lookAt(eye, at, up);
```
- No método <b>generateSpiral</b>, o espiral é criado com base nas variáveis declaradas.
```cpp
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
```

 - No método <b>onUpdate</b> a posição dos cubos é atualizada com base em deltatime, assim como o foco da câmera com base nos cubos. 
```cpp
 // incrementando angulo em 90 graus por segundo
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f) * deltaTime);

  int index{0};
  // atualizando objetos
  for (auto &objRef : m_objects) {
    // incrementando z em 15 unidades por segundo
    objRef.m_position.z += deltaTime * 15.0f;

    // resetando posição da estrela quando chega em z = -20
    if (objRef.m_position.z > -20.0f) {
      // orientando câmera para olhar para os cubos mais próximos
      // valores empíricos
      orientCamera(
          {objRef.m_position.x / 80.0f, objRef.m_position.y / 80.0f, -1.0f});

      // voltando para o fundo da tela
      objRef.m_position.z = -100.0f;
    }
    index++;
  }
```
 - No método <b>onPaint</b> são desenhados o viewport e os objetos da aplicação. 
```cpp
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
```
  - No método <b>onPaintUI</b> são desenhados os elementos de UI da cena.
```cpp
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
      // projeção em perspectiva
      if (currentIndex == 0) {
        m_projMatrix =
            glm::perspective(glm::radians(m_FOV), aspect, 0.01f, 100.0f);

        ImGui::SliderFloat("FOV", &m_FOV, 5.0f, 179.0f, "%.0f degrees");
      } else {
      // projeção ortogonal
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
```
#### model.cpp e model.hpp
-   Estes arquivos seguem a mesma estrutura utilizada no projeto starfield, sendo utilizados para renderizar diferentes objetos de forma dinâmica.


### Resultado Final
<p align="center">
    <img alt="gif spiral effect" src="https://user-images.githubusercontent.com/55899445/202920443-ca3d11d5-d9c5-4e99-8041-49b2a468fd32.gif"/>
</p>

* * *
</details>

### Atividade 04 - RollerCoaster
<details>
  <summary>EXPANDIR PARA VISUALIZAR</summary>

<p align="center">
     
</p>

-   Link GitHub Pages: [RollerCoaster](https://diego-ss.github.io/cg-2022.3-UFABC/rollercoaster/)

### Descrição geral

-   O propósito do projeto é ser uma aplicação 3D cujo desenvolvimento e funcionalidades utilizem dos conceitos vistos em aula relacionados à matrizes e transformações geométricas e formas primitivas do openGL, utilizando a biblioteca ABCg e renderização dos gráficos usando shaders.
-   As primitivas utilizadas resumem-se em triângulos, utilizados de forma singular para formar os triângulos e em conjunto para formar os cubos.
-   A aplicação simula a geração de trilhos em movimento que podem ser controlados em diferentes direções. Quando o trilho é direcionado para cima, a velocidade diminui e, para baixo, aumenta. Há também a utilização do shaders para criação da "neblina" e simulação do aumento de temperatura dos trilhos por conta do atrito, através da cor vermelha.

### Detalhes da implementação

#### Assets
-   Como assets auxiliares, foi utilzado o objeto vetoriado box.obj, para gerar os cubos da aplicação.
###  depth.vert
-   Shader utilizado na renderização dos cubos que compoem os trilhos. A lógica relacionada é que a coloração dos cubos ficará mais branca de forma proporcional à altura e à proximidade do objeto à câmera, simulando uma espécie de iluminação nos objetos.
```cpp
#version 300 es

layout(location = 0) in vec3 inPosition;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  // o tom da cor varia de acordo com a distância e a altura 
  float i = 1.0 - (-posEyeSpace.z / 50.0) + (posEyeSpace.y / 50.0);
  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projMatrix * posEyeSpace;
}
```
###  depth.frag
-   Fragment shader responsável pelo processamento de pixels na GPU. Foi definido como abaixo, apenas redirecionando as cores de saída.
```cpp
#version 300 es

precision mediump float;

in vec4 fragColor;
out vec4 outColor;

void main() { outColor = fragColor; }
```
#### main.cpp
-   No arquivo main foi utilizada a implementação padrão que vimos em aula, removendo os  FPS, botão de tela cheia e utilizando 4 samples.

#### window.hpp
-   Para a definição da classe Window, foram sobrescritos sete métodos da classe OpenGLWindow (da qual Window tem herança):
```cpp
  void onCreate() override; // AÇÕES AO CRIAR A APLICAÇÃO
  void onUpdate() override; // AÇÕES AO ATUALIZAR A JANELA
  void onPaint() override; // AÇÕES AO RENDERIZAR A JANELA
  void onPaintUI() override; // AÇÕES DE UI
  void onResize(glm::ivec2 const &size) override; // AÇÕES AO REDIMENSIONAR A JANELA
  void onDestroy() override; // AÇÕES AO DESTRUIR A JANELA
  void onEvent(SDL_Event const &event) override; // CAPTURANDO EVENTOS NA APLICAÇÃO
```
-   Além disso, foram definidas as seguintes variáveis e métodos auxiliares para o processamento:
```cpp
 // cubos usados para gerar o espiral
  struct Block {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
    float m_inital_x{};
  };

  // array de blocos
  std::array<Block, 240> m_blocks;

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  GLint m_colorLocation{};

  // ângulo de abertura vertical do campo de visão da projeção perspectiva
  float m_FOV{30.0f};
  // suavização das curvas
  float m_lerp_ref{0.0f};
  // velocidade
  float m_velocity;

  GLuint m_program{};

  void changeXDirection(Block &block, float deltaTime, float orientation);
  void changeYDirection(Block &block, float deltaTime, float orientation);
```

#### window.cpp
-   Neste arquivo foram implementados os métodos sobrescritos e os novos definidos na classe Window, do arquivo window.hpp.
-   A ideia da composição da janela foi considerar apenas a janela da aplicação e uma janela pequena para selecionar o modo de projeção.

- No método <b>OnCreate</b> são realizadas as operações necessárias ao inicializar o ambiente, como inicialização de variáveis e carregamento dos shaders:
```cpp
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
```
- No método <b>changeYDirection</b>, os cubos do trilho são orientados a formar semi-circulos para cima ou para baixo, dependendo do input do usuário. Isso para simular subidas e descidas.
```cpp
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
```
- No método <b>changeXDirection</b>, os cubos do trilho são orientados a formar semi-circulos para esquerda ou para direita, dependendo do input do usuário. Isso para simular curvas.
```cpp
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
```

 - No método <b>onUpdate</b> a posição dos cubos é atualizada com base em deltatime. Aqui também são detectados e tratados os inputs do usuário. 
```cpp
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
```
 - No método <b>onPaint</b> são desenhados o viewport e os objetos da aplicação. 
```cpp
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
```
  - No método <b>onPaintUI</b> são desenhados os elementos de UI da cena, bem como definida a projeção como perspectiva.
```cpp
abcg::OpenGLWindow::onPaintUI();

  auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                    gsl::narrow<float>(m_viewportSize.y)};
  m_projMatrix = glm::perspective(glm::radians(m_FOV), aspect, 0.01f, 100.0f);
```
#### model.cpp e model.hpp
-   Estes arquivos seguem a mesma estrutura utilizada no projeto starfield, sendo utilizados para renderizar diferentes objetos de forma dinâmica.

### Resultado Final
<p align="center">
    <!--GIF--/>
</p>

* * *
</details>

## License

ABCg is licensed under the MIT License. See [LICENSE](https://github.com/hbatagelo/abcg/blob/main/LICENSE) for more information.
