//
// Created by Joshua Pare on 3/22/21.
//

#include <vector>
#include <string>
#include <iostream>
#include "Db.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"

using namespace db;

// execution functions
static int exec(const char* dbdir, sqlite3 *DB, std::string query){
    try {
        int exit = 0;
        exit = sqlite3_open(dbdir, &DB);

        char *messageError;
        exit = sqlite3_exec(DB, query.c_str(), nullptr, nullptr, &messageError);

        if (exit != SQLITE_OK) {
            std::cerr << "Error Executing Query" << std::endl;
            sqlite3_free(messageError);
        } else
            std::cout << "Query Executed Successfully" << std::endl;
        sqlite3_close(DB);

    } catch (const std::exception &e) {
        std::cerr << e.what();
    }
}

// callback
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// initial starting functions
int db::createDB(const char* dbdir){
    sqlite3 *DB;
    int exit = 0;

    exit = sqlite3_open(dbdir, &DB);

    sqlite3_close(DB);

    return 0;
}

int db::createTables(const char* dbdir) {
    //create pointer reference
    sqlite3 *DB;

    // create vector of sql statements to initiate
    std::vector<std::string> sqlCreateTables;

    // create accounts tables
    sqlCreateTables.emplace_back(
            "CREATE TABLE IF NOT EXISTS ACCOUNT("
            "ID         INTEGER PRIMARY KEY AUTOINCREMENT, "
            "NAME       TEXT NOT NULL,"
            "BALANCE    REAL NOT NULL"
            ");"
    );
    sqlCreateTables.emplace_back(
            "CREATE TABLE IF NOT EXISTS CHECKINGACCOUNT("
            "ACCOUNTID      INTEGER NOT NULL REFERENCES ACCOUNT(ID), "
            "MINBALANCE     REAL NOT NULL,"
            "MAXDEPOSIT     REAL NOT NULL,"
            "MAXWITHDRAW    INTEGER NOT NULL"
            ");"
    );
    sqlCreateTables.emplace_back(
            "CREATE TABLE IF NOT EXISTS SAVINGSACCOUNT("
            "ACCOUNTID      INTEGER NOT NULL REFERENCES ACCOUNT(ID), "
            "INTERESTRATE   REAL NOT NULL"
            ");"
    );

    // create transactions table
    sqlCreateTables.emplace_back(
            "CREATE TABLE IF NOT EXISTS TRANSACTIONLOG("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
            "ACCOUNTID  INTEGER NOT NULL REFERENCES ACCOUNT(ID),"
            "TIMESTAMP  DATETIME NOT NULL,"
            "AMTCHANGE  REAL NOT NULL,"
            "TRANSACTIONTYPE INTEGER NOT NULL"
            ");"
    );

    for (const std::string &sql : sqlCreateTables) {
        exec(dbdir, DB, sql);
    }

    return (0);
}

// Account entry actions
Account db::addAccount(const char* dbdir, AccountType accountType, std::string name){

}

int db::deleteAccount(const char* dbdir, int accId){

}

float db::deposit(const char* dbdir, int accId){

}

float db::withdraw(const char* dbdir, int accId){

}

// Account query actions
static std::vector<Account> multipleAccountCallback(void *NotUsed, int argc, char **argv, char **azColName){

    for (int i = 0; i < argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
}

static int singleAccountCallback(void *account, int argc, char **argv, char **azColName){
    // account info
    int aId{0};
    std::string aName;
    float aBalance{0.0};
    AccountType aType;

    // checking account info
    float aMaxDeposit = NULL;
    float aMinBalance = NULL;
    float aMaxWithdraw = NULL;

    // savings account info
    float aInterestRate = NULL;

    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

        // log into account object
        if (azColName[i] == "ID"){
            aId = atoi(argv[i]);
        } else if (azColName[i] == "NAME"){
            aName = argv[i];
        } else if (azColName[i] == "BALANCE"){
            aBalance = atof(argv[i]);
        } else if (azColName[i] == "MINBALANCE"){
            aBalance = atof(argv[i]);
            aType = Checking;
        } else if (azColName[i] == "MAXDEPOSIT"){
            aBalance = atof(argv[i]);
            aType = Checking;
        } else if (azColName[i] == "MAXWITHDRAW"){
            aBalance = atof(argv[i]);
            aType = Checking;
        } else if (azColName[i] == "INTERESTRATE"){
            aBalance = atof(argv[i]);
            aType = Savings;
        }
    }

    if (aType == Savings){
        account = new SavingsAccount(aName);
    } else if (aType == Checking) {
        account = new CheckingAccount(aName);
    }

    printf("\n");
    return 0;
}

Account db::getAccountById(const char* dbdir, int accId){
    //create pointer reference
    sqlite3 *DB;
    std::string stmt =
            "SELECT * FROM ACCOUNT"
            "WHERE ID = " + std::to_string(accId) + ";";

    // account ptr
    void *account;

    try {
        int exit = 0;
        exit = sqlite3_open(dbdir, &DB);

        char *messageError;
        exit = sqlite3_exec(DB, stmt.c_str(), singleAccountCallback, account, &messageError);

        if (exit != SQLITE_OK) {
            std::cerr << "Error Getting Account" << std::endl;
            sqlite3_free(messageError);
        } else
            std::cout << "GetAccountById Executed Successfully" << std::endl;

        sqlite3_close(DB);

    } catch (const std::exception &e) {
        std::cerr << e.what();
    }
}

std::vector<Account> db::getAllAccounts(const char* dbdir ){

}
