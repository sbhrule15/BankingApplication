//
// Created by Joshua Pare on 3/20/21.
//

#ifndef BANKINGAPPLICATION_ACCOUNT_H
#define BANKINGAPPLICATION_ACCOUNT_H

#include <string>
#include <vector>
#include "Transaction.h"

enum AccountType {
    Checking, Savings
};

class Account {
protected:
    static int currId;
    int acctNumber;
    std::string name;
    float balance;
    AccountType accountType;
    std::vector<Transaction> transactions;

public:
    // constructor
    Account(std::string name, float balance, AccountType accountType);

    // functions
    int getAcctNumber() const;

    const std::string &getName() const;

    void setName(const std::string &name);

    float getBalance() const;

    AccountType getAccountType() const;

    std::string getAccountTypeString() const;

    void setAccountType(AccountType type);

};


#endif //BANKINGAPPLICATION_ACCOUNT_H
