Bank System

A simple C++ console-based banking system that allows users to create accounts, view account details, withdraw, deposit, and pay bills.

Features

User authentication with login credentials

Account creation with name, email, password, CNIC, and initial balance

View account details

Withdraw and deposit money

Pay bills (Gas, Electricity, Water)

Data stored in text files for persistence

Installation

Clone the repository:

git clone https://github.com/yourusername/bank-system.git

Navigate to the project directory:

cd bank-system

Compile the program using g++:

g++ -o bank_system bank_system.cpp

Run the program:

./bank_system

Usage

Run the program and log in.

Once logged in, you can:

Create a new account

View account details

Withdraw money

Deposit money

Pay bills

Follow the on-screen instructions for each operation.

File Structure

bank_system.cpp - The main source code file.

login_db.txt - Stores login credentials.

account_db.txt - Stores account details.

Notes

The system uses text files (login_db.txt and account_db.txt) to store user information.

Ensure these files exist before running the program.
