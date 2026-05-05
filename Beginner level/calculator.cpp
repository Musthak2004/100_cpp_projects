#include <iostream>
using namespace std;

int main()
{
    double num1;
    double num2;
    int choice;

    cout << "***** Calculator *****\n";

    while (choice != 5)
    {
        cout << "\n";
        cout << "1. Addition\n";
        cout << "2. Substraction\n";
        cout << "3. Multiplication\n";
        cout << "4. Division\n";
        cout << "5. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;
    }

    return 0;
}