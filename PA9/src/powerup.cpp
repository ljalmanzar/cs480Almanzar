#ifndef __POWERUP_CPP_
#define __POWERUP_CPP_

#include "powerup.h"
#include <time.h>

Powerup::Powerup(){

}

Powerup::~Powerup(){

}

void Powerup::init(){

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

#endif