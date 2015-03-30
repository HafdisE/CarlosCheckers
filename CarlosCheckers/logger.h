#ifndef LOGGER_H
#define LOGGER_H

#define VERSION 0
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class Logger {
public:
	Logger() {};
	~Logger();
	/* log a message from an identifying location */
	void log(string location, string message);
	void setFile(string file);
private:
	char fname[128];
	ofstream file;
	char buf[255];
};

#endif
