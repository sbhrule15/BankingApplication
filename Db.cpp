//
// Created by Joshua Pare on 3/22/21.
//

#include <vector>
#include <string>
#include <iostream>
#include "Db.h"

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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int createDB(const char* dbdir){
    sqlite3 *DB;
    int exit = 0;

    exit = sqlite3_open(dbdir, &DB);

    sqlite3_close(DB);

    return 0;
}

static int createTables(const char* dbdir) {
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

