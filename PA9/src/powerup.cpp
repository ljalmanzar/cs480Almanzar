#ifndef __POWERUP_CPP_
#define __POWERUP_CPP_

#include "powerup.h"
#include <time.h>

Powerup::Powerup(){

}

Powerup::~Powerup(){

}

void Powerup::init(Player* p1, Player* p2, GLD* table){

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

	_p1 = p1;
	_p2 = p2;
	_table = table;
}	

bool Powerup::spawnRandPU(GLD* puck){
	srand(time(NULL));

	int randNum = rand()% 5;

	glm::mat4 tempModel;

	if (randNum == 0){
		_multiPuck.anim_MoveUp(30.0);
		cout << "MULTI PUCK" << endl;
	} 

	else if (randNum == 1){
		// show powerup
		_lightPuck.anim_MoveUp(30.0);
		puck->getRigidBody()->setMassProps(.1,btVector3(0,0,0));
		puck->getRigidBody()->setRestitution(10);
	}

	else if (randNum == 2){
		_iceRink.anim_MoveUp(30.0);
		cout << "ICE RING" << endl;
		_table->getRigidBody()->setFriction(0);
	}
	
	else if (randNum == 3){
		_minusPoint.anim_MoveUp(30.0);
		cout << "HEALTH " << endl;
		_p1->incrementScore(-1);
		_p2->incrementScore(-1);
	}
	
	else  if (randNum == 4) {
		_heavyPuck.anim_MoveUp(30.0);
		cout << "HEAVY PUCK" << endl;
		puck->getRigidBody()->setFriction(10);

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