#include <iostream>
#include <iomanip>
using namespace std;

void cToF()
{
    double celsius;

    cout << "Enter temperature in Celsius: ";
    cin >> celsius;

    double fahrenheit = (celsius * 9.0 / 5.0) + 32;

    cout << fixed << setprecision(2);
    cout << "Output: " << fahrenheit << " F\n";
}

void fToC()
{
    double fahrenheit;

    cout << "Enter temperature in Fahrenheit: ";
    cin >> fahrenheit;

    double celsius = (fahrenheit - 32) * 5.0 / 9.0;

    cout << fixed << setprecision(2);
    cout << "Output: " << celsius << " C\n";
}

void cToK()
{
    double celsius;

    cout << "Enter temperature in Celsius: ";
    cin >> celsius;

    double kelvin = celsius + 273.15;

    cout << fixed << setprecision(2);
    cout << "Output: " << kelvin << " K\n";
}

int main()
{
    cout << "===== TEMPERATURE CONVERTER =====\n";

    int choice;

    do
    {
        cout << "\n";
        cout << "1. Celsius to Fahrenheit\n";
        cout << "2. Fahrenheit to Celsius\n";
        cout << "3. Celsius to Kelvin\n";
        cout << "4. Exit\n";

        cout << "\nEnter your choice (1-4): ";
        cin >> choice;

        cout << "\n";

        switch (choice)
        {
        case 1:
            cToF();
            break;

        case 2:
            fToC();
            break;

        case 3:
            cToK();
            break;

        case 4:
            cout << "Thank you for using the Temperature Converter!\n";
            break;

        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }

    } while (choice != 4);

    cout << "\n=================================\n";

    return 0;
}