#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS

#include "logger.h"
#include <cstdio>
#include <assert.h>
#include <time.h>

using namespace std;

bool Logger::isSet() const {
	return set;
}

void Logger::setFile(string filename) {
	set = true;
	stringstream ss;
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%d%m%y%H%M%S", timeinfo);
	ss << PATH << filename << "_" << VERSION << "_" <<  buffer << ".log";
	ss >> fname;
	file.open(fname, std::ios::out | std::ios::trunc);
	
	//assert(file->is_open() == true);
}

void Logger::log(string loc, string msg) {
	if (file.is_open()) {
		file.clear();
		file << "[" << loc << "] " << msg << endl;
		//assert(file->good() == true);
		
	}
}
Logger::~Logger() {
	file.flush();
	file.close();
}

#endif