/*
 * Log.cpp
 *
 *  Created on: Dec 24, 2018
 *      Author: nadav
 */

#include "Log.h"

//**************************************************************************************
// function name: Log
// Description: constructor
// Parameters: log file name
// Returns: log object
//**************************************************************************************
Log::Log(const char* fileName) {
	logStream.open(fileName);
	pthread_mutex_init(&lock, NULL);

}

//**************************************************************************************
// function name: ~Log
// Description: destructor
// Parameters: none
// Returns: none
//**************************************************************************************
Log::~Log() {
	logStream.close();
	pthread_mutex_destroy(&lock);
}

//**************************************************************************************
// function name: Write
// Description: writes a string to log
// Parameters: string
// Returns: none
//**************************************************************************************
void Log::Write(std::string str){
	pthread_mutex_lock(&lock);
	logStream << str;
	pthread_mutex_unlock(&lock);
}

//**************************************************************************************
// function name: Write
// Description: writes a string to log and starts a new line
// Parameters: string
// Returns: none
//**************************************************************************************
void Log::WriteLine(std::string str){
	pthread_mutex_lock(&lock);
	logStream << str << std::endl;
	pthread_mutex_unlock(&lock);
}

//**************************************************************************************
// function name: toString
// Description: converts an ofstream to string
// Parameters: ofstream object
// Returns: string
//**************************************************************************************
std::string toString(std::ofstream& ofstream){
	std::ostringstream ss;
	ss << ofstream.rdbuf();
	return ss.str();
}

//**************************************************************************************
// function name: WriteLine
// Description: writes an sstream to log
// Parameters: sstream object
// Returns: none
//**************************************************************************************
void Log::WriteLine(std::stringstream& sstream){
	WriteLine(sstream.str());
}
