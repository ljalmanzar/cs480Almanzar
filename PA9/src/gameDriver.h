#ifndef __GAMEDRIVER_H_
#define __GAMEDRIVER_H_

#include <iostream>
#include "player.cpp"
#include "GLD.cpp"
#include "powerup.cpp"
#include "camera.h"


#ifndef __PLAYER_H_
class Player;
#endif

class GameDriver{
	public:
		GameDriver();
		~GameDriver();

		void initGame();
		void updateP1Score(GLD* puck);
		void updateP2Score(GLD* puck);
		void printScores() const;

		//game logic checkers
		bool checkForGoal( btDiscreteDynamicsWorld * world );

		// setters
		void stepPhysicsAnimation( float dt );
		void setP1PaddlePos(PaddleDirection, Camera*);
		void setP1PaddleMouse(glm::vec3, Camera* camera);
		void setP2PaddlePos(PaddleDirection, Camera* );
		void addPuck(int side, const std::string &objFile, const std::string &textureFile);

		// getters
		Player* getPlayer1();
		Player* getPlayer2(); 
		std::vector<GLD*>* getPucks();
		Powerup* getPowerup();
		GLD* getTable();

		std::vector<GLD*> getAllObjects();

		bool getPU();
		void activateMysteryBox();
		void activatePowerUp();
	

	private:
		Player _player1;
		Player _player2;

		Powerup _powerup;

		GLD _table;
		GLD _gamePuck;
		GLD _backGround;
		
		std::vector<GLD*> _pucks;
		std::vector<GLD*> _allObjects;

		bool _activePowerUp;
};

#endif