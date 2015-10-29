#ifndef __POWERUP_H_
#define __POWERUP_H_

#include <iostream>

#define MAX_ANIMATION_FRAMES 60

// this class is the parent class for all powerup types.s
class Powerup{
	public:
		Powerup();
		~Powerup();
		
		void init();

		void spawnRandPU();

		// getters
		GLD* getMysteryBox();
		GLD* getMultiPuck();
		GLD* getHeavyPuck();
		GLD* getLightPuck();
		GLD* getIceRing();
		GLD* getHealth();
		GLD* getPutinPaddle();

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

		struct {
			bool nothing;
		} _keyframe[ MAX_ANIMATION_FRAMES ];
};

#endif