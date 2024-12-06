#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>

class BankSystem {
private:
    const std::string loginDB = "login_db.txt";
    const std::string accountDB = "account_db.txt";

    std::string generateAccountNumber() {
        srand(time(0));
        return "PK" + std::to_string(100000 + rand() % 900000);
    }

    bool validateLogin(const std::string& username, const std::string& password) {
        std::ifstream file(loginDB);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open login database.\n";
            return false;
        }

        std::string dbUsername, dbPassword;
        while (file >> dbUsername >> dbPassword) {
            if (dbUsername == username && dbPassword == password) {
                return true;
            }
        }
        return false;
    }

    bool loadAccount(const std::string& accountNumber, std::map<std::string, std::string>& account) {
        std::ifstream file(accountDB);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open account database.\n";
            return false;
        }

        std::string accNum, name, email, password, cnic, balance;
        while (file >> accNum >> name >> email >> password >> cnic >> balance) {
            if (accNum == accountNumber) {
                account["accountNumber"] = accNum;
                account["name"] = name;
                account["email"] = email;
                account["password"] = password;
                account["cnic"] = cnic;
                account["balance"] = balance;
                return true;
            }
        }
        return false;
    }

    bool updateBalance(const std::string& accountNumber, const std::string& newBalance) {
        std::ifstream file(accountDB);
        std::ofstream tempFile("temp_db.txt");
        if (!file.is_open() || !tempFile.is_open()) {
            std::cerr << "Error: Unable to update account database.\n";
            return false;
        }

        std::string accNum, name, email, password, cnic, balance;
        bool updated = false;
        while (file >> accNum >> name >> email >> password >> cnic >> balance) {
            if (accNum == accountNumber) {
                tempFile << accNum << " " << name << " " << email << " " << password << " " << cnic << " " << newBalance << "\n";
                updated = true;
            } else {
                tempFile << accNum << " " << name << " " << email << " " << password << " " << cnic << " " << balance << "\n";
            }
        }
        file.close();
        tempFile.close();

        remove(accountDB.c_str());
        rename("temp_db.txt", accountDB.c_str());
        return updated;
    }

    void createAccount() {
        std::string name, email, password, cnic;
        double balance;

        std::cout << "\n===== Create Account =====\n";
        std::cout << "Enter Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "Enter Email: ";
        std::getline(std::cin, email);
        std::cout << "Enter Password: ";
        std::getline(std::cin, password);
        std::cout << "Enter CNIC: ";
        std::getline(std::cin, cnic);
        std::cout << "Enter Initial Balance: ";
        std::cin >> balance;

        std::string accountNumber = generateAccountNumber();

        std::ofstream file(accountDB, std::ios::app);
        if (file.is_open()) {
            file << "accountnumber= " << accountNumber << " " << "name= " << name << " " << "email= " << email << " " << "password= " << password << " " << "cnic= " << cnic << " " << "balance= " << balance << "\n";
            std::cout << "Account Created Successfully!\n";
        } else {
            std::cout << "Error: Unable to create account.\n";
        }
    }

    void viewAccount() {
        std::string accountNumber, password;
        std::cout << "\nEnter Account Number: ";
        std::cin >> accountNumber;
        std::cout << "Enter Password: ";
        std::cin >> password;

        std::map<std::string, std::string> account;
        if (loadAccount(accountNumber, account) && account["password"] == password) {
            std::cout << "\n===== Account Details =====\n";
            std::cout << "Account Number: " << account["accountNumber"] << "\n";
            std::cout << "Name: " << account["name"] << "\n";
            std::cout << "Email: " << account["email"] << "\n";
            std::cout << "CNIC: " << account["cnic"] << "\n";
            std::cout << "Balance: " << account["balance"] << "\n";
        } else {
            std::cout << "Invalid account number or password.\n";
        }
    }

    void withdraw() {
        std::string accountNumber;
        double amount;
        std::cout << "\nEnter Account Number: ";
        std::cin >> accountNumber;
        std::cout << "Enter Amount to Withdraw: ";
        std::cin >> amount;

        std::map<std::string, std::string> account;
        if (loadAccount(accountNumber, account)) {
            double currentBalance = std::stod(account["balance"]);
            if (amount <= 0 || amount > currentBalance) {
                std::cout << "Invalid withdrawal amount.\n";
            } else {
                currentBalance -= amount;
                if (updateBalance(accountNumber, std::to_string(currentBalance))) {
                    std::cout << "Withdrawal successful. New Balance: " << accountNumber << " " << currentBalance << "\n";
                }
            }
        } else {
            std::cout << "Invalid account number.\n";
        }
    }

    void deposit() {
        std::string accountNumber;
        double amount;
        std::cout << "\nEnter Account Number: ";
        std::cin >> accountNumber;
        std::cout << "Enter Amount to Deposit: ";
        std::cin >> amount;

        std::map<std::string, std::string> account;
        if (loadAccount(accountNumber, account)) {
            if (amount <= 0) {
                std::cout << "Invalid deposit amount.\n";
            } else {
                double currentBalance = std::stod(account["balance"]);
                currentBalance += amount;
                if (updateBalance(accountNumber, std::to_string(currentBalance))) {
                    std::cout << "Deposit successful. New Balance: " << accountNumber << " " << currentBalance << "\n";
                }
            }
        } else {
            std::cout << "Invalid account number.\n";
        }
    }

    void payBill() {
        int choice;
        std::cout << "\n===== Pay Bill Menu =====\n";
        std::cout << "1. Gas\n";
        std::cout << "2. Electricity\n";
        std::cout << "3. Water\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice < 1 || choice > 4) {
            std::cout << "Invalid choice.\n";
            return;
        }
        if (choice == 4) return;

        std::string accountNumber, password, billID;
        double amount;
        std::cout << "Enter Bill ID: ";
        std::cin >> billID;
        std::cout << "Enter Payment Amount: ";
        std::cin >> amount;
        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;
        std::cout << "Enter Password: ";
        std::cin >> password;

        std::map<std::string, std::string> account;
        if (loadAccount(accountNumber, account) && account["password"] == password) {
            double currentBalance = std::stod(account["balance"]);
            if (amount <= 0 || amount > currentBalance) {
                std::cout << "Invalid payment amount.\n";
            } else {
                currentBalance -= amount;
                if (updateBalance(accountNumber, std::to_string(currentBalance))) {
                    std::cout << "Payment successful for Bill ID: " << billID << ". New Balance: " << currentBalance << "\n";
                }
            }
        } else {
            std::cout << "Invalid account number or password.\n";
        }
    }

    void mainMenu() {
        while (true) {
            std::cout << "\n===== Main Menu =====\n";
            std::cout << "1. Login\n";
            std::cout << "2. Exit\n";
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;

            if (choice == 1) {
                loginMenu();
            } else if (choice == 2) {
                std::cout << "Exiting the program. Goodbye!\n";
                break;
            } else {
                std::cout << "Invalid choice! Please try again.\n";
            }
        }
    }

    void loginMenu() {
        std::string username, password;
        std::cout << "\n===== Login Menu =====\n";
        std::cout << "Enter Username: ";
        std::cin >> username;
        std::cout << "Enter Password: ";
        std::cin >> password;

        if (validateLogin(username, password)) {
            std::cout << "Login successful!\n";
            postLoginMenu();
        } else {
            std::cout << "Invalid username or password.\n";
        }
    }

    void postLoginMenu() {
        while (true) {
            std::cout << "\n===== Post-Login Menu =====\n";
            std::cout << "1. Create Account\n";
            std::cout << "2. View Account\n";
            std::cout << "3. Withdraw\n";
            std::cout << "4. Deposit\n";
            std::cout << "5. Pay Bill\n";
            std::cout << "6. Exit\n";
            std::cout << "Enter your choice: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: createAccount(); break;
                case 2: viewAccount(); break;
                case 3: withdraw(); break;
                case 4: deposit(); break;
                case 5: payBill(); break;
                case 6: std::cout << "Exiting. Goodbye!\n"; return;
                default: std::cout << "Invalid choice. Please try again.\n"; break;
            }
        }
    }

public:
    void run() {
        mainMenu();
    }
};

int main() {
    BankSystem bankSystem;
    bankSystem.run();
    return 0;
}
