#include "logger.h"
#include <cstdio>

using namespace std;

Logger::Logger(string filename) {
	fname = filename;
	file.open(fname);
}

void Logger::log(string loc, string msg) {	
	int n = sprintf_s(buf, "[%s] %s\n", loc, msg);
	file.write(buf, n);
}
Logger::~Logger() {
	file.close();
}