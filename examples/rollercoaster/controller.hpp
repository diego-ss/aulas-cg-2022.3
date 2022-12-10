#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <bitset>

enum class Input { Right, Left, Down, Up };

struct Controller {
  std::bitset<4> m_input; // [up, down, left, right]
};

#endif