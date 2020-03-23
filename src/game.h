#ifndef GAME_H
#define GAME_H

#include <random>
// #include "gameManager.h"
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>

class GameInstance;
class GameManager;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, GameInstance * gameInst); // constructor

  // void Run(Controller const &controller, Renderer &renderer,
  //          std::size_t target_frame_duration);

  // void Run(Controller &controller, Renderer *renderer,
  //          std::size_t target_frame_duration);

// void Run(Renderer *renderer,
//        std::size_t target_frame_duration);

void Run(std::size_t target_frame_duration);

  int getScore() const { return score; };
  int getSize() const { return snake.size; };

  void setGameInst(GameInstance * gameInst){_currGameInst = gameInst;}
  GameInstance * getGameInst(){return _currGameInst;}

  void sendToGameMngr();

  std::mutex* getGameMtxHandle(){return &_gmMtx;};

  std::condition_variable* getGameCondVarHandle(){return &_condVar;};

  std::deque<SDL_Keycode>* getKeyQ(){return &_pressedKey;};

  void applyChangeDirection(Snake &snake, SDL_Keycode inputDir);

 private:

  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;  

  Snake snake;
  SDL_Point food;  

  std::deque<SDL_Keycode> _pressedKey;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  GameInstance * _currGameInst;

  std::mutex _gmMtx;

  std::condition_variable _condVar;

  int score{0};

  void PlaceFood();
  void Update();
};

#endif