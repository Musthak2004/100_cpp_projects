#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    int choice;
    string note;

    cout << "===== FILE HANDLING NOTES APP =====\n";

    do
    {
        cout << "\n";
        cout << "1. Add Note\n";
        cout << "2. View Notes\n";
        cout << "3. Clear Notes\n";
        cout << "4. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        cin.ignore(); // clear newline after number input

        cout << "\n";

        switch (choice)
        {
        case 1:
        {
            ofstream file("notes.txt", ios::app); // append mode

            if (!file)
            {
                cout << "File error!\n";
                break;
            }

            cout << "Enter your note: ";
            getline(cin, note);

            file << note << "\n";

            file.close();

            cout << "Note added successfully!\n";
            break;
        }

        case 2:
        {
            ifstream file("notes.txt");

            if (!file)
            {
                cout << "No notes found!\n";
                break;
            }

            cout << "----- YOUR NOTES -----\n";

            while (getline(file, note))
            {
                cout << note << "\n";
            }

            file.close();

            break;
        }

        case 3:
        {
            ofstream file("notes.txt", ios::trunc); // clear file

            file.close();

            cout << "All notes cleared!\n";
            break;
        }

        case 4:
            cout << "Exiting Notes App...\n";
            break;

        default:
            cout << "Invalid choice!\n";
            break;
        }

    } while (choice != 4);

    cout << "\n============================\n";

    return 0;
}