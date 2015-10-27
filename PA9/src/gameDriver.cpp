#ifndef __GAMEDRIVER_CPP_
#define __GAMEDRIVER_CPP_

#include "gameDriver.h"

GameDriver::GameDriver(){
	
}

GameDriver::~GameDriver(){

}

void GameDriver::initGame(){
	// create players
	_player1.init();
	_player2.init();

	// add player's paddle
	_allObjects.push(_player1.getPaddle());
	_allObjects.push(_player2.getPaddle());

	// init/add powerups
	_powerup.init();

	_allObjects.push(_powerup.getMysteryBox());
	_allObjects.push(_powerup.getMultiPuck());
	_allObjects.push(_powerup.getHeavyPuck());
	_allObjects.push(_powerup.getLightPuck());
	_allObjects.push(_powerup.getIceRing());
	_allObjects.push(_powerup.getHealth());
	_allObjects.push(_powerup.getPutinPaddle());

}

void GameDriver::addPowerups(){

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

std::std::vector<GLD*> GameDriver::getAllObjects() const{
	return _allObjects;
}

#endif