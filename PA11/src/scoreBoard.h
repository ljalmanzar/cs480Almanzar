#ifndef __SCOREBOARD_H_
#define __SCOREBOARD_H_

#include <iostream>

class ScoreBoard{
public:
	ScoreBoard();
	~ScoreBoard();

	/** gets high scores from data sheet, dependent on difficulty int */
	std::vector<std::string> getHighScores(Difficulty);

	/** Saves the passed score to file based on difficulty level */
	void saveScore(Difficulty, string);

	std::string getPlayerScore();
private:

	std::string easyName;
	std::string mediumName;
	std::string hardName;

	std::string playerScore;
};

#endif