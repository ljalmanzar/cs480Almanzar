#ifndef __POWERUP_H_
#define __POWERUP_H_

#include <iostream>
#include "GLD.h"
#include "player.h"
#define MAX_ANIMATION_FRAMES 60

// this class is the parent class for all powerup types.s
class Powerup{
	public:
		Powerup();
		~Powerup();
		
		void init(Player*, Player*, GLD*);

		bool spawnRandPU(GLD*);

		// getters
		GLD* getMysteryBox();
		GLD* getMultiPuck();
		GLD* getHeavyPuck();
		GLD* getLightPuck();
		GLD* getIceRink();
		GLD* getHealth();
		GLD* getPutinPaddle();

		void moveMysteryBoxUp();
		void moveMysteryBoxDown();

		// setters

	private:

		// powerup hub
		GLD _mysteryBox;

		// powerup types
		GLD _multiPuck;
		GLD _heavyPuck;
		GLD _lightPuck;
		GLD _iceRink;
		GLD _minusPoint;
		GLD _putinPaddle;

		Player* _p1;
		Player* _p2;
		GLD* _table;

		struct {
			bool nothing;
		} _keyframes [ MAX_ANIMATION_FRAMES ];
};

#endif