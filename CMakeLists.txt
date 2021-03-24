cmake_minimum_required(VERSION 3.17)
project(BankingApplication)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp/include
)

add_executable(BankingApplication src/main.cpp src/Account.cpp src/Account.h src/CheckingAccount.cpp src/CheckingAccount.h src/Transaction.cpp src/Transaction.h src/SavingsAccount.cpp src/SavingsAccount.h src/Db.cpp src/Db.h)

target_link_libraries(
        BankingApplication
        SQLiteCpp
        sqlite3
        pthread
        dl
)