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
#include <iostream>
using namespace std;

typedef enum {SUCCESS, PASSFAIL, AMNTFAIL,FAILURE} Result;

class Account {
private:
	const int accountNumber;
	const string password;
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
	Account(int acc_num_, string password_, int balance_);
	virtual ~Account();

    Result Deposit(string atm_password, int sum);
    Result Withdraw(string atm_password, int sum);
	bool IsVIP();
    Result MakeVIP(string atm_password);
    int GetBalance(string atm_password);
    int GetID() const;
    friend Result Transfer(string src_password, Account& dst, Account& src, int amount);
};

#endif /* ACCOUNT_H_ */
