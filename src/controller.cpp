#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

// void Controller::applyChangeDirection(Snake &snake, SDL_Keycode inputDir) {
//   switch (inputDir) {
//     case SDLK_UP:
//       ChangeDirection(snake, Snake::Direction::kUp,
//                       Snake::Direction::kDown);
//       std::cout<< "up\n";
//     case SDLK_DOWN:
//       ChangeDirection(snake, Snake::Direction::kDown,
//                       Snake::Direction::kUp);
//     case SDLK_LEFT:
//       ChangeDirection(snake, Snake::Direction::kLeft,
//                       Snake::Direction::kRight);
//     case SDLK_RIGHT:
//       ChangeDirection(snake, Snake::Direction::kRight,
//                       Snake::Direction::kLeft);
//     case SDLK_w:
//       ChangeDirection(snake, Snake::Direction::kUp,
//                       Snake::Direction::kDown);
//     case SDLK_s:
//       ChangeDirection(snake, Snake::Direction::kDown,
//                       Snake::Direction::kUp);
//     case SDLK_a:
//       ChangeDirection(snake, Snake::Direction::kLeft,
//                       Snake::Direction::kRight);
//     case SDLK_d:
//       ChangeDirection(snake, Snake::Direction::kRight,
//                       Snake::Direction::kLeft);
//     case SDLK_COMPUTER:
//       //Do Nothing          
//   }
// }

std::pair<int,SDL_Keycode> Controller::pollInput(){

  SDL_Event e;

  int screen = 0;
  // do nothing key
  SDL_Keycode pressedKey = this->getDoNothing();
  // SDLK_EJECT means set running flag to false

  while (SDL_PollEvent(&e)) {
    // std::cout << "Got in SDL_PollEvent \n";
    if (e.type == SDL_QUIT) {
      // running = false;
      pressedKey = SDLK_EJECT;
    } 
    else if (e.type == SDL_KEYDOWN) {  
        switch (e.key.keysym.sym) {
          case SDLK_UP:
              pressedKey = SDLK_UP;
              // std::cout << "up\n";
              screen = 1;
              break;
          case SDLK_DOWN:
              pressedKey = SDLK_DOWN;
              // std::cout << "down\n";
              screen = 1;
              break;
          case SDLK_LEFT:
              pressedKey = SDLK_LEFT;
              // std::cout << "left\n";
              screen = 1;
              break;
          case SDLK_RIGHT:
              pressedKey = SDLK_RIGHT;
              // std::cout << "right\n";
              screen = 1;
              break;
          case SDLK_w:
              pressedKey = SDLK_w;
              // std::cout << "up\n";
              screen = 0;
              break;
          case SDLK_s:
              pressedKey = SDLK_s;
              // std::cout << "down\n";
              screen = 0;
              break;
          case SDLK_a:
              pressedKey = SDLK_a;
              // std::cout << "left\n";
              screen = 0;
              break;
          case SDLK_d:
              pressedKey = SDLK_d;
              // std::cout << "right\n";
              screen = 0;
              break;
      }      
    }
  }
  return std::make_pair(screen,pressedKey);
}

void Controller::HandleInput(bool &running, Snake &snake, int instId) {

  std::unique_lock<std::mutex> lck(_mtx);

  SDL_Event e;

  std::cout << "Got in lock_guard \n";  

  while (SDL_PollEvent(&e)) {
    std::cout << "Got in SDL_PollEvent \n";
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
        if (_altControlFlag) {
          switch (e.key.keysym.sym) {
            case SDLK_UP:
              ChangeDirection(snake, Snake::Direction::kUp,
                              Snake::Direction::kDown);
              break;
              // std::cout<< "up\n";
            case SDLK_DOWN:
              ChangeDirection(snake, Snake::Direction::kDown,
                              Snake::Direction::kUp);
              break;

            case SDLK_LEFT:
              ChangeDirection(snake, Snake::Direction::kLeft,
                              Snake::Direction::kRight);
              break;

            case SDLK_RIGHT:
              ChangeDirection(snake, Snake::Direction::kRight,
                              Snake::Direction::kLeft);
              break;
        }
      } else {
          switch (e.key.keysym.sym) {
            case SDLK_w:
              ChangeDirection(snake, Snake::Direction::kUp,
                              Snake::Direction::kDown);
              break;

            case SDLK_s:
              ChangeDirection(snake, Snake::Direction::kDown,
                              Snake::Direction::kUp);
              break;

            case SDLK_a:
              ChangeDirection(snake, Snake::Direction::kLeft,
                              Snake::Direction::kRight);
              break;

            case SDLK_d:
              ChangeDirection(snake, Snake::Direction::kRight,
                              Snake::Direction::kLeft);
              break;        
          }
        }
    }
  }

  std::cout << "Got out of poll loop \n";  
}