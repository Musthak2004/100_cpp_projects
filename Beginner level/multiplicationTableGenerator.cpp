#include <iostream>
using namespace std;

int main()
{
    int number;
    int limit;

    cout << "***** Multiplication Table Generator *****\n\n";

    cout << "Enter a number: ";
    cin >> number;

    cout << "Enter table limit: ";
    cin >> limit;

    cout << "\nMultiplication Table of " << number << "\n\n";

    for (int i = 1; i <= limit; i++)
    {
        cout << number << " x " << i << " = " << number * i << "\n";
    }

    cout << "\n******************************************\n";

    return 0;
}