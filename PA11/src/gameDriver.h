#ifndef __GAMEDRIVER_H_
#define __GAMEDRIVER_H_

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include "GLD.cpp"
#include "camera.h"

class GameDriver{
	public:
		// initializers
		GameDriver();
		~GameDriver();

		// game changes (teehee)
		void initGame();
		void addBall();

		// board tilters (to tilt the world)
		void tiltOnX( float angle );
		void tiltOnZ( float angle );

		// check for win
		bool checkForWin();
		bool checkIfBallOK();

		// level advancing
		void resetGame(btDiscreteDynamicsWorld * world);
		void printTimeElapsed();
		void pickLevel();


		// printing encalsulators
		std::vector<GLD*> getAllObjects();
		glm::mat4 getMasterTransform();

	private:
		GLD _backGround;
		GLD _casket;
		vector<GLD*> _mazes;
		vector<GLD*> _balls;
		vector<GLD*> _allObjects;
		
		glm::mat4 _empty;

		timeval _startingTime;
		timeval _endingTime;
};

#endif