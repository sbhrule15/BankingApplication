cmake_minimum_required(VERSION 3.17)
project(BankingApplication)

set(CMAKE_CXX_STANDARD 17)

add_executable(BankingApplication main.cpp Account.cpp Account.h CheckingAccount.cpp CheckingAccount.h Transaction.cpp Transaction.h SavingsAccount.cpp SavingsAccount.h sqlite3.c sqlite3.h)