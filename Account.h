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
#include <unistd.h>
using namespace std;

typedef enum {	SUCCESS,
				PASSWORD_FAIL,
				AMOUNT_FAIL,
				ACCOUNT_ALREADY_EXISTS,
				ACCOUNT_DOESNT_EXIST,
				ACCOUNT_FAILURE,
				INITIAL_BALANCE_FAIL,
				TRANSFER_TARGET_DOESNT_EXIST,
} Result;

class Account {
private:
	int accountNumber;
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
    Account();
	Account(int acc_num_, string password_, int balance_);
	virtual ~Account();

    Result Deposit(string atm_password, int sum);
    Result Withdraw(string atm_password, int sum, bool needAtmWait = true);
	bool IsVIP();
    Result MakeVIP(string atm_password);
    int GetBalance(string atm_password, bool needAtmWait = true);
    string GetPassword();
    friend Result Transfer(string src_password, Account& dst, Account& src, int amount);
    int GetAccountNumber();
    bool operator<(Account&);
};

#endif /* ACCOUNT_H_ */
