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
    // constructors
    explicit CheckingAccount(const std::string &name, int id);
    CheckingAccount(int id, const std::string &name, float balance, float minBalance,
                    float maxDeposit, float maxWithdraw);

    // functions
    bool deposit(float amt) override;
    bool withdraw(float amt) override;

    // print functions
    void printAccountDetails();

    // getters
    float getMinBalance() const;
    float getMaxDeposit() const;
    float getMaxWithdraw() const;

    // setters
    void setMinBalance(float minBalance);
    void setMaxDeposit(float maxDeposit);
    void setMaxWithdraw(float mw);

};


#endif //BANKINGAPPLICATION_CHECKINGACCOUNT_H
