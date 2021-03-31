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
    virtual bool deposit(double amt) = 0;
    virtual bool withdraw(double amt) = 0;
    virtual void printAccountDetails() = 0;

    // getters
    [[nodiscard]] int getAcctNumber() const;
    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] double getBalance() const;
    [[nodiscard]] AccountType getAccountType() const;
    [[nodiscard]] std::string getAccountTypeString() const;
    [[nodiscard]] int getId() const;

    // setters
    void setName(const std::string &name);
    void setAccountType(AccountType type);
    void setId(int iD);
    void setBalance(double b);

    // functions

};


#endif //BANKINGAPPLICATION_ACCOUNT_H