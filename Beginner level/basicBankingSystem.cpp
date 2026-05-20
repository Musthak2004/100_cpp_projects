#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

int main()
{
    double balance = 1000.00;
    double amount;
    int choice = 0;

    cout << "===== BASIC BANKING SYSTEM =====\n";

    do
    {
        cout << "\n";
        cout << "1. Show Balance\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Exit\n";

        cout << "\nEnter your action: ";
        cin >> choice;

        // validate menu input
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input! Numbers only.\n";
            continue;
        }

        cout << "\n";

        switch (choice)
        {
        case 1:

            cout << "Your account balance is: Rs."
                 << fixed << setprecision(2)
                 << balance << "\n";

            break;

        case 2:

            cout << "Enter your deposit amount: Rs.";
            cin >> amount;

            // validate deposit input
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Invalid amount!\n";
                break;
            }

            if (amount <= 0)
            {
                cout << "Deposit amount must be greater than 0!\n";
            }
            else
            {
                balance += amount;

                cout << "Deposit successful!\n";

                cout << "Your current balance is: Rs."
                     << fixed << setprecision(2)
                     << balance << "\n";
            }

            break;

        case 3:

            cout << "Enter your withdrawal amount: Rs.";
            cin >> amount;

            // validate withdrawal input
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Invalid amount!\n";
                break;
            }

            if (amount <= 0)
            {
                cout << "Withdrawal amount must be greater than 0!\n";
            }
            else if (amount > balance)
            {
                cout << "Insufficient balance!\n";
            }
            else
            {
                balance -= amount;

                cout << "Withdrawal successful!\n";

                cout << "Your current balance is: Rs."
                     << fixed << setprecision(2)
                     << balance << "\n";
            }

            break;

        case 4:

            cout << "Thank you for banking with us!\n";

            break;

        default:

            cout << "Invalid action! Please try again.\n";

            break;
        }

    } while (choice != 4);

    cout << "\n********************************\n";

    return 0;
}