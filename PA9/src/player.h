#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "GLD.cpp"
#include <iostream>

class Player{
	public:
		// constructor
		Player();
		~Player();

		void init();

		// setters
		void incrementScore(int amount);
		void setScore(int score);
		void resetScore();
		void setPlayerNumber(int);
		void setPaddlePos(int, int);

		//getters 
		int getScore() const;
		GLD* getPaddle();

	private:
		int _score;
		GLD _paddle; 
		int _playerNumber;
};

#endif