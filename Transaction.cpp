//
// Created by Joshua Pare on 3/20/21.
//

#include "Transaction.h"

#include <utility>
#include <iostream>

// constructor
Transaction::Transaction(int id, std::string name, float amtChange, TransactionType type)
    : id(id), timestamp(std::time(nullptr)), name(std::move(name)), amtChange(amtChange), transactionType(type) {}

// functions
std::string Transaction::getTransactionTypeStr() {
    switch (transactionType) {
        case Withdrawal:
            return "Withdrawal";
        case Deposit:
            return "Deposit";
        case AccountCreated:
            return "Account Created";
        case Other:
            return "Other";
    }
}

void Transaction::printTransaction() {
    std::cout   << "Transaction ID: " << id << "\n"
            << "Timestamp: " << timestamp << "\n"
            << "Name on Account: " << name << "\n"
            << "Transaction Type: " << getTransactionTypeStr() << "\n"
            << "Amount Change: " << amtChange << "\n"
            << std::endl;
}


