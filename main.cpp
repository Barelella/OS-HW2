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
#include "Account.h"
#include "Atm.h"
#include "Log.h"

const int BANK_STATUS_SLEEP = 500000;
const int BANK_COM_SLEEP = 3;

bool atmsFinish;

//**************************************************************************************
// function name: ValidateInputArguments
// Description: checks if input args ar valid
// Parameters: number of input arguments, argument array
// Returns: -1 if not valid, number of Atms otherwise
//**************************************************************************************
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

//**************************************************************************************
// function name: AtmThreadFunction
// Description: initial function for Atm thread
// Parameters: atm object
// Returns: none
//**************************************************************************************
void* AtmThreadFunction(void* atmInput){
	Atm atm = *(Atm*) atmInput;
	atm.Run();
	pthread_exit(NULL);
}

//**************************************************************************************
// function name: BankPrintThreadFunction
// Description: initial function for bank print thread
// Parameters: bank object
// Returns: none
//**************************************************************************************
void* BankPrintThreadFunction(void* inputBank){
	Bank& bank = *(Bank *) inputBank;
    usleep(BANK_STATUS_SLEEP); // added this so bank won't print status immediately
	while(!atmsFinish){
		bank.PrintStatus();
		usleep(BANK_STATUS_SLEEP);
	}
	pthread_exit(NULL);
}

//**************************************************************************************
// function name: BankComThreadFunction
// Description: initial function for bank commission thread
// Parameters: bank object
// Returns: none
//**************************************************************************************
void* BankComThreadFunction(void* inputBank){
    Bank& bank = *(Bank *) inputBank;
    //sleep(BANK_COM_SLEEP);
    while(!atmsFinish){
        bank.ChargeCommissions();
        sleep(BANK_COM_SLEEP);
    }
    pthread_exit(NULL);
}

//**************************************************************************************
// function name: main
// Description: main function
// Parameters: number of arguments, argument array
// Returns: integer
//**************************************************************************************
int main(int argc, char* argv[]){

	int atmsNum = ValidateInputArguments(argc, argv);
	if(-1 == atmsNum){
		cout << "Illegal program arguments\n" << endl;
		return 0;
	}

	// Initializations
	atmsFinish = false;
	Log log("log.txt");
	Bank bank(log);

	list<Atm> atm_list;

	// Create all Atms
	for(int i = 0; i < atmsNum; i++){

		Atm newAtm(bank, i+1, argv[i+2], log);
		atm_list.push_back(newAtm);
	}

	// Run all Atms
	pthread_t atmThreads[atmsNum];
	int i=0;
	for(list<Atm>::iterator it = atm_list.begin(); it != atm_list.end(); ++it){
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

	//Create commission thread
    pthread_t bankComThread;
    if(0 != pthread_create(&bankComThread, NULL, BankComThreadFunction, (void *)&bank)){
        perror("Failed to create bank commission thread");
        return 0;
    }

	// Run all atms & threads
	for(int i = 0; i < atmsNum; ++i){
		pthread_join(atmThreads[i], NULL);
	}
	atmsFinish = true;

	pthread_join(bankPrintThread, NULL);
    pthread_join(bankComThread, NULL);

	bank.PrintStatus();		// Print bank final status
	return 0;
}
