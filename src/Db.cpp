//
// Created by Joshua Pare on 3/22/21.
//

#include <vector>
#include <string>
#include <iostream>
#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Transaction.h>

#include "Db.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"


using namespace db;

// execution functions
static bool exec(const char* dbdir, sqlite3 *DB, const std::string& query){
    try {
        int exit = 0;
        exit = sqlite3_open(dbdir, &DB);

        char *messageError;
        exit = sqlite3_exec(DB, query.c_str(), nullptr, nullptr, &messageError);

        if (exit != SQLITE_OK) {
            std::cerr << "Error Executing Query" << std::endl;
            sqlite3_free(messageError);
            sqlite3_close(DB);
            return false;
        } else{
            std::cout << "Query Executed Successfully" << std::endl;
            return true;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what();
        return false;
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

int db::initDB() {
    // create vector of sql statements to initiate
    std::vector<std::string> sqlCreateTables;

    // create accounts tables statements
    sqlCreateTables.emplace_back(
            "CREATE TABLE IF NOT EXISTS ACCOUNT("
            "ID         INTEGER PRIMARY KEY AUTOINCREMENT, "
            "NAME       TEXT NOT NULL,"
            "BALANCE    REAL NOT NULL DEFAULT 0.00"
            ");"
    );
    sqlCreateTables.emplace_back(
            "CREATE TABLE IF NOT EXISTS CHECKINGACCOUNT("
            "ACCOUNTID      INTEGER NOT NULL REFERENCES ACCOUNT(ID), "
            "MINBALANCE     REAL NOT NULL DEFAULT 500.00,"
            "MAXDEPOSIT     REAL NOT NULL DEFAULT 10000.00,"
            "MAXWITHDRAW    INTEGER NOT NULL DEFAULT 5000.00"
            ");"
    );
    sqlCreateTables.emplace_back(
            "CREATE TABLE IF NOT EXISTS SAVINGSACCOUNT("
            "ACCOUNTID      INTEGER NOT NULL REFERENCES ACCOUNT(ID), "
            "INTERESTRATE   REAL NOT NULL DEFAULT 0.01"
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

    // loop through vector and execute queries
    try
    {
        SQLite::Database    db("data.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

        // Begin transaction
        SQLite::Transaction transaction(db);
        // Loop through tables creates.
        for (const std::string &sql : sqlCreateTables) {
            db.exec(sql);
        }

        // Commit transaction
        transaction.commit();
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }

    return (0);
}


// Account entry actions
CheckingAccount db::createCheckingAccount(const std::string& aName){
    try
    {
        // Open a database file
        SQLite::Database    db("data.db");

        // Compile a SQL query, containing one parameter (index 1)
        SQLite::Statement   query(db, "CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");

        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep())
        {
            // Demonstrate how to get some typed column value
            int         id      = query.getColumn(0);
            const char* value   = query.getColumn(1);
            int         size    = query.getColumn(2);

            std::cout << "row: " << id << ", " << value << ", " << size << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

SavingsAccount db::createSavingsAccount(std::string aName){
    // query string
    std::string query =
            "BEGIN TRANSACTION;"
            "INSERT INTO ACCOUNT (NAME) VALUES ('"+ aName +"');"
            "INSERT INTO SAVINGSACCOUNT(ACCOUNTID) VALUES (last_insert_rowid());"
            "SELECT * FROM ACCOUNT INNER JOIN SAVINGSACCOUNT S on ACCOUNT.ID = S.ACCOUNTID WHERE ACCOUNT.ID= last_insert_rowid();"
            "INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE) VALUES"
            "(last_insert_rowid(), current_timestamp, 0.00, 2);"
            "END TRANSACTION;";

}


int db::deleteAccount(const char* dbdir, int accId){

}

float db::deposit(const char* dbdir, int accId){

}

float db::withdraw(const char* dbdir, int accId){

}


Account db::getAccountById(const char* dbdir, int accId){
    //create pointer reference
    sqlite3 *DB;
    std::string stmt =
            "SELECT CHECKINGACCOUNT.MINBALANCE, MAXDEPOSIT, MAXWITHDRAW, ACCOUNT.ID, NAME, BALANCE, SAVINGSACCOUNT.INTERESTRATE"
            "FROM CHECKINGACCOUNT, SAVINGSACCOUNT, ACCOUNT"
            "WHERE SAVINGSACCOUNT.ACCOUNTID = " + std::to_string(accId) + " OR SAVINGSACCOUNT.ACCOUNTID = "+std::to_string(accId)+";";

    // account ptr
    void *account;

    try {
        int exit = 0;
        exit = sqlite3_open(dbdir, &DB);

        char *messageError;
        exit = sqlite3_exec(DB, stmt.c_str(), nullptr, nullptr, &messageError);

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
