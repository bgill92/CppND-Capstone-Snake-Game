#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include <mutex>
#include <condition_variable>

#include <deque>

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake, int instId);

  Controller(bool altControlFlag) : _altControlFlag(altControlFlag){};

  std::pair<int,SDL_Keycode> pollInput();

  SDL_Keycode getDoNothing(){return _doNothingKey;};

  // void applyChangeDirection(Snake &snake, SDL_Keycode inputDir);

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;

  SDL_Keycode _doNothingKey{SDLK_COMPUTER};

  bool _altControlFlag{0};

  std::mutex _mtx;  

  std::condition_variable _condVar;

};

#endif