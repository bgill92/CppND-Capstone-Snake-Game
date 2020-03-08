#include <iostream>
#include <string>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "gameManager.h"


int main() {

  // Move all of these to gameManager  
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  int mode{-1};

  while (mode == -1) { // Collect input

    std::cout << "Single player or VS. mode? (0 - Exit, 1 - Single player, 2 - VS. mode)" << "\n";
    std::cin >> mode;

    if (!(std::cin.good())) {
      std::cin.clear();
      std::cin.ignore(256, '\n');
      mode = -1;
      std::cout << "Please try again." << "\n";
    } else {

      if ((mode == 0) || (mode == 1) || (mode == 2)) {

        break;

      } else {
        mode = -1;
        std::cout << "Please try again." << "\n";
      }      
    }

  }

  if (mode == 1) { // Single player mode

    // Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    // Controller controller;
    // Game game(kGridWidth, kGridHeight);
    // game.Run(controller, renderer, kMsPerFrame);    

    // Make a game manager
    auto gameMan = std::make_unique<GameManager>();
    std::cout << "1\n";

    // Run the game
    gameMan.get()->Run();

    std::cout << "Game has terminated successfully!\n";
    // std::cout << "Score: " << game.GetScore() << "\n";
    // std::cout << "Size: " << game.GetSize() << "\n";


    // std::cout << "Game has terminated successfully!\n";
    // std::cout << "Score: " << game.GetScore() << "\n";
    // std::cout << "Size: " << game.GetSize() << "\n";

  } else if (mode == 2) { // Vs. mode

    std::cout << "VS. mode not implemented yet, sorry!" << "\n";

  } else if (mode == 0) { // Exit

    std::cout << "Exiting..." << "\n";

  }

  return 0;
}