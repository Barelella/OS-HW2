/*
 * Bank.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#include "Bank.h"
using namespace std;

Bank::Bank() {
	// TODO Auto-generated constructor stub

}

Bank::~Bank() {
	// TODO Auto-generated destructor stub
}

//void Bank::Init(void* inputFiles){	// not sure if necessary
//	return;
//}

void Bank::ChargeCommissions(){
	return;
}

void Bank::PrintStatus(){

}

Result Bank::CreateAccount(int accountNumber, string password, int initialBalance){
    //checking if there's an account with the same number
    for(list<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it){
        if (it->GetID() == accountNumber)
            return FAILURE;
    }
    Account newAcc(accountNumber, password, initialBalance);
    return SUCCESS;
}
