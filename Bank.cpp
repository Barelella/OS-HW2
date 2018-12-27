/*
 * Bank.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: nadav
 */

#include "Bank.h"
using namespace std;

//**************************************************************************************
// function name: Bank
// Description: constructor
// Parameters: log file
// Returns: new bank object
//**************************************************************************************
Bank::Bank(Log& log) : bankBalance(0), bankLog(log){
	int ret1 = pthread_mutex_init(&printLock, NULL);
    int ret2 = pthread_mutex_init(&balanceLock, NULL);
    if ((ret1 != 0)||(ret2 != 0))
    	perror("Bank mutex init error:");
}

//**************************************************************************************
// function name: ~Bank
// Description: default destructor
// Parameters: none
// Returns: none
//**************************************************************************************
Bank::~Bank() {
	int ret1 = pthread_mutex_destroy(&printLock);
	int ret2 = pthread_mutex_destroy(&balanceLock);
	if ((ret1 != 0)||(ret2 != 0))
		perror("Bank mutex destroy error:");
}

//**************************************************************************************
// function name: ChargeCommissions
// Description: every 3 seconds charges commissions from accounts in account list
//              that are not VIP
// Parameters: none
// Returns: none
//**************************************************************************************
void Bank::ChargeCommissions(){
	int bank_gain;
    int percentage = 2 + rand()/(RAND_MAX/3 + 1); //getting a random num between [2,4]
    double doublepercent = 100/percentage;
    for(list<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it){
		if(!it->IsVIP()){
			stringstream aux;
			string password = it->GetPassword();
			bank_gain = (int)(((double)(it->GetBalance(password, false)))/doublepercent+0.5);
			it->Withdraw(password, bank_gain, false);
			pthread_mutex_lock(&balanceLock);
				bankBalance += bank_gain;
			pthread_mutex_unlock(&balanceLock);
			aux << "Bank: commission of " << percentage << " % were charged, ";
			aux << "the bank gained "<< bank_gain << " $ ";
			aux << "from account " << it->GetAccountNumber();
			bankLog.WriteLine(aux);
		}
    }
	return;
}

//**************************************************************************************
// function name: PrintStatus
// Description: prints bank status every 0.5 seconds
// Parameters: none
// Returns: none
//**************************************************************************************
void Bank::PrintStatus(){
	pthread_mutex_lock(&printLock);
	printf("\033[2J");		// Clear screen
	printf("\033[1;1H");	// Initialize cursor
	cout << "Current Bank Status" << endl;

	for(list<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it){
		cout << "Account " << it->GetAccountNumber() << ": ";
		cout << "Balance - " << it->GetBalance(it->GetPassword(), false) << " $ , ";
		cout << "Account Password - " << it->GetPassword() << endl;
	}
	pthread_mutex_lock(&balanceLock);
	cout << "The Bank has " << bankBalance << " $" << endl;
	pthread_mutex_unlock(&balanceLock);
	pthread_mutex_unlock(&printLock);
}

//**************************************************************************************
// function name: GetAccount
// Description: finds an account with accountNumber in account list
// Parameters: account number
// Returns: account object
//**************************************************************************************
Account& Bank::GetAccount(int accountNumber){
	for(list<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it){
		if(it->GetAccountNumber() == accountNumber){
			return (*it);
		}
	}
	Account *defaultAccount = new Account();
	return *defaultAccount;
}

//**************************************************************************************
// function name: IsAccountExist
// Description: checks if account with accountNumber exists
// Parameters: account number
// Returns: true if exists, false otherwise
//**************************************************************************************
bool Bank::IsAccountExist(int accountNumber){
	if(0 == accountNumber){		// atoi failed, given illegal number
		return false;
	}
	Account& accountFound = GetAccount(accountNumber);
	// return (-1 != accountFound.GetAccountNumber());
	if(-1 == accountFound.GetAccountNumber()){
		delete &accountFound;	// made a default account, must delete it
		return false;
	}
	return true;
}

//**************************************************************************************
// function name: IsLegalPassword
// Description: checks if password contains four digits
// Parameters: password
// Returns: true if legal, false otherwise
//**************************************************************************************
bool IsLegalPassword(string password){
	if(password.length() != PASSWORD_LENGTH){
		return false;
	}

	for(int i = 0; i < PASSWORD_LENGTH; ++i){
		if(password[i] < '0' || password[i] > '9'){
			return false;
		}
	}
	return true;
}

//**************************************************************************************
// function name: CreateAccount
// Description: creates an account object and adds it to account list
// Parameters: account number, password, initial balance
// Returns: result of action
//**************************************************************************************
Result Bank::CreateAccount(int accountNumber, string password, int initialBalance){
	if(0 == accountNumber){		// atoi failed, given illegal account number
		return ACCOUNT_DOESNT_EXIST;
	}
	if(IsAccountExist(accountNumber)){
		return ACCOUNT_ALREADY_EXISTS;
	}
	if(!IsLegalPassword(password)){
		return PASSWORD_FAIL;
	}
	if(initialBalance < 0 ){
		return INITIAL_BALANCE_FAIL;
	}
	// All parameters are OK, insert new account to list (and keep it sorted)
	Account newAccount(accountNumber, password, initialBalance);
	accounts.push_back(newAccount);
	accounts.sort();
	sleep(1);

	return SUCCESS;
}

//**************************************************************************************
// function name: MakeVip
// Description: changes an account status to VIP
// Parameters: account number, password
// Returns: result of action
//**************************************************************************************
Result Bank::MakeVip(int accountNumber, string password){
	if(!IsAccountExist(accountNumber)){
		return ACCOUNT_DOESNT_EXIST;
	}
	return GetAccount(accountNumber).MakeVIP(password);
}

//**************************************************************************************
// function name: Deposit
// Description: deposits money in an account
// Parameters: account number, password, amount to deposit
// Returns: result of action
//**************************************************************************************
Result Bank::Deposit(int accountNumber, string password, int amount){
	if(!IsAccountExist(accountNumber)){
		return ACCOUNT_DOESNT_EXIST;
	}
	return GetAccount(accountNumber).Deposit(password, amount);
}

//**************************************************************************************
// function name: Withdraw
// Description: withdraws money from an account
// Parameters: account number, password, amount to withdraw
// Returns: result of action
//**************************************************************************************
Result Bank::Withdraw(int accountNumber, string password, int amount){
	if(!IsAccountExist(accountNumber)){
		return ACCOUNT_DOESNT_EXIST;
	}
	return GetAccount(accountNumber).Withdraw(password, amount);
}

//**************************************************************************************
// function name: CreateAccount
// Description: gets an account balance
// Parameters: account number, password
// Returns: result of action
//**************************************************************************************
Result Bank::GetBalance(int accountNumber, string password){
	if(!IsAccountExist(accountNumber)){
		return ACCOUNT_DOESNT_EXIST;
	}
	return (GetAccount(accountNumber).GetBalance(password) == -1 ? PASSWORD_FAIL : SUCCESS);
}

//**************************************************************************************
// function name: BankTransfer
// Description: transfers money from one account to another
// Parameters: source account number, password, target account number, amount
// Returns: result of action
//**************************************************************************************
Result Bank::BankTransfer(int srcAccountNum, string password, int dstAccountNum, int amount){
	if(!IsAccountExist(srcAccountNum)){
		return ACCOUNT_DOESNT_EXIST;
	}
	if(!IsAccountExist(dstAccountNum)){
		return ACCOUNT_DOESNT_EXIST;
	}
	Account& srcAccount = GetAccount(srcAccountNum);
	Account& dstAccount = GetAccount(dstAccountNum);
	return Transfer(password, dstAccount, srcAccount, amount);
}
