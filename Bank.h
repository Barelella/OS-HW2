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

class Bank {
private:
	std::list<Account> accounts;
	int y;

public:
	Bank();
	virtual ~Bank();
	void Init(void* inputFiles);
	void ChargeCommissions();
	void PrintStatus();
	void CreateAccount(int accountNumber, string password, int initialBalance);
};

#endif /* BANK_H_ */
