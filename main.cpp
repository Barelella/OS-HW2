/*
 * main.cpp
 *
 *  Main source file of the project, runs the whole bank project
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <pthread.h>

#include "Bank.h"
#include "Atm.h"

int ValidateInputArguments(int argc, char* argv[]){

	if(argc <= 1){		// Not enough input arguments
		return -1;
	}

	int atmsNum = atoi(argv[1]);
	if(0 >= atmsNum){			// atoi failed, or negative ATMs number
		return -1;
	}

	if(atmsNum != (argc - 2)){	// N and number of files doesn't match
		return -1;
	}

	return atmsNum;
}

void* AtmThreadFunction(void* atmInput){
	Atm atm = *(Atm*) atmInput;
	atm.Run();
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){

	int atmsNum = ValidateInputArguments(argc, argv);
	if(-1 == atmsNum){
		perror("Illegal program arguments\n");
		return 0;
	}

	// Initializations
	Bank bank;
	ofstream log;
	log.open("log.txt");
	list<Atm> atm_list;

	// Create all Atms
	for(int i = 0; i < atmsNum; i++){
		Atm newAtm(bank, i+1, argv[i+2], log);
		atm_list.push_back(newAtm);
	}

	// Run all Atms
	pthread_t* atmThreads = new pthread_t[atmsNum];
	int i=0;
	for(list<Atm>::iterator it = atm_list.begin(); it != atm_list.end(); ++it){
		if(0 != pthread_create(&atmThreads[i], NULL, AtmThreadFunction, (void*) &(*it))){
			perror("Failed to create thread");
			delete[] atmThreads;
			return 0;
		}
		++i;
	}

	// TODO: create printing and commission threads

	// Run all atms
	for(int i = 0; i < atmsNum; ++i){
		pthread_join(atmThreads[i], NULL);
	}

	// TODO: wait for printing and commission thread

	delete[] atmThreads;
	// Account account(2345, "ct", 0);
	return 0;
}

