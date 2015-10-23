#ifndef __POWERUP_H_
#define __POWERUP_H_

#include <iostream>

// this class is the parent class for all powerup types.s
class Powerup{
	public:
		Powerup();
		~Powerup();
		
		void init();

		void spawnRandPU();

		// getters

		// setters

	private:

		// powerup hub
		GLD _mysteryPU;

		// powerup types
		GLD _multiPuck;
		GLD _heavyPuck;
		GLD _lightPuck;
		GLD _iceRing;
		GLD _putinPaddle;
};

#endif