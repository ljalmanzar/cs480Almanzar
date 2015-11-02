#ifndef __GAMEDRIVER_CPP_
#define __GAMEDRIVER_CPP_

#include "gameDriver.h"


GameDriver::GameDriver(){
	//initialize the puck(s)
}

GameDriver::~GameDriver(){
	// clean up after all of the dynamically allocated GLD's
	for( unsigned int i = 0; i < _allObjects.size(); i++ ){
		//delete _allObjects[i];
	}
}

void GameDriver::initGame(){
	// create players
	_player1.setPlayerNumber(1);
	_player2.setPlayerNumber(2);
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
	_allObjects.push_back(_powerup.getIceRink());
	_allObjects.push_back(_powerup.getHealth());
	_allObjects.push_back(_powerup.getPutinPaddle());

	//get the table in here as well
	_allObjects.push_back( new GLD("../bin/hockey_table_1.obj","../bin/powerup/metal.jpg") );

	_pucks.push_back( GLD("../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg", true) );

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