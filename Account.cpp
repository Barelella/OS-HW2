/*
 * Account.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#include "Account.h"
using namespace std;

///read-write functions
void AddReader(pthread_mutex_t rd_lock, pthread_mutex_t wrt_lock, int rd_count){
    pthread_mutex_lock(&rd_lock);
    rd_count++;
    if(rd_count==1)
        //if someone is writing, wait
        pthread_mutex_lock(&wrt_lock);
    pthread_mutex_unlock(&rd_lock);
}

void RemoveReader(pthread_mutex_t rd_lock, pthread_mutex_t wrt_lock, int rd_count){
    pthread_mutex_lock(&rd_lock);
    rd_count--;
    if(rd_count==0)
        pthread_mutex_unlock(&wrt_lock);
    pthread_mutex_unlock(&rd_lock);
}

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
	pthread_mutex_destroy(&wrt_balance);
	pthread_mutex_destroy(&rd_balance);
	pthread_mutex_destroy(&wrt_VIP);
	pthread_mutex_destroy(&rd_VIP);
}

Result Account::Deposit(string atm_password, int sum){
	if(atm_password == password){
		pthread_mutex_lock(&wrt_balance);
		balance += sum;
		pthread_mutex_unlock(&wrt_balance);
		return SUCCESS;
	}
    else{
		return PASSFAIL;
    }
}

Result Account::Withdraw(string atm_password, int sum){
    if(atm_password == password){
        //read lock
        AddReader(rd_balance, wrt_balance, balance_rd_cnt);
        // now there's at least one reader from balance
        if(balance>=sum){
            RemoveReader(rd_balance, wrt_balance, balance_rd_cnt);
            //changing the balance
            pthread_mutex_lock(&wrt_balance);
            balance -= sum;
            pthread_mutex_unlock(&wrt_balance);
            return SUCCESS;
        }
        else{
            RemoveReader(rd_balance, wrt_balance, balance_rd_cnt);
            return AMNTFAIL;
        }
    }
    else{
        return PASSFAIL;
    }
}

//does not need a password, used only by the bank
bool Account::IsVIP(){
	bool vip_stat;
	AddReader(rd_VIP,wrt_VIP,VIP_rd_cnt);
	vip_stat = isVIP;
	RemoveReader(rd_VIP,wrt_VIP,VIP_rd_cnt);
	return vip_stat;
}

void Account::MakeVIP(){
    pthread_mutex_lock(&wrt_VIP);
    isVIP = true;
    pthread_mutex_unlock(&wrt_VIP);
}

string Account::GetPassword() const {
    return password;
}



