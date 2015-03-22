#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Logger {
public:
	Logger(string filename);
	~Logger();
	void log(string location, string message);
private:
	string fname;
	ofstream file;
	char buf[255];
};
