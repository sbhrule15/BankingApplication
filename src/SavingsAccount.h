//
// Created by Joshua Pare on 3/20/21.
//

#ifndef BANKINGAPPLICATION_SAVINGSACCOUNT_H
#define BANKINGAPPLICATION_SAVINGSACCOUNT_H


#include "Account.h"

class SavingsAccount : public Account{
protected:
    double interestRate;
public:
    SavingsAccount(const std::string &name, int id);
    SavingsAccount(int id, const std::string &name, double balance, double intRate);

public:
    //print functions
    void printAccountDetails();

    // getters
    double getInterestRate() const;

    // setters
    void setInterestRate(double interestRate);

    // functions
    bool deposit(double amt) override;
    bool withdraw(double amt) override;
};


#endif //BANKINGAPPLICATION_SAVINGSACCOUNT_H
