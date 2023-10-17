#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "Card.hpp"
#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  static int const m_N{4}; // Board size is m_N x m_N
  int m_matches; // current matches made

  enum class GameState { Play, Win, Wait };
  GameState m_gameState;

  bool m_noneRevealed{true};
  int current_card_offset;
  int last_card_offset;
  std::vector<Card> m_board{}; // Game board

  ImFont *m_font{};

  bool checkEndCondition();
  void restartGame();
};

#endif