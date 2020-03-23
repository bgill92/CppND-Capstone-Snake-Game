#include <iostream>
#include <string>
#include "controller.h"
#include "renderer.h"
#include "gameManager.h"
#include "game.h"

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

    std::string temp;

    std::cout << "Controls are:\n W = up \n S = down \n D = right \n A = left \n Enter any button to continue...";    
    getline(std::cin, temp);
    getline(std::cin, temp);

    // Make a game manager
    auto gameMan = std::make_unique<GameManager>();
    // std::cout << "1\n";

    // Run the game
    gameMan.get()->Run();

    std::cout << "Game has terminated successfully!\n";
    gameMan->ending();

  } else if (mode == 2) { // Vs. mode

    // std::cout << "VS. mode not implemented yet, sorry!" << "\n";

    std::string temp;

    std::cout << "Controls for player 1 are:\n W = up \n S = down \n D = right \n A = left \n";
    std::cout << "Controls for player 2 are:\n up arrow = up \n down arrow = down \n right arrow = right \n left arrow = left \n";
    std::cout << "Enter any button to continue...";    
    getline(std::cin, temp);
    getline(std::cin, temp);

    auto gameMan = std::make_unique<GameManager>(2);
    // std::cout << "1\n";

    // Run the game
    gameMan.get()->Run();

    std::cout << "Game has terminated successfully!\n";    
    gameMan->ending();

  } else if (mode == 0) { // Exit

    std::cout << "Exiting..." << "\n";

  }

  return 0;
}