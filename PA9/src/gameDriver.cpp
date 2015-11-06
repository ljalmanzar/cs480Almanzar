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

	//_pucks.push_back( GLD("../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg", true, CYLINDER) );

	_gamePuck.initialize("../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg", true, CYLINDER, true);
	
	glm::mat4 tempModel;

	_gamePuck.setModel(tempModel);

	_allObjects.push_back(&this->_gamePuck);

	//this->addPuck(0, "../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg");
	
	//get the table in here as well
	_table.initialize("../bin/GEO_airhockeytable.obj","../bin/ah_final_texture.png", true, TRIMESH);
	_allObjects.push_back(&this->_table);

	// reset player's score
	_player1.resetScore();
	_player2.resetScore();

	// create background
	_backGround.initialize("../bin/planet.obj", "../bin/background.jpeg");
	_allObjects.push_back(&_backGround);
	tempModel = glm::scale(_backGround.getModel(), glm::vec3(100));
	_backGround.setModel(tempModel);
}

// setters
void GameDriver::stepPhysicsAnimation( float dt ){
	
}

void GameDriver::setP1PaddlePos(int x_pos, int y_pos, int width, int height){
	_player1.setPaddlePosMouse(x_pos, y_pos, width, height);
}

void GameDriver::setP2PaddlePos(unsigned char key){
	_player2.setPaddlePosKey(key);
}

void GameDriver::addPuck(int side, const std::string &objFile, const std::string &textureFile){
	// create puck in puck vector
	_pucks.push_back( GLD(objFile, textureFile, true, CYLINDER, true) );

	glm::mat4 tempModel;
	// place it on side
	switch(side){
		case 0: // spawn center
			 tempModel = glm::translate( 
	        	_pucks[_pucks.size() - 1].getModel(),
	        	glm::vec3(0.0f, 5.0f, 0.0f) 
       		); 		
		break;

		case 1: // spawn at player 1
			 tempModel = glm::translate( 
	        	_pucks[_pucks.size() - 1].getModel(),
	        	glm::vec3(-10.0f, 5.0f, 0.0f) 
       		);
		break;

		case 2: // spawn at player 2
			 tempModel = glm::translate( 
	        	_pucks[_pucks.size() - 1].getModel(),
	        	glm::vec3(10.0f, 5.0f, 0.0f) 
       		);
		break;
	}

	// scale puck
	tempModel = glm::scale(tempModel, glm::vec3(0.14));

	_pucks[_pucks.size() - 1].setModel(tempModel);
	// add reference to puck to all objects
	_allObjects.push_back(&_pucks[_pucks.size() - 1]);
}

void GameDriver::updateP1Score(GLD* puck){
	_player1.incrementScore(1);
	if (_player1.getScore() >= 11){
		// end game menu

	} else{
		// reset puck pos in front of p2
		this -> addPuck(2, "../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg");
	}
}

void GameDriver::updateP2Score(GLD* puck){
	_player2.incrementScore(1);
	if (_player2.getScore() >= 11){
		// end game menu

	} else{
		// reset puck pos in front of p1
		this -> addPuck(1, "../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg");
	}
}

void GameDriver::printScores() const {
	//temp variables
	char * tempStr;
	int cursor = 0;
	std::string information[10];
	//declare varibles for positioning
	//generate strings for each corner and print them out
		//top left hand corner
		information[0] = "Player 1 Score: " + std::to_string(_player1.getScore());
		information[1] = "Player 2 Score: " + std::to_string(_player2.getScore());
		//top right hand corner
	//no program needed to print text
	glUseProgram(0);

	//display the text
	glColor3f(1.0,1.0,1.0);

	glRasterPos2f(-.95,.95);
	tempStr = &information[0][0];
	while( tempStr[cursor] ){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, tempStr[cursor++] );
	}
	cursor = 0;

	glRasterPos2f(-.95,.85);
	tempStr = &information[1][0];
	while( tempStr[cursor] ){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, tempStr[cursor++] );
	}
	cursor = 0;

}

// getters
Player* GameDriver::getPlayer1(){
	return &_player1;
}

Player* GameDriver::getPlayer2(){
	return &_player2;
}

GLD* GameDriver::getTable(){
	return &_table;
}

Powerup* GameDriver::getPowerup(){
	return &_powerup;
}

std::vector<GLD>* GameDriver::getPucks(){
	return &_pucks;
}

std::vector<GLD*> GameDriver::getAllObjects(){
	return _allObjects;
}

#endif