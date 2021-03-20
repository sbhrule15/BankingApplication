#include <iostream>
#include <iomanip>
#include <string>
#include "Account.h"

enum MainMenu {
    MainDeposit= 1, MainWithdraw, MainTransactions, MainOpenAccount, MainCloseAccount, MainQuit
};

// print functions
static void printMenuHeader(std::string text){
    std::cout <<'|'<< std::setfill('=') << std::setw(80) << text << "====================|\n" << std::endl;
}

static int printMenu(std::vector<std::string> *options) {
    int userSel;
    for (int i = 0; i < options->size(); i++){
        std::cout << "\t" << i+1 << "." << options->at(i) << std::endl;
    }
    std::cout << "\nPlease make a selection:" << std::endl;
    std::cin >> userSel;
    std::cout << "\n";
    delete options;
    return userSel;
}

static bool noAccounts(std::vector<Account> &accounts){
    if (accounts.empty()){
        return true;
    } else {return false}
}

// menu functions
static void viewDepositMenu(std::vector<Account> &accounts) {
    printMenuHeader("MAKE A DEPOSIT");
    std::cout << "This part of the application is still in production. Please try again later.\n\n" << std::endl;
}

static void viewWithdrawMenu(std::vector<Account> &accounts) {
    printMenuHeader("MAKE A WITHDRAWAL");
    std::cout << "This part of the application is still in production. Please try again later.\n\n" << std::endl;
}

static void viewTransactionsMenu(std::vector<Account> &accounts) {
    printMenuHeader("VIEW TRANSACTIONS");
    std::cout << "This part of the application is still in production. Please try again later.\n\n" << std::endl;
}

static void openAccountMenu(std::vector<Account> &accounts) {
    printMenuHeader("OPEN AN ACCOUNT");
    std::cout << "This part of the application is still in production. Please try again later.\n\n" << std::endl;
}

static void closeAccountMenu(std::vector<Account> &accounts) {
    printMenuHeader("CLOSE AN ACCOUNT");
    std::cout << "This part of the application is still in production. Please try again later.\n\n" << std::endl;
}

int main(int argc, char* argv[]) {
    // load vector of accounts - blank for now
    std::vector<Account> accounts;

    // welcome screen
    printMenuHeader("JOSHUA'S BANKING PROGRAM");
    std::cout << "Welcome to my C++ banking program.\n" << std::endl;

    // start program loop
    while (true){
        printMenuHeader("MAIN MENU");
        int usrSel = printMenu(new std::vector<std::string>{"Deposit","Withdraw","View Transactions","Open Account","Close Account",
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
                // get rid of failure state
                std::cin.clear();

                // discard 'bad' character(s)
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    exit_program: ;
    std::cout << "\nThank you for using the program. Goodbye.\n" << std::endl;
}
