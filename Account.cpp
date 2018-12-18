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

void Account::Withdraw(string atm_password, int sum){
    if(atm_password == password){
        //read lock
        pthread_mutex_lock(&rd_balance);
        balance_rd_cnt++;
        if(balance_rd_cnt==1)
            //if someone is writing, wait
            pthread_mutex_lock(&wrt_balance);
        pthread_mutex_unlock(&rd_balance);
        // now there's at least one reader from balance
        if(balance>=sum){
            pthread_mutex_lock(&rd_balance);
            //stopped reading from balance
            balance_rd_cnt--;
            if(balance_rd_cnt==0)
                pthread_mutex_unlock(&wrt_balance);
            pthread_mutex_unlock(&rd_balance);
            //changing the balance
            pthread_mutex_lock(&wrt_balance);
            balance -= sum;
            pthread_mutex_unlock(&wrt_balance);
            return SUCCESS;
        }
        else{
            pthread_mutex_lock(&rd_balance);
            //stopped reading from balance
            balance_rd_cnt--;
            if(balance_rd_cnt==0)
                pthread_mutex_unlock(&wrt_balance);
            pthread_mutex_unlock(&rd_balance);
            return AMNTFAIL;
        }
    }
    else{
        return PASSFAIL;
    }
}

bool Account::IsVIP(){
	return false;
}

void Account::MakeVIP(){

}

string Account::GetPassword() const {
    return password;
}



