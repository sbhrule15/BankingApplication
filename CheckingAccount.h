//
// Created by Joshua Pare on 3/20/21.
//

#ifndef BANKINGAPPLICATION_CHECKINGACCOUNT_H
#define BANKINGAPPLICATION_CHECKINGACCOUNT_H


#include "Account.h"

class CheckingAccount : public Account {
protected:
    float minBalance{};
    float maxDeposit{};

public:
    CheckingAccount(int acctNumber, const std::string &name, float balance, AccountType accountType, float minBalance,
                    float maxDeposit);
};


#endif //BANKINGAPPLICATION_CHECKINGACCOUNT_H
