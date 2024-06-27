#include <iostream>
#include <unordered_map>
#include <ctime>
#include <iomanip>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#ifdef _WIN32
#include <cstdlib>

void clearScreen() {
    std::system("cls");
}
#else
#include <cstdlib>

void clearScreen() {
    std::system("clear");
}
#endif

// Class Transaction
class Transaction {
private:
    std::string transactionType;
    int amount;
    std::time_t transactionDate;

public:
    Transaction(const std::string& type, int amt) {
        transactionType = type;
        amount = amt;
        transactionDate = std::time(nullptr);
    }

    std::string getTransactionType() const {
        return transactionType;
    }

    int getAmount() const {
        return amount;
    }

    std::string getTransactionDate() const {
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&transactionDate));
        return std::string(buffer);
    }
};

// Class Account
class Account {
private:
    std::string accountNumber;
    std::string accountHolder;
    int balance;
    std::vector<Transaction> transactions;

public:
    Account(const std::string& number, const std::string& holder, int initialBalance) {
        accountNumber = number;
        accountHolder = holder;
        balance = initialBalance;
    }

    std::string getAccountNumber() const {
        return accountNumber;
    }

    std::string getAccountHolder() const {
        return accountHolder;
    }

    int getBalance() const {
        return balance;
    }

    bool deposit(int amount) {
        if (amount > 0) {
            balance += amount;
            transactions.emplace_back("Deposit", amount);
            return true;
        }
        return false;
    }

    bool withdraw(int amount) {
        if (balance >= amount && amount > 0) {
            balance -= amount;
            transactions.emplace_back("Withdrawal", amount);
            return true;
        }
        return false;
    }

    void printTransactions() const {
        std::cout << std::setw(25) << std::left << "Transaction Type"
                  << std::setw(25) << std::left << "Amount"
                  << std::setw(25) << std::left << "Date" << std::endl;
        std::cout << std::string(75, '-') << std::endl;
        for (const Transaction& transaction : transactions) {
            std::cout << std::setw(25) << std::left << transaction.getTransactionType()
                      << "Rp " << std::setw(25) << std::left << transaction.getAmount()
                      << std::setw(25) << std::left << transaction.getTransactionDate() << std::endl;
        }
    }
};

// Class Bank
class Bank {
private:
    std::unordered_map<std::string, std::unique_ptr<Account>> accounts;

public:
    void addAccount(std::unique_ptr<Account> account) {
        std::string accountNumber = account->getAccountNumber();
        accounts[accountNumber] = std::move(account);
    }

    bool accountExists(const std::string& accountNumber) const {
        return accounts.find(accountNumber) != accounts.end();
    }

    bool deposit(const std::string& accountNumber, int amount) {
        auto it = accounts.find(accountNumber);
        if (it != accounts.end()) {
            return it->second->deposit(amount);
        }
        return false;
    }

    bool withdraw(const std::string& accountNumber, int amount) {
        auto it = accounts.find(accountNumber);
        if (it != accounts.end()) {
            return it->second->withdraw(amount);
        }
        return false;
    }

    int getBalance(const std::string& accountNumber) const {
        auto it = accounts.find(accountNumber);
        if (it != accounts.end()) {
            return it->second->getBalance();
        }
        return 0;
    }

    void printTransactions(const std::string& accountNumber) const {
        auto it = accounts.find(accountNumber);
        if (it != accounts.end()) {
            std::cout << "Transaction history for account " << accountNumber << ":" << std::endl;
            it->second->printTransactions();
        } else {
            std::cout << "Account " << accountNumber << " does not exist." << std::endl;
        }
    }
};

// Function to display ATM menu
void displayMenu() {
    std::cout << "============================================" << std::endl;
    std::cout << std::setw(40) << std::left << "Welcome to ATM" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::setw(40) << std::left << "1. Create Account" << std::endl;
    std:: cout << std::setw(40) << std::left << "2. Deposit" << std::endl;
    std::cout << std::setw(40) << std::left << "3. Withdrawal" << std::endl;
    std::cout << std::setw(40) << std::left << "4. Check Balance" << std::endl;
    std::cout << std::setw(40) << std::left << "5. Transaction History" << std::endl;
    std::cout << std::setw(40) << std::left << "6. Exit" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "Enter your choice: ";
}

// Function to pause execution (for Windows only)
void pause() {
    #ifdef _WIN32
        system("pause");
    #endif
}

// Main function
int main() {
    Bank bank;

    int choice;
    std::string accountNumber;
    std::string accountHolder;
    int amount;

    do {
        clearScreen();
        displayMenu();

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please try again." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pause(); // Pause execution after displaying error message
            continue;
        }

        switch (choice) {
            case 1:
                system("cls");
                std::cout << "============================================" << std::endl;
                std::cout << "Create Account" << std::endl;
                std::cout << "============================================" << std::endl;
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;

                if (bank.accountExists(accountNumber)) {
                    std::cout << "Account number already exists. Try again with a different number." << std::endl;
                    break;
                }

                std::cout << "Enter account holder name: ";
                std::cin.ignore(); // Ignore the newline character left in the buffer
                std::getline(std::cin, accountHolder);
                std::cout << "Enter initial balance: ";
                if (!(std::cin >> amount) || amount < 0) {
                    std::cout << "Invalid initial balance." << std::endl;
                    system("pause");
                    break;
                }
                bank.addAccount(std::make_unique<Account>(accountNumber, accountHolder, amount));
                std::cout << "Account created successfully." << std::endl;

                //system("pause");
                break;

            case 2:
                system("cls");
                std::cout << "============================================" << std::endl;
                std::cout << "Deposit" << std::endl;
                std::cout << "============================================" << std::endl;
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                std::cout << "Enter deposit amount: ";
                if (!(std::cin >> amount) || amount <= 0) {
                    std::cout << "Invalid deposit amount." << std::endl;
                    system("pause");
                    break;
                }
                //system("cls");
                if (bank.deposit(accountNumber, amount)) {
                    std::cout << "Deposit successful." << std::endl;
                } else {
                    std::cout << "Invalid account number." << std::endl;
                }
                //system("pause");
                break;

            case 3:
            system("cls");
                std::cout << "============================================" << std::endl;
                std::cout << "Withdrawal" << std::endl;
                std::cout << "============================================" << std::endl;
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                std::cout << "Enter withdrawal amount: ";
                if (!(std::cin >> amount) || amount <= 0) {
                    std::cout << "Invalid withdrawal amount." << std::endl;
                    break;
                }

                if (bank.withdraw(accountNumber, amount)) {
                    std::cout << "Withdrawal successful." << std::endl;
                } else {
                    std::cout << "Invalid account number or insufficient balance." << std::endl;
                }
                system("pause");
                break;

            case 4:
            system("cls");
                std::cout << "============================================" << std::endl;
                std::cout << "Check Balance" << std::endl;
                std::cout << "============================================" << std::endl;
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                std::cout << "Balance for account " << accountNumber << ": Rp " << bank.getBalance(accountNumber) << std::endl;
                system("pause");
                break;

            case 5:
            system("cls");
                std::cout << "============================================" << std::endl;
                std::cout << "Transaction History" << std::endl;
                std::cout << "============================================" << std::endl;
                std::cout << "Enter account number: ";
                std::cin >> accountNumber;
                bank.printTransactions(accountNumber);
                system("pause");
                break;

            case 6:
                std::cout << "Exiting..." << std::endl;
                break;

            default:
                std::cout << "Invalid choice. Try again." << std::endl;
                break;
        }

        std::cout << std::string(40, '=') << std::endl; // Print separator after each menu
        pause(); // Pause execution after displaying menu
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    } while (choice != 6);

    return 0;
}