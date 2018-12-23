/*
 * Atm.h
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#ifndef ATM_H_
#define ATM_H_

#include <fstream>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include "Bank.h"

typedef enum {
	ILLEGAL_ACTION,
	CREATE_NEW_ACCOUNT,
	VIP_ACCOUNT,
	DEPOSIT,
	WITHDRAW,
	CHECK_BALANCE,
	TRANSFER
} Action;

class Atm {
private:
	Bank& bank;
	int serialNumber;
	string fileName;
	ofstream& log;

public:
	Atm(Bank& bank, int serialNumber, string fileName, ofstream& log);
	virtual ~Atm();

	void Run();
	void WriteToLog(Action action, char* args[], Result error);
};

#endif /* ATM_H_ */
