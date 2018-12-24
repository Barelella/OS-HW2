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

template <class T>
std::ostream& operator<<(Log& log, T input){
	pthread_mutex_lock(&log.lock);
	std::ostream& resultStream = log.logStream << input.toString();
	pthread_mutex_unlock(&log.lock);
	return resultStream;
}

std::ostream& operator<<(Log& log, int num){
	pthread_mutex_lock(&log.lock);
	std::ostream& resultStream = log.logStream << num;
	pthread_mutex_unlock(&log.lock);
	return resultStream;
}

std::ostream& operator<<(Log& log, double num){
	pthread_mutex_lock(&log.lock);
	std::ostream& resultStream = log.logStream << num;
	pthread_mutex_unlock(&log.lock);
	return resultStream;
}

std::ostream& operator<<(Log& log, char* str){
	pthread_mutex_lock(&log.lock);
	std::ostream& resultStream = log.logStream << str;
	pthread_mutex_unlock(&log.lock);
	return resultStream;
}

std::ostream& operator<<(Log& log, const char* str){
	pthread_mutex_lock(&log.lock);
	std::ostream& resultStream = log.logStream << str;
	pthread_mutex_unlock(&log.lock);
	return resultStream;
}

