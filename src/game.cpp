#include "game.h"
#include <iostream>
#include "SDL.h"
#include "gameManager.h"


class Gameinstance;

Game::Game(std::size_t grid_width, std::size_t grid_height, GameInstance* gameInst)
    : snake(grid_width, grid_height),
      _currGameInst(gameInst),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}


void Game::applyChangeDirection(Snake &snake, SDL_Keycode inputDir) {
  switch (inputDir) {
    case SDLK_UP:
      ChangeDirection(snake, Snake::Direction::kUp,
                      Snake::Direction::kDown);
      // std::cout<< "up\n";
      break;
    case SDLK_DOWN:
      ChangeDirection(snake, Snake::Direction::kDown,
                      Snake::Direction::kUp);
      // std::cout<< "down\n";
      break;
    case SDLK_LEFT:
      ChangeDirection(snake, Snake::Direction::kLeft,
                      Snake::Direction::kRight);
      // std::cout<< "left\n";
      break;
    case SDLK_RIGHT:
      ChangeDirection(snake, Snake::Direction::kRight,
                      Snake::Direction::kLeft);
      // std::cout<< "right\n";
      break;
    case SDLK_w:
      ChangeDirection(snake, Snake::Direction::kUp,
                      Snake::Direction::kDown);
      // std::cout<< "up\n";
      break;
    case SDLK_s:
      ChangeDirection(snake, Snake::Direction::kDown,
                      Snake::Direction::kUp);
      // std::cout<< "down\n";
      break;
    case SDLK_a:
      ChangeDirection(snake, Snake::Direction::kLeft,
                      Snake::Direction::kRight);
      // std::cout<< "left\n";
      break;
    case SDLK_d:
      ChangeDirection(snake, Snake::Direction::kRight,
                      Snake::Direction::kLeft);
      // std::cout<< "right\n";       
      break;
  }
}


// void Game::Run(Controller const &controller, Renderer &renderer,
//                std::size_t target_frame_duration) {
// void Game::Run(Controller &controller, Renderer *renderer,
//                std::size_t target_frame_duration) {
// void Game::Run(Renderer *renderer,
//                std::size_t target_frame_duration) { 
void Game::Run(std::size_t target_frame_duration) { 
  // std::cout << "In Game::Run\n";
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  int counter = 0;

  // std::cout << "Got here " << getGameInst()->getId() << "\n";

  while (running) {

    // Acquire a lock on the current game mutex
    std::unique_lock<std::mutex> cl(_gmMtx);

    // Wait until we get a key press
    _condVar.wait(cl,[this](){return this->_pressedKey.size() > 0;});

    // Start the frame
    frame_start = SDL_GetTicks();

    auto currKey = std::move(_pressedKey.back());
    _pressedKey.clear();

    cl.unlock();

    // Apply the key press
    this->applyChangeDirection(snake, currKey);

    // Get control first and then start the frame    

    // Input, Update, Render - the main game loop.

    // controller.HandleInput(running, snake, getGameInst()->getId());    

    // Get control mutex
    //

    // std::cout << "Got here after the controller " << getGameInst()->getId() << "\n";

    Update();

    // renderer->Render(snake, food);
    // counter++;
    // if (counter > 60) {      

    // std::cout << "Got here after the Update " << getGameInst()->getId() << "\n";

    {

      counter = 0;

      // std::cout << "Before Snake Mutex " << getGameInst()->getId() << "\n";      

      std::unique_lock<std::mutex> ul(*(getGameInst()->getGameManagerHandle()->getMtxHandle()));          

      // std::cout << "After Snake Mutex " << getGameInst()->getId() << "\n";      

      auto temp_snake = snake;

      auto temp_food = food;

      getGameInst()->getGameManagerHandle()->getMovedSnakesQ(getGameInst()->getId())->push_back(std::move(temp_snake));      

      getGameInst()->getGameManagerHandle()->getFoodQ(getGameInst()->getId())->push_back(std::move(temp_food));

      ul.unlock();
      getGameInst()->getGameManagerHandle()->getCondVarHandle()->notify_one();
    //   std::cout << getGameInst()->getGameManagerHandle()->getTempSnakies()->size() << "\n";

    }
    // std::cout << "After moving snake " << getGameInst()->getId() << "\n";

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      // renderer.UpdateWindowTitle(score, frame_count);
      // renderer->UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }

    if (currKey == SDLK_EJECT) {
      running = false;
    }

    if (!running){
      std::lock_guard lck(*(getGameInst()->getGameManagerHandle()->getMtxHandle()));
      // std::cout << "Setting flag to false\n";
      getGameInst()->getGameManagerHandle()->setGameRunningFlag(false);
      getGameInst()->getGameManagerHandle()->getCondVarHandle()->notify_one();
    }    
  }
  // std::cout << "Out of running loop\n";
}

void Game::sendToGameMngr(){
  std::lock_guard<std::mutex> lck(_gmMtx);

};

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;

      // std::cout << "Food placed at x,y: " << food.x << "," << food.y << "\n";

      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}