#include <iostream>
#include <fstream>
#include "scoreBoard.h"

ScoreBoard::ScoreBoard(){
	easyName = "easyTopScores.gld";
	mediumName = "mediumTopScores.gld";
	hardName = "hardTopScores.gld";
}

ScoreBoard::~ScoreBoard(){

}

/** Get all high scores dependent on the difficulty level */ 
std::vector<std::string> ScoreBoard::getHighScores(const int difficulty){
	std::string *fileName;
	ifstream fin;
	std::string line;
	std::vector<std::string> scoresVector;

	/** determine difficulty*/
	switch(difficulty){
		case 1:
			fileName = &easyName;
			break;
		case 2:
			fileName = &mediumName;
			break;
		case 3:
			fileName = &hardName;
			break;
	}

	/** Open file */

	fin.clear();

    fin.open( fileName->c_str() );
    if (!fin.good()){
        std::cerr << "ERROR: Cannot find file: "
                   << fileName << std::endl;
    }
        
    /** Get all scores */
    while (fin.good()){
        std::getline(fin, line);
        scoresVector.push_back(line);
    }

    return scoresVector;
}

void ScoreBoard::saveScore(int difficulty, string score){
	std::string *fileName;
	ofstream fout;

	switch(difficulty){
		case 1:
			fileName = &easyName;
			break;
		case 2:
			fileName = &mediumName;
			break;
		case 3:
			fileName = &hardName;
			break;
	}

	fout.open(fileName->c_str());

    fout.seekp(-1,ios_base::end);
    
}