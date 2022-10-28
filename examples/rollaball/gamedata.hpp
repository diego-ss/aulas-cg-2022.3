#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Up, Space };
enum class State { Playing, GameOver };

struct GameData {
  State m_state{State::Playing}; // ESTADO DO JOGO
  std::bitset<2> m_input;        // ARRAAY DOS INPUTS
  int m_score;                   // PONTUAÇÃO
};

#endif