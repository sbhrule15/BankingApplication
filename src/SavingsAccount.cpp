//
// Created by Joshua Pare on 3/20/21.
//

#include <iostream>
#include "SavingsAccount.h"


SavingsAccount::SavingsAccount(const std::string &name, const int id)
        : Account(id, name, 0.0, Savings), interestRate(0.01) {}

SavingsAccount::SavingsAccount(int id, const std::string &name, double balance, double intRate)
        : Account(id, name, balance, Savings), interestRate(intRate) {}

bool SavingsAccount::withdraw(double amt) {
    return false;
}

bool SavingsAccount::deposit(double amt) {
    return false;
}

void SavingsAccount::printAccountDetails() {
    std::cout << "ID: " << getId() << "\n"
              << "Name: " << getName() << "\n"
              << "Balance: " << getBalance() << "\n"
              << "Interest Rate: " << getInterestRate() << "\n"
              << std::endl;

}

double SavingsAccount::getInterestRate() const {
    return interestRate;
}

void SavingsAccount::setInterestRate(double interestRate) {
    SavingsAccount::interestRate = interestRate;
}
