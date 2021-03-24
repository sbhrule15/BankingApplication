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
        SQLite::Database    db("data.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

        // Begin transaction
        SQLite::Transaction transaction(db);

        // insert account
        int ia = db.exec("INSERT INTO ACCOUNT (NAME) VALUES ('"+ aName +"');");
        std::cout << "INSERT INTO ACCOUNT query returned " << ia << std::endl;
        // insert checking account
        int ic = db.exec("INSERT INTO CHECKINGACCOUNT(ACCOUNTID) VALUES (last_insert_rowid());");
        std::cout << "INSERT INTO CHECKING ACCOUNT query returned " << ic << std::endl;
        // insert transaction
        int it = db.exec("INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE) VALUES(last_insert_rowid(), current_timestamp, 0.00, 2);");
        std::cout << "SELECT CHECKING ACCOUNT query returned " << it << std::endl;

        // Commit transaction
        transaction.commit();

        // Select account just made
        SQLite::Statement   query(db, "SELECT * FROM ACCOUNT INNER JOIN CHECKINGACCOUNT C on ACCOUNT.ID = C.ACCOUNTID WHERE ACCOUNT.NAME = '"+aName+"' ;");

        // create vector to store results (in case more than one, return first)
        std::vector<CheckingAccount> checkingAccountsVector;

        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep())
        {
            // Get typed column values
            int         id          = query.getColumn(0);
            const char* name        = query.getColumn(1);
            double      balance     = query.getColumn(2);
            double      minBal      = query.getColumn(4);
            double      maxDep      = query.getColumn(5);
            double      maxWith     = query.getColumn(6);

            // Create temp, print out details and push onto vector
            CheckingAccount temp(id,name,balance,minBal,maxDep,maxWith);
            temp.printAccountDetails();
            checkingAccountsVector.push_back(temp);
        }

        // return first account object
        return checkingAccountsVector.at(0);
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
    try
    {
        SQLite::Database    db("data.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

        // Begin transaction
        SQLite::Transaction transaction(db);

        // insert account
        int ia = db.exec("INSERT INTO ACCOUNT (NAME) VALUES ('"+ aName +"');");
        std::cout << "INSERT INTO ACCOUNT query returned " << ia << std::endl;
        // insert checking account
        int ic = db.exec("INSERT INTO SAVINGSACCOUNT(ACCOUNTID) VALUES (last_insert_rowid());");
        std::cout << "INSERT INTO CHECKING ACCOUNT query returned " << ic << std::endl;
        // insert transaction
        int it = db.exec("INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE) VALUES(last_insert_rowid(), current_timestamp, 0.00, 2);");
        std::cout << "SELECT CHECKING ACCOUNT query returned " << it << std::endl;

        // Commit transaction
        transaction.commit();

        // Select account just made
        SQLite::Statement   query(db, "SELECT * FROM ACCOUNT INNER JOIN SAVINGSACCOUNT S on ACCOUNT.ID = S.ACCOUNTID WHERE ACCOUNT.NAME = '"+aName+"' ;");

        // create vector to store results (in case more than one, return first)
        std::vector<SavingsAccount> savingsAccountsVector;

        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep())
        {
            // Get typed column values
            int         id          = query.getColumn(0);
            const char* name        = query.getColumn(1);
            double      balance     = query.getColumn(2);
            double      intRate      = query.getColumn(4);

            // Create temp, print details and push onto vector
            SavingsAccount temp(id,name,balance,intRate);
            temp.printAccountDetails();
            savingsAccountsVector.push_back(temp);
        }

        // return first account object
        return savingsAccountsVector.at(0);
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }

}


int db::deleteAccount(const char* dbdir, int accId){

}

float db::deposit(const char* dbdir, int accId){

}

float db::withdraw(const char* dbdir, int accId){

}


Account db::getAccountById(const char* dbdir, int accId){
    std::string stmt =
            "SELECT CHECKINGACCOUNT.MINBALANCE, MAXDEPOSIT, MAXWITHDRAW, ACCOUNT.ID, NAME, BALANCE, SAVINGSACCOUNT.INTERESTRATE"
            "FROM CHECKINGACCOUNT, SAVINGSACCOUNT, ACCOUNT"
            "WHERE SAVINGSACCOUNT.ACCOUNTID = " + std::to_string(accId) + " OR SAVINGSACCOUNT.ACCOUNTID = "+std::to_string(accId)+";";

}

std::vector<Account> db::getAllAccounts(){

}

std::vector<CheckingAccount> db::getAllCheckingAccounts(){
    try
    {
        // open database
        SQLite::Database    db("data.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        // select all accounts with join on checking
        SQLite::Statement   query(db, "SELECT * FROM ACCOUNT INNER JOIN CHECKINGACCOUNT C on ACCOUNT.ID = C.ACCOUNTID;");
        // create vector to store results (in case more than one, return first)
        std::vector<CheckingAccount> checkingAccountsVector;

        while (query.executeStep())
        {
            // Get typed column values
            int         id          = query.getColumn(0);
            const char* name        = query.getColumn(1);
            double      balance     = query.getColumn(2);
            double      minBal      = query.getColumn(4);
            double      maxDep      = query.getColumn(5);
            double      maxWith     = query.getColumn(6);

            // Create temp, print out details and push onto vector
            CheckingAccount temp(id,name,balance,minBal,maxDep,maxWith);
            temp.printAccountDetails();
            checkingAccountsVector.push_back(temp);
        }

        // return first account object
        return checkingAccountsVector;
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

std::vector<SavingsAccount> db::getAllSavingsAccounts(){
    try
    {
        // open database
        SQLite::Database    db("data.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        // select all accounts with join on savings
        SQLite::Statement   query(db, "SELECT * FROM ACCOUNT INNER JOIN SAVINGSACCOUNT S on ACCOUNT.ID = S.ACCOUNTID;");
        // create vector to store results (in case more than one, return first)
        std::vector<SavingsAccount> savingsAccountsVector;

        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep())
        {
            // Get typed column values
            int         id          = query.getColumn(0);
            const char* name        = query.getColumn(1);
            double      balance     = query.getColumn(2);
            double      intRate      = query.getColumn(4);

            // Create temp, print details and push onto vector
            SavingsAccount temp(id,name,balance,intRate);
            temp.printAccountDetails();
            savingsAccountsVector.push_back(temp);
        }

        // return first account object
        return savingsAccountsVector;
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
}
