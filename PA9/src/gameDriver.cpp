#ifndef __GAMEDRIVER_CPP_
#define __GAMEDRIVER_CPP_

#include "gameDriver.h"

GameDriver::GameDriver(){

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
	_powerup.init(&_player1,&_player2, &_table);

	_allObjects.push_back(_powerup.getMysteryBox());
	_allObjects.push_back(_powerup.getMultiPuck());
	_allObjects.push_back(_powerup.getHeavyPuck());
	_allObjects.push_back(_powerup.getLightPuck());
	_allObjects.push_back(_powerup.getIceRink());
	_allObjects.push_back(_powerup.getHealth());
	_allObjects.push_back(_powerup.getPutinPaddle());

	this->addPuck(1, "../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg");

	//get the table in here as well
	_table.initialize("../bin/GEO_airhockeytable.obj","../bin/ah_final_texture.png", true, TRIMESH, STATIC);
	_allObjects.push_back(&this->_table);

	_tableWalls.initialize("../bin/walls.obj", "../bin/ah_final_texture.png", false, TRIMESH, STATIC);
	_allObjects.push_back(&this->_tableWalls);

	// reset player's score
	_player1.resetScore();
	_player2.resetScore();

	// create background
	_backGround.initialize("../bin/planet.obj", "../bin/background.jpeg");
	_allObjects.push_back(&_backGround);
	glm::mat4 tempModel = glm::scale(_backGround.getModel(), glm::vec3(100));
	_backGround.setModel(tempModel);
	_isPowerupActive = false;
	_isAiActive = false;
}

// setters
void GameDriver::stepPhysicsAnimation( float dt ){
	
}

void GameDriver::setP1PaddlePos(PaddleDirection key, Camera* camera){
	_player1.setPaddlePosKey(key, camera);
}

void GameDriver::setP1PaddleMouse(MousePicker mouseRay, Camera* camera){
	_player1.setPaddlePosMouse(mouseRay, camera);
}

void GameDriver::setP2PaddlePos(PaddleDirection key, Camera* camera){
	_player2.setPaddlePosKey(key, camera);
}

void GameDriver::addPuck(int side, const std::string &objFile, const std::string &textureFile){
	// create puck in puck vector
	GLD * newPuck = new GLD();

	glm::mat4 tempModel;
	// place it on side
	switch(side){
		case 0: // spawn center
			 tempModel = glm::translate( 
	        	newPuck->getModel(),
	        	glm::vec3(0.0f, 0.0f, 0.0f) 
       		); 		
		break;

		case 1: // spawn at player 1
			 tempModel = glm::translate( 
	        	newPuck->getModel(),
	        	glm::vec3(-8.0f, 0.0f, 0.0f) 
       		);
		break;

		case 2: // spawn at player 2
			 tempModel = glm::translate( 
	        	newPuck->getModel(),
	        	glm::vec3(8.0f, 0.0f, 0.0f) 
       		);
		break;
	}

	// scale puck
	//tempModel = glm::scale(tempModel, glm::vec3(0.14));

	newPuck->initialize(objFile, textureFile, true, CYLINDER, DYNAMIC);
	newPuck->setModel(tempModel);
	// add reference to puck to all objects
	_pucks.push_back(newPuck);
	_allObjects.push_back(newPuck);
}

void GameDriver::updateP1Score(GLD* puck){
	_player1.incrementScore(1);
}

void GameDriver::updateP2Score(GLD* puck){
	_player2.incrementScore(1);
}

bool GameDriver::isGameOver(){
	return(_player1.getScore() >= 7 ||_player2.getScore() >= 7);
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
	for (int i = 0; i < 7 - _player2.getScore(); i++ )
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
	for (int i = 0; i < 7 - _player1.getScore(); i++ )
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
	if (_isPowerupActive){

		return false;
	}

	srand(time(NULL));

	int randNum = rand() % 25;

	if (randNum == 0){
		activateMysteryBox();
		return true;
	}

	return false;
}

void GameDriver::activateMysteryBox(){
	_powerup.moveMysteryBoxUp();
	_isPowerupActive = true;
}

bool GameDriver::checkForGoal( btDiscreteDynamicsWorld * world ){
	//go through all the pucks
	for( unsigned int i = 0; i < _pucks.size(); i++ ){
		//player one scored goal
		if( _pucks[i]->getModel()[3].x >= 15.0 ){
			//remove it from the physics world
			world->removeRigidBody( _pucks[i]->getRigidBody() );

			//remove it from all Objects and Pucks
			GLD * puckToBeRemoved = _pucks[i];
			for( unsigned int j = 0; j < _allObjects.size(); j++ ){
				if( _allObjects[j] == puckToBeRemoved ){
					_allObjects.erase( _allObjects.begin() + j );
					break;
				}
			}
			_pucks.erase( _pucks.begin() + i);
			delete puckToBeRemoved;
			
			//add a brand new one to pucks and all objects
			this->addPuck(1, "../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg");

			//update player 1 score
			updateP2Score(NULL);

			//add the new rigid body
			_pucks[_pucks.size()-1]->addPhysics();
			world->addRigidBody( _pucks[_pucks.size()-1]->getRigidBody() );
			return true;

		} else if ( _pucks[i]->getModel()[3].x <= -15.0 ){
			//remove it from the physics world
			world->removeRigidBody( _pucks[i]->getRigidBody() );

			//remove it from all Objects and Pucks
			GLD * puckToBeRemoved = _pucks[i];
			for( unsigned int j = 0; j < _allObjects.size(); j++ ){
				if( _allObjects[j] == puckToBeRemoved ){
					_allObjects.erase( _allObjects.begin() + j );
					break;
				}
			}
			_pucks.erase( _pucks.begin() + i);
			delete puckToBeRemoved;
			
			//add a brand new one to pucks and all objects
			this->addPuck(2, "../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg");

			//update player 1 score
			updateP1Score(NULL);

			//add the new rigid body
			_pucks[_pucks.size()-1]->addPhysics();
			world->addRigidBody( _pucks[_pucks.size()-1]->getRigidBody() );
			return true;
		}
	}
	//I made it through all of the pucks, none of them won
	return false;
}

bool GameDriver::checkForMysteryBox(btDiscreteDynamicsWorld * world){
	for( unsigned int i = 0; i < _pucks.size(); i++ ){
		float radius = 1.7;

		if (    (_pucks[i]->getModel()[3].x <= radius) // puck is within box
			&&  (_pucks[i]->getModel()[3].x >= -radius)
			&&  (_pucks[i]->getModel()[3].z <= radius) 
			&&  (_pucks[i]->getModel()[3].z >= -radius)){
			
			_isPowerupActive = false;

		// move icon down
			if (_powerup.getCurrentPu() != NULL)
				_powerup.moveCurrentPuDown();

			// spawn new pu
			_powerup.spawnRandPU(_pucks[i]);

			// move ? box down
			_powerup.moveMysteryBoxDown();

			// spawn another puck if player got multi puck
			if (_powerup.isMultiPuck()){
				addPuck(0, "../bin/powerup/puck_red_leaf.obj","../bin/powerup/red_black_yellow_colorbars.jpg");
				_pucks[_pucks.size()-1]->addPhysics();
				world->addRigidBody( _pucks[_pucks.size()-1]->getRigidBody() );

				_powerup.setIsMultiPuck(false);
			}
			return true;
		}
	}
	//I made it through all of the pucks, none of them won
	return false;
}

bool GameDriver::checkForMidBoundry(){
	 //check if player 1 paddle has crossed boundry
	btVector3 physicsDirection;
	if (_player1.getPaddle()->getModel()[3].x < 0.5){
		 physicsDirection  = btVector3(10, 0, 0);
		_player1.getPaddle()->getRigidBody()->setLinearVelocity(physicsDirection);
	}
	if (_player2.getPaddle()->getModel()[3].x > -0.5){
		 physicsDirection  = btVector3(-10, 0, 0);
		_player2.getPaddle()->getRigidBody()->setLinearVelocity(physicsDirection);
	}

	return true;
}

void GameDriver::addNewObj(GLD * incomingGLD){
	_allObjects.push_back(incomingGLD);
}

bool GameDriver::isPowerupActive(){
	return _isPowerupActive;
}

bool GameDriver::isAiActive(){
	return _isAiActive;
}

void GameDriver::setAi(bool incomingAI){
	_isAiActive = incomingAI;
}

int GameDriver::findPuck(){
	unsigned int i = 0;
	for (i = 0; i < (_allObjects.size()); ++i)
		{
			if (_allObjects[i]->getMovement() == DYNAMIC){
				break;
			}
		}

	return i; 
}

void GameDriver::resetGame(btDiscreteDynamicsWorld * world){
	_player1.resetScore();
	_player2.resetScore();
	
	// delete all pucks in game
	for( unsigned int i = 0; i < _pucks.size(); i++ ){
		world->removeRigidBody( _pucks[i]->getRigidBody() );

		//remove it from all Objects and Pucks
		GLD * puckToBeRemoved = _pucks[i];
		for( unsigned int j = 0; j < _allObjects.size(); j++ ){
			if( _allObjects[j] == puckToBeRemoved ){
				_allObjects.erase( _allObjects.begin() + j );
				break;
			}
		}
		_pucks.erase( _pucks.begin() + i);
		delete puckToBeRemoved;
	}

	// add new puck to game
	this->addPuck(1, "../bin/powerup/puck_red_leaf.obj", "../bin/powerup/red_black_yellow_colorbars.jpg");
	_pucks[_pucks.size()-1]->addPhysics();
	world->addRigidBody( _pucks[_pucks.size()-1]->getRigidBody() );
}

void GameDriver::initTableAttributes(){
	_table.getRigidBody()->setRestitution(0);
	_tableWalls.getRigidBody()->setRestitution(1);
}

#endif