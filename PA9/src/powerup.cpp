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
						 ,"../bin/powerup/Color_icon_yellow.png",true, NONE);

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
						 ,"../bin/blue_while_red.png",
						 true, NONE);

	_putinPaddle.initialize("../bin/powerup/health.obj"
						 ,"../bin/powerup/feather.jpg",
						 true, NONE);

	// init powerup starting location
		// all below the table

	glm::mat4 tempModel = glm::translate( 
        _mysteryBox.getModel(),
        glm::vec3(0.0f, -1.0f, 0.0f) 
        );

	tempModel = glm::scale(tempModel, glm::vec3(0.25));
	_mysteryBox.setModel(tempModel);

	tempModel = glm::translate( 
        _multiPuck.getModel(),
        glm::vec3(0.0f, -15.0f, 0.0f) 
        );
	_multiPuck.setModel(tempModel);

	tempModel = glm::translate( 
        _lightPuck.getModel(),
        glm::vec3(0.0f, -12.0f, 0.0f) 
        );
	_lightPuck.setModel(tempModel);

	tempModel = glm::translate( 
        _iceRink.getModel(),
        glm::vec3(0.0f, -9.0, 0.0) 
        );
	_iceRink.setModel(tempModel);

	tempModel = glm::translate( 
        _minusPoint.getModel(),
        glm::vec3(0.0f, -6.0, 0.0) 
        );
	_minusPoint.setModel(tempModel);

	tempModel = glm::translate( 
        _putinPaddle.getModel(),
        glm::vec3(0.0f, -3.0, 0.0) 
        );
	_putinPaddle.setModel(tempModel);
}	

bool Powerup::spawnRandPU(GLD* puck){
	srand(time(NULL));

	int randNum = rand()% 120 + 1;

	glm::mat4 tempModel;

	// 1 <= randNum < 20
	if ((randNum >= 1) && (randNum < 20)){
		_multiPuck.anim_MoveUp(30.0);

	} 

	// 20 <= randNum < 40
	else if ((randNum >= 20) && (randNum < 40)){
		// show powerup
		_lightPuck.anim_MoveUp(30.0);
	}

	// 40 <= randNum < 60
	else if ((randNum >= 40) && (randNum < 60)){
		_iceRink.anim_MoveUp(30.0);

	}
	
	// 40 <= randNum < 60
	else if ((randNum >= 60) && (randNum < 80)){
		_minusPoint.anim_MoveUp(30.0);

	}
	
	// 40 <= randNum <= 100
	else  if ((randNum >= 80) && (randNum < 100)){
		_heavyPuck.anim_MoveUp(30.0);

		// update score
	}
	else{
		_putinPaddle.anim_MoveUp(30.0);
	}
	return true;
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

void Powerup::moveMysteryBoxUp(){
	_mysteryBox.anim_MoveUp(30.0);
}

void Powerup::moveMysteryBoxDown(){
	_mysteryBox.anim_MoveDown(30.0);
}
#endif