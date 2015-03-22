#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Logger {
public:
	/* create instnace of logger and initialise with file filename */
	Logger(string filename);
	~Logger();
	/* log a message from an identifying location */
	void log(string location, string message);
private:
	string fname;
	ofstream file;
	char buf[255];
};
