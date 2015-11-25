#ifndef __GAMEDRIVER_CPP_
#define __GAMEDRIVER_CPP_

#include "gameDriver.h"

GameDriver::GameDriver(){
	//constructor
}

GameDriver::~GameDriver(){
	//deconstructor
}

void GameDriver::initGame(){
	//declare helper variables
	glm::mat4 transformation;

	//initialize the mazes
	string files[] = {
		"../bin/maze_interm_1.obj"
	};
	for( int i = 0; i < 1; i++ ){
		_mazes.push_back( new GLD( files[i], "../bin/metal.jpg" ) );
	}

	//add the default ball
	addBall();

	//initialize the world
	_backGround.initialize("../bin/planet.obj", "../bin/star_map.jpg");
	//make this guy really big
	transformation = glm::scale(
		_backGround.getModel(),
		glm::vec3( 100.0, 100.0, 100.0 ) 
		);
	_backGround.setModel( transformation );

	//set all the appropriate pointers
	_allObjects.push_back( &_backGround );

	//set the master transformaiton
	glm::mat4 _empty = glm::mat4(1.0f);
}

void GameDriver::addBall(){
	GLD * temp = new GLD( "../bin/peeps_model.obj", "../bin/metal.jpg" );

	//add it to the correct places
	_balls.push_back( temp );
}

void GameDriver::printTimeElapsed() const {

}

void GameDriver::resetGame(btDiscreteDynamicsWorld * world){

}

void GameDriver::tiltOnX( float angle ){
	//angle is given in degrees
	_empty = glm::rotate(
		_empty,
		angle/180.0f*3.14159265f,
		glm::vec3(1.0,0.0,0.0)
		);
}

void GameDriver::tiltOnZ( float angle ){
	//angle is given in degrees
	_empty = glm::rotate(
		_empty,
		angle/180.0f*3.14159265f,
		glm::vec3(0.0,0.0,1.0)
		);
}  

bool GameDriver::checkForWin(){

}

bool GameDriver::checkIfBallOK(){

}

void GameDriver::pickLevel(){

}

std::vector<GLD*> GameDriver::getAllObjects(){
	std::vector<GLD*> temp = _allObjects;
	temp.insert( temp.end(), _mazes.begin(), _mazes.end() );
	temp.insert( temp.end(), _balls.begin(), _balls.end() );
	return temp;
}

glm::mat4 GameDriver::getMasterTransform(){
	return _empty;
}

#endif