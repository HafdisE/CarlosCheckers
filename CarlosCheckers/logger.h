#ifndef LOGGER_H
#define LOGGER_H

#define VERSION 0.0
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class Logger {
public:
	Logger() : file(NULL) {};
	/* create instnace of logger and initialise with file filename */
	Logger(string filename);
	~Logger();
	/* log a message from an identifying location */
	void log(string location, string message);
private:
	char fname[128];
	ofstream* file;
	char buf[255];
};

#endif
