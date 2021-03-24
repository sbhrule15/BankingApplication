//
// Created by Joshua Pare on 3/20/21.
//

#ifndef BANKINGAPPLICATION_SAVINGSACCOUNT_H
#define BANKINGAPPLICATION_SAVINGSACCOUNT_H


#include "Account.h"

class SavingsAccount : public Account{
protected:
    float interestRate;
public:
    SavingsAccount(const std::string &name, int id);

public:
    bool deposit(float amt);
    bool withdraw(float amt);
};


#endif //BANKINGAPPLICATION_SAVINGSACCOUNT_H
