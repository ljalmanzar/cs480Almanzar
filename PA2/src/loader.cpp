#include <fstream>
#include <sstream>
#include <string>
using namespace std;

string shaderLoader(const char* fileName)
{
	// open file
	ifstream fin(fileName);

	// create stream object
	stringstream sStream;

	// read in the entire file
	sStream << fin.rdbuf();

	// save the string for return
	string str = sStream.str();

	return str;
}