#ifndef __GAMEDRIVER_CPP_
#define __GAMEDRIVER_CPP_

#include "gameDriver.h"

GameDriver::GameDriver(){
	//constructor
}

GameDriver::~GameDriver(){
	//deconstructor
	//destroy everything
	for( unsigned int i = 0; i < _mazes.size(); i++ ){
		delete _mazes[i];
	}
	for( unsigned int i = 0; i < _balls.size(); i++ ){
		delete _balls[i];
	}
}

void GameDriver::initGame( btDiscreteDynamicsWorld * incomingWorld ){
	//set incoming variables
	_world = incomingWorld;

	//declare helper variables
	glm::mat4 transformation;

	//initialize the mazes
	_maze_index = 0;
	vector<string> files = {
		"../bin/maze_easy.obj"
	};
	for( unsigned int i = 0; i < files.size(); i++ ){
		GLD * temp = new GLD;
		temp->initialize( files[i], "../bin/metal.jpg", true, TRIMESH, KINEMATIC );
		_mazes.push_back( temp );
//		_mazes.push_back( new GLD( files[i], "../bin/metal.jpg", true, TRIMESH, KINEMATIC ) );
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
	_casket.initialize("../bin/casket.obj", "../bin/deathStar2.jpg");//, true, TRIMESH, KINEMATIC );
	_casket.translate(glm::vec3(0,-1.25,0));

	//set all the appropriate pointers
	_allObjects.push_back( &_casket );
	_allObjects.push_back( &_backGround );

	//set the master transformation
	_empty = glm::mat4(1.0f);

	//set the starting time
	gettimeofday( &_startingTime, NULL );
}

void GameDriver::addBall(){
	//creating a new ball
	GLD * temp = new GLD;
	temp->initialize( "../bin/planet.obj", "../bin/deathStar.jpg", true, SPHERE, DYNAMIC );

	//setting initial position
	temp->translate(glm::vec3(0,10,0));
	glm::mat4 translation = glm::scale( temp->getModel(), glm::vec3(.0001, .0001, .0001) );
	temp->setModel( translation );
	
	//temp->setShape(SPHERE);

	temp->addPhysics();

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

void GameDriver::resetGame(){
	//reset the time
	gettimeofday( &_startingTime, NULL );
	//flush out all of the balls and start over
	for( unsigned int i = 0; i < _balls.size(); i++ ){
		//how many balls?

		//remove the rigid body
		_world->removeRigidBody( _balls[i]->getRigidBody() );
		delete _balls[i];
	}
	//clear out everything in that array
	_balls.clear();

	//set the empty back to normal
	_empty = glm::mat4(1.0f);

	//put in the default ball
	addBall();
	//add in the new rigid body
	_world->addRigidBody( _balls[0]->getRigidBody() );
}

glm::vec3 GameDriver::tiltOnX( float angle ){
	//angle is given in degrees
	_empty = glm::rotate(
		_empty,
		angle/90.0f,
		glm::vec3(1.0,0.0,0.0)
		);
	glm::mat3 rotationMatrix( _empty );

/*
	//tilt the mazes accordingly
	for( unsigned int i = 0; i < _mazes.size(); i++ ){
		btTransform trans;
		trans.setFromOpenGLMatrix( glm::value_ptr(_empty) );
		//trans set origin
		//trans set orientation
		_mazes[i]->getRigidBody()->setWorldTransform(trans);
		cout << "debug" << endl;
//		_mazes[i]->getRigidBody()->getMotionState()->getWorldTransform(trans);
	}
*/


	//use newGravity to update shit
	return rotationMatrix * glm::vec3(0.0, -9.81, 0.0);
}

glm::vec3 GameDriver::tiltOnZ( float angle ){
	//angle is given in degrees
	_empty = glm::rotate(
		_empty,
		angle/90.0f,
		glm::vec3(0.0,0.0,1.0)
		);
	
	glm::mat3 rotationMatrix( _empty );

	//use newGravity to update shit
	return rotationMatrix * glm::vec3(0.0, -9.81, 0.0);
}  

bool GameDriver::checkForWin(){
	return false;
}

bool GameDriver::checkIfBallOK(){
	//is the balls position too low?
	if( _balls.size() == 1 && _balls[0]->getModel()[3][1] > -5 ){
		//if it is, reset the positions
		resetGame();
	}
	return false;
}

void GameDriver::pickLevel(){

}

std::vector<GLD*> GameDriver::getAllObjects(){
	//update all of the GLDs

	//keep static stuff static

	std::vector<GLD*> temp = _allObjects;
	temp.push_back( _mazes[_maze_index] );
	temp.insert( temp.end(), _balls.begin(), _balls.end() );
	return temp;
}

glm::mat4 GameDriver::getMasterTransform(){
	return _empty;
}

std::string GameDriver::getFinalTime(){
	//declare variables
	char buffer[100];

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
	snprintf( buffer, 100, "%4i.%.2i",
		seconds, microseconds );
	string tempStr( buffer );

	return tempStr;
}

#endif