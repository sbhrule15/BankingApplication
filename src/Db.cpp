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

static bool insertAccount(const std::string& acctName, const std::string& typeInsertStmt){
    try {
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        // Begin transaction
        SQLite::Transaction transaction(db);

        // insert account
        int accountRowsAffected = db.exec("INSERT INTO ACCOUNT (NAME) VALUES ('" + acctName + "');");
        // insert typed account
        int typeRowsAffected = db.exec(typeInsertStmt);
        // insert transaction
        int transactionRowsAffected = db.exec("INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE) VALUES(last_insert_rowid(), current_timestamp, 0.00, 2);");

        // Commit transaction
        transaction.commit();

        // log transaction error if there is one
        if (transactionRowsAffected == 0)
            std::cout << "There was an error creating the transaction log." << std::endl;

        // return true if insert is successful in checking and account tables
        return (accountRowsAffected > 0 && typeRowsAffected > 0);
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
        return false;
    }
}

// Account entry actions
bool db::createAccount(const std::string &aName, AccountType accountType) {
    switch (accountType) {
        case Checking:
            return insertAccount(aName, "INSERT INTO CHECKINGACCOUNT(ACCOUNTID) VALUES (last_insert_rowid());");
        case Savings:
            return insertAccount(aName, "INSERT INTO SAVINGSACCOUNT(ACCOUNTID) VALUES (last_insert_rowid());");
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

    return updateAccountQuery(stmt,logstmt) != 0;
}

bool db::deposit(int accId, double d) {
    std::string stmt =
            "UPDATE ACCOUNT"
            " SET BALANCE = BALANCE + " + std::to_string(d) +
            " WHERE ID = " + std::to_string(accId) + ";";
    std::string logstmt =
            "INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE)"
            " VALUES("+std::to_string(accId)+", current_timestamp,"+std::to_string(d)+", "+std::to_string(Deposit)+");";

    return updateAccountQuery(stmt,logstmt) != 0;
}

bool db::withdraw(int accId, double w) {
    std::string stmt =
            "UPDATE ACCOUNT"
            " SET BALANCE = BALANCE - " + std::to_string(w) +
            " WHERE ID = " + std::to_string(accId) +
            " AND BALANCE > " + std::to_string(w) + ";";
    std::string logstmt =
            "INSERT INTO TRANSACTIONLOG(ACCOUNTID, TIMESTAMP, AMTCHANGE, TRANSACTIONTYPE)"
            " VALUES("+std::to_string(accId)+", current_timestamp, "+std::to_string(-w)+", "+std::to_string(Withdrawal)+");";

    return updateAccountQuery(stmt,logstmt) != 0;
}

// Get Accounts Function
static std::map<int, std::shared_ptr<Account>> getAccountsQuery(const std::string& stmt) {
    try {
        // open database
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE);
        // select all accounts with join on checking
        SQLite::Statement query(db, stmt);
        // create vector to store results (in case more than one, return first)
        std::map<int, std::shared_ptr<Account>> accountsVector;

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
                std::shared_ptr<Account> p = std::make_shared<CheckingAccount>(id, name, balance, minBal, maxDep, maxWith);
                accountsVector.emplace(p->getId(),p);
            } else {
                std::shared_ptr<Account> p = std::make_shared<SavingsAccount>(id, name, balance, intRate);
                accountsVector.emplace(p->getId(),p);
            }
        }
        // return first account object
        return accountsVector;
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

std::map<int, std::shared_ptr<Account>> db::getAccountsById(int accId) {
    std::string stmt =
            "SELECT a.ID, NAME, BALANCE, C.MINBALANCE, MAXDEPOSIT, MAXWITHDRAW, S.INTERESTRATE FROM ACCOUNT as a"
            "INNER JOIN CHECKINGACCOUNT C on a.ID = C.ACCOUNTID"
            "INNER JOIN SAVINGSACCOUNT S on a.ID = S.ACCOUNTID"
            "WHERE ID = " + std::to_string(accId) + ";";

    return getAccountsQuery(stmt);
}

std::map<int, std::shared_ptr<Account>> db::getAccountsByName(const std::string& name) {
    std::string stmt =
            "SELECT a.ID, NAME, BALANCE, C.MINBALANCE, MAXDEPOSIT, MAXWITHDRAW, S.INTERESTRATE FROM ACCOUNT as a"
            "LEFT JOIN CHECKINGACCOUNT C on a.ID = C.ACCOUNTID"
            "LEFT JOIN SAVINGSACCOUNT S on a.ID = S.ACCOUNTID"
            "WHERE NAME = " + name + ";";

    return getAccountsQuery(stmt);
}

std::map<int, std::shared_ptr<Account>> db::getAllAccounts() {
    std::string stmt =
            "SELECT a.ID, NAME, BALANCE, C.MINBALANCE, MAXDEPOSIT, MAXWITHDRAW, S.INTERESTRATE FROM ACCOUNT as a "
            "LEFT JOIN CHECKINGACCOUNT C on a.ID = C.ACCOUNTID "
            "LEFT JOIN SAVINGSACCOUNT S on a.ID = S.ACCOUNTID;";

    return getAccountsQuery(stmt);
}

std::map<int, std::shared_ptr<CheckingAccount>> db::getAllCheckingAccounts() {
    try {
        // open database
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE);
        // select all accounts with join on checking
        SQLite::Statement query(db, "SELECT * FROM ACCOUNT LEFT JOIN CHECKINGACCOUNT C on ACCOUNT.ID = C.ACCOUNTID;");
        // create vector to store results (in case more than one, return first)
        std::map<int, std::shared_ptr<CheckingAccount>> checkingAccountsMap;

        while (query.executeStep()) {
            // Get typed column values
            int id = query.getColumn(0);
            const char *name = query.getColumn(1);
            double balance = query.getColumn(2);
            double minBal = query.getColumn(4);
            double maxDep = query.getColumn(5);
            double maxWith = query.getColumn(6);

            // Create temp, print out details and push onto vector
            std::shared_ptr<CheckingAccount> p = std::make_shared<CheckingAccount>(id, name, balance, minBal, maxDep, maxWith);
            checkingAccountsMap.emplace(p->getId(),p);
        }

        // return account vector
        return checkingAccountsMap;
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

std::map<int, std::shared_ptr<SavingsAccount>> db::getAllSavingsAccounts() {
    try {
        // open database
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE);
        // select all accounts with join on savings
        SQLite::Statement query(db, "SELECT * FROM ACCOUNT INNER JOIN SAVINGSACCOUNT S on ACCOUNT.ID = S.ACCOUNTID;");
        // create vector to store results (in case more than one, return first)
        std::map<int, std::shared_ptr<SavingsAccount>> savingsAccountsMap;

        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep()) {
            // Get typed column values
            int id = query.getColumn(0);
            const char *name = query.getColumn(1);
            double balance = query.getColumn(2);
            double intRate = query.getColumn(4);

            // Create temp, print details and push onto vector
            std::shared_ptr<SavingsAccount> p = std::make_shared<SavingsAccount>(id, name, balance, intRate);
            savingsAccountsMap.emplace(p->getId(),p);
        }

        // return first account object
        return savingsAccountsMap;
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

static std::map<int, Transaction> getTransactionsQuery(const std::string& stmt) {
    try {
        // open database
        SQLite::Database db("data.db", SQLite::OPEN_READWRITE);
        // select all accounts with join on savings
        SQLite::Statement query(db, stmt);
        // create vector to store results (in case more than one, return first)
        std::map<int, Transaction> transactions;

        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep()) {
            // Get typed column values
            int id = query.getColumn(0);
            int aId = query.getColumn(1);
            std::string timestamp = query.getColumn(2);
            double amtChange = query.getColumn(3);
            int tt = query.getColumn(4);

            // Create temp, print details and push onto vector
            transactions.emplace(id,Transaction(id, aId, timestamp, amtChange, static_cast<TransactionType>(tt)));
        }

        // return first account object
        return transactions;
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

std::map<int, Transaction> db::getAllTransactions(){
    std::string stmt = "SELECT * FROM TRANSACTIONLOG;";

    return getTransactionsQuery(stmt);
}

std::map<int, Transaction> db::getTransactionsByAccount(const int &accountId){
    std::string stmt =
            "SELECT * FROM TRANSACTIONLOG"
            " WHERE ACCOUNTID = "+std::to_string(accountId)+";";

    return getTransactionsQuery(stmt);
}