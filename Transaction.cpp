//
// Created by Joshua Pare on 3/20/21.
//

#include "Transaction.h"

#include <utility>

// constructor
Transaction::Transaction(int id, std::string name, float amtChange, TransactionType type)
    : id(id), timestamp(std::time(nullptr)), name(std::move(name)), amtChange(amtChange), transactionType(type) {}