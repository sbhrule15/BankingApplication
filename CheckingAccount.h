//
// Created by Joshua Pare on 3/20/21.
//

#ifndef BANKINGAPPLICATION_CHECKINGACCOUNT_H
#define BANKINGAPPLICATION_CHECKINGACCOUNT_H


#include "Account.h"

class CheckingAccount : public Account {
protected:
    float minBalance;
    float maxDeposit;
    float maxWithdraw;

public:
    // constructor
    CheckingAccount(const std::string &name, float balance);

    // required functions
    bool deposit(float amt);

    bool withdraw(float amt);

    // functions

    float getMinBalance() const;

    void setMinBalance(float minBalance);

    float getMaxDeposit() const;

    void setMaxDeposit(float maxDeposit);

};


#endif //BANKINGAPPLICATION_CHECKINGACCOUNT_H
