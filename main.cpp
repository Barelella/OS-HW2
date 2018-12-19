/*
 * main.cpp
 *
 *  Main source file of the project, runs the whole bank project
 */

#include <iostream>

#include "Bank.h"
using namespace std;

int main(int argc, char* argv[]){
	//Bank bank;
	//bank.Run();
	string pass = "abcd";
	Account account(2345, pass, 0);
	account.Deposit(pass, 100);
	account.Withdraw("cdef", 7);
	account.Withdraw(pass, 111);
	account.Withdraw(pass, 7);
	return 0;
}

