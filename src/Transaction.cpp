//
// Created by Joshua Pare on 3/20/21.
//

#include "Transaction.h"

#include <utility>
#include <iostream>

// constructor
Transaction::Transaction(int id, int accountId, std::string timestamp, double amtChange, TransactionType transactionType)
        : id(id), accountId(accountId), timestamp(std::move(timestamp)), amtChange(amtChange), transactionType(transactionType) {}

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
            << "Account ID: " << accountId << "\n"
            << "Transaction Type: " << getTransactionTypeStr() << "\n"
            << "Amount Change: " << amtChange << "\n"
            << "-------------------------------\n"
            << std::endl;
}