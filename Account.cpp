/*
 * Account.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#include "Account.h"
using namespace std;


///read-write functions
//**************************************************************************************
// function name: AddReader
// Description: part of the reader-writer protection
// Parameters: reader's lock, writer's lock, readers counter
// Returns: None
//**************************************************************************************
void AddReader(pthread_mutex_t& rd_lock, pthread_mutex_t& wrt_lock, int& rd_count){
    pthread_mutex_lock(&rd_lock);
        rd_count++;
        if(rd_count==1) {
            //if someone is writing, wait
            pthread_mutex_lock(&wrt_lock);
        }
    pthread_mutex_unlock(&rd_lock);
}

//**************************************************************************************
// function name: RemoveReader
// Description: part of the reader-writer protection
// Parameters: reader's lock, writer's lock, readers counter
// Returns: None
//**************************************************************************************
void RemoveReader(pthread_mutex_t& rd_lock, pthread_mutex_t& wrt_lock, int& rd_count){
    pthread_mutex_lock(&rd_lock);
        rd_count--;
        if(rd_count==0) {
            pthread_mutex_unlock(&wrt_lock);
        }
    pthread_mutex_unlock(&rd_lock);
}

///Account methods
//**************************************************************************************
// function name: Account
// Description: default constructor
// Parameters: none
// Returns: new account
//**************************************************************************************
Account::Account():
		accountNumber(-1), password("0000"), balance(0), isVIP(false){
    balance_rd_cnt = 0;
    VIP_rd_cnt = 0;
    pthread_mutex_init(&wrt_balance, NULL);
    pthread_mutex_init(&rd_balance, NULL);
    pthread_mutex_init(&wrt_VIP, NULL);
    pthread_mutex_init(&rd_VIP, NULL);
}

//**************************************************************************************
// function name: Account
// Description: constructor
// Parameters: account number, password, initial balance
// Returns: new account
//**************************************************************************************
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

//**************************************************************************************
// function name: ~Account
// Description: default destructor
// Parameters: none
// Returns: none
//**************************************************************************************
Account::~Account() {
	pthread_mutex_destroy(&wrt_balance);
	pthread_mutex_destroy(&rd_balance);
	pthread_mutex_destroy(&wrt_VIP);
	pthread_mutex_destroy(&rd_VIP);
}

//**************************************************************************************
// function name: Deposit
// Description: deposits money in the account
// Parameters: password, sum: amount of money to deposit
// Returns: PASSWORD_FAIL if password is incorrect, SUCCESS otherwise
//**************************************************************************************
Result Account::Deposit(string atm_password, int sum){
    if(atm_password != password)
        return PASSWORD_FAIL;
    pthread_mutex_lock(&wrt_balance);
		balance += sum;
		sleep(1);
    pthread_mutex_unlock(&wrt_balance);
    return SUCCESS;
}

//**************************************************************************************
// function name: Withdraw
// Description: withdraws money from the account
// Parameters: password, sum: amount of money to withdraw,
//             needAtmWait: true if atm has to wait after action
// Returns: PASSWORD_FAIL if password is incorrect, AMOUNT_FAIL if there's not enough money,
//          SUCCESS otherwise
//**************************************************************************************
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

//**************************************************************************************
// function name: IsVip
// Description: checks the account VIP status
// Parameters: none
// Returns: VIP status
//**************************************************************************************
//does not need a password, used only by the bank
bool Account::IsVIP(){
	bool vip_stat;
	AddReader(rd_VIP,wrt_VIP,VIP_rd_cnt);
	    vip_stat = isVIP;
	RemoveReader(rd_VIP,wrt_VIP,VIP_rd_cnt);
	return vip_stat;
}

//**************************************************************************************
// function name: MakeVip
// Description: changes account status to VIP
// Parameters: password
// Returns: PASSWORD_FAIL if password is incorrect, SUCCESS otherwise
//**************************************************************************************
Result Account::MakeVIP(string atm_password){
    if(password!=atm_password)
        return PASSWORD_FAIL;
    pthread_mutex_lock(&wrt_VIP);
        isVIP = true;
        sleep(1);
    pthread_mutex_unlock(&wrt_VIP);
    return SUCCESS;
}

//**************************************************************************************
// function name: GetBalance
// Description: returns the account balance
// Parameters: password, needAtmWait: true if atm has to wait after action
// Returns: -1 if password is incorrect, account balance otherwise
//**************************************************************************************
int Account::GetBalance(string atm_password, bool needAtmWait){
    if(password!=atm_password)
        return -1;
    int balance_stat;
    AddReader(rd_balance,wrt_balance,balance_rd_cnt);
        balance_stat = balance;
        if(needAtmWait){
        	sleep(1);
        }
    RemoveReader(rd_balance,wrt_balance,balance_rd_cnt);
    return balance_stat;
}

//**************************************************************************************
// function name: GetPassword
// Description: gets account password - for bank status printing
// Parameters: none
// Returns: account password
//**************************************************************************************
string Account::GetPassword(){
	return password;
}

//**************************************************************************************
// function name: Transfer
// Description: transfers money from one account to another
// Parameters: source account password, target account, source account, amount to transfer
// Returns: PASSWORD_FAIL if password is incorrect, AMOUNT_FAIL if there's not enough money,
//          SUCCESS otherwise
//**************************************************************************************
Result Transfer(const string src_password, Account& dst, Account& src, int amount){
    if(src_password != src.password)
        return PASSWORD_FAIL;
    AddReader(src.rd_balance, src.wrt_balance, src.balance_rd_cnt);
    if(src.balance < amount){
        RemoveReader(src.rd_balance, src.wrt_balance, src.balance_rd_cnt);
        return AMOUNT_FAIL;
    }
    RemoveReader(src.rd_balance, src.wrt_balance, src.balance_rd_cnt);
    //checking account numbers to prevent deadlock
    if (src.accountNumber < dst.accountNumber) {
        pthread_mutex_lock(&src.wrt_balance);
        pthread_mutex_lock(&dst.wrt_balance);
        src.balance -= amount;
        dst.balance += amount;
        sleep(1);
        pthread_mutex_unlock(&src.wrt_balance);
        pthread_mutex_unlock(&dst.wrt_balance);
    }
    else{
        pthread_mutex_lock(&dst.wrt_balance);
        pthread_mutex_lock(&src.wrt_balance);
        src.balance -= amount;
        dst.balance += amount;
        sleep(1);
        pthread_mutex_unlock(&dst.wrt_balance);
        pthread_mutex_unlock(&src.wrt_balance);
    }
    return SUCCESS;
}

//**************************************************************************************
// function name: GetAccountNumber
// Description: gets account number
// Parameters: none
// Returns: account number
//**************************************************************************************
int Account::GetAccountNumber() const{
	return accountNumber;
}

//**************************************************************************************
// function name: operator <
// Description: checks which account number is bigger
// Parameters: other account
// Returns: bool variable
//**************************************************************************************
bool Account::operator<(Account& otherAccount){
	return (GetAccountNumber() < otherAccount.GetAccountNumber());
}
