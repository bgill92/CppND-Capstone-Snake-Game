#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <iostream>
#include <vector>
#include "game.h"
#include "renderer.h"
#include "snake.h"
#include "controller.h"
#include <stdexcept>
#include <memory>

class GameManager;
class GameInstance;

class GameManager {

public:

	GameManager();
	GameManager(int numInstances2Launch);

	void Run();

	std::uintmax_t getNumInstances() {return GameManager::_numInstances;};
	void incrementNumInstances() { GameManager::_numInstances++;};
	void decrementNumInstances() { GameManager::_numInstances--;};

	Renderer * getRendererHandle() {return _renderer.get();}

private:

	// Number of game instances running
	static std::uintmax_t _numInstances;

	// Vector of instances
	std::vector<GameInstance> _gameInstances;

	// There is one renderer for all of the instances
	std::unique_ptr<Renderer> _renderer;

	std::size_t kFramesPerSecond{60};
	std::size_t kMsPerFrame{1000 / kFramesPerSecond};
	std::size_t kScreenWidth{640};
	std::size_t kScreenHeight{640};
	std::size_t kGridWidth{32};
	std::size_t kGridHeight{32};

};

class GameInstance {

public:

	// GameInstance();
	GameInstance(std::uintmax_t gridWidth, std::uintmax_t gridHeight, std::uintmax_t msPerFrame, int idNum, GameManager * manager) : 
				_curr_gridWidth(gridWidth), _curr_gridHeight(gridHeight), _curr_msPerFrame(msPerFrame), _id(idNum), _curr_gameManager(manager) {
					std::cout << "In GameInstance constructor with all of the inputs\n";
				    // _curr_cont = new Controller();
				    // _curr_game = new Game(gridWidth, gridHeight);				    
				    _curr_cont = std::make_unique<Controller>();
				    _curr_game = std::make_unique<Game>(gridWidth,gridHeight);
				    std::cout << "In GameInstance constructor and made the things\n";
				 }; // Constructor with arguments

	~GameInstance() {/*delete _curr_game; delete _curr_cont;*/ _curr_gameManager->decrementNumInstances();}; // Destructor

	GameInstance(const GameInstance& other) { // copy constructor 
		std::cout << "In GameInstance copy constructor\n";
		// Set the grid parameters
		_curr_gridWidth = other._curr_gridWidth;		
		_curr_gridHeight = other._curr_gridHeight;
		_curr_msPerFrame = other._curr_msPerFrame;

		// Set the game manager
		_curr_gameManager = other.getGameManagerHandle();

		// There is another game instance now
		_curr_gameManager->incrementNumInstances();
		// Set the current id to the new number of game instances
		_id = _curr_gameManager->getNumInstances();

		// Creat the new game and controller object
		// _curr_game = new Game(_curr_gridWidth,_curr_gridHeight);
		// _curr_cont = new Controller();

		_curr_game = std::make_unique<Game>(_curr_gridWidth,_curr_gridHeight);
		_curr_cont = std::make_unique<Controller>();

		std::cout << "Leaving GameInstance copy constructor\n";
	}; 

	GameInstance& operator=(const GameInstance& other) { // copy assignment operator
		std::cout << "In GameInstance copy assignment constructor\n";
		if (this == &other) {
			return *this;			
		}
		// Release resources
		// delete _curr_game;
		// delete _curr_cont;
		_curr_game.reset();
		_curr_cont.reset();

		// Set the grid parameters
		_curr_gridWidth = other._curr_gridWidth;		
		_curr_gridHeight = other._curr_gridHeight;
		_curr_msPerFrame = other._curr_msPerFrame;

		// Set the game manager
		_curr_gameManager = other.getGameManagerHandle();

		// The total number of instance don't change since this is an assignment
		_id = other._id;

		// Creat the new game and controller object
		// _curr_game = new Game(_curr_gridWidth,_curr_gridHeight);
		// _curr_cont = new Controller();		
		_curr_game = std::make_unique<Game>(_curr_gridWidth,_curr_gridHeight); 
		_curr_cont = std::make_unique<Controller>();

		std::cout << "Leaving GameInstance copy assignment constructor\n";

		return *this;

	}

	GameInstance(GameInstance&& other) noexcept { // move constructor
		std::cout << "In GameInstance move constructor\n";
		// move and invalidate grid parameters
		_curr_gridWidth   = std::exchange(other._curr_gridWidth,NULL);
		_curr_gridHeight  = std::exchange(other._curr_gridHeight,NULL);
		_curr_msPerFrame  = std::exchange(other._curr_msPerFrame,NULL);

		// move game manager and set current game manager to nullptr
		_curr_gameManager = std::exchange(other._curr_gameManager,nullptr);
		// Since there is an empty game instance decrement the number of instances
		_curr_gameManager->decrementNumInstances();

		// Switch the id
		_id = std::exchange(other._id,NULL);

		// transfer ownserhip and invalidate pointers
		// _curr_game = std::exchange(other._curr_game,nullptr);		
		_curr_game.swap(other._curr_game);
		other._curr_game.reset();

		// _curr_cont = std::exchange(other._curr_cont,nullptr);
		_curr_cont.swap(other._curr_cont);
		other._curr_cont.reset();

		std::cout << "Leaving GameInstance move constructor\n";

	}

	GameInstance& operator=(GameInstance&& other) noexcept{ // move assignment
		std::cout << "In GameInstance move assignment constructor\n";
		if (this == &other) {
			return *this;
		}
		// release currently held resources
		// delete _curr_game;
		// delete _curr_cont;
		_curr_game.reset();
		_curr_cont.reset();

		// move and invalidate grid parameters
		_curr_gridWidth   = std::exchange(other._curr_gridWidth,NULL);
		_curr_gridHeight  = std::exchange(other._curr_gridHeight,NULL);
		_curr_msPerFrame  = std::exchange(other._curr_msPerFrame,NULL);

		// move game manager and set current game manager to nullptr
		_curr_gameManager = std::exchange(other._curr_gameManager,nullptr);
		// Since there is an empty game instance decrement the number of instances
		_curr_gameManager->decrementNumInstances();

		// Switch the id
		_id = std::exchange(other._id,NULL);

		// transfer ownserhip and invalidate pointers
		// _curr_game = std::exchange(other._curr_game,nullptr);		
		// _curr_cont = std::exchange(other._curr_cont,nullptr);		
	
		_curr_game.swap(other._curr_game);
		other._curr_game.reset();

		_curr_cont.swap(other._curr_cont);
		other._curr_cont.reset();

		std::cout << "Leaving GameInstance move assignment constructor\n";

		return *this;
	}


	GameManager * getGameManagerHandle() const {return this->_curr_gameManager;}

	void Run();

private:

	int _id; // Instance ID
	// Game * _curr_game;
	// Controller * _curr_cont;

	std::unique_ptr<Game> _curr_game;
	std::unique_ptr<Controller> _curr_cont;

	GameManager * _curr_gameManager;

	std::uintmax_t _curr_gridWidth;
	std::uintmax_t _curr_gridHeight;
	std::uintmax_t _curr_msPerFrame;
	
};


#endif