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
		GLD* getMysteryBox()const;
		GLD* getMultiPuck()const;
		GLD* getHeavyPuck()const;
		GLD* getLightPuck()const;
		GLD* getIceRing()const;
		GLD* getHealth()const;
		GLD* getPutinPaddle()const;

		// setters

	private:

		// powerup hub
		GLD _mysteryBox;

		// powerup types
		GLD _multiPuck;
		GLD _heavyPuck;
		GLD _lightPuck;
		GLD _iceRing;
		GLD _health;
		GLD _putinPaddle;
};

#endif