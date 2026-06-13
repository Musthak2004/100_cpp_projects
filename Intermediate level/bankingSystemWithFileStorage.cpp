#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cctype>

using namespace std;

const string ACCOUNTS_FILE = "accounts.dat";
const string TRANSACTIONS_FILE = "transactions.dat";

/**
 * @brief Utility to get current timestamp as string.
 */
string getCurrentTimestamp()
{
    time_t now = time(nullptr);
    char buf[64];
#ifdef _WIN32
    ctime_s(buf, sizeof(buf), &now);
    string result(buf);
#else
    string result(ctime(&now));
#endif
    if (!result.empty() && result.back() == '\n')
        result.pop_back();
    return result;
}

/**
 * @brief Transaction structure to record account activity.
 */
struct Transaction
{
    string accountNumber;
    string type;
    double amount;
    double balanceAfter;
    string timestamp;

    string serialize() const
    {
        return accountNumber + "|" + type + "|" + to_string(amount) + "|" +
               to_string(balanceAfter) + "|" + timestamp;
    }

    static Transaction deserialize(const string &line)
    {
        Transaction t;
        stringstream ss(line);
        string token;
        getline(ss, t.accountNumber, '|');
        getline(ss, t.type, '|');
        getline(ss, token, '|');
        t.amount = stod(token);
        getline(ss, token, '|');
        t.balanceAfter = stod(token);
        getline(ss, t.timestamp, '|');
        return t;
    }

    void display() const
    {
        cout << left << setw(12) << accountNumber << setw(14) << type
             << setw(12) << fixed << setprecision(2) << amount
             << setw(14) << balanceAfter << timestamp << endl;
    }
};

/**
 * @brief Account class representing a bank account.
 * Stores account details and manages balance operations with PIN protection.
 */
class Account
{
protected:
    string accountNumber;
    string holderName;
    string pin;
    double balance;
    bool isActive;

public:
    Account() : balance(0.0), isActive(true) {}

    Account(string accNo, string name, string pinCode, double initialDeposit)
        : accountNumber(accNo), holderName(name), pin(pinCode),
          balance(initialDeposit), isActive(true) {}

    // Getters
    string getAccountNumber() const { return accountNumber; }
    string getHolderName() const { return holderName; }
    double getBalance() const { return balance; }
    bool getIsActive() const { return isActive; }

    bool verifyPin(const string &inputPin) const { return pin == inputPin; }

    bool changePin(const string &oldPin, const string &newPin)
    {
        if (pin != oldPin)
            return false;
        if (newPin.length() < 4)
            return false;
        pin = newPin;
        return true;
    }

    virtual bool deposit(double amount)
    {
        if (amount <= 0 || !isActive)
            return false;
        balance += amount;
        return true;
    }

    virtual bool withdraw(double amount)
    {
        if (amount <= 0 || amount > balance || !isActive)
            return false;
        balance -= amount;
        return true;
    }

    void closeAccount() { isActive = false; }
    void activateAccount() { isActive = true; }

    virtual string getAccountType() const = 0;

    virtual string serialize() const
    {
        return accountNumber + "|" + holderName + "|" + pin + "|" +
               to_string(balance) + "|" + (isActive ? "1" : "0") + "|" +
               getAccountType();
    }

    static Account *deserialize(const string &line);

    virtual void display() const
    {
        cout << left << setw(14) << accountNumber << setw(22) << holderName
             << setw(12) << getAccountType() << setw(14) << fixed << setprecision(2) << balance
             << (isActive ? "Active" : "Closed") << endl;
    }

    virtual ~Account() {}
};

/**
 * @brief SavingsAccount with interest and minimum balance requirements.
 */
class SavingsAccount : public Account
{
private:
    static constexpr double MIN_BALANCE = 500.0;
    static constexpr double INTEREST_RATE = 0.04;

public:
    SavingsAccount() : Account() {}
    SavingsAccount(string accNo, string name, string pinCode, double initialDeposit)
        : Account(accNo, name, pinCode, initialDeposit) {}

    string getAccountType() const override { return "Savings"; }

    bool withdraw(double amount) override
    {
        if (amount <= 0 || !isActive)
            return false;
        if (balance - amount < MIN_BALANCE)
        {
            cout << "Cannot withdraw. Minimum balance of $" << MIN_BALANCE << " required.\n";
            return false;
        }
        balance -= amount;
        return true;
    }

    void applyInterest()
    {
        if (!isActive)
            return;
        double interest = balance * INTEREST_RATE / 12.0;
        balance += interest;
        cout << "Interest of $" << fixed << setprecision(2) << interest << " applied.\n";
    }
};

/**
 * @brief CheckingAccount with transaction fees and overdraft limit.
 */
class CheckingAccount : public Account
{
private:
    static constexpr double TRANSACTION_FEE = 1.50;
    static constexpr double OVERDRAFT_LIMIT = 500.0;

public:
    CheckingAccount() : Account() {}
    CheckingAccount(string accNo, string name, string pinCode, double initialDeposit)
        : Account(accNo, name, pinCode, initialDeposit) {}

    string getAccountType() const override { return "Checking"; }

    bool withdraw(double amount) override
    {
        if (amount <= 0 || !isActive)
            return false;
        if (balance - amount < -OVERDRAFT_LIMIT)
        {
            cout << "Overdraft limit of $" << OVERDRAFT_LIMIT << " exceeded.\n";
            return false;
        }
        balance -= amount;
        return true;
    }

    bool deposit(double amount) override
    {
        if (amount <= 0 || !isActive)
            return false;
        balance += amount;
        balance -= TRANSACTION_FEE;
        if (balance < 0)
            balance = 0;
        cout << "Transaction fee of $" << TRANSACTION_FEE << " deducted.\n";
        return true;
    }
};

Account *Account::deserialize(const string &line)
{
    stringstream ss(line);
    string accNo, name, pinCode, token, type;
    double bal;
    bool active;

    getline(ss, accNo, '|');
    getline(ss, name, '|');
    getline(ss, pinCode, '|');
    getline(ss, token, '|');
    bal = stod(token);
    getline(ss, token, '|');
    active = (token == "1");
    getline(ss, type, '|');

    Account *acc = nullptr;
    if (type == "Savings")
        acc = new SavingsAccount(accNo, name, pinCode, bal);
    else if (type == "Checking")
        acc = new CheckingAccount(accNo, name, pinCode, bal);

    if (acc)
        acc->isActive = active;
    return acc;
}

/**
 * @brief FileManager handles all file I/O for accounts and transactions.
 */
class FileManager
{
public:
    static bool saveAccounts(const map<string, Account *> &accounts)
    {
        ofstream file(ACCOUNTS_FILE, ios::trunc);
        if (!file.is_open())
            return false;

        for (const auto &pair : accounts)
            file << pair.second->serialize() << endl;

        file.close();
        return true;
    }

    static bool loadAccounts(map<string, Account *> &accounts)
    {
        ifstream file(ACCOUNTS_FILE);
        if (!file.is_open())
            return true;

        string line;
        while (getline(file, line))
        {
            if (line.empty())
                continue;
            Account *acc = Account::deserialize(line);
            if (acc)
                accounts[acc->getAccountNumber()] = acc;
        }
        file.close();
        return true;
    }

    static bool saveTransactions(const vector<Transaction> &transactions)
    {
        ofstream file(TRANSACTIONS_FILE, ios::trunc);
        if (!file.is_open())
            return false;

        for (const auto &t : transactions)
            file << t.serialize() << endl;

        file.close();
        return true;
    }

    static bool loadTransactions(vector<Transaction> &transactions)
    {
        ifstream file(TRANSACTIONS_FILE);
        if (!file.is_open())
            return true;

        string line;
        while (getline(file, line))
        {
            if (line.empty())
                continue;
            transactions.push_back(Transaction::deserialize(line));
        }
        file.close();
        return true;
    }
};

/**
 * @brief Bank class managing all accounts, transactions, and user interface.
 */
class Bank
{
private:
    map<string, Account *> accounts;
    vector<Transaction> transactions;
    int nextAccountNumber;

    string generateAccountNumber()
    {
        int num = nextAccountNumber;
        nextAccountNumber++;
        return "ACC" + string(6 - to_string(num).length(), '0') + to_string(num);
    }

    void addTransaction(const string &accNo, const string &type,
                        double amount, double balanceAfter)
    {
        Transaction t;
        t.accountNumber = accNo;
        t.type = type;
        t.amount = amount;
        t.balanceAfter = balanceAfter;
        t.timestamp = getCurrentTimestamp();
        transactions.push_back(t);
    }

    void saveData()
    {
        FileManager::saveAccounts(accounts);
        FileManager::saveTransactions(transactions);
    }

    void loadData()
    {
        FileManager::loadAccounts(accounts);
        FileManager::loadTransactions(transactions);

        int maxNum = 0;
        for (const auto &pair : accounts)
        {
            string numStr = pair.first.substr(3);
            int num = stoi(numStr);
            if (num > maxNum)
                maxNum = num;
        }
        nextAccountNumber = maxNum + 1;
    }

    Account *findAccount(const string &accNo)
    {
        auto it = accounts.find(accNo);
        if (it == accounts.end())
            return nullptr;
        return it->second;
    }

    bool authenticate(Account *acc)
    {
        string pin;
        cout << "Enter PIN: ";
        cin >> pin;
        if (!acc->verifyPin(pin))
        {
            cout << "Incorrect PIN.\n";
            return false;
        }
        return true;
    }

    /**
     * @brief Validates that input is a positive number.
     */
    double getPositiveAmount(const string &prompt)
    {
        double amount;
        cout << prompt;
        cin >> amount;
        if (amount <= 0)
        {
            cout << "Amount must be positive.\n";
            return -1;
        }
        return amount;
    }

public:
    Bank() : nextAccountNumber(1) { loadData(); }

    ~Bank()
    {
        saveData();
        for (auto &pair : accounts)
            delete pair.second;
    }

    void createAccount()
    {
        string name, pin, pinConfirm, typeChoice;

        cin.ignore();
        cout << "Enter account holder name: ";
        getline(cin, name);

        cout << "Enter account type (S for Savings, C for Checking): ";
        cin >> typeChoice;

        cout << "Set a 4-6 digit PIN: ";
        cin >> pin;
        if (pin.length() < 4 || pin.length() > 6)
        {
            cout << "PIN must be 4-6 digits.\n";
            return;
        }
        cout << "Confirm PIN: ";
        cin >> pinConfirm;
        if (pin != pinConfirm)
        {
            cout << "PINs do not match.\n";
            return;
        }

        double initialDeposit = getPositiveAmount("Enter initial deposit: $");
        if (initialDeposit < 0)
            return;

        string accNo = generateAccountNumber();

        Account *acc = nullptr;
        if (typeChoice == "S" || typeChoice == "s")
            acc = new SavingsAccount(accNo, name, pin, initialDeposit);
        else if (typeChoice == "C" || typeChoice == "c")
            acc = new CheckingAccount(accNo, name, pin, initialDeposit);
        else
        {
            cout << "Invalid account type.\n";
            return;
        }

        accounts[accNo] = acc;
        addTransaction(accNo, "OPEN", initialDeposit, initialDeposit);
        saveData();

        cout << "Account created successfully!\n";
        cout << "Your account number is: " << accNo << endl;
    }

    void depositMoney()
    {
        string accNo;
        cout << "Enter account number: ";
        cin >> accNo;

        Account *acc = findAccount(accNo);
        if (!acc)
        {
            cout << "Account not found.\n";
            return;
        }

        if (!acc->getIsActive())
        {
            cout << "Account is closed.\n";
            return;
        }

        if (!authenticate(acc))
            return;

        double amount = getPositiveAmount("Enter deposit amount: $");
        if (amount < 0)
            return;

        if (acc->deposit(amount))
        {
            addTransaction(accNo, "DEPOSIT", amount, acc->getBalance());
            saveData();
            cout << "Deposited $" << fixed << setprecision(2) << amount
                 << ". New balance: $" << acc->getBalance() << endl;
        }
        else
        {
            cout << "Deposit failed.\n";
        }
    }

    void withdrawMoney()
    {
        string accNo;
        cout << "Enter account number: ";
        cin >> accNo;

        Account *acc = findAccount(accNo);
        if (!acc)
        {
            cout << "Account not found.\n";
            return;
        }

        if (!acc->getIsActive())
        {
            cout << "Account is closed.\n";
            return;
        }

        if (!authenticate(acc))
            return;

        double amount = getPositiveAmount("Enter withdrawal amount: $");
        if (amount < 0)
            return;

        if (acc->withdraw(amount))
        {
            addTransaction(accNo, "WITHDRAW", amount, acc->getBalance());
            saveData();
            cout << "Withdrew $" << fixed << setprecision(2) << amount
                 << ". New balance: $" << acc->getBalance() << endl;
        }
        else
        {
            cout << "Withdrawal failed. Insufficient funds or minimum balance violation.\n";
        }
    }

    void checkBalance()
    {
        string accNo;
        cout << "Enter account number: ";
        cin >> accNo;

        Account *acc = findAccount(accNo);
        if (!acc)
        {
            cout << "Account not found.\n";
            return;
        }

        if (!authenticate(acc))
            return;

        cout << fixed << setprecision(2);
        cout << "\n--- Account Balance ---\n";
        cout << "Holder: " << acc->getHolderName() << endl;
        cout << "Type: " << acc->getAccountType() << endl;
        cout << "Balance: $" << acc->getBalance() << endl;
        cout << "Status: " << (acc->getIsActive() ? "Active" : "Closed") << endl;
    }

    void viewTransactionHistory()
    {
        string accNo;
        cout << "Enter account number: ";
        cin >> accNo;

        Account *acc = findAccount(accNo);
        if (!acc)
        {
            cout << "Account not found.\n";
            return;
        }

        if (!authenticate(acc))
            return;

        cout << "\n--- Transaction History for " << accNo << " ---\n";
        cout << left << setw(12) << "Account" << setw(14) << "Type"
             << setw(12) << "Amount" << setw(14) << "Balance" << "Timestamp" << endl;
        cout << string(75, '-') << endl;

        bool found = false;
        for (const auto &t : transactions)
        {
            if (t.accountNumber == accNo)
            {
                t.display();
                found = true;
            }
        }

        if (!found)
            cout << "No transactions found.\n";
    }

    void changeAccountPin()
    {
        string accNo;
        cout << "Enter account number: ";
        cin >> accNo;

        Account *acc = findAccount(accNo);
        if (!acc)
        {
            cout << "Account not found.\n";
            return;
        }

        string oldPin, newPin;
        cout << "Enter current PIN: ";
        cin >> oldPin;
        cout << "Enter new PIN (4-6 digits): ";
        cin >> newPin;

        if (acc->changePin(oldPin, newPin))
        {
            saveData();
            cout << "PIN changed successfully.\n";
        }
        else
        {
            cout << "Failed to change PIN. Check current PIN or new PIN length.\n";
        }
    }

    void closeAccount()
    {
        string accNo;
        cout << "Enter account number: ";
        cin >> accNo;

        Account *acc = findAccount(accNo);
        if (!acc)
        {
            cout << "Account not found.\n";
            return;
        }

        if (!authenticate(acc))
            return;

        char confirm;
        cout << "Are you sure you want to close account " << accNo << "? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y')
        {
            double finalBalance = acc->getBalance();
            acc->closeAccount();
            addTransaction(accNo, "CLOSE", 0, 0);
            saveData();

            cout << "Account closed. Final balance of $"
                 << fixed << setprecision(2) << finalBalance
                 << " can be collected.\n";
        }
    }

    void listAllAccounts()
    {
        if (accounts.empty())
        {
            cout << "No accounts in the system.\n";
            return;
        }

        cout << "\n--- All Accounts ---\n";
        cout << left << setw(14) << "Account No" << setw(22) << "Holder Name"
             << setw(12) << "Type" << setw(14) << "Balance" << "Status" << endl;
        cout << string(75, '-') << endl;

        for (const auto &pair : accounts)
            pair.second->display();
    }

    void applyMonthlyInterest()
    {
        int count = 0;
        for (auto &pair : accounts)
        {
            SavingsAccount *sa = dynamic_cast<SavingsAccount *>(pair.second);
            if (sa && sa->getIsActive())
            {
                sa->applyInterest();
                count++;
            }
        }
        saveData();
        cout << "Interest applied to " << count << " savings account(s).\n";
    }

    void run()
    {
        int choice;
        do
        {
            cout << "\n========================================\n";
            cout << "        BANKING MANAGEMENT SYSTEM       \n";
            cout << "========================================\n";
            cout << "1.  Create Account\n";
            cout << "2.  Deposit Money\n";
            cout << "3.  Withdraw Money\n";
            cout << "4.  Check Balance\n";
            cout << "5.  Transaction History\n";
            cout << "6.  Change PIN\n";
            cout << "7.  Close Account\n";
            cout << "8.  List All Accounts (Admin)\n";
            cout << "9.  Apply Monthly Interest (Admin)\n";
            cout << "0.  Exit\n";
            cout << "========================================\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                createAccount();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                withdrawMoney();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                viewTransactionHistory();
                break;
            case 6:
                changeAccountPin();
                break;
            case 7:
                closeAccount();
                break;
            case 8:
                listAllAccounts();
                break;
            case 9:
                applyMonthlyInterest();
                break;
            case 0:
                cout << "Thank you for using the Banking System!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 0);
    }
};

int main()
{
    Bank bank;
    bank.run();
    return 0;
}
