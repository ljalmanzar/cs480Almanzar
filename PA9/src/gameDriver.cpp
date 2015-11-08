#ifndef __GAMEDRIVER_CPP_
#define __GAMEDRIVER_CPP_

#include "gameDriver.h"

GameDriver::GameDriver(){
	//initialize the puck(s)
	_activePowerUp = false;
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

	_gamePuck.initialize("../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg", true, CYLINDER, DYNAMIC);
	
	glm::mat4 tempModel;

	_gamePuck.setModel(tempModel);

	_allObjects.push_back(&_gamePuck);

	//this->addPuck(0, "../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg");
	
	//get the table in here as well
	_table.initialize("../bin/GEO_airhockeytable.obj","../bin/ah_final_texture.png", true, TRIMESH, STATIC);
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

void GameDriver::setP1PaddlePos(PaddleDirection key, Camera* camera){
	_player1.setPaddlePosKey(key, camera);
}

void GameDriver::setP1PaddleMouse(glm::vec3 mouseRay, Camera* camera){
	_player1.setPaddlePosMouse(mouseRay, camera);
}

void GameDriver::setP2PaddlePos(PaddleDirection key, Camera* camera){
	_player2.setPaddlePosKey(key, camera);
}

void GameDriver::addPuck(int side, const std::string &objFile, const std::string &textureFile){
	// create puck in puck vector
	_pucks.push_back( new GLD(objFile, textureFile, true, CYLINDER, DYNAMIC) );

	glm::mat4 tempModel;
	// place it on side
	switch(side){
		case 0: // spawn center
			 tempModel = glm::translate( 
	        	_pucks[_pucks.size() - 1]->getModel(),
	        	glm::vec3(0.0f, 5.4f, 0.0f) 
       		); 		
		break;

		case 1: // spawn at player 1
			 tempModel = glm::translate( 
	        	_pucks[_pucks.size() - 1]->getModel(),
	        	glm::vec3(-10.0f, 5.0f, 0.0f) 
       		);
		break;

		case 2: // spawn at player 2
			 tempModel = glm::translate( 
	        	_pucks[_pucks.size() - 1]->getModel(),
	        	glm::vec3(10.0f, 5.0f, 0.0f) 
       		);
		break;
	}

	// scale puck
	//tempModel = glm::scale(tempModel, glm::vec3(0.14));

	_pucks[_pucks.size() - 1]->setModel(tempModel);
	// add reference to puck to all objects
	_allObjects.push_back(_pucks[_pucks.size() - 1]);
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
	std::string controls[10];
	std::string playerHealth[2];
	std::string heart = "<3 ";
	//declare varibles for positioning
	//generate strings for each corner and print them out
		//top left hand corner
		information[0] = "Player 1 Health: ";
		information[1] = "Player 2 Health: ";

		controls[0] = "Player 1 Controls:";
		controls[1] = "W - Move Forwards";
		controls[2] = "S - Move Backwards";
		controls[3] = "A - Move Left";
		controls[4] = "D - Move Right";

		controls[5] = "Player 2 Controls:";
		controls[6] = "Up Arrow    - Move Forwards";
		controls[7] = "Down Arrow  - Move Backwards";
		controls[8] = "Left Arrow  - Move Left";
		controls[9] = "Right Arrow - Move Right";
		//top right hand corner
	//no program needed to print text
	glUseProgram(0);

	//player 1 info
	glColor3f(1,0,0);

	glRasterPos2f(-.95,.95);
	tempStr = &information[0][0];
	while( tempStr[cursor] ){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, tempStr[cursor++] );
	}
	cursor = 0;
	
	// show health
	for (int i = 0; i < 11 - _player2.getScore(); i++ )
		playerHealth[0].append(heart);


	glRasterPos2f(-.95,.90);
	tempStr = &playerHealth[0][0];
	while( tempStr[cursor] ){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, tempStr[cursor++] );
	}
	cursor = 0;

	float dec = .85;
	for (int i = 0; i < 5; i++){
		glRasterPos2f(-.95,dec);
		tempStr = &controls[i][0];
		while( tempStr[cursor] ){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, tempStr[cursor++] );
		}
		dec -= .05;
		cursor = 0;
	}

// player 2
	glColor3f(0,0,1);
	glRasterPos2f(.60,.95);
	tempStr = &information[1][0];
	while( tempStr[cursor] ){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, tempStr[cursor++] );
	}
	cursor = 0;

	// show health
	for (int i = 0; i < 11 - _player1.getScore(); i++ )
		playerHealth[1].append(heart);


	glRasterPos2f(.60,.90);
	tempStr = &playerHealth[1][0];
	while( tempStr[cursor] ){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, tempStr[cursor++] );
	}
	cursor = 0;

	dec = .85;
	for (int i = 5; i < 10; i++){
		glRasterPos2f(.60,dec);
		tempStr = &controls[i][0];
		while( tempStr[cursor] ){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, tempStr[cursor++] );
		}
		dec -= .05;
		cursor = 0;
	}

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

std::vector<GLD*>* GameDriver::getPucks(){
	return &_pucks;
}

std::vector<GLD*>GameDriver::getAllObjects(){
	return _allObjects;
}

bool GameDriver::getPU(){
	if (_activePowerUp)
		return false;

	srand(time(NULL));

	int randNum = rand() % 200;

	if (randNum < 5)
		return true;

	return false;
}

void GameDriver::activateMysteryBox(){
	_powerup.moveMysteryBoxUp();
}

void GameDriver::activatePowerUp(){
	_powerup.spawnRandPU();
}

#endif