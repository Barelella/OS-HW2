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
#include "Log.h"

const int BANK_STATUS_SLEEP = 3;

bool atmsFinish;

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

void* BankPrintThreadFunction(void* inputBank){
	Bank& bank = *(Bank *) inputBank;
	while(!atmsFinish){
		bank.PrintStatus();
		sleep(BANK_STATUS_SLEEP);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){

	int atmsNum = ValidateInputArguments(argc, argv);
	if(-1 == atmsNum){
		perror("Illegal program arguments\n");
		return 0;
	}

	// Initializations
	atmsFinish = false;
	Log log("log.txt");
	Bank bank(log);

	list<Atm> atm_list;

	cout << "initializations ok" << endl;
	// Create all Atms
	for(int i = 0; i < atmsNum; i++){

		Atm newAtm(bank, i+1, argv[i+2], log);
		atm_list.push_back(newAtm);
	}
	cout << "ATMs created ok" << endl;

	// Run all Atms
	pthread_t atmThreads[atmsNum];
	int i=0;
	for(list<Atm>::iterator it = atm_list.begin(); it != atm_list.end(); ++it){
		cout << "ATM" << i << "created" << endl;
		if(0 != pthread_create(&atmThreads[i], NULL, AtmThreadFunction, (void*) &(*it))){
			perror("Failed to create thread");
			return 0;
		}
		++i;
	}

	// Create bank thread
	pthread_t bankPrintThread;
	if(0 != pthread_create(&bankPrintThread, NULL, BankPrintThreadFunction, (void *)&bank)){
		perror("Failed to create bank printing thread");
		return 0;
	}

	// TODO: create commission threads

	// Run all atms
	for(int i = 0; i < atmsNum; ++i){
		pthread_join(atmThreads[i], NULL);
	}
	atmsFinish = true;

	pthread_join(bankPrintThread, NULL);
	cout << "ATMs finish" << endl;
	// TODO: wait for commission thread

	bank.PrintStatus();		// Print bank final status
	// Account account(2345, "ct", 0);
	return 0;
}
