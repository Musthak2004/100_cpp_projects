#include <iostream>
using namespace std;

int main()
{
    int number;
    bool isPrime = true;

    cout << "***** Prime Number Checker *****\n\n";

    cout << "Enter a number: ";
    cin >> number;

    if (number < 2)
    {
        isPrime = false;
    }
    else
    {
        for (int i = 2; i < number; i++)
        {
            if (number % i == 0)
            {
                isPrime = false;
                break;
            }
        }
    }

    if (isPrime)
    {
        cout << number << " is a PRIME number.\n";
    }
    else
    {
        cout << number << " is NOT a prime number.\n";
    }

    cout << "\n*********************************\n";

    return 0;
}