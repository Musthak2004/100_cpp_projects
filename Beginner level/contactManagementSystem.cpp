#include <iostream>
using namespace std;

void add(string names[], string phones[], int &totalContacts);
void view(string names[], string phones[], int totalContacts);
void search(string names[], string phones[], int totalContacts);

int main()
{
    cout << "===== CONTACT MANAGEMENT SYSTEM =====\n";

    string names[100];
    string phones[100];

    int totalContacts = 0;
    int choice;

    do
    {
        cout << "\n";
        cout << "1. Add Contact\n";
        cout << "2. View Contacts\n";
        cout << "3. Search Contact\n";
        cout << "4. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        cout << "\n";

        switch (choice)
        {
        case 1:
            add(names, phones, totalContacts);
            break;

        case 2:
            view(names, phones, totalContacts);
            break;

        case 3:
            search(names, phones, totalContacts);
            break;

        case 4:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice!\n";
            break;
        }

    } while (choice != 4);

    cout << "\n=====================================\n";

    return 0;
}

void add(string names[], string phones[], int &totalContacts)
{
    if (totalContacts >= 100)
    {
        cout << "Contact storage full!\n";
        return;
    }

    cout << "Enter name: ";
    cin >> names[totalContacts];

    cout << "Enter phone: ";
    cin >> phones[totalContacts];

    totalContacts++;

    cout << "Contact added successfully!\n";
}

void view(string names[], string phones[], int totalContacts)
{
    if (totalContacts == 0)
    {
        cout << "No contacts available!\n";
        return;
    }

    cout << "===== CONTACT LIST =====\n";

    for (int i = 0; i < totalContacts; i++)
    {
        cout << "\nContact #" << i + 1 << "\n";

        cout << "Name : " << names[i] << "\n";
        cout << "Phone: " << phones[i] << "\n";
    }
}

void search(string names[], string phones[], int totalContacts)
{
    if (totalContacts == 0)
    {
        cout << "No contacts available!\n";
        return;
    }

    string searchName;
    bool found = false;

    cout << "Enter name to search: ";
    cin >> searchName;

    for (int i = 0; i < totalContacts; i++)
    {
        if (names[i] == searchName)
        {
            cout << "\nContact Found!\n";

            cout << "Name : " << names[i] << "\n";
            cout << "Phone: " << phones[i] << "\n";

            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Contact not found!\n";
    }
}