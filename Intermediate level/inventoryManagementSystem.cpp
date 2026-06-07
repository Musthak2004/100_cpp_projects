#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

// ============================================================================
// Enums/Types
// ============================================================================
enum class OrderStatus
{
    PENDING,
    APPROVED,
    IN_QUEUE,
    DELIVERED,
    CANCELLED,
    COMPLETED,
    REJECTED
};

// Helper function to print OrderStatus as a string
std::string statusToString(OrderStatus status)
{
    switch (status)
    {
    case OrderStatus::PENDING:
        return "PENDING";
    case OrderStatus::APPROVED:
        return "APPROVED";
    case OrderStatus::IN_QUEUE:
        return "IN_QUEUE";
    case OrderStatus::DELIVERED:
        return "DELIVERED";
    case OrderStatus::CANCELLED:
        return "CANCELLED";
    case OrderStatus::COMPLETED:
        return "COMPLETED";
    case OrderStatus::REJECTED:
        return "REJECTED";
    default:
        return "UNKNOWN";
    }
}

enum class TransactionType
{
    SALE,
    RETURN,
    RESIZE,
    ADJUSTMENT,
    OTHER
};

struct OrderItem
{
    std::string itemName;
    double quantity;
    double unit_price;

    OrderItem() : itemName(""), quantity(0.0), unit_price(0.0) {}
    OrderItem(std::string name, double qty, double price)
        : itemName(name), quantity(qty), unit_price(price) {}
};

class Transaction
{
public:
    std::string transactionId;
    TransactionType type;
    OrderStatus status;
    std::vector<OrderItem> items;
    double subtotal;
    double tax;
    double total;

    // Default Constructor
    Transaction() : type(TransactionType::SALE), status(OrderStatus::PENDING), subtotal(0.0), tax(0.0), total(0.0) {}

    // Parametric Constructor
    Transaction(std::string id, TransactionType t, OrderStatus s)
        : transactionId(id), type(t), status(s), subtotal(0.0), tax(0.0), total(0.0) {}

    void addItem(const std::string &itemName, double quantity, double unitPrice)
    {
        items.push_back(OrderItem(itemName, quantity, unitPrice));
        subtotal += (quantity * unitPrice);
        total = subtotal + tax;
    }

    void setStatus(OrderStatus status)
    {
        this->status = status;
    }

    void applyTax(double taxRate = 0.08)
    {
        tax = subtotal * (taxRate / 100.0);
        total = subtotal + tax;
    }

    double getSubTotal() const { return subtotal; }
    double getTotal() const { return total; }
    TransactionType getType() const { return type; }
};

class InventoryManager
{
private:
    std::map<std::string, double> inventory;
    std::map<std::string, double> prices; // Added missing tracking map for prices
    std::map<std::string, OrderStatus> orderStatus;
    std::map<std::string, Transaction> orderTransactions; // Changed type from OrderItem to Transaction

public:
    InventoryManager() {}

    void addProduct(const std::string &name, double quantity, double price)
    {
        inventory[name] = quantity;
        prices[name] = price;
    }

    double getQuantity(const std::string &name)
    {
        auto it = inventory.find(name);
        return it != inventory.end() ? it->second : 0.0;
    }

    double getProductPrice(const std::string &name)
    {
        auto it = prices.find(name);
        return it != prices.end() ? it->second : 0.0;
    }

    OrderStatus getStatus(const std::string &name)
    {
        return orderStatus.find(name) != orderStatus.end() ? orderStatus[name] : OrderStatus::PENDING;
    }

    void createTransaction(const std::string &transactionId, const std::string &itemName,
                           double quantity, double price, OrderStatus status, double taxRate = 0.08)
    {
        Transaction trans(transactionId, TransactionType::SALE, status);
        trans.addItem(itemName, quantity, price);
        trans.applyTax(taxRate);
        trans.setStatus(status);

        orderStatus[transactionId] = status; // Map transaction status by transactionId or item
        orderTransactions[transactionId] = trans;

        // Deduct from stock if available
        if (inventory.find(itemName) != inventory.end())
        {
            inventory[itemName] -= quantity;
        }
    }

    double calculateTotal(const std::string &itemName, double quantity, double price)
    {
        return price * quantity;
    }

    void printSummary()
    {
        std::cout << "\n========================================\n";
        std::cout << "        INVENTORY SYSTEM SUMMARY\n";
        std::cout << "========================================\n\n";

        std::cout << "INVENTORY:\n";
        std::cout << std::left << std::setw(15) << "Item Name"
                  << std::setw(15) << "Quantity"
                  << std::setw(15) << "Price" << std::endl;

        for (const auto &item : inventory)
        {
            std::cout << std::left << std::setw(15) << item.first;
            std::cout << std::setw(15) << item.second;
            std::cout << std::fixed << std::setprecision(2) << std::setw(15) << prices[item.first];
            std::cout << std::endl;
        }
        std::cout << "\n";

        std::cout << "ORDER STATUS:\n";
        for (const auto &pair : orderStatus)
        {
            std::cout << "Order ID/Item: " << pair.first << " - Status: " << statusToString(pair.second) << "\n";
        }
        std::cout << "\n";
    }
};

// ============================================================================
// Main Menu Class
// ============================================================================
class InventoryManagerMenu
{
private:
    InventoryManager *mgr;

public:
    InventoryManagerMenu(InventoryManager *manager) : mgr(manager) {}

    void run()
    {
        std::string option;

        while (true)
        {
            std::cout << "\n========================================\n";
            std::cout << "         INVENTORY MANAGEMENT SYSTEM\n";
            std::cout << "========================================\n\n";
            std::cout << "Inventory Items:\n";
            std::cout << "1. Add/Update Product\n";
            std::cout << "2. View Products & Print Summary\n";
            std::cout << "3. Create Transaction\n";
            std::cout << "7. System Exit\n";
            std::cout << "--------------------------------\n";

            std::cout << "Enter your choice (1-7): ";
            std::cin >> option;

            if (option == "1")
            {
                std::string name;
                double qty, price;
                std::cout << "Enter product name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter quantity: ";
                std::cin >> qty;
                std::cout << "Enter price: ";
                std::cin >> price;
                mgr->addProduct(name, qty, price);
                std::cout << "Product successfully updated.\n";
            }
            else if (option == "2")
            {
                mgr->printSummary();
            }
            else if (option == "3")
            {
                std::string txId, itemName;
                double qty;
                std::cout << "Enter Transaction ID: ";
                std::cin >> txId;
                std::cout << "Enter existing Item Name: ";
                std::cin.ignore();
                std::getline(std::cin, itemName);
                std::cout << "Enter sales quantity: ";
                std::cin >> qty;

                double itemPrice = mgr->getProductPrice(itemName);
                mgr->createTransaction(txId, itemName, qty, itemPrice, OrderStatus::APPROVED);
                std::cout << "Transaction processed!\n";
            }
            else if (option == "7")
            {
                std::cout << "Goodbye!\n";
                break;
            }
            else
            {
                std::cout << "Invalid choice. Try again.\n";
            }
        }
    }
};

int main()
{
    InventoryManager *manager = nullptr;

    try
    {
        // Create Inventory Manager
        manager = new InventoryManager();

        // Initialize inventory with some products
        manager->addProduct("Widget A", 100.00, 5.00);
        manager->addProduct("Widget B", 200.00, 30.00);
        manager->addProduct("Gadget X", 300.00, 75.00);
        manager->printSummary();

        // Create Menu Manager and pass reference to data layer
        InventoryManagerMenu menu(manager);
        menu.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    delete manager;
    return 0;
}
