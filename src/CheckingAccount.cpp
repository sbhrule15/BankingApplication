//
// Created by Joshua Pare on 3/20/21.
//

#include <iostream>
#include "CheckingAccount.h"

CheckingAccount::CheckingAccount(const std::string &name, const int id)
: Account(id, name, 0.0, Checking), minBalance(500.0), maxDeposit(10000.0), maxWithdraw(5000.0) {}

CheckingAccount::CheckingAccount(int id, const std::string &name, double balance, double minBalance, double maxDeposit, double maxWithdraw)
: Account(id, name, balance, Checking), minBalance(minBalance), maxDeposit(maxDeposit), maxWithdraw(maxWithdraw) {}

double CheckingAccount::getMinBalance() const {
    return minBalance;
}

void CheckingAccount::setMinBalance(double minBal) {
    CheckingAccount::minBalance = minBal;
}

double CheckingAccount::getMaxDeposit() const {
    return maxDeposit;
}

void CheckingAccount::setMaxDeposit(double maxDep) {
    CheckingAccount::maxDeposit = maxDep;
}

bool CheckingAccount::deposit(double amt) {
    if (amt > maxDeposit){
        return false;
    } else {
        balance = balance + amt;
        return true;
    }
}

bool CheckingAccount::withdraw(double amt) {
    if (amt > balance || amt > maxWithdraw){
        return false;
    } else {
        balance = balance - amt;
        return true;
    }
}

double CheckingAccount::getMaxWithdraw() const {
    return maxWithdraw;
}

void CheckingAccount::setMaxWithdraw(double mw) {
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