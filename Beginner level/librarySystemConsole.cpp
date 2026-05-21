#include <iostream>
using namespace std;

void add(string titles[], string authors[], bool available[], int &totalBooks)
{
    if (totalBooks >= 100)
    {
        cout << "Library storage full!\n";
        return;
    }

    cin.ignore();

    cout << "Enter title: ";
    getline(cin, titles[totalBooks]);

    cout << "Enter author name: ";
    getline(cin, authors[totalBooks]);

    available[totalBooks] = true;

    totalBooks++;

    cout << "Book added successfully!\n";
}
void view(string titles[], string authors[], bool available[], int totalBooks)
{
    if (totalBooks == 0)
    {
        cout << "No books available!\n";
        return;
    }

    cout << "===== BOOK LIST =====\n";

    for (int i = 0; i < totalBooks; i++)
    {
        cout << "\nBook #" << i + 1 << "\n";
        cout << "Title : " << titles[i] << "\n";
        cout << "Author: " << authors[i] << "\n";

        if (available[i])
        {
            cout << "Status: Available\n";
        }
        else
        {
            cout << "Status: Borrowed\n";
        }
    }
}

void search(string titles[], string authors[], bool available[], int totalBooks)
{
    if (totalBooks == 0)
    {
        cout << "No books available!\n";
    }

    string title;
    bool found = false;

    cin.ignore();

    cout << "Enter title to search: ";
    getline(cin, title);

    for (int i = 0; i < totalBooks; i++)
    {
        if (titles[i] == title)
        {
            cout << "\nBook Found!\n";
            cout << "Title : " << titles[i] << "\n";
            cout << "Author: " << authors[i] << "\n";

            if (available[i])
            {
                cout << "Status: Available\n";
            }
            else
            {
                cout << "Status: Borrowed\n";
            }

            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Book not found!\n";
    }
}

void borrowBook(string titles[], bool available[], int totalBooks)
{

    if (totalBooks == 0)
    {
        cout << "No books available!\n";
        return;
    }

    string title;

    cin.ignore();

    cout << "Enter title to borrow: ";
    getline(cin, title);

    for (int i = 0; i < totalBooks; i++)
    {
        if (titles[i] == title)
        {
            if (available[i])
            {
                available[i] = false;
                cout << "Book borrowed successfully!\n";
            }
            else
            {
                cout << "Book already borrowed!\n";
            }
            return;
        }
    }

    cout << "Book not found!\n";
}

void returnBook(string titles[], bool available[], int totalBooks)
{
    if (totalBooks == 0)
    {
        cout << "No books available!\n";
        return;
    }

    string title;

    cin.ignore();

    cout << "Enter title to return: ";
    getline(cin, title);

    for (int i = 0; i < totalBooks; i++)
    {
        if (titles[i] == title)
        {
            if (!available[i])
            {
                available[i] = true;
                cout << "Book returned successfully!\n";
            }
            else
            {
                cout << "This book is already available!\n";
            }
            return;
        }
    }

    cout << "Book not found!\n";
}

int main()
{
    cout << "===== LIBRARY SYSTEM CONSOLE =====\n";

    string titles[100];
    string authors[100];
    bool available[100];

    int totalBooks = 0;
    int choice;

    do
    {
        cout << "\n";
        cout << "1. Add Book\n";
        cout << "2. View Books\n";
        cout << "3. Search Book\n";
        cout << "4. Borrow Book\n";
        cout << "5. Return Book\n";
        cout << "6. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        cout << "\n";

        switch (choice)
        {
        case 1:
            add(titles, authors, available, totalBooks);
            break;
        case 2:
            view(titles, authors, available, totalBooks);
            break;
        case 3:
            search(titles, authors, available, totalBooks);
            break;
        case 4:
            borrowBook(titles, available, totalBooks);
            break;
        case 5:
            returnBook(titles, available, totalBooks);
            break;
        case 6:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice!\n";
            break;
        }

    } while (choice != 6);

    cout << "\n==================================\n";

    return 0;
}