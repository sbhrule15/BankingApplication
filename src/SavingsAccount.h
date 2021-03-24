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
    SavingsAccount(int id, const std::string &name, float balance, float intRate);

public:
    //print functions
    void printAccountDetails();

    // getters
    float getInterestRate() const;

    // setters
    void setInterestRate(float interestRate);

    // functions
    bool deposit(float amt) override;
    bool withdraw(float amt) override;
};


#endif //BANKINGAPPLICATION_SAVINGSACCOUNT_H
