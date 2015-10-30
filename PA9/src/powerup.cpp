#ifndef __POWERUP_CPP_
#define __POWERUP_CPP_

#include "powerup.h"
#include <time.h>

Powerup::Powerup(){

}

Powerup::~Powerup(){

}

void Powerup::init(){

	// init powerups
	// (geometry file, texture file) 
	_mysteryBox.initialize("../bin/powerup/mario_mystery_box.obj"
						 ,"../bin/powerup/Color_icon_yellow.png");

	_multiPuck.initialize("../bin/powerup/puck_yellow_leaf.obj"
						 ,"../bin/powerup/red_black_yellow_colorbars.jpg");

	_heavyPuck.initialize("../bin/powerup/weight.obj"
						 ,"../bin/powerup/red_black_yellow_colorbars.jpg");

	_lightPuck.initialize("feather.obj"
						 ,"feather.jpg");

	_iceRink.initialize("../bin/powerup/snowflake.obj"
						 ,"../bin/ice.jpg");

	_minusPoint.initialize("../bin/powerup/health.obj"
						 ,"../bin/powerup/firstaid.jpg");

	_putinPaddle.initialize("../bin/powerup/putin.obj"
						 ,"putin.jpg");

	// init powerup starting location
		// all below the table

	glm::mat4 tempModel = glm::translate( 
        _mysteryBox.getModel(),
        glm::vec3(0.0f, -1.0, 0.0) 
        );
	_mysteryBox.setModel(tempModel);

	tempModel = glm::translate( 
        _multiPuck.getModel(),
        glm::vec3(0.0f, -1.0, 0.0) 
        );
	_multiPuck.setModel(tempModel);

	tempModel = glm::translate( 
        _lightPuck.getModel(),
        glm::vec3(0.0f, -1.0, 0.0) 
        );
	_lightPuck.setModel(tempModel);

	tempModel = glm::translate( 
        _iceRink.getModel(),
        glm::vec3(0.0f, -1.0, 0.0) 
        );
	_iceRink.setModel(tempModel);

	tempModel = glm::translate( 
        _minusPoint.getModel(),
        glm::vec3(0.0f, -1.0, 0.0) 
        );
	_minusPoint.setModel(tempModel);

	tempModel = glm::translate( 
        _putinPaddle.getModel(),
        glm::vec3(0.0f, -1.0, 0.0) 
        );
	_putinPaddle.setModel(tempModel);
}	

void spawnRandPU(){
	srand(time(NULL));

	int randNum = rand()% 120 + 1;

	glm::mat4 tempModel;

	// 1 <= randNum < 20
	if ((randNum >= 1) && (randNum < 20)){
		// *** GLD ***
		// set pos over ? box
		// show object
		tempModel = glm::translate( 
        _mysteryBox.getModel(),
        glm::vec3(0.0f, 2.0, 0.0) 
        );
		_mysteryBox.setModel(tempModel);

		// set properties 
	} 

	// 20 <= randNum < 40
	else if ((randNum >= 20) && (randNum < 40)){
		// *** GLD ***
		// set pos over ? box
		// show object
		tempModel = glm::translate( 
        _multiPuck.getModel(),
        glm::vec3(0.0f, 2.0, 0.0) 
        );
		_multiPuck.setModel(tempModel);

		// set properties 
	}

	// 40 <= randNum < 60
	else if ((randNum >= 40) && (randNum < 60)){
		// *** GLD ***
		// set pos over ? box
		// show object

		tempModel = glm::translate( 
        _lightPuck.getModel(),
        glm::vec3(0.0f, 2.0, 0.0) 
        );
		_lightPuck.setModel(tempModel);

		// set props
	}
	
	// 40 <= randNum < 60
	else if ((randNum >= 60) && (randNum < 80)){
		// *** GLD ***
		// set pos over ? box
		// show object
		tempModel = glm::translate( 
        _iceRink.getModel(),
        glm::vec3(0.0f, 2.0, 0.0) 
        );
		_iceRink.setModel(tempModel);

		// set properties 
	}
	
	// 40 <= randNum <= 100
	else  if ((randNum >= 80) && (randNum < 100)){
		// *** GLD ***
		// set pos over ? box
		// show object
		tempModel = glm::translate( 
        _minusPoint.getModel(),
        glm::vec3(0.0f, 2.0, 0.0) 
        );
		_minusPoint.setModel(tempModel);

		// set properties 
	}
	else{
		tempModel = glm::translate( 
        _putinPaddle.getModel(),
        glm::vec3(0.0f, 2.0, 0.0) 
        );
		_putinPaddle.setModel(tempModel);
	}
}

// getters
GLD* Powerup::getMysteryBox(){
	return &_mysteryBox;
}

GLD* Powerup::getMultiPuck(){
	return &_multiPuck;
}

GLD* Powerup::getHeavyPuck(){
	return &_heavyPuck;
}

GLD* Powerup::getLightPuck(){
	return &_lightPuck;
}

GLD* Powerup::getIceRing(){
	return &_iceRink;
}

GLD* Powerup::getHealth(){
	return &_minusPoint;
}

GLD* Powerup::getPutinPaddle(){
	return &_putinPaddle;
}

#endif