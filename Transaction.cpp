//
// Created by Joshua Pare on 3/20/21.
//

#include "Transaction.h"

#include <utility>
#include <iostream>

int Transaction::currId;

// constructor
Transaction::Transaction(float amtChange, TransactionType type)
    : id(currId++), timestamp(std::time(nullptr)), amtChange(amtChange), transactionType(type) {}

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
            << "Transaction Type: " << getTransactionTypeStr() << "\n"
            << "Amount Change: " << amtChange << "\n"
            << std::endl;
}


