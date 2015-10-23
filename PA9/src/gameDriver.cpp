#ifndef __GAMEDRIVER_CPP_
#define __GAMEDRIVER_CPP_

#include "gameDriver.h"

GameDriver::GameDriver(){
	
}

GameDriver::~GameDriver(){

}

void GameDriver::initGame(){
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

std::vector<GLD> GameDriver::getPucks() const{
	return _pucks;
}

#endif