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
	_table.initialize("../bin/GEO_airhockeytable.obj","../bin/ah_final_texture.png", true, PLANE);
	_allObjects.push_back(&this->_table);

	_pucks.push_back( GLD("../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg", true, CYLINDER) );

	_gamePuck.initialize("../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg", true, CYLINDER);
	glm::mat4 tempModel = glm::translate( 
        _gamePuck.getModel(),
        glm::vec3(0.0f, 1.0f, 0.0f) 
        );
	_gamePuck.setModel(tempModel);
	_allObjects.push_back(&this->_gamePuck);

	// reset player's score
	_player1.resetScore();
	_player2.resetScore();
}

// setters
void GameDriver::stepPhysicsAnimation( float dt ){
	
}

void GameDriver::setP1PaddlePos(int x_pos, int y_pos, int width, int height){
	_player1.setPaddlePos(x_pos, y_pos, width, height);
}

void GameDriver::updateP1Score(GLD* puck){
	_player1.incrementScore(1);
	if (_player1.getScore() >= 11){
		// end game menu

	} else{
		// reset puck pos in front of p2

	}
}

void GameDriver::updateP2Score(GLD* puck){
	_player2.incrementScore(1);
	if (_player2.getScore() >= 11){
		// end game menu

	} else{
		// reset puck pos in front of p1
		
	}
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

GLD* GameDriver::getTable(){
	return &_table;
}

#endif