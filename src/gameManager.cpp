#include "gameManager.h"

#include <thread>

//Initialize number of instances
std::uintmax_t GameManager::_numInstances = 0;

GameManager::GameManager() {
	std::cout << "In GameManager constructor and current address is " << this << "\n";
	int numInstances2Launch = 1;
	_renderer = std::make_unique<Renderer>(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);

	for (auto i = 0; i < numInstances2Launch; i++) {						
		std::cout << "Constructing and emplacing GameInstance\n";
		_gameInstances.emplace_back(kGridWidth, kGridHeight, kMsPerFrame, this->getNumInstances(), this);
		_movedSnakes.emplace_back();
		_movedFoods.emplace_back();
		this->incrementNumInstances();
	}
};

GameManager::GameManager(int numInstances2Launch) {
	// for (auto i = 0; i < numInstances2Launch; i++) {
	// 	_gameInstances.emplace_back(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight, kMsPerFrame);
	// }
};



void GameManager::Run() {
	for (auto &a :_gameInstances) {
		std::cout << "Beginning to run...\n";
		// std::thread t(GameInstance::Run,&a);
		t_all.emplace_back(&GameInstance::Run,&a);
	}
	while (true) {
				
		std::unique_lock<std::mutex> ul(_mtx);
		_cond_var.wait(ul, [this](){return ((this->_movedSnakes[0].size()) > 0) | !(this->getGameRunningFlag());});				

		if (this->getGameRunningFlag()) {
			getRendererHandle()->Render(std::move(this->_movedSnakes[0].back()), std::move(this->_movedFoods[0].back()));
			this->_movedSnakes[0].pop_back();  
			this->_movedFoods[0].pop_back();
		}

		if (!getGameRunningFlag()){
			ul.unlock();
			break;
		}		

	}
	for (auto &t : t_all) {	
		std::cout << "Waiting for join\n";
		t.join();
	}
}

void GameInstance::Run(){
	std::cout << "In GameInstance Run\n";
	_curr_game->Run(*_curr_cont, this->getGameManagerHandle()->getRendererHandle(), _curr_msPerFrame);
}