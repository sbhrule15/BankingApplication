//
// Created by Joshua Pare on 3/20/21.
//

#ifndef BANKINGAPPLICATION_TRANSACTION_H
#define BANKINGAPPLICATION_TRANSACTION_H

#include <chrono>
#include <string>

enum TransactionType {
    Withdrawal, Deposit, AccountCreated, Other
};

class Transaction {
protected:
    int id;
    std::time_t timestamp;
    std::string name;
    float amtChange;

public:
    // constructor
    Transaction(int id, std::string name, float amtChange);
};


#endif //BANKINGAPPLICATION_TRANSACTION_H
