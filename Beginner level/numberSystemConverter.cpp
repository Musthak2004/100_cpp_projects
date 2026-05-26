#include <iostream>
using namespace std;

void decimmalToBinary()
{
    int number;
    int binary[32];
    int index = 0;

    cout << "Enter decimal number: ";
    cin >> number;

    if (number == 0)
    {
        cout << "Binary: 0\n";
        return;
    }

    while (number > 0)
    {
        binary[index] = number % 2;
        number /= 2;
        index++;
    }

    cout << "Binary: ";

    for (int i = index - 1; i >= 0; i--)
    {
        cout << binary[i];
    }

    cout << "\n";
}

void decimalToOctal()
{
    int number;
    int octal[32];
    int index = 0;

    cout << "Enter decimal number: ";
    cin >> number;

    if (number == 0)
    {
        cout << "Octal: 0\n";
        return;
    }

    while (number > 0)
    {
        octal[index] = number % 8;
        number /= 8;
        index++;
    }

    cout << "Octal: ";

    for (int i = index - 1; i >= 0; i--)
    {
        cout << octal[i];
    }

    cout << "\n";
}

void decimalToHex()
{
    int number;
    char hex[32];
    int index = 0;

    cout << "Enter decimal number: ";
    cin >> number;

    if (number == 0)
    {
        cout << "Hexadecimal: 0\n";
        return;
    }

    while (number > 0)
    {
        int remainder = number % 16;

        if (remainder < 10)
        {
            hex[index] = remainder + '0';
        }
        else
        {
            hex[index] = remainder - 10 + 'A';
        }

        number /= 16;
        index++;
    }

    cout << "Hexadecimal: ";

    for (int i = index - 1; i >= 0; i--)
    {
        cout << hex[i];
    }

    cout << "\n";
}

int main()
{
    cout << "===== NUMBER SYSTEM CONVERTER =====\n";

    int choice;

    do
    {

        cout << "\n";
        cout << "1. Decimal to Binary\n";
        cout << "2. Decimal to Octal\n";
        cout << "3. Decimal to Hexadecimal\n";
        cout << "4. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        cout << "\n";

        switch (choice)
        {
        case 1:
            decimmalToBinary();
            break;
        case 2:
            decimalToOctal();
            break;
        case 3:
            decimalToHex();
            break;
        case 4:
            cout << "Thank you!\n";
            break;

        default:
            cout << "Invalid choice!\n";
            break;
        }

    } while (choice != 4);

    cout << "\n===================================\n";

    return 0;
}