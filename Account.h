//
// Created by Joshua Pare on 3/20/21.
//

#ifndef BANKINGAPPLICATION_ACCOUNT_H
#define BANKINGAPPLICATION_ACCOUNT_H

#include <string>

enum AccountType {
    Checking, Savings
};

class Account {
protected:
    int acctNumber;
    std::string name;
    float balance;
    AccountType accountType;

public:
    // constructor
    Account(int acctNumber, std::string name, float balance, AccountType accountType);

    // pure virtual functions
    virtual bool deposit(float amt) = 0;

    virtual bool withdraw(float amt) = 0;

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
