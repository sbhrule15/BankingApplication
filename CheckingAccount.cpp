//
// Created by Joshua Pare on 3/20/21.
//

#include "CheckingAccount.h"

CheckingAccount::CheckingAccount(const std::string &name, float balance)
: Account(name, balance, Checking), minBalance(500.0), maxDeposit(10000.0), maxWithdraw(5000.0) {}

float CheckingAccount::getMinBalance() const {
    return minBalance;
}

void CheckingAccount::setMinBalance(float minBal) {
    CheckingAccount::minBalance = minBal;
}

float CheckingAccount::getMaxDeposit() const {
    return maxDeposit;
}

void CheckingAccount::setMaxDeposit(float maxDep) {
    CheckingAccount::maxDeposit = maxDep;
}

bool CheckingAccount::deposit(float amt) {
    if (amt > maxWithdraw){
        return false;
    } else {
        balance = balance + amt;
        return true;
    }
}

bool CheckingAccount::withdraw(float amt) {
    if (amt > balance || amt > maxDeposit){
        return false;
    } else {
        balance = balance - amt;
        return true;
    }
}
