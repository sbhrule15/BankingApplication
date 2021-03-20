#include <iostream>
#include <iomanip>
#include <string>

enum MainMenu {
    MainViewAccounts = 1, MainCreateAccount = 2, MainDeleteAccount = 3, MainQuit = 4
};

static void mainMenu() {

}

static void viewAccountsMenu() {

}

static void createAccountMenu() {

}

static void deleteAccountMenu() {

}

static void startupScreen() {

}

static std::string genMenuHeader(std::string text){
    std::cout <<'|'<< std::setfill('=') << std::setw(80) << text << "==========|" << std::endl;
}

int main() {
    // welcome screen
    genMenuHeader("JOSHUA'S BANKING PROGRAM");
    std::cout << "Welcome to my C++ banking program.\n\n" << std::endl;

    // start program loop
    while (true){
        int usrSel{0};
        genMenuHeader("MAIN MENU");
        std::cout << "Please select from the following options:\n"
            <<"\t1. View Accounts\n"
            <<"\t2. Create Account\n"
            <<"\t3. Delete Account\n"
            << std::endl;

        std::cin >> usrSel;
        switch (usrSel) {
            case MainViewAccounts:
                viewAccountsMenu();
                break;
            case MainCreateAccount:
                createAccountMenu();
                break;
            case MainDeleteAccount:
                deleteAccountMenu();
                break;
            case MainQuit:
                goto exit_program;
        }
    }

    exit_program: ;
}
