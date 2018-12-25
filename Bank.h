/*
 * Bank.h
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#ifndef BANK_H_
#define BANK_H_

#include <list>
#include <fstream>
#include "Account.h"
#include "Log.h"

const int PASSWORD_LENGTH = 4;

class Bank {
private:
	std::list<Account> accounts;
	int bankBalance;
	pthread_mutex_t printLock;
	Log& bankLog;

public:
	Bank(Log& log);
	virtual ~Bank();
	void Init(void* inputFiles);
	void ChargeCommissions();
	void PrintStatus();
	Account& GetAccount(int accountNumber);
	bool IsAccountExist(int serialNumber);
	Result CreateAccount(int accountNumber, string password, int initialBalance);
	Result MakeVip(int accountNumber, string password);
	Result Deposit(int accountNumber, string password, int amount);
	Result Withdraw(int accountNumber, string password, int amount);
	Result GetBalance(int accountNumber, string password);
	Result BankTransfer(int srcAccount, string password, int dstAccount, int amount);
};

#endif /* BANK_H_ */
