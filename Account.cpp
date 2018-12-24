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

// Default constructor
Account::Account():
		accountNumber(-1), password("0000"), balance(0), isVIP(false){
    balance_rd_cnt = 0;
    VIP_rd_cnt = 0;
}

///Account methods
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
    if(atm_password != password)
        return PASSWORD_FAIL;
    pthread_mutex_lock(&wrt_balance);
		balance += sum;
		sleep(1);
    pthread_mutex_unlock(&wrt_balance);
    return SUCCESS;
}


Result Account::Withdraw(string atm_password, int sum, bool needAtmWait){
    if(atm_password != password)
        return PASSWORD_FAIL;
    AddReader(rd_balance, wrt_balance, balance_rd_cnt);
    if(balance < sum){
        RemoveReader(rd_balance, wrt_balance, balance_rd_cnt);
        return AMOUNT_FAIL;
    }
    RemoveReader(rd_balance, wrt_balance, balance_rd_cnt);
    //changing the balance
    pthread_mutex_lock(&wrt_balance);
		balance -= sum;
		if(needAtmWait){
			sleep(1);
		}
    pthread_mutex_unlock(&wrt_balance);
    return SUCCESS;
}


//does not need a password, used only by the bank
bool Account::IsVIP(){
	bool vip_stat;
	AddReader(rd_VIP,wrt_VIP,VIP_rd_cnt);
	    vip_stat = isVIP;
	RemoveReader(rd_VIP,wrt_VIP,VIP_rd_cnt);
	return vip_stat;
}


Result Account::MakeVIP(string atm_password){
    if(password==atm_password)
        return PASSWORD_FAIL;
    pthread_mutex_lock(&wrt_VIP);
        isVIP = true;
        sleep(1);
    pthread_mutex_unlock(&wrt_VIP);
    return SUCCESS;
}

int Account::GetID() const{
    return accountNumber;
}

int Account::GetBalance(string atm_password,bool needAtmWait){
    if(password!=atm_password)
        return -1;
    int balance_stat;
    AddReader(rd_balance,wrt_balance,balance_rd_cnt);
        balance_stat = balance;
        if(needAtmWait){
        	sleep(1);
        }
    RemoveReader(rd_balance,wrt_balance,balance_rd_cnt);
    return balance;
}

// Needed by the bank for printing
string Account::GetPassword(){
	return password;
}


Result Transfer(const string src_password, Account& dst, Account& src, int amount){
    if(src_password != src.password)
        return PASSWORD_FAIL;
    AddReader(src.rd_balance, src.wrt_balance, src.balance_rd_cnt);
    if(src.balance < amount){
        RemoveReader(src.rd_balance, src.wrt_balance, src.balance_rd_cnt);
        return AMOUNT_FAIL;
    }
    RemoveReader(src.rd_balance, src.wrt_balance, src.balance_rd_cnt);
    pthread_mutex_lock(&src.wrt_balance);
        pthread_mutex_lock(&dst.wrt_balance);
            src.balance -= amount;
            dst.balance += amount;
            sleep(1);
        pthread_mutex_unlock(&dst.wrt_balance);
    pthread_mutex_unlock(&src.wrt_balance);
    return SUCCESS;
}

int Account::GetAccountNumber(){
	return accountNumber;
}

bool Account::operator<(Account& otherAccount){
	return (GetAccountNumber() < otherAccount.GetAccountNumber());
}
