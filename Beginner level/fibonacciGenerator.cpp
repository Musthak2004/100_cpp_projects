#include <iostream>
using namespace std;

int main()
{
    int terms;

    cout << "***** Fibonacci Generator *****\n\n";

    cout << "How many terms do you want? ";
    cin >> terms;

    int first = 0;
    int second = 1;
    int next;

    cout << "\nFibonacci Series:\n";

    for (int i = 1; i <= terms; i++)
    {
        cout << first << " ";

        next = first + second;
        first = second;
        second = next;
    }

    cout << "\n\n********************************\n";

    return 0;
}