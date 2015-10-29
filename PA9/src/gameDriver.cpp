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
	_allObjects.push_back(_player1.getPaddle());
	_allObjects.push_back(_player2.getPaddle());

	// init/add powerups
	_powerup.init();

	_allObjects.push_back(_powerup.getMysteryBox());
	_allObjects.push_back(_powerup.getMultiPuck());
	_allObjects.push_back(_powerup.getHeavyPuck());
	_allObjects.push_back(_powerup.getLightPuck());
	_allObjects.push_back(_powerup.getIceRing());
	_allObjects.push_back(_powerup.getHealth());
	_allObjects.push_back(_powerup.getPutinPaddle());

}

void GameDriver::addPowerups(){

}

// setters
void stepPhysicsAnimation( float dt ){
	
}

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

std::vector<GLD*> GameDriver::getAllObjects(){
	return _allObjects;
}

#endif