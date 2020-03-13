#ifndef GAME_H
#define GAME_H

#include <random>
#include "gameManager.h"
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

#include <mutex>

class GameInstance;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, GameInstance * gameInst); // constructor

  // void Run(Controller const &controller, Renderer &renderer,
  //          std::size_t target_frame_duration);

  void Run(Controller const &controller, Renderer *renderer,
           std::size_t target_frame_duration);

  int GetScore() const { return score; };
  int GetSize() const { return snake.size; };

  void setGameInst(GameInstance * gameInst){_currGameInst = gameInst;}
  GameInstance * getGameInst(){return _currGameInst;}

  void sendToGameMngr();

 private:
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  GameInstance * _currGameInst;

  std::mutex _gmMtx;

  int score{0};

  void PlaceFood();
  void Update();
};

#endif