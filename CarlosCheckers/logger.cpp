#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS

#include "logger.h"
#include <cstdio>
#include <assert.h>

using namespace std;

Logger::Logger(string filename) {
	stringstream ss;

	ss << "C:/Logs/" << filename << "_" << VERSION << ".log";
	ss >> fname;
	file = new ofstream(fname);
	file->open(fname, std::ios::out | std::ios::trunc);
	//assert(file->is_open() == true);
}

void Logger::log(string loc, string msg) {
	if (file) {
		*(file) << "[" << loc << "] " << msg << "\n";
		//assert(file->good() == true);		
	}
}
Logger::~Logger() {
	if (file) {
		file->flush();
		file->close();
		delete file;
	}
}

#endif