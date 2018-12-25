/*
 * Log.cpp
 *
 *  Created on: Dec 24, 2018
 *      Author: nadav
 */

#include "Log.h"

Log::Log(const char* fileName) {
	logStream.open(fileName);
	pthread_mutex_init(&lock, NULL);

}

Log::~Log() {
	logStream.close();
	pthread_mutex_destroy(&lock);
}

void Log::Write(std::string str){
	pthread_mutex_lock(&lock);
	logStream << str;
	pthread_mutex_unlock(&lock);
}

void Log::WriteLine(std::string str){
	pthread_mutex_lock(&lock);
	logStream << str << std::endl;
	pthread_mutex_unlock(&lock);
}

std::string toString(std::ofstream& ofstream){
	std::ostringstream ss;
	ss << ofstream.rdbuf();
	return ss.str();
}

void Log::WriteLine(std::stringstream& sstream){
	WriteLine(sstream.str());
}
