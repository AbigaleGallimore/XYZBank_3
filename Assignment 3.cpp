#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// =======================================================
// 1. Transaction Class
// =======================================================
class Transaction {
private:
    double amount; 
    std::string type; 

public:
    // Parameterized constructor
    Transaction(double amt, const std::string& kind)
        : amount(amt), type(kind) {}

    // Public function report()
    std::string report() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "[" << type << "] Amount: $" << amount;
        return ss.str(); 
    }
};

// =======================================================
// 2. Account Class
// =======================================================
class Account {
protected:
    double balance;
    std::vector<Transaction> log; 

    void addTransaction(const std::string& type, double amount) {
        log.push_back(Transaction(amount, type)); 
    }

public:
    Account(double init_balance) : balance(init_balance) {}
    
    virtual ~Account() = default;

    double getBalance() const { return balance; }

    bool deposit(double amount) {
        try {
            if (amount <= 0) {
                throw std::invalid_argument("Deposit amount must be positive.");
            }
            balance += amount;
            addTransaction("Deposit", amount);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Deposit failed: " << e.what() << std::endl;
            return false;
        }
    }

    virtual bool withdraw(double amount) {
        try {
            if (amount <= 0) {
                throw std::invalid_argument("Withdrawal amount must be positive.");
            }
            if (amount > balance) {
                throw std::runtime_error("Insufficient funds for withdrawal.");
            }
            balance -= amount;
            addTransaction("Withdrawal", amount);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Withdrawal failed: " << e.what() << std::endl;
            return false;
        }
    }
    
    std::vector<std::string> report() const {
        std::vector<std::string> transactionHistory;
        std::stringstream header;
        header << "--- Transaction Report for Account (Balance: $" 
               << std::fixed << std::setprecision(2) << balance << ") ---";
        transactionHistory.push_back(header.str());

        if (log.empty()) {
            transactionHistory.push_back("No transactions recorded.");
        } else {
            for (const auto& t : log) {
                transactionHistory.push_back(t.report());
            }
        }
        transactionHistory.push_back("--- End of Report ---");
        return transactionHistory;
    }
};


// =======================================================
// 3. Main File Testing and File Handling
// =======================================================

void saveReportToFile(const std::vector<std::string>& reportData) {
    const std::string filename = "C:\\transactions.txt";
    
    try {
        std::ofstream outfile(filename);

        if (!outfile.is_open()) {
            throw std::runtime_error("Could not open file for writing. Check permissions or path.");
        }

        for (const std::string& line : reportData) {
            outfile << line << "\n";
        }

        outfile.close();
        std::cout << "\n[SUCCESS] Report successfully saved to: " << filename << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "\n[ERROR] File saving failed: " << e.what() << std::endl;
        std::cerr << "NOTE: Saving directly to C:\\ may require administrative privileges or a different path." << std::endl;
    }
}

int main() {
    std::cout << "--- XYZ Bank Application Testing ---\n";
    Account myAccount(1000.00);
    
    // Test Transactions
    std::cout << "\nTesting Deposit...\n";
    myAccount.deposit(500.00);
    myAccount.deposit(15.50);

    std::cout << "\nTesting Valid Withdrawal...\n";
    myAccount.withdraw(200.00);

    // Test Error Handling 
    std::cout << "\nTesting Invalid Withdrawal (Defensive Programming)...\n";
    myAccount.withdraw(5000.00); 

    std::cout << "Testing Invalid Deposit (Defensive Programming)...\n";
    myAccount.deposit(-100.00); 

    // Generate Report
    std::vector<std::string> report = myAccount.report(); 
    
    std::cout << "\n--- Displaying Transaction Report ---\n";
    for (const std::string& line : report) {
        std::cout << line << "\n";
    }

    // File Saving
    saveReportToFile(report); 

    return 0;
}