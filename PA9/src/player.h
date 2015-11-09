#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "GLD.cpp"
#include <iostream>
#include "camera.h"
#include "mousePicker.h"

enum PaddleDirection {
	D_UP,
	D_DOWN,
	D_LEFT,
	D_RIGHT,
	D_UP_LEFT,
	D_UP_RIGHT,
	D_DOWN_LEFT,
	D_DOWN_RIGHT
};

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
		void setPlayerNumber(int playerNumber);
		void setPaddlePosMouse(MousePicker, Camera*);
		void setPaddlePosKey(PaddleDirection key, Camera*);
		void resetPaddlePosition();
		//getters 
		int getScore() const;
		GLD* getPaddle();

	private:
		int _score;
		GLD _paddle; 
		int _playerNumber;

		float _horizontalAngle;
		float _verticalAngle;

		float _oldX;
		float _oldY;

		float _myX;
		float _myY;
		glm::vec3 _position;
};

#endif