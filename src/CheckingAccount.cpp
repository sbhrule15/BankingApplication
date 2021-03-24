//
// Created by Joshua Pare on 3/20/21.
//

#include <iostream>
#include "CheckingAccount.h"

CheckingAccount::CheckingAccount(const std::string &name, const int id)
: Account(id, name, 0.0, Checking), minBalance(500.0), maxDeposit(10000.0), maxWithdraw(5000.0) {}

CheckingAccount::CheckingAccount(int id, const std::string &name, float balance, float minBalance, float maxDeposit, float maxWithdraw)
: Account(id, name, balance, Checking), minBalance(minBalance), maxDeposit(maxDeposit), maxWithdraw(maxWithdraw) {}

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
    if (amt > maxDeposit){
        return false;
    } else {
        balance = balance + amt;
        return true;
    }
}

bool CheckingAccount::withdraw(float amt) {
    if (amt > balance || amt > maxWithdraw){
        return false;
    } else {
        balance = balance - amt;
        return true;
    }
}

float CheckingAccount::getMaxWithdraw() const {
    return maxWithdraw;
}

void CheckingAccount::setMaxWithdraw(float mw) {
    CheckingAccount::maxWithdraw = mw;
}

void CheckingAccount::printAccountDetails() {
    // Print to console (for now)
    std::cout << "ID: " << getId() << "\n"
              << "Name: " << getName() << "\n"
              << "Balance: " << getBalance() << "\n"
              << "Minimum Balance: " << getMinBalance() << "\n"
              << "Max Deposit: " << getMaxDeposit() << "\n"
              << "Max Withdrawal: " << getMaxWithdraw() << "\n"
              << std::endl;

}
