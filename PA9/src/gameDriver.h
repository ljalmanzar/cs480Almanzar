#ifndef __GAMEDRIVER_H_
#define __GAMEDRIVER_H_

#include "player.cpp"
#include "GLD.cpp"
#include "powerup.cpp"
#include <iostream>

#ifndef __PLAYER_H_
class Player;
#endif

class GameDriver{
	public:
		GameDriver();
		~GameDriver();

		void initGame();

		// setters

		// getters
		Player getPlayer1()const;
		Player getPlayer2()const; 
		std::vector<GLD> getPucks()const;

	private:
		Player _player1;
		Player _player2;

		GLD _table;
		std::vector<GLD> _pucks;
};

#endif