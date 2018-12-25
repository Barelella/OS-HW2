/*
 * Atm.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#include "Atm.h"

const int MAX_ARG = 15;
const unsigned int ATM_USLEEP = 100000;

Atm::Atm(Bank& bank, int serialNumber, string fileName, Log& log) :
	bank(bank), serialNumber(serialNumber), fileName(fileName), log(log){

}

Atm::~Atm() {

}

Action CommandToAction(char commandChar){
	switch(commandChar){
	case 'O':
		return CREATE_NEW_ACCOUNT;
	case 'L':
		return VIP_ACCOUNT;
	case 'D':
		return DEPOSIT;
	case 'W':
		return WITHDRAW;
	case 'B':
		return CHECK_BALANCE;
	case 'T':
		return TRANSFER;
	default:
		return ILLEGAL_ACTION;
	}
}

void Atm::WriteToLog(Action action, char* args[], Result error){
	stringstream aux;
	switch(error){
		case ACCOUNT_DOESNT_EXIST:
			aux << "Error " << serialNumber << ": Your transaction failed - ";
			aux << "account id " << args[1] << " does not exist";
			break;
		case ACCOUNT_ALREADY_EXISTS:
			aux << "Error " << serialNumber << ": Your transaction failed - ";
			aux << "account with the same id exists";
			break;
		case PASSWORD_FAIL:
			aux << "Error " << serialNumber << ": Your transaction failed - ";
			aux << "Password for account id " << args[1] << " is incorrect";
			break;
		case AMOUNT_FAIL:
			aux << "Error " << serialNumber << ": Your transaction failed - ";
			aux << "account id " << args[1] << " balance is lower than " ;
			aux << args[(action == TRANSFER) ? 4 : 3];
			break;
		case INITIAL_BALANCE_FAIL:
			aux << "Error " << serialNumber << ": Your transaction failed - ";
			aux << "account id " << args[1] << " initial balance is lower than 0";
			break;
		case TRANSFER_TARGET_DOESNT_EXIST:
			aux << "Error " << serialNumber << ": Your transaction failed - ";
			aux << "account id " << args[3] << " does not exist";
			break;

		case SUCCESS:
			switch(action){
				case CREATE_NEW_ACCOUNT:
					aux << serialNumber << ": New account id is " << args[1];
					aux << " with password " << args[2];
					aux << " and initial balance " << args[3];
					break;
				case VIP_ACCOUNT:
					break;
				case DEPOSIT:
					aux << serialNumber << ": Account " << args[1] << " new balance is ";
					aux <<  bank.GetAccount(atoi(args[1])).GetBalance(args[2], false);
					aux << " after " << args[3] << " $ was deposited";
					break;
				case WITHDRAW:
					aux << serialNumber << ": Account " << args[1] << " new balance is ";
					aux <<  bank.GetAccount(atoi(args[1])).GetBalance(args[2], false);
					aux << " after " << args[3] << " $ was withdrew";
					break;
				case CHECK_BALANCE:
					aux << serialNumber << ": Account " << args[1] << " balance is ";
					aux <<  bank.GetAccount(atoi(args[1])).GetBalance(args[2], false);
					break;
				case TRANSFER:
                    Account src_acc = bank.GetAccount(atoi(args[1]));
                    Account dst_acc = bank.GetAccount(atoi(args[3]));
					aux << serialNumber << ": Transfer " << args[4] << " from account ";
					aux << args[1] << " to account " << args[3];
					aux << " new account balance is " ;
					aux <<  src_acc.GetBalance(args[2], false);
					aux << " new target account balance is ";
					aux <<  dst_acc.GetBalance(dst_acc.GetPassword(), false); //target account balance
					break;
				default:
					break;
			}
			break;

		default:
			break;

	}

	if(aux.str().size() > 0){
		log.WriteLine(aux);
	}

}

void Atm::Run(){
	// cout << "Atm no. " << serialNumber << " runs with input file " << fileName << endl;
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
		Result commandResult;
		Action currentAction = CommandToAction(cmd[0]);
		switch (currentAction){
		case CREATE_NEW_ACCOUNT:
			commandResult = bank.CreateAccount(atoi(args[1]), args[2], atoi(args[3]));
			WriteToLog(CREATE_NEW_ACCOUNT, args, commandResult);
			break;
		case VIP_ACCOUNT:
			commandResult = bank.MakeVip(atoi(args[1]), args[2]);
			WriteToLog(VIP_ACCOUNT, args, commandResult);
			break;
		case DEPOSIT:
			commandResult = bank.Deposit(atoi(args[1]), args[2], atoi(args[3]));
			WriteToLog(DEPOSIT, args, commandResult);
			break;
		case WITHDRAW:
			commandResult = bank.Withdraw(atoi(args[1]), args[2], atoi(args[3]));
			WriteToLog(WITHDRAW, args, commandResult);
			break;
		case CHECK_BALANCE:
			commandResult = bank.GetBalance(atoi(args[1]), args[2]);
			WriteToLog(CHECK_BALANCE, args, commandResult);
			break;
		case TRANSFER:
			commandResult = bank.BankTransfer(atoi(args[1]), args[2], atoi(args[3]), atoi(args[4]));
			WriteToLog(TRANSFER, args, commandResult);
			break;
		default:
			// TODO: illegal action (not needed)
			break;
		}
		delete[] command;
		usleep(ATM_USLEEP);
	}
}
