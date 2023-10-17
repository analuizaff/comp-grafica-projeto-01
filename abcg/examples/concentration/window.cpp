#include "window.hpp"
#include "card.hpp"
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <thread>

void Window::restartGame() {
  // Create a vector to store unique card values (e.g., numbers, letters, symbols).
  std::vector<std::string> uniqueCardValues;

  // Populate the vector with unique card values.
  for (char c = 'A'; c <= 'Z'; ++c) {
      uniqueCardValues.push_back(std::string(1, c));  // Add uppercase letters as values.
  }

  // Shuffle the vector of card values to randomize their order.
  std::shuffle(uniqueCardValues.begin(), uniqueCardValues.end(), std::mt19937(std::random_device()()));

  // Clear the game board (if needed).
  m_board.clear();

  // Create pairs of cards with unique values and distribute them on the game board.
  for (size_t i = 0; i < m_N * m_N / 2; ++i) {
      const std::string value = uniqueCardValues[i];

      // Create two Card objects with the same value.
      Card card1(value);
      Card card2(value);

      // Add the Card objects to the game board.
      m_board.push_back(card1);
      m_board.push_back(card2);
  }

  // Shuffle the game board to randomize the card positions.
  std::shuffle(m_board.begin(), m_board.end(), std::mt19937(std::random_device()()));
  
  // Reset game variables
  m_matches = 0;
  m_noneRevealed = true;
  m_gameState = GameState::Play;
}

void Window::onCreate() {
  // Load font with bigger size for the Cards
  auto const filename{abcg::Application::getAssetsPath() +
                      "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 72.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }
  restartGame();
}

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "Memory Match" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Memory Match", nullptr, flags);

     {
      bool restartSelected{};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Game")) {
          ImGui::MenuItem("Restart", nullptr, &restartSelected);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (restartSelected) {
        restartGame();
      }
    }
    // Static text showing current turn or win/draw messages
    {
      std::string text;
      switch (m_gameState) {
      case GameState::Play:
        text = m_noneRevealed ? "Clique numa carta" : "Clique noutra carta";
        break;
      case GameState::Win:
        text = "Player wins!";
        break;
      case GameState::Wait:
        text = "Errou! Clique em qualquer carta para desvirar.";
        break;
      }
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
      ImGui::Text("%s", text.c_str());
      ImGui::Spacing();
    }

    ImGui::Spacing();
    // Game board
    {
      auto const gridHeight{appWindowHeight - 22 - 60 - (m_N * 10) - 60};
      auto const buttonHeight{gridHeight / m_N};

      std::string button_label;

      // Use custom font
      ImGui::PushFont(m_font);
      if (ImGui::BeginTable("Game board", m_N)) {
        for (auto i : iter::range(m_N)) {
          ImGui::TableNextRow();
          for (auto j : iter::range(m_N)) {
            ImGui::TableSetColumnIndex(j);
            auto const offset{i * m_N + j};

            // Replace null character with whitespace because the button label
            // cannot be an empty string
            if (m_board.at(offset).isFaceUp()) {
              button_label = m_board.at(offset).getValue();
            }
            else {
              button_label = " ";
            }

            // Button text is button_label followed by an ID in the format ##ij
            auto buttonText{fmt::format("{}##{}{}", button_label, i, j)};
            if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
              if (m_gameState == GameState::Play && button_label == " ") {
                
                // Flip
                m_board.at(offset).flip();

                if (m_noneRevealed) {
                  current_card_offset = offset;
                } else {
                  last_card_offset = current_card_offset;
                  current_card_offset = offset;
                  checkEndCondition();
                }
                m_noneRevealed = !m_noneRevealed;
              }
              else if (m_gameState == GameState::Wait) {
                m_board.at(current_card_offset).flip();
                m_board.at(last_card_offset).flip();
                m_gameState = GameState::Play;
              }
            }
          }
          ImGui::Spacing();
        }
        ImGui::EndTable();
      }
      ImGui::PopFont();
    }

    ImGui::Spacing();
    // "Restart game" button
    {
      if (ImGui::Button("Restart game", ImVec2(-1, 50))) {
        restartGame();
      }
    }

    ImGui::End();
  }
}

bool Window::checkEndCondition() {
  if (m_gameState != GameState::Play) {
    return false;
  }
  if (m_board.at(current_card_offset) == m_board.at(last_card_offset)) {
    m_matches += 1;
    if (m_matches == m_N*2) {
      m_gameState = GameState::Win;
      return true;
    }
  }
  else {
    m_gameState = GameState::Wait;
  }
  return false;
}