#ifndef __SCOREBOARD_H_
#define __SCOREBOARD_H_

#include <iostream>

class ScoreBoard{
public:
	ScoreBoard();
	~ScoreBoard();

	std::vector<std::string> getHighScores(const int);
	void saveScore(int, string);


private:

	std::string easyName;
	std::string mediumName;
	std::string hardName;
};

#endif