#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

/**
 * @brief Book class representing a book in the library.
 * Encapsulates book details and availability status.
 */
class Book
{
private:
    string isbn;
    string title;
    string author;
    bool isAvailable;

public:
    Book(string isbn, string title, string author)
        : isbn(isbn), title(title), author(author), isAvailable(true) {}

    // Getters
    string getIsbn() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool checkAvailability() const { return isAvailable; }

    // Setters
    void setAvailability(bool status) { isAvailable = status; }

    void displayBook() const
    {
        cout << left << setw(15) << isbn << setw(25) << title << setw(20) << author
             << (isAvailable ? "[Available]" : "[Borrowed]") << endl;
    }
};

/**
 * @brief User class representing a member of the library.
 * Manages member details and a list of borrowed books.
 */
class User
{
private:
    string userId;
    string name;
    vector<string> borrowedBooks; // Stores ISBNs of borrowed books

public:
    User(string userId, string name) : userId(userId), name(name) {}

    string getUserId() const { return userId; }
    string getName() const { return name; }
    const vector<string> &getBorrowedBooks() const { return borrowedBooks; }

    void borrowBook(const string &isbn)
    {
        borrowedBooks.push_back(isbn);
    }

    void returnBook(const string &isbn)
    {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), isbn);
        if (it != borrowedBooks.end())
        {
            borrowedBooks.erase(it);
        }
    }

    void displayUser() const
    {
        cout << "User ID: " << userId << " | Name: " << name << " | Books Borrowed: " << borrowedBooks.size() << endl;
    }
};

/**
 * @brief Library class managing the collection of books and users.
 * Provides functionality to add books, register users, and handle transactions.
 */
class Library
{
private:
    map<string, Book *> books; // ISBN -> Book pointer
    map<string, User *> users; // UserID -> User pointer

public:
    ~Library()
    {
        // Clean up dynamically allocated memory
        for (auto const &[isbn, book] : books)
            delete book;
        for (auto const &[id, user] : users)
            delete user;
    }

    void addBook(const string &isbn, const string &title, const string &author)
    {
        if (books.find(isbn) != books.end())
        {
            cout << "Error: Book with ISBN " << isbn << " already exists." << endl;
            return;
        }
        books[isbn] = new Book(isbn, title, author);
        cout << "Book added successfully!" << endl;
    }

    void registerUser(const string &userId, const string &name)
    {
        if (users.find(userId) != users.end())
        {
            cout << "Error: User with ID " << userId << " already exists." << endl;
            return;
        }
        users[userId] = new User(userId, name);
        cout << "User registered successfully!" << endl;
    }

    void borrowBook(const string &userId, const string &isbn)
    {
        if (users.find(userId) == users.end())
        {
            cout << "Error: User not found." << endl;
            return;
        }
        if (books.find(isbn) == books.end())
        {
            cout << "Error: Book not found." << endl;
            return;
        }

        Book *book = books[isbn];
        User *user = users[userId];

        if (book->checkAvailability())
        {
            book->setAvailability(false);
            user->borrowBook(isbn);
            cout << "Book '" << book->getTitle() << "' borrowed successfully by " << user->getName() << "." << endl;
        }
        else
        {
            cout << "Error: Book is currently borrowed by someone else." << endl;
        }
    }

    void returnBook(const string &userId, const string &isbn)
    {
        if (users.find(userId) == users.end())
        {
            cout << "Error: User not found." << endl;
            return;
        }
        if (books.find(isbn) == books.end())
        {
            cout << "Error: Book not found." << endl;
            return;
        }

        User *user = users[userId];
        Book *book = books[isbn];

        const vector<string> &borrowed = user->getBorrowedBooks();
        if (find(borrowed.begin(), borrowed.end(), isbn) != borrowed.end())
        {
            user->returnBook(isbn);
            book->setAvailability(true);
            cout << "Book '" << book->getTitle() << "' returned successfully." << endl;
        }
        else
        {
            cout << "Error: This user did not borrow this book." << endl;
        }
    }

    void displayAllBooks() const
    {
        cout << "\n--- Library Books ---" << endl;
        cout << left << setw(15) << "ISBN" << setw(25) << "Title" << setw(20) << "Author" << "Status" << endl;
        cout << string(65, '-') << endl;
        for (auto const &[isbn, book] : books)
        {
            book->displayBook();
        }
        cout << endl;
    }

    void displayAllUsers() const
    {
        cout << "\n--- Registered Users ---" << endl;
        for (auto const &[id, user] : users)
        {
            user->displayUser();
        }
        cout << endl;
    }
};

/**
 * @brief Main function to handle the user interface.
 */
void showMenu()
{
    cout << "========================================" << endl;
    cout << "    Library Management System (OOP)    " << endl;
    cout << "========================================" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. Register User" << endl;
    cout << "3. Borrow Book" << endl;
    cout << "4. Return Book" << endl;
    cout << "5. Display All Books" << endl;
    cout << "6. Display All Users" << endl;
    cout << "0. Exit" << endl;
    cout << "Choose an option: ";
}

int main()
{
    Library lib;
    int choice;
    string isbn, title, author, userId, name;

    do
    {
        showMenu();
        if (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            cout << "Enter ISBN: ";
            cin >> isbn;
            cin.ignore(); // clear newline
            cout << "Enter Title: ";
            getline(cin, title);
            cout << "Enter Author: ";
            getline(cin, author);
            lib.addBook(isbn, title, author);
            break;
        case 2:
            cout << "Enter User ID: ";
            cin >> userId;
            cin.ignore(); // clear newline
            cout << "Enter Name: ";
            getline(cin, name);
            lib.registerUser(userId, name);
            break;
        case 3:
            cout << "Enter User ID: ";
            cin >> userId;
            cout << "Enter Book ISBN: ";
            cin >> isbn;
            lib.borrowBook(userId, isbn);
            break;
        case 4:
            cout << "Enter User ID: ";
            cin >> userId;
            cout << "Enter Book ISBN: ";
            cin >> isbn;
            lib.returnBook(userId, isbn);
            break;
        case 5:
            lib.displayAllBooks();
            break;
        case 6:
            lib.displayAllUsers();
            break;
        case 0:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
