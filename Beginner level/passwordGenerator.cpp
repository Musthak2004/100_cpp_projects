#include <iostream>
#include <ctime>
using namespace std;

int main()
{
    cout << "\n===== Password Generator =====\n";

    int length;

    string chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#$%";

    srand(time(0));

    while (true)
    {

        cout << "Enter password length (min 8): ";
        cin >> length;

        if (length < 8)
        {
            cout << "Password too short! Try again.\n";
        }
        else
        {
            break;
        }
    }

    string password = "";

    for (int i = 0; i < length; i++)
    {
        int index = rand() % chars.length();
        password += chars[index];
    }

    cout << "\nGenerated Password: " << password << "\n";

    cout
        << "\n==============================\n";

    return 0;
}