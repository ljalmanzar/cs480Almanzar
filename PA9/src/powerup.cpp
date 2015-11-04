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
						 ,"../bin/powerup/Color_icon_yellow.png",true, BOX);

	_multiPuck.initialize("../bin/powerup/puck_yellow_leaf.obj"
						 ,"../bin/powerup/red_black_yellow_colorbars.jpg",
						 true, NONE);

	_heavyPuck.initialize("../bin/powerup/weight.obj"
						 ,"../bin/powerup/red_black_yellow_colorbars.jpg",
						 true, NONE);

	_lightPuck.initialize("../bin/powerup/feather.obj"
						 ,"../bin/powerup/feather.jpg",
						 true, NONE);

	_iceRink.initialize("../bin/powerup/snowflake.obj"
						 ,"../bin/ice.jpg",
						 true, NONE);

	_minusPoint.initialize("../bin/powerup/health.obj"
						 ,"../bin/powerup/firstaid.jpg",
						 true, NONE);

	_putinPaddle.initialize("../bin/powerup/health.obj"
						 ,"../bin/powerup/feather.jpg",
						 true, NONE);

	// init powerup starting location
		// all below the table

	glm::mat4 tempModel = glm::translate( 
        _mysteryBox.getModel(),
        glm::vec3(0.0f, -15.0f, 0.0f) 
        );

	tempModel = glm::scale(tempModel, glm::vec3(0.3));
	_mysteryBox.setModel(tempModel);

	tempModel = glm::translate( 
        _multiPuck.getModel(),
        glm::vec3(0.0f, -15.0f, 0.0f) 
        );
	_multiPuck.setModel(tempModel);

	tempModel = glm::translate( 
        _lightPuck.getModel(),
        glm::vec3(0.0f, -15.0f, 0.0f) 
        );
	_lightPuck.setModel(tempModel);

	tempModel = glm::translate( 
        _iceRink.getModel(),
        glm::vec3(0.0f, -15.0, 0.0) 
        );
	_iceRink.setModel(tempModel);

	tempModel = glm::translate( 
        _minusPoint.getModel(),
        glm::vec3(0.0f, -15.0, 0.0) 
        );
	_minusPoint.setModel(tempModel);

	tempModel = glm::translate( 
        _putinPaddle.getModel(),
        glm::vec3(0.0f, -15.0, 0.0) 
        );
	_putinPaddle.setModel(tempModel);
}	

void Powerup::spawnRandPU(){
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

		// set propertieus 
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

GLD* Powerup::getIceRink(){
	return &_iceRink;
}

GLD* Powerup::getHealth(){
	return &_minusPoint;
}

GLD* Powerup::getPutinPaddle(){
	return &_putinPaddle;
}

#endif