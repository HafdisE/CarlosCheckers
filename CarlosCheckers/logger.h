#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "defines.h"

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
