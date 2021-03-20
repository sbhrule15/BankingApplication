//
// Created by Joshua Pare on 3/20/21.
//

#include "CheckingAccount.h"

CheckingAccount::CheckingAccount(int acctNumber, const std::string &name, float balance, AccountType accountType,
                                 float minBalance, float maxDeposit) : Account(acctNumber, name, balance, Checking),
                                                                       minBalance(500.0), maxDeposit(10000.0) {}
