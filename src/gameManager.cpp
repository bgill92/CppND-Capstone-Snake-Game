#include "gameManager.h"
#include "game.h"
#include "controller.h"

#include <thread>

//Initialize number of instances
std::uintmax_t GameManager::_numInstances = 0;

GameManager::GameManager() {
	// std::cout << "In GameManager constructor and current address is " << this << "\n";
	int numInstances2Launch = 1;
	_renderer = std::make_unique<Renderer>(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
	_controller = std::make_unique<Controller>(0);

	for (auto i = 0; i < numInstances2Launch; i++) {						
		// std::cout << "Constructing and emplacing GameInstance\n";
		_gameInstances.emplace_back(kGridWidth, kGridHeight, kMsPerFrame, this->getNumInstances(), this);
		_movedSnakes.emplace_back();
		_movedFoods.emplace_back();
	}

	// std::cout << "numInstances: " << GameManager::_numInstances << "\n";	

};

void GameManager::ending() {

	// std::cout << this->_numInstances << "\n";

	if (GameManager::_numInstances == 1) {

		std::cout << "Score: " << _gameInstances[0].getGameHandle()->getScore() << "\n";
		std::cout << "Size: " << _gameInstances[0].getGameHandle()->getSize() << "\n";

	} else if (GameManager::_numInstances == 2) {		

		std::cout << "Score for player 1: " << _gameInstances[0].getGameHandle()->getScore() << "\n\n";
		std::cout << "Size for player 1: " << _gameInstances[0].getGameHandle()->getSize() << "\n\n";

		std::cout << "Score for player 2: " << _gameInstances[1].getGameHandle()->getScore() << "\n\n";
		std::cout << "Size for player 2: " << _gameInstances[1].getGameHandle()->getSize() << "\n\n";

		auto p1score = _gameInstances[0].getGameHandle()->getScore();
		auto p2score = _gameInstances[1].getGameHandle()->getScore();

		if(p1score > p2score) {
			std::cout << "Player 1 wins!\n";
		}else if(p1score < p2score) {
			std::cout << "Player 2 wins!\n";
		} else {
			std::cout << "It was a tie, both players lose!\n";
		}

	}

}

GameManager::GameManager(int numInstances2Launch) {
	// std::cout << "In GameManager constructor and current address is " << this << "\n";

	_renderer = std::make_unique<Renderer>(kScreenWidth*numInstances2Launch, kScreenHeight, kGridWidth*numInstances2Launch, kGridHeight);
	_controller = std::make_unique<Controller>(0);

// numInstances2Launch = 1;
	for (auto i = 0; i < numInstances2Launch; i++) {						
		// std::cout << "Constructing and emplacing GameInstance\n";
		// std::cout << "Getting numInstances " << this->getNumInstances() << "\n";
		_gameInstances.emplace_back(kGridWidth, kGridHeight, kMsPerFrame, this->getNumInstances(), this);
		_movedSnakes.emplace_back();
		_movedFoods.emplace_back();
	}

	// std::cout << "numInstances: " << GameManager::_numInstances << "\n";	

	// std::cout << "Leaving GameManager constructor" << "\n";	
};

bool GameManager::startRenderFlag(){
	int sum = 0;
	for (int ii = 0; ii < GameManager::_numInstances; ii++) {
		sum += ((this->_movedSnakes[ii].size()) > 0);
	}
	return (sum >= GameManager::_numInstances) | (!(this->getGameRunningFlag()));
}

void GameManager::Run() {
	for (auto &a :_gameInstances) {
		// std::cout << "Beginning to run...\n";
		// std::thread t(GameInstance::Run,&a);
		t_all.emplace_back(&GameInstance::Run,&a);
	}
	while (true) {
		
		// Read controller
		auto curr_inputs = _controller->pollInput();

		// std::cout << "Read controller\n";

		// Send controller inputs to the proper games
		for (int ii = 0; ii < _gameInstances.size(); ii++) {

			if (ii == curr_inputs.first) { // Send the instance the command

				// std::cout << "Sending input to " << ii << "\n";

				// Lock the game mutex
				std::unique_lock<std::mutex> cl(*(_gameInstances[ii].getGameHandle()->getGameMtxHandle()));

				// Transfer the input to the correct game
				(*(_gameInstances[ii].getGameHandle()->getKeyQ())).push_back(curr_inputs.second);

				// Unlock the mutex
				cl.unlock();

				// Notify the game it has a message
				_gameInstances[ii].getGameHandle()->getGameCondVarHandle()->notify_one();

				// std::cout << "Sent input to " << ii << "\n";

			} else {

				// std::cout << "Sending input to " << ii << "\n";

				// Lock the game mutex
				std::unique_lock<std::mutex> cl(*(_gameInstances[ii].getGameHandle()->getGameMtxHandle()));

				// Transfer the input to the correct game
				if (curr_inputs.second == SDLK_EJECT) {

					(*(_gameInstances[ii].getGameHandle()->getKeyQ())).push_back(SDLK_EJECT);
				
				} else {


					(*(_gameInstances[ii].getGameHandle()->getKeyQ())).push_back(_controller.get()->getDoNothing());

				}

				// Unlock the mutex
				cl.unlock();

				// Notify the game it has a message
				_gameInstances[ii].getGameHandle()->getGameCondVarHandle()->notify_one();								

				// std::cout << "Sent input to " << ii << "\n";

			}
		}

		std::unique_lock<std::mutex> ul(_mtx);
		// _cond_var.wait(ul, [this](){return ((this->_movedSnakes[0].size()) > 0) | !(this->getGameRunningFlag());});	

		// std::cout << "Waiting to mutex \n";
		_cond_var.wait(ul, [this](){return this->startRenderFlag();});

		for (int ii = 0; ii < GameManager::_numInstances; ii++) {
			// std::cout << "Size of Q number " << ii << ": " << (this->_movedSnakes[ii].size()) << "\n";
		}

		// std::cout << GameManager::_numInstances << "\n";

		// std::cout << "Starting to render \n";
		if (this->getGameRunningFlag()) {
			// getRendererHandle()->Render(std::move(this->_movedSnakes[0].back()), std::move(this->_movedFoods[0].back()),GameManager::_numInstances);
			// std::cout << "Right before render \n";
			getRendererHandle()->Render(this->getMovedSnakesVec(), this->getMovedFoodsVec(), GameManager::_numInstances);
			// std::cout << "Rendered\n";

			// std::cout << "After Render \n";

			for (int ii = 0; ii < (this->_movedSnakes).size(); ii++){
				this->_movedSnakes[ii].clear();
			}
			for (int ii = 0; ii < (this->_movedFoods).size(); ii++){
				this->_movedFoods[ii].clear();
			}			
			// this->_movedSnakes[0].pop_back();  
			// this->_movedFoods[0].pop_back();
		}

		if (!getGameRunningFlag()){
			ul.unlock();
			break;
		}		
		// std::cout << "After Game Run Check\n";
	}
	for (auto &t : t_all) {	
		// std::cout << "Waiting for join\n";
		t.join();
	}
}

void GameInstance::Run(){
	// std::cout << "In GameInstance Run\n";
	// _curr_game->Run(*_curr_cont, this->getGameManagerHandle()->getRendererHandle(), _curr_msPerFrame);
	// _curr_game->Run(this->getGameManagerHandle()->getRendererHandle(), _curr_msPerFrame);	
	_curr_game->Run(_curr_msPerFrame);	
}