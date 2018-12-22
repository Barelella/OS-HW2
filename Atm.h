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
#include "Bank.h"

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
};

#endif /* ATM_H_ */
