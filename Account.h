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
using namespace std;

typedef enum {SUCCESS, PASSFAIL, AMNTFAIL,FAILURE} Result;

class Account {
private:
	int accountNumber;
	string password const;
	int balance;
	bool isVIP;
	//counters for reader-writer problem
	int balance_rd_cnt;
	int VIP_rd_cnt;

	// We need two locks: one for each variable
	pthread_mutex_t wrt_balance;
    pthread_mutex_t rd_balance;
    pthread_mutex_t wrt_VIP;
    pthread_mutex_t rd_VIP;

public:
	Account();
	virtual ~Account();

	void Deposit(string atm_password, int sum);
	void Withdraw(string atm_password, int sum);
	bool IsVIP();
	void MakeVIP();
	string GetPassword() const;
};

#endif /* ACCOUNT_H_ */
