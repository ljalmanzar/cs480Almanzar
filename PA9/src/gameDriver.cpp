#include "gameDriver"

GameDriver::GameDriver(){

}

GameDriver::~GameDriver(){

}

void initGame(){
	// create players
	// assign AI
	//
}

// setters

// getters
Player GameDriver::getPlayer1() const{
	return _player1;
}

Player GameDriver::getPlayer2() const{
	return _player2;
}

std::std::vector<GLD> getPucks() const{
	return _pucks;
}