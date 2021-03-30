#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include "Account.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include "Db.h"

enum MainMenu {
    MainDeposit = 1, MainWithdraw, MainTransactions, MainOpenAccount, MainCloseAccount, MainQuit
};

enum AdditionalAccountInfo {
    NameOnly, WithBalance
};

enum MenuChoiceOptions {
    convertChoiceToAccountId, convertChoiceToName
};

//utility functions
static void clearCinGuard() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static bool noAccounts(std::map<int,Account> &accounts) {
    if (accounts.empty()) {
        return true;
    } else { return false; }
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

// can return string or id
template <typename T>
static T chooseFromAccountMenu(std::map<int,Account> &accts, MenuChoiceOptions mco, AdditionalAccountInfo info) {
    int userSel;
    std::map<int, T> accountChoiceMap;
    int menuNum = 1;

    // iterate through map
    for (auto const& [key, val] : accts) {
        std::cout << "\t" << menuNum << "." << val.getName();
        // print additional options if present
        if (info == WithBalance) {
            std::cout << std::setprecision(2) << val.getBalance();
        }
        // end print
        std::cout << std::endl;

        // add val to accountChoiceMap with desired data to chose
        if (mco == convertChoiceToName){
            accountChoiceMap.emplace(std::make_pair(menuNum, val.getName()));
        } else if (mco == convertChoiceToAccountId){
            accountChoiceMap.emplace(std::make_pair(menuNum, key));
        }

        menuNum++;
    }
    std::cout << "\nPlease choose an account:" << std::endl;
    std::cin >> userSel;
    std::cout << "\n";

    // get mapped selection value and return
    return accountChoiceMap.at(userSel);
}

// menu functions
static void viewDepositMenu(std::map<int,Account> &accounts) {
    float depAmt;
    printMenuHeader("MAKE A DEPOSIT");
    if (noAccounts(accounts)) {
        std::cout << "\nThere are no accounts to deposit into. Please open an account to deposit money.\n" << std::endl;
        return;
    }
    while (true) {
        std::cout << "Here are your accounts:\n" << std::endl;
        // list accounts with balance
        int accountSel = chooseFromAccountMenu<int>(accounts,convertChoiceToAccountId, WithBalance);
        if (accounts.count(accountSel)) {
            std::cout << "\nPlease enter the amount to deposit:" << std::endl;
            std::cin >> depAmt;
            if (accounts.at(accountSel).deposit(depAmt))
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

static void viewWithdrawMenu(std::map<int,Account> &accounts) {
    float witAmt;
    printMenuHeader("MAKE A WITHDRAWAL");
    if (noAccounts(accounts)) {
        std::cout << "\nThere are no accounts to withdraw from. Please open an account to withdraw money.\n"
                  << std::endl;
        return;
    }
    while (true) {
        std::cout << "Please select an account to make a deposit:" << std::endl;
        int accountSel = chooseFromAccountMenu<int>(accounts,convertChoiceToAccountId,WithBalance);
        if (accounts.count(accountSel)) {
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

static void viewTransactionsMenu(std::map<int,Account> &accounts) {
    printMenuHeader("VIEW TRANSACTIONS");
    std::cout << "This part of the application is still in production. Please try again later.\n\n" << std::endl;
}

static void openAccountMenu(std::map<int,Account> &accounts) {
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
        try {
            // add to db and push returned CheckingAccount object to map with key as id
            CheckingAccount newCheck = db::createCheckingAccount(newAcctName);
            accounts.emplace(std::make_pair(newCheck.getId(),newCheck));
            // confirm message
            std::cout << "\nNew Checking account " + newAcctName + " created.\n\n" << std::endl;

        } catch(std::exception &e) {
            std::cout << "There was an error creating your account. Please try again later." << std::endl;
        }
    } else if (newAcctType == Savings) {
        try {
            // add to db and push returned SavingsAccount object to vector
            SavingsAccount newSaving = db::createSavingsAccount(newAcctName);
            accounts.emplace(std::make_pair(newSaving.getId(),newSaving));
            // confirm message
            std::cout << "\nNew Savings account " + newAcctName + " created.\n\n" << std::endl;

        } catch(std::exception &e) {
            std::cout << "There was an error creating your account. Please try again later." << std::endl;
        }
    } else {
        std::cout << "There was an error creating your account. Please try again later." << std::endl;
    }
}

static void closeAccountMenu(std::map<int,Account> &accounts) {
    printMenuHeader("CLOSE AN ACCOUNT");
    if (noAccounts(accounts)) {
        std::cout << "\nThere are no accounts to deposit into. Please open an account to deposit money.\n" << std::endl;
        return;
    }
}

int main(int argc, char *argv[]) {
    // create tables, if don't exist
    db::initDB();
    // load all accounts into map for quick find
    std::map<int, Account> accounts;
    for (const Account& a : db::getAllAccounts()){
        accounts.emplace(std::make_pair(a.getId(),a));
    }

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
                viewDepositMenu(accounts);
                break;
            case MainWithdraw:
                viewWithdrawMenu(accounts);
                break;
            case MainTransactions:
                viewTransactionsMenu(accounts);
                break;
            case MainOpenAccount:
                openAccountMenu(accounts);
                break;
            case MainCloseAccount:
                closeAccountMenu(accounts);
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