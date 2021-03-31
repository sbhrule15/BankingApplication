//
// Created by Joshua Pare on 3/20/21.
//

#ifndef BANKINGAPPLICATION_CHECKINGACCOUNT_H
#define BANKINGAPPLICATION_CHECKINGACCOUNT_H


#include "Account.h"

class CheckingAccount : public Account {
protected:
    double minBalance;
    double maxDeposit;
    double maxWithdraw;

public:
    // constructors
    explicit CheckingAccount(const std::string &name, int id);
    CheckingAccount(int id, const std::string &name, double balance, double minBalance,
                    double maxDeposit, double maxWithdraw);

    // functions
    bool deposit(double amt) override;
    bool withdraw(double amt) override;

    // print functions
    void printAccountDetails() override;

    // getters
    [[nodiscard]] double getMinBalance() const;
    [[nodiscard]] double getMaxDeposit() const;
    [[nodiscard]] double getMaxWithdraw() const;

    // setters
    void setMinBalance(double minBalance);
    void setMaxDeposit(double maxDeposit);
    void setMaxWithdraw(double mw);

};


#endif //BANKINGAPPLICATION_CHECKINGACCOUNT_H