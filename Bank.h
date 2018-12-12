/*
 * Bank.h
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#ifndef BANK_H_
#define BANK_H_

#include <list>
#include "Account.h"
#include "Atm.h"

class Bank {
private:
	std::list<Account> accounts;
	std::list<Atm> atms;

public:
	Bank();
	virtual ~Bank();
	void Init(void* inputFiles);
	void Run();
	void ChargeCommissions();
	void PrintStatus();
};

#endif /* BANK_H_ */
