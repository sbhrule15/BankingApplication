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
    int id;
    std::string name;
    double balance;
    AccountType accountType;

public:
    // constructor
    Account(int id, std::string name, double balance, AccountType accountType);

    // virtual functions
    virtual bool deposit(double amt);
    virtual bool withdraw(double amt);

    // getters
    int getAcctNumber() const;
    const std::string &getName() const;
    double getBalance() const;
    AccountType getAccountType() const;
    std::string getAccountTypeString() const;
    int getId() const;

    // setters
    void setName(const std::string &name);
    void setAccountType(AccountType type);
    void setId(int iD);
    void setBalance(double b);

    // functions

};


#endif //BANKINGAPPLICATION_ACCOUNT_H