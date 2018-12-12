/*
 * Account.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#include "Account.h"

Account::Account() :
	accountNumber(0), password("0000"), balance(0), isVIP(false){
	pthread_mutex_init(&accountLock, NULL);
}

Account::~Account() {
	// TODO Auto-generated destructor stub
}

void Account::Deposit(){
}

void Account::Withdraw(){

}

bool Account::IsVIP(){
	return false;
}

void Account::MakeVIP(){

}

