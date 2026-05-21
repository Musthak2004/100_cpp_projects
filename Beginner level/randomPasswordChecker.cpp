#include <iostream>
using namespace std;

int main()
{
    cout << "===== PASSWORD CHECKER =====\n";

    string password;

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    cout << "Enter password: ";
    getline(cin, password);

    for (int i = 0; i < password.length(); i++)
    {
        if (isupper(password[i]))
        {
            hasUpper = true;
        }
        else if (islower(password[i]))
        {
            hasLower = true;
        }
        else if (isdigit(password[i]))
        {
            hasDigit = true;
        }
        else
        {
            hasSpecial = true;
        }
    }

    cout << "\n===== RESULT =====\n";

    if (password.length() < 8)
    {
        cout << "Password is too short!\n";
    }

    cout << "Uppercase Letter : " << (hasUpper ? "Yes" : "No") << "\n";
    cout << "Lowercase Letter : " << (hasLower ? "Yes" : "No") << "\n";
    cout << "Number           : " << (hasDigit ? "Yes" : "No") << "\n";
    cout << "Special Character: " << (hasSpecial ? "Yes" : "No") << "\n";

    if (password.length() >= 8 && hasUpper && hasLower && hasDigit && hasSpecial)
    {
        cout << "\nStrong Password\n";
    }
    else
    {
        cout << "\nWeak Password\n";
    }
    cout << "\n============================\n";

    return 0;
}