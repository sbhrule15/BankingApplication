//
// Created by Joshua Pare on 3/20/21.
//

#include "Account.h"

#include <utility>

// Constructor
Account::Account(int acctNumber, std::string name, float balance, AccountType accountType) : acctNumber(
        acctNumber), name(std::move(name)), balance(balance), accountType(accountType) {}

// Functions
int Account::getAcctNumber() const {
    return acctNumber;
}

const std::string &Account::getName() const {
    return name;
}

void Account::setName(const std::string &acctName) {
    Account::name = acctName;
}

float Account::getBalance() const {
    return balance;
}

AccountType Account::getAccountType() const {
    return accountType;
}

void Account::setAccountType(AccountType type) {
    Account::accountType = type;
}

std::string Account::getAccountTypeString() const {
    if (accountType == Checking) {
        return "Checking";
    } else if (accountType == Savings) {
        return "Savings";
    } else {
        return "N/A";
    }
}
