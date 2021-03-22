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
    static int addAccount(const char* dbdir, AccountType accountType, std::string name);
    static int deleteAccount(const char* dbdir, int accId);
    static int deposit(const char* dbdir, int accId);
    static int withdraw(const char* dbdir, int accId);

    // Transaction Queries

    // Report Queries


}


#endif //BANKINGAPPLICATION_DB_H
