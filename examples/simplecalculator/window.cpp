#include "window.hpp"

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

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // calculator window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Calculator", nullptr, flags);

    // grid superior
    {
      auto const buttonHeight{70};
      static std::vector comboItems{"+", "-", "*", "/"};
      static std::size_t currentIndex{};

      // Usando a fonte customizada
      ImGui::PushFont(m_font);
      if (ImGui::BeginTable("Grid1", 1)) {

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

        ImGui::Spacing();

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
                calcOperator = comboItems.at(currentIndex);
              }

              if (isSelected)
                ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
          }
        }

        ImGui::Spacing();

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

        ImGui::Spacing();

        // quarta linha da tabela
        ImGui::TableNextRow();
        {
          // botão de realizar operação
          ImGui::TableSetColumnIndex(0);
          if (ImGui::Button("=", ImVec2(-1, buttonHeight))) {
            calculate();
          }
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        // quinta linha da tabela
        ImGui::TableNextRow();
        {
          // resultado da operação
          ImGui::TableSetColumnIndex(0);
          ImGui::SetCursorPosX(
              (appWindowWidth -
               ImGui::CalcTextSize(std::to_string(result).c_str()).x) /
              2);
          ImGui::Text("%s", std::to_string(result).c_str());
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        // sexta linha da tabela
        ImGui::TableNextRow();
        {
          // botão de limpar números
          ImGui::TableSetColumnIndex(0);
          if (ImGui::Button("clear", ImVec2(-1, buttonHeight))) {
            clear();
          }
        }

        ImGui::Spacing();

        ImGui::Spacing();
        ImGui::EndTable();
      }
      ImGui::PopFont();
    }

    ImGui::End();
  }
}

void Window::clear() {
  operator1 = 0.0f;
  operator2 = 0.0f;
  result = 0.0f;
}

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