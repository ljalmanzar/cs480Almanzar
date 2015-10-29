#ifndef __GAMEDRIVER_H_
#define __GAMEDRIVER_H_

#include <iostream>
#include "player.cpp"
#include "GLD.cpp"
#include "powerup.cpp"


#ifndef __PLAYER_H_
class Player;
#endif

class GameDriver{
	public:
		GameDriver();
		~GameDriver();

		void initGame();
		void addPowerups();

		// setters
		void stepPhysicsAnimation( float dt );

		// getters
		Player getPlayer1()const;
		Player getPlayer2()const; 
		std::vector<GLD> getPucks()const;
		std::vector<GLD*> getAllObjects()const;

	private:
		Player _player1;
		Player _player2;

		Powerup _powerup;

		GLD _table;
		std::vector<GLD> _pucks;
		std::vector<GLD*> _allObjects;
};

#endif