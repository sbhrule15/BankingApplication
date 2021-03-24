//
// Created by Joshua Pare on 3/20/21.
//

#include "Account.h"

#include <utility>

// Constructor
Account::Account(int id, std::string name, float balance, AccountType accountType)
: id(id), name(std::move(name)), balance(balance), accountType(accountType) {}

// Functions
int Account::getAcctNumber() const {
    return id;
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

void Account::setBalance(float b) {
    Account::balance = b;
}

int Account::getId() const {
    return id;
}

void Account::setId(int iD) {
    Account::id = iD;
}

bool Account::deposit(float amt) {
    balance = balance + amt;
    return true;
}

bool Account::withdraw(float amt) {
    if (amt > balance)
        return false;
    else {
        balance = balance - amt;
        return true;
    }
}




