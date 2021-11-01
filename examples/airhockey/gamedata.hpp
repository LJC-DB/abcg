#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Down, Up, D, A, S, W };
enum class State { Playing, WinP1, WinP2 };

struct GameData {
  State m_state{State::Playing};
  std::bitset<8> m_input;
  std::array<int, 2> m_score;
};

#endif
