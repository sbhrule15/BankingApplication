//
// Created by Joshua Pare on 3/20/21.
//

#include <iostream>
#include "SavingsAccount.h"
#include "Db.h"

SavingsAccount::SavingsAccount(const std::string &name, const int id)
        : Account(id, name, 0.0, Savings), interestRate(0.01) {}

SavingsAccount::SavingsAccount(int id, const std::string &name, double balance, double intRate)
        : Account(id, name, balance, Savings), interestRate(intRate) {}

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

void SavingsAccount::setInterestRate(double ir) {
    SavingsAccount::interestRate = ir;
}

bool SavingsAccount::deposit(double amt) {
    return db::deposit(this->getId(), amt);
}

bool SavingsAccount::withdraw(double amt) {
    if (amt > this->getBalance()) {
        std::cout << "You do not have enough money in this account to withdraw.\n" << std::endl;
        return false;
    }

    return db::withdraw(this->getId(), amt);
}