#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    double num1, num2, result;
    int choice = 0;

    cout << "***** Calculator *****\n";

    while (choice != 5)
    {
        cout << "\n";
        cout << "1. Addition\n";
        cout << "2. Subtraction\n";
        cout << "3. Multiplication\n";
        cout << "4. Division\n";
        cout << "5. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        cout << "\n";

        switch (choice)
        {
        case 1:
            cout << "Enter 1st number: ";
            cin >> num1;
            cout << "Enter 2nd number: ";
            cin >> num2;

            result = num1 + num2;
            cout << "Result is: " << result << "\n";
            break;

        case 2:
            cout << "Enter 1st number: ";
            cin >> num1;
            cout << "Enter 2nd number: ";
            cin >> num2;

            result = num1 - num2;
            cout << "Result is: " << result << "\n";
            break;

        case 3:
            cout << "Enter 1st number: ";
            cin >> num1;
            cout << "Enter 2nd number: ";
            cin >> num2;

            result = num1 * num2;
            cout << "Result is: " << result << "\n";
            break;

        case 4:
            cout << "Enter 1st number: ";
            cin >> num1;
            cout << "Enter 2nd number: ";
            cin >> num2;

            if (num2 == 0)
            {
                cout << "Cannot divide by zero!\n";
            }
            else
            {
                result = num1 / num2;
                cout << "Result is: " << setprecision(2) << fixed << result << "\n";
            }
            break;

        case 5:
            cout << "Thank you!\n";
            break;

        default:
            cout << "Please choose a valid operation\n";
        }

        cout << "\n**********************\n";
    }

    return 0;
}