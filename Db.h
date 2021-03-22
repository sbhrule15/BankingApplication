//
// Created by Joshua Pare on 3/22/21.
//

#ifndef BANKINGAPPLICATION_DB_H
#define BANKINGAPPLICATION_DB_H

#include <cstdio>
#include "sqlite3.h"
#include "Account.h"
#include "Transaction.h"

// namespace for all db executions

namespace db {
    static int callback(void *NotUsed, int argc, char **argv, char **azColName);
    static int createDB(const char* dbdir);
    static int createTables(const char* dbdir);

    // Account Queries
    static Account addAccount(const char* dbdir, AccountType accountType, std::string name);
    static int deleteAccount(const char* dbdir, int accId);
    static float deposit(const char* dbdir, int accId);
    static float withdraw(const char* dbdir, int accId);
    static Account getAccountById(const char* dbdir, int accId);
    static std::vector<Account> getAllAccounts(const char* dbdir );


    // Transaction Queries
    static int createTransaction(const char* dbdir, float amtChange, TransactionType transactionType);
    static std::vector<Transaction> getAllTransactions(const char* dbdir);
    static std::vector<Transaction> getTransactionsByAccount(const char* dbdir, const int &accountId);
    static std::vector<Transaction> getTransactionsByType(const char* dbdir, TransactionType transactionType);
    static Transaction getTransactionById(const char* dbdir, const int transactionId);

    // Report Queries


}


#endif //BANKINGAPPLICATION_DB_H
