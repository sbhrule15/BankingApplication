//
// Created by Joshua Pare on 3/20/21.
//

#include "SavingsAccount.h"

bool SavingsAccount::withdraw(float amt) {
    return false;
}

bool SavingsAccount::deposit(float amt) {
    return false;
}

SavingsAccount::SavingsAccount(const std::string &name)
        : Account(name, 0.0, Savings), interestRate(0.01) {}
