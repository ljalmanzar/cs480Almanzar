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

	//initialize the static object
	_casket.initialize("../bin/casket.obj", "../bin/metal.jpg");

	//set all the appropriate pointers
	_allObjects.push_back( &_backGround );
	_allObjects.push_back( &_casket );

	//set the master transformation
	_empty = glm::mat4(1.0f);

	//set the starting time
	gettimeofday( &_startingTime, NULL );
}

void GameDriver::addBall(){
	GLD * temp = new GLD( "../bin/planet.obj", "../bin/metal.jpg" );

	//add it to the correct places
	_balls.push_back( temp );
}

void GameDriver::printTimeElapsed() {
	//declare variables
	char buffer[100];
	int cursor = 0;

	//get the new time
	gettimeofday( &_endingTime, NULL );
	int seconds = _endingTime.tv_sec - _startingTime.tv_sec;
	int microseconds = _endingTime.tv_usec - _startingTime.tv_usec;
	if( microseconds < 0 ){
		microseconds += 1000000;
		seconds--;
	}
	microseconds /= 10000;

	//make the string
	snprintf( buffer, 100, "Time Elapsed: [%4i.%.2i]",
		seconds, microseconds );
	string tempStr( buffer );

	//print the amount of time it's taken
		//no program for printing
		glUseProgram(0);
		glColor3f(1,1,1);
		glRasterPos2f( -.95, .95 );
		while( tempStr[cursor] ){
			glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, tempStr[cursor++] );
		}
		cursor = 0;
}

void GameDriver::resetGame(btDiscreteDynamicsWorld * world){
	//set the time
	gettimeofday( &_startingTime, NULL );
	//flush out all of the balls and start over
	for( int i = 0; i < _balls.size(); i++ ){
		delete _balls[i];
	}

	//put in the default ball
	addBall();
}

void GameDriver::tiltOnX( float angle ){
	//angle is given in degrees
	_empty = glm::rotate(
		_empty,
		angle/180.0f*3.14159265f,
		glm::vec3(1.0,0.0,0.0)
		);
	//_backGround.setModel( glm::mat4(1.0f) * glm::inverse(_empty) );
}

void GameDriver::tiltOnZ( float angle ){
	//angle is given in degrees
	_empty = glm::rotate(
		_empty,
		angle/180.0f*3.14159265f,
		glm::vec3(0.0,0.0,1.0)
		);
	//_backGround.setModel( glm::mat4(1.0f) * glm::inverse(_empty) );
}  

bool GameDriver::checkForWin(){

}

bool GameDriver::checkIfBallOK(){

}

void GameDriver::pickLevel(){

}

std::vector<GLD*> GameDriver::getAllObjects(){
	//keep static stuff static

	std::vector<GLD*> temp = _allObjects;
	temp.insert( temp.end(), _mazes.begin(), _mazes.end() );
	temp.insert( temp.end(), _balls.begin(), _balls.end() );
	return temp;
}

glm::mat4 GameDriver::getMasterTransform(){
	return _empty;
}

#endif