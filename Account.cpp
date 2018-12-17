/*
 * Account.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#include "Account.h"
using namespace std;


Account::Account(int acc_num_, string password_, int balance_) :
	accountNumber(acc_num_), password(password_), balance(balance_), isVIP(false){
    //counters
    balance_rd_cnt = 0;
    VIP_rd_cnt = 0;
    //semaphores
	pthread_mutex_init(&wrt_balance, NULL);
	pthread_mutex_init(&rd_balance, NULL);
	pthread_mutex_init(&wrt_VIP, NULL);
	pthread_mutex_init(&rd_VIP, NULL);
}

Account::~Account() {
	pthread_mutex_destroy(&wrt_balance, NULL);
	pthread_mutex_destroy(&rd_balance, NULL);
	pthread_mutex_destroy(&wrt_VIP, NULL);
	pthread_mutex_destroy(&rd_VIP, NULL);
}

void Account::Deposit(int sum){
    pthread_mutex_lock(wrt_balance);
    balance += sum;
    pthread_mutex_unlock(wrt_balance);
}

void Account::Withdraw(int sum){

}

bool Account::IsVIP(){
	return false;
}

void Account::MakeVIP(){

}



