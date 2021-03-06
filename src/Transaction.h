//
// Created by Joshua Pare on 3/20/21.
//

#ifndef BANKINGAPPLICATION_TRANSACTION_H
#define BANKINGAPPLICATION_TRANSACTION_H

#include <chrono>
#include <ctime>
#include <string>

enum TransactionType {
    Withdrawal = 0,
    Deposit = 1,
    AccountCreated = 2,
    AccountDeleted = 3,
    Other = 3
};

class Transaction {
protected:
    int id{};
    int accountId{};
    std::string timestamp{};
    double amtChange{};
    TransactionType transactionType;

public:
    // constructors
    Transaction(int id, int accountId, std::string timestamp, double amtChange, TransactionType transactionType);

    // functions
    void printTransaction();

    std::string getTransactionTypeStr();


};


#endif //BANKINGAPPLICATION_TRANSACTION_H