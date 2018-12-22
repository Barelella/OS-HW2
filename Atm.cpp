/*
 * Atm.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#include "Atm.h"

const int MAX_ARG = 15;

Atm::Atm(Bank& bank, int serialNumber, string fileName, ofstream& log) :
	bank(bank), serialNumber(serialNumber), fileName(fileName), log(log){

}

Atm::~Atm() {

}

void Atm::Run(){
	cout << "Atm no. " << serialNumber << " runs with input file " << fileName << endl;
	std::ifstream file(fileName.c_str());
	string currentCommand;

	while(std::getline(file, currentCommand)){
		// Convert command from string to char*
		int commandLength = currentCommand.length();
		char* command = new char[commandLength + 1];
		strcpy(command, currentCommand.c_str());

		// Parse command (using parser from HW1)
		char* args[MAX_ARG];
		const char* delimiters = " \t\n";
		int num_arg = 0;
		char* cmd = strtok(command, delimiters);
		if(NULL == cmd){
			delete[] command;
			continue;
		}
		args[0] = cmd;
		for(int i = 1; i < MAX_ARG; ++i){
			args[i] = strtok(NULL, delimiters);
			if(NULL != args[i]){
				++num_arg;
			}
		}

		// Check current command and direct to relevant function
		char cmdChar = cmd[0];
		switch (cmdChar){
		case 'O':
			bank.CreateAccount(atoi(args[1]), args[2], atoi(args[3]));
			// TODO: log, sleep, and a bunch of stuff
			break;
		case 'L':
			// TODO: vip account
			break;
		case 'D':
			// TODO: deposit
			break;
		case 'W':
			// TODO: withdraw
			break;
		case 'B':
			// TODO: check balance
			break;
		case 'T':
			// TODO: transfer money
			break;
		default:
			// TODO: illegal action (not needed)
			break;
		}
		delete[] command;
	}
}
