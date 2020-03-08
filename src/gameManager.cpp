#include "gameManager.h"

//Initialize number of instances
std::uintmax_t GameManager::_numInstances = 0;

GameManager::GameManager() {
	std::cout << "In GameManager constructor and current address is " << this << "\n";
	int numInstances2Launch = 1;
	_renderer = std::make_unique<Renderer>(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
	for (auto i = 0; i < numInstances2Launch; i++) {				
		this->incrementNumInstances();
		std::cout << "Constructing and emplacing GameInstance\n";
		_gameInstances.emplace_back(kGridWidth, kGridHeight, kMsPerFrame, this->getNumInstances(), this);
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
		a.Run();
	}
}

void GameInstance::Run(){
	std::cout << "In GameInstance Run\n";
	_curr_game->Run(*_curr_cont, this->getGameManagerHandle()->getRendererHandle(), _curr_msPerFrame);
}