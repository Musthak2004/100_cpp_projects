#include <iostream>
using namespace std;

void add(string titles[], string author[], bool available[], int &totalbooks)
{
    if (totalbooks >= 100)
    {
        cout << "Library storage full!\n";
        return;
    }

    cout << "Enter title: ";
    cin >> titles[totalbooks];

    cout << "Enter author name: ";
    cin >> author[totalbooks];

    available[totalbooks] = true;

    totalbooks++;

    cout << "Book added successfully!\n";
}

int main()
{
    cout << "===== LIBRARY SYSTEM CONSOLE =====\n";

    string titles[100];
    string author[100];
    bool available[100];

    int totalbooks = 0;
    int choice;

    cout << "\n==================================\n";

    return 0;
}