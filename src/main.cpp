#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include "Account.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include "Db.h"

// enums
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

static bool noAccounts(const std::map<int, std::shared_ptr<Account>> &accounts) {
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
template<typename T>
static T printAccountMenu(const std::map<int, std::shared_ptr<Account>> &accts, MenuChoiceOptions mco,
                          AdditionalAccountInfo info) {
    int userSel;
    std::map<int, T> accountChoiceMap;
    int menuNum = 1;

    // iterate through map
    for (auto const&[key, val] : accts) {
        std::cout << "\t" << menuNum << "." << val->getName() + " - $" << std::setprecision(2) << std::fixed
                  << val->getBalance() << std::endl;

        // add val to accountChoiceMap with desired data
        if (mco == convertChoiceToAccountId)
            accountChoiceMap.emplace(std::make_pair(menuNum, key));

        menuNum++;
    }
    while (true) {
        std::cout << "\nPlease choose an account:" << std::endl;
        std::cin >> userSel;
        std::cout << "\n";
        if (accountChoiceMap.count(userSel)) {
            std::cout << "Account Chosen: " << accts.at(accountChoiceMap.at(userSel))->getName() << std::endl;
            break;
        } else {
            std::cout << "That was an invalid option." << std::endl;
            clearCinGuard();
        }
    }

    // get mapped selection value and return
    return accountChoiceMap.at(userSel);
}

// menu functions
static void viewDepositMenu() {
    std::map<int, std::shared_ptr<Account>> accounts = db::getAllAccounts();

    float depAmt;
    printMenuHeader("MAKE A DEPOSIT");
    if (noAccounts(accounts)) {
        std::cout << "\nThere are no accounts to deposit into. Please open an account to deposit money.\n" << std::endl;
        return;
    }
    while (true) {
        std::cout << "Here are your accounts:\n" << std::endl;
        // list accounts with balance
        int accountId = printAccountMenu<int>(accounts, convertChoiceToAccountId, WithBalance);
        if (accounts.count(accountId)) {
            std::cout << "\nPlease enter the amount to deposit:" << std::endl;
            std::cin >> depAmt;
            if (accounts.at(accountId)->deposit(depAmt))
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

static void viewWithdrawMenu() {
    std::map<int, std::shared_ptr<Account>> accounts = db::getAllAccounts();

    float witAmt;
    printMenuHeader("MAKE A WITHDRAWAL");
    if (noAccounts(accounts)) {
        std::cout << "\nThere are no accounts to withdraw from. Please open an account to withdraw money.\n"
                  << std::endl;
        return;
    }
    while (true) {
        std::cout << "Here are your accounts:\n" << std::endl;
        // list accounts with balance
        int accountId = printAccountMenu<int>(accounts, convertChoiceToAccountId, WithBalance);
        if (accounts.count(accountId)) {
            std::cout << "\nPlease enter the amount to withdraw:" << std::endl;
            std::cin >> witAmt;
            if (accounts.at(accountId)->withdraw(witAmt))
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

static void viewTransactionsMenu() {
    printMenuHeader("VIEW TRANSACTIONS");
    std::map<int, std::vector<Transaction>> transactionLog;

    // menu option validator
    bool awaitingValid{true};
    int userSel = 0;

    // get account type
    while (awaitingValid) {
        std::cout << "Which transactions would you like to see?\n" << std::endl;
        userSel = printMenu(std::vector<std::string>{"All Transactions by Date", "All Transactions by Account"});

        switch (userSel) {
            case 1:
                transactionLog = db::getAllTransactions();
                std::cout << "\nHere are all of your transactions by date:\n" << std::endl;
                awaitingValid = false;
                break;
            case 2:
                transactionLog = db::getTransactionsByAccount();
                std::cout << "\nHere are the transactions associated each account:\n" << std::endl;
                awaitingValid = false;
                break;
            default:
                std::cout << "That was not a valid option. Please choose one of the given options by number.\n"
                          << std::endl;
                clearCinGuard();
                break;
        }
    }

    for (auto [key, val] : transactionLog) {
        switch (userSel) {
            case 1:
                for (auto t : val)
                    t.printTransaction();
                break;
            case 2:
                printMenuHeader(&"ACCOUNT ID: " [ key]);
                for (auto t : val)
                    t.printTransaction();
                break;
            default:
                std::cout << "There was an error. Please try again.";
                break;
        }
    }
}

static void openAccountMenu() {
    std::map<int, std::shared_ptr<Account>> accounts = db::getAllAccounts();

    printMenuHeader("OPEN AN ACCOUNT");

    // new account info
    AccountType newAcctType;
    std::string newAcctName;
    std::string acctTypeStr;

    // menu option validator
    bool awaitingValid{true};

    // get account type
    while (awaitingValid) {
        std::cout << "What type of account would you like to open up?\n" << std::endl;
        int userSel = printMenu(std::vector<std::string>{"Checking", "Savings"});

        switch (userSel) {
            case 1:
                newAcctType = Checking;
                acctTypeStr = "checking";
                awaitingValid = false;
                break;
            case 2:
                newAcctType = Savings;
                acctTypeStr = "savings";
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

    // create account and insert into database
    try {
        if (db::createAccount(newAcctName, newAcctType))
            std::cout << "\nNew " << acctTypeStr << " account '" + newAcctName + "' created.\n\n" << std::endl;
        else {
            throw;
        }
    } catch (std::exception &e) {
        std::cout << "There was an error creating your account. Please try again later." << std::endl;
    }
}

static void closeAccountMenu() {
    std::map<int, std::shared_ptr<Account>> accounts = db::getAllAccounts();

    printMenuHeader("CLOSE AN ACCOUNT");
    if (noAccounts(accounts)) {
        std::cout << "\nThere are no accounts to deposit into. Please open an account to deposit money.\n" << std::endl;
        return;
    }
}

int main(int argc, char *argv[]) {
    // create tables, if don't exist
    db::initDB();

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
                viewDepositMenu();
                break;
            case MainWithdraw:
                viewWithdrawMenu();
                break;
            case MainTransactions:
                viewTransactionsMenu();
                break;
            case MainOpenAccount:
                openAccountMenu();
                break;
            case MainCloseAccount:
                closeAccountMenu();
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