#include <iostream>
using namespace std;

void add(string titles[], string authors[], bool available[], int &totalBooks)
{
    if (totalBooks >= 100)
    {
        cout << "Library storage full!\n";
        return;
    }

    cout << "Enter title: ";
    cin >> titles[totalBooks];

    cout << "Enter author name: ";
    cin >> authors[totalBooks];

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
        cout << "Author : " << authors[i] << "\n";

        if (available[i])
        {
            cout << "Status: Availabel\n";
        }
        else
        {
            cout << "Status: Borrowed\n";
        }
    }
}

int main()
{
    cout << "===== LIBRARY SYSTEM CONSOLE =====\n";

    string titles[100];
    string authors[100];
    bool available[100];

    int totalBooks = 0;
    int choice;

    cout << "\n==================================\n";

    return 0;
}