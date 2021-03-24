//
// Created by Joshua Pare on 3/22/21.
//

#ifndef BANKINGAPPLICATION_DB_H
#define BANKINGAPPLICATION_DB_H

#include <cstdio>
#include "sqlite3.h"
#include "Account.h"
#include "Transaction.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"

// namespace for all db executions

namespace db {

    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
    int createDB(const char* dbdir);
    int initDB();

    // Account Queries
    CheckingAccount createCheckingAccount(const std::string& aName);
    SavingsAccount createSavingsAccount(std::string aName);
    int deleteAccount(const char* dbdir, int accId);
    float deposit(const char* dbdir, int accId);
    float withdraw(const char* dbdir, int accId);
    Account getAccountById(const char* dbdir, int accId);
    std::vector<Account> getAllAccounts();
    std::vector<CheckingAccount> getAllCheckingAccounts();
    std::vector<SavingsAccount> getAllSavingsAccounts();


    // Transaction Queries
    int createTransaction(const char* dbdir, float amtChange, TransactionType transactionType);
    std::vector<Transaction> getAllTransactions(const char* dbdir);
    std::vector<Transaction> getTransactionsByAccount(const char* dbdir, const int &accountId);
    std::vector<Transaction> getTransactionsByType(const char* dbdir, TransactionType transactionType);
    Transaction getTransactionById(const char* dbdir, const int transactionId);

    // Report Queries


}


#endif //BANKINGAPPLICATION_DB_H
