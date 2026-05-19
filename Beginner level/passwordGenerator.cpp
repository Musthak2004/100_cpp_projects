#include <iostream>
#include <ctime>
using namespace std;

int main()
{
    cout << "\n===== Password Generator =====\n";

    int lenght;

    cout << "Enter your password length: minimum 8 chars: ";
    cin >> lenght;

    string chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#$%";

    srand(time(0));

    cout
        << "\n==============================\n";

    return 0;
}