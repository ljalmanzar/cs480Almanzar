#include "GLD.cpp"

class Player{
	public:
		// constructor
		Player();
		~Player();

		// setters
		void incrementScore(int amount);
		void setScore(int score);
		void resetScore();
		void setPaddle(GLD paddle);

		//getters 
		int getScore() const;
		GLD getPaddle() const;

	private:
		int _score;
		GLD _paddle;
};