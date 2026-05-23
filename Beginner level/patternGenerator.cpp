#include <iostream>
using namespace std;

int main()
{
    cout << "===== PATTERN GENERATOR =====\n";

    int choice;
    int rows;

    do
    {
        cout << "\n";
        cout << "1. Square Pattern\n";
        cout << "2. Right Triangle\n";
        cout << "3. Inverted Triangle\n";
        cout << "4. Pyramid\n";
        cout << "5. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 4)
        {
            cout << "Enter number of rows: ";
            cin >> rows;
            cout << "\n";
        }

        switch (choice)
        {
        case 1:
            // Square
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < rows; j++)
                {
                    cout << "* ";
                }
                cout << "\n";
            }
            break;

        case 2:
            // Right Triangle
            for (int i = 1; i <= rows; i++)
            {
                for (int j = 1; j <= i; j++)
                {
                    cout << "* ";
                }
                cout << "\n";
            }
            break;

        case 3:
            // Inverted Triangle
            for (int i = rows; i >= 1; i--)
            {
                for (int j = 1; j <= i; j++)
                {
                    cout << "* ";
                }
                cout << "\n";
            }
            break;

        case 4:
            // Pyramid
            for (int i = 1; i <= rows; i++)
            {
                for (int j = 1; j <= rows - i; j++)
                {
                    cout << " ";
                }

                for (int k = 1; k <= (2 * i - 1); k++)
                {
                    cout << "*";
                }

                cout << "\n";
            }
            break;

        case 5:
            cout << "Thank you!\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 5);

    cout << "\n=============================\n";

    return 0;
}