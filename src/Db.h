//
// Created by Joshua Pare on 3/22/21.
//

#ifndef BANKINGAPPLICATION_DB_H
#define BANKINGAPPLICATION_DB_H

#include <cstdio>
#include <map>
#include "Account.h"
#include "Transaction.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"

// namespace for all db executions

namespace db {

    //===========GENERAL DB RELATED============//
    int initDB();

    //===========ACCOUNTS============//
    // CREATE
    bool createAccount(const std::string& aName, AccountType accountType);

    // READ
    std::map<int, std::shared_ptr<Account>> getAccountsById(int accId);
    std::map<int, std::shared_ptr<Account>> getAccountsByName(const std::string& name);
    std::map<int, std::shared_ptr<Account>> getAccountsByType(AccountType type);
    std::map<int, std::shared_ptr<Account>> getAllAccounts();
    std::map<int, std::shared_ptr<CheckingAccount>> getAllCheckingAccounts();
    std::map<int, std::shared_ptr<SavingsAccount>> getAllSavingsAccounts();

    // UPDATE
    bool deposit(int accId, double d);
    bool withdraw(int accId, double w);

    // DESTROY
    bool deleteAccount(int accId);

    //===========TRANSACTIONS============//
    // CREATE
    int createTransaction(const char* dbdir, float amtChange, TransactionType transactionType);

    // READ
    std::map<int, Transaction> getAllTransactions();
    Transaction getTransactionById(int transactionId);
    std::map<int, Transaction> getTransactionsByAccount(const int &accountId);
    std::vector<Transaction> getTransactionsByType(TransactionType transactionType);

    // UPDATE


    // DESTROY



}


#endif //BANKINGAPPLICATION_DB_H