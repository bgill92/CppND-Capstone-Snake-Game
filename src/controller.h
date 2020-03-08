#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;

  // Controller(bool altControlFlag) : _altControlFlag(altControlFlag){};

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;

  // bool _altControlFlag{0};

};

#endif