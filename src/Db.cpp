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

// initial starting functions

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
    try {
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        // Begin transaction
        SQLite::Transaction transaction(db);
        // Loop through tables creates.
        for (const std::string &sql : sqlCreateTables) {
            db.exec(sql);
        }

        // Commit transaction
        transaction.commit();
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
    return (0);
}


// Account entry actions
CheckingAccount db::createCheckingAccount(const std::string &aName) {
    try {
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        // Begin transaction
        SQLite::Transaction transaction(db);

        // insert account
        db.exec("INSERT INTO ACCOUNT (NAME) VALUES ('" + aName + "');");
        // insert checking account
        db.exec("INSERT INTO CHECKINGACCOUNT(ACCOUNTID) VALUES (last_insert_rowid());");
        // insert transaction
        db.exec("INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE) VALUES(last_insert_rowid(), current_timestamp, 0.00, 2);");

        // Commit transaction
        transaction.commit();

        // Select account just made
        SQLite::Statement query(db,
                                "SELECT * FROM ACCOUNT INNER JOIN CHECKINGACCOUNT C on ACCOUNT.ID = C.ACCOUNTID WHERE ACCOUNT.NAME = '" +
                                aName + "' ;");

        // create vector to store results (in case more than one, return first)
        std::vector<CheckingAccount> checkingAccountsVector;

        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep()) {
            // Get typed column values
            int id = query.getColumn(0);
            const char *name = query.getColumn(1);
            double balance = query.getColumn(2);
            double minBal = query.getColumn(4);
            double maxDep = query.getColumn(5);
            double maxWith = query.getColumn(6);

            // Create temp, print out details and push onto vector
            CheckingAccount temp(id, name, balance, minBal, maxDep, maxWith);
            temp.printAccountDetails();
            checkingAccountsVector.push_back(temp);
        }
        // return first account object
        return checkingAccountsVector.at(0);
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

SavingsAccount db::createSavingsAccount(const std::string& aName) {
    try {
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE);
        // Begin transaction
        SQLite::Transaction transaction(db);

        // insert account
        db.exec("INSERT INTO ACCOUNT (NAME) VALUES ('" + aName + "');");
        // insert checking account
        db.exec("INSERT INTO SAVINGSACCOUNT(ACCOUNTID) VALUES (last_insert_rowid());");
        // insert transaction
        db.exec("INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE) VALUES(last_insert_rowid(), current_timestamp, 0.00, 2);");

        // Commit transaction
        transaction.commit();

        // Select account just made
        SQLite::Statement query(db,
                                "SELECT * FROM ACCOUNT INNER JOIN SAVINGSACCOUNT S on ACCOUNT.ID = S.ACCOUNTID WHERE ACCOUNT.NAME = '" +
                                aName + "' ;");
        // create vector to store results (in case more than one, return first)
        std::vector<SavingsAccount> savingsAccountsVector;
        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep()) {
            // Get typed column values
            int id = query.getColumn(0);
            const char *name = query.getColumn(1);
            double balance = query.getColumn(2);
            double intRate = query.getColumn(4);

            // Create temp, print details and push onto vector
            SavingsAccount temp(id, name, balance, intRate);
            temp.printAccountDetails();
            savingsAccountsVector.push_back(temp);
        }
        // return first account object
        return savingsAccountsVector.at(0);
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

static int updateAccountQuery(const std::string& stmt, const std::string& logstmt) {
    try {
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        SQLite::Transaction transaction(db);
        // insert account
        int affected = db.exec(stmt);
        // log transaction
        db.exec(logstmt);
        // Commit transaction
        transaction.commit();

        return affected;
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
        return 0;
    }
}

bool db::deleteAccount(int accId) {
    std::string stmt =
                "DELETE FROM CHECKINGACCOUNT"
                "WHERE ACCOUNTID = "+std::to_string(accId)+";"
                "DELETE FROM SAVINGSACCOUNT"
                "WHERE ACCOUNTID = "+std::to_string(accId)+";"
                "DELETE FROM ACCOUNT"
                "WHERE ID = "+std::to_string(accId)+";";
    std::string logstmt =
            "INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE) "
            "VALUES("+std::to_string(accId)+", current_timestamp, 0, "+std::to_string(AccountDeleted)+");";
    if (updateAccountQuery(stmt,logstmt) != 0)
        return true;
    else
        return false;
}

bool db::deposit(int accId, float d) {
    std::string stmt =
            "UPDATE ACCOUNT"
            "SET BALANCE = BALANCE + " + std::to_string(d) +
            " WHERE ID = " + std::to_string(accId) + ";";
    std::string logstmt =
            "INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE) "
            "VALUES("+std::to_string(accId)+", current_timestamp,"+std::to_string(d)+", "+std::to_string(Deposit)+");";

    if (updateAccountQuery(stmt,logstmt) != 0)
        return true;
    else
        return false;
}

bool db::withdraw(int accId, float w) {
    std::string stmt =
            "UPDATE ACCOUNT"
            "SET BALANCE = BALANCE - " + std::to_string(w) +
            "WHERE ID = " + std::to_string(accId) +
            " AND BALANCE > " + std::to_string(w) + ";";
    std::string logstmt =
            "INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE) "
            "VALUES("+std::to_string(accId)+", current_timestamp, "+std::to_string(-w)+", "+std::to_string(Withdrawal)+");";

    if (updateAccountQuery(stmt,logstmt) != 0)
        return true;
    else
        return false;
}

// Get Accounts Function
static std::vector<Account> getAccountsQuery(const std::string& stmt) {
    try {
        // open database
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE);
        // select all accounts with join on checking
        SQLite::Statement query(db, stmt);
        // create vector to store results (in case more than one, return first)
        std::vector<Account> accountsVector;

        while (query.executeStep()) {
            // Get typed column values
            int id = query.getColumn(0);
            const char *name = query.getColumn(1);
            double balance = query.getColumn(2);
            double minBal = query.getColumn(3);
            double maxDep = query.getColumn(4);
            double maxWith = query.getColumn(5);
            double intRate = query.getColumn(6);

            // Create temp, print out details and push onto vector
            if (intRate == 0) {
                CheckingAccount temp(id, name, balance, minBal, maxDep, maxWith);
                temp.printAccountDetails();
                accountsVector.push_back(temp);
            } else {
                SavingsAccount temp(id, name, balance, intRate);
                temp.printAccountDetails();
                accountsVector.push_back(temp);
            }
        }
        // return first account object
        return accountsVector;
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

Account db::getAccountById(int accId) {
    std::string stmt =
            "SELECT a.ID, NAME, BALANCE, C.MINBALANCE, MAXDEPOSIT, MAXWITHDRAW, S.INTERESTRATE FROM ACCOUNT as a"
            "INNER JOIN CHECKINGACCOUNT C on a.ID = C.ACCOUNTID"
            "INNER JOIN SAVINGSACCOUNT S on a.ID = S.ACCOUNTID"
            "WHERE ID = " + std::to_string(accId) + ";";

    return getAccountsQuery(stmt).at(0);
}

std::vector<Account> db::getAccountsByName(const std::string& name) {
    std::string stmt =
            "SELECT a.ID, NAME, BALANCE, C.MINBALANCE, MAXDEPOSIT, MAXWITHDRAW, S.INTERESTRATE FROM ACCOUNT as a"
            "LEFT JOIN CHECKINGACCOUNT C on a.ID = C.ACCOUNTID"
            "LEFT JOIN SAVINGSACCOUNT S on a.ID = S.ACCOUNTID"
            "WHERE NAME = " + name + ";";

    return getAccountsQuery(stmt);
}

std::vector<Account> db::getAllAccounts() {
    std::string stmt =
            "SELECT a.ID, NAME, BALANCE, C.MINBALANCE, MAXDEPOSIT, MAXWITHDRAW, S.INTERESTRATE FROM ACCOUNT as a "
            "LEFT JOIN CHECKINGACCOUNT C on a.ID = C.ACCOUNTID "
            "LEFT JOIN SAVINGSACCOUNT S on a.ID = S.ACCOUNTID;";

    return getAccountsQuery(stmt);
}

std::vector<CheckingAccount> db::getAllCheckingAccounts() {
    try {
        // open database
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE);
        // select all accounts with join on checking
        SQLite::Statement query(db, "SELECT * FROM ACCOUNT LEFT JOIN CHECKINGACCOUNT C on ACCOUNT.ID = C.ACCOUNTID;");
        // create vector to store results (in case more than one, return first)
        std::vector<CheckingAccount> checkingAccountsVector;

        while (query.executeStep()) {
            // Get typed column values
            int id = query.getColumn(0);
            const char *name = query.getColumn(1);
            double balance = query.getColumn(2);
            double minBal = query.getColumn(4);
            double maxDep = query.getColumn(5);
            double maxWith = query.getColumn(6);

            // Create temp, print out details and push onto vector
            CheckingAccount temp(id, name, balance, minBal, maxDep, maxWith);
            temp.printAccountDetails();
            checkingAccountsVector.push_back(temp);
        }

        // return account vector
        return checkingAccountsVector;
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

std::vector<SavingsAccount> db::getAllSavingsAccounts() {
    try {
        // open database
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE);
        // select all accounts with join on savings
        SQLite::Statement query(db, "SELECT * FROM ACCOUNT INNER JOIN SAVINGSACCOUNT S on ACCOUNT.ID = S.ACCOUNTID;");
        // create vector to store results (in case more than one, return first)
        std::vector<SavingsAccount> savingsAccountsVector;

        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep()) {
            // Get typed column values
            int id = query.getColumn(0);
            const char *name = query.getColumn(1);
            double balance = query.getColumn(2);
            double intRate = query.getColumn(4);

            // Create temp, print details and push onto vector
            SavingsAccount temp(id, name, balance, intRate);
            temp.printAccountDetails();
            savingsAccountsVector.push_back(temp);
        }

        // return first account object
        return savingsAccountsVector;
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}