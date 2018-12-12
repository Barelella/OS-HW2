/*
 * Account.h
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include <string>
#include <pthread.h>

class Account {
private:
	int accountNumber;
	std::string password;
	int balance;
	bool isVIP;

	// TODO: Not sure whether account should hold its own mutex, or should the hold all mutexes
	pthread_mutex_t accountLock;

public:
	Account();
	virtual ~Account();

	void Deposit();
	void Withdraw();
	bool IsVIP();
	void MakeVIP();
};

#endif /* ACCOUNT_H_ */
