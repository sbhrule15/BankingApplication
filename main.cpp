#include <iostream>
#include <iomanip>
#include <string>
#include "Account.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include "Db.h"

#include "sqlite3.h"

enum MainMenu {
    MainDeposit = 1, MainWithdraw, MainTransactions, MainOpenAccount, MainCloseAccount, MainQuit
};

//utility functions
static void clearCinGuard() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static bool noAccounts(std::vector<Account> &accounts) {
    if (accounts.empty()) {
        return true;
    } else { return false; }
}

static std::vector<std::string> getAccountNames(std::vector<Account> &accounts) {
    std::vector<std::string> actstrings;
    actstrings.reserve(accounts.size());

    for (const Account &a : accounts)
        actstrings.push_back(a.getName());

    return actstrings;
}

// print functions
static void printMenuHeader(const std::string &text) {
    std::cout << '|' << std::setfill('=') << std::setw(80) << text << "====================|\n" << std::endl;
}

static int printMenu(std::vector<std::string> options) {
    int userSel;
    for (int i = 0; i < options.size(); i++) {
        std::cout << "\t" << i + 1 << "." << options.at(i) << std::endl;
    }
    std::cout << "\nPlease make a selection:" << std::endl;
    std::cin >> userSel;
    std::cout << "\n";
    return userSel;
}

// menu functions
static void viewDepositMenu(std::vector<Account> &accounts, const char *db) {
    float depAmt;
    printMenuHeader("MAKE A DEPOSIT");
    if (noAccounts(accounts)) {
        std::cout << "\nThere are no accounts to deposit into. Please open an account to deposit money.\n" << std::endl;
        return;
    }
    while (true) {
        std::cout << "Please select an account to make a deposit:" << std::endl;
        int accountSel = printMenu(getAccountNames(accounts));
        if (accountSel <= accounts.size() && accountSel > 0) {
            std::cout << "\nPlease enter the amount to deposit:" << std::endl;
            std::cin >> depAmt;
            if (accounts.at(accountSel - 1).deposit(depAmt))
                std::cout << "\nDeposit successfully processed.\n" << std::endl;
            else
                std::cout << "\nDeposit was unsuccessful.\n" << std::endl;
            break;
        } else {
            std::cout << "That was an invalid option. Please try again.\n" << std::endl;
            clearCinGuard();
        }
    }
}

static void viewWithdrawMenu(std::vector<Account> &accounts, const char *db) {
    float witAmt;
    printMenuHeader("MAKE A WITHDRAWAL");
    if (noAccounts(accounts)) {
        std::cout << "\nThere are no accounts to withdraw from. Please open an account to withdraw money.\n"
                  << std::endl;
        return;
    }
    while (true) {
        std::cout << "Please select an account to make a deposit:" << std::endl;
        int accountSel = printMenu(getAccountNames(accounts));
        if (accountSel <= accounts.size() && accountSel > 0) {
            std::cout << "\nPlease enter the amount to withdraw:" << std::endl;
            std::cin >> witAmt;
            if (accounts.at(accountSel - 1).withdraw(witAmt))
                std::cout << "\nWithdrawal successfully processed.\n" << std::endl;
            else
                std::cout << "\nWithdrawal was unsuccessful.\n" << std::endl;
            break;
        } else {
            std::cout << "That was an invalid option. Please try again.\n" << std::endl;
            clearCinGuard();
        }
    }
}

static void viewTransactionsMenu(std::vector<Account> &accounts, const char *db) {
    printMenuHeader("VIEW TRANSACTIONS");
    std::cout << "This part of the application is still in production. Please try again later.\n\n" << std::endl;
}

static void openAccountMenu(std::vector<Account> &accounts, const char *db) {
    printMenuHeader("OPEN AN ACCOUNT");

    // new account info
    AccountType newAcctType;
    std::string newAcctName;

    // menu option validator
    bool awaitingValid{true};

    // get account type
    while (awaitingValid) {
        std::cout << "What type of account would you like to open up?\n" << std::endl;
        int userSel = printMenu(std::vector<std::string>{"Checking", "Savings"});
        switch (userSel) {
            case 1:
                newAcctType = Checking;
                awaitingValid = false;
                break;
            case 2:
                newAcctType = Savings;
                awaitingValid = false;
                break;
            default:
                std::cout << "That was not a valid option. Please choose one of the given options by number.\n"
                          << std::endl;
                clearCinGuard();
                break;
        }
    }

    // clear buffer and get account name
    std::cout << "Please name the account:" << std::endl;
    std::cin.ignore();
    getline(std::cin, newAcctName);

    //add to accounts vector
    if (newAcctType == Checking) {
        // create new checking account
        CheckingAccount newCheck(newAcctName);
        // add to database
        if (db::createCheckingAccount(db, newCheck)){
            // push to array and confirmation message
            accounts.push_back(newCheck);
            std::cout << "\nNew Checking account " + newAcctName + " created.\n\n" << std::endl;
        } else {
            std::cout << "There was an error creating your account. Please try again later." << std::endl;
        }
    } else if (newAcctType == Savings) {
        // create new savings account
        SavingsAccount newSavings(newAcctName);
        // add to database
        if (db::createSavingsAccount(db, newSavings)){
            // push to array and confirmation message
            accounts.push_back(newSavings);
            std::cout << "\nNew Savings account " + newAcctName + " created.\n\n" << std::endl;
        } else {
            std::cout << "There was an error creating your account. Please try again later." << std::endl;
        }
    } else {
        std::cout << "There was an error creating your account. Please try again later." << std::endl;
    }
}

static void closeAccountMenu(std::vector<Account> &accounts, const char *db) {
    printMenuHeader("CLOSE AN ACCOUNT");
    if (noAccounts(accounts)) {
        std::cout << "\nThere are no accounts to deposit into. Please open an account to deposit money.\n" << std::endl;
        return;
    }
}

int main(int argc, char *argv[]) {
    // database connection
    const char *db = "accountapp.db";
    // create database, if doesn't exist
    db::createDB(db);
    // create tables, if don't exist
    db::createTables(db);

    // load vector of accounts - blank for now
    std::vector<Account> accounts;

    // welcome screen
    printMenuHeader("JOSHUA'S BANKING PROGRAM");
    std::cout << "Welcome to my C++ banking program.\n" << std::endl;

    // start program loop
    while (true) {
        printMenuHeader("MAIN MENU");
        int usrSel = printMenu(
                std::vector<std::string>{"Deposit", "Withdraw", "View Transactions", "Open Account", "Close Account",
                                         "Quit Application"});

        switch (usrSel) {
            case MainDeposit:
                viewDepositMenu(accounts, db);
                break;
            case MainWithdraw:
                viewWithdrawMenu(accounts, db);
                break;
            case MainTransactions:
                viewTransactionsMenu(accounts, db);
                break;
            case MainOpenAccount:
                openAccountMenu(accounts, db);
                break;
            case MainCloseAccount:
                closeAccountMenu(accounts, db);
                break;
            case MainQuit:
                goto exit_program;
            default:
                std::cout << "That was an invalid option. Please try again.\n" << std::endl;
                // get rid of failure state and delete bad characters
                clearCinGuard();
        }
    }
    exit_program:;
    std::cout << "\nThank you for using the program. Goodbye.\n" << std::endl;
}
