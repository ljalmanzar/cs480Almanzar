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
	_mysteryBox.initialize("../bin/capsule.obj"
						 ,"../bin/capsule0.jpg");

	_multiPuck.initialize("../bin/powerup/puck_yellow_leaf.obj"
						 ,"../bin/powerup/red_black_yellow_colorbars.jpg");

	_heavyPuck.initialize("../bin/powerup/weight.obj"
						 ,"../bin/powerup/red_black_yellow_colorbars.jpg");

	_lightPuck.initialize("feather.obj"
						 ,"feather.jpg");

	_iceRing.initialize("../bin/powerup/snowflake.obj"
						 ,"../bin/ice.jpg");

	_health.initialize("../bin/powerup/health.obj"
						 ,"../bin/powerup/firstaid.jpg");

	_putinPaddle.initialize("../bin/powerup/putin.obj"
						 ,"putin.jpg");

	// init powerup starting location

}

void spawnRandPU(){
	srand(time(NULL));

	int randNum = rand()% 100 + 1;

	// 1 <= randNum < 20
	if ((randNum >= 1) && (randNum < 20)){
		// *** GLD ***
		// set pos over ? box
		// show object

		// set properties 
	} 

	// 20 <= randNum < 40
	else if ((randNum >= 20) && (randNum < 40)){
		// *** GLD ***
		// set pos over ? box
		// show object

		// set properties 
	}

	// 40 <= randNum < 60
	else if ((randNum >= 40) && (randNum < 60)){
		// *** GLD ***
		// set pos over ? box
		// show object

		// set properties 
	}
	
	// 40 <= randNum < 60
	else if ((randNum >= 60) && (randNum < 80)){
		// *** GLD ***
		// set pos over ? box
		// show object

		// set properties 
	}
	
	// 40 <= randNum <= 100
	else {
		// *** GLD ***
		// set pos over ? box
		// show object

		// set properties 
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
	return &_iceRing;
}

GLD* Powerup::getPutinPaddle(){
	return &_putinPaddle;
}

#endif