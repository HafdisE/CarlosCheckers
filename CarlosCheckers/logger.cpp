#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS

#include "logger.h"
#include <cstdio>
#include <assert.h>

using namespace std;

void Logger::setFile(string filename) {
	stringstream ss;

	ss << "C:/Logs/" << filename << "_" << VERSION << ".log";
	ss >> fname;
	file.open(fname, std::ios::out | std::ios::trunc);
	
	//assert(file->is_open() == true);
}

void Logger::log(string loc, string msg) {
	if (file.is_open()) {
		file.clear();
		file << "[" << loc << "] " << msg << "\n";
		//assert(file->good() == true);
		
	}
}
Logger::~Logger() {
	file.flush();
	file.close();
}

#endif