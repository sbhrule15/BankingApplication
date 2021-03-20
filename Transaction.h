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
    static int currId;
    int id;
    std::time_t timestamp;
    std::string name;
    float amtChange;
    TransactionType transactionType;

public:
    // constructor
    Transaction(std::string name, float amtChange, TransactionType type);

    // functions
    void printTransaction();

    std::string getTransactionTypeStr();


};


#endif //BANKINGAPPLICATION_TRANSACTION_H
