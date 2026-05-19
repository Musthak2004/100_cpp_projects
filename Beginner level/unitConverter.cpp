#include <iostream>
using namespace std;

int main()
{
    cout << "\n===== GLOBAL UNIT CONVERTER =====\n";

    char conversion;
    char choice;

    do
    {
        // Main Menu Display
        cout << "\n[ Main Menu ]\n";
        cout << "l = Length\n";
        cout << "w = Weight\n";
        cout << "t = Temperature\n";
        cout << "q = Quit program\n";

        cout << "\nChoose your conversion category: ";
        cin >> conversion;

        // Gracefully exit if the user wants to quit
        if (conversion == 'q' || conversion == 'Q')
        {
            break;
        }

        switch (conversion)
        {
        // ==================== LENGTH SECTION ====================
        case 'l':
        case 'L':
            cout << "\n***** Length Conversions *****\n";
            cout << "1. Meters to Kilometers (m -> km)\n";
            cout << "2. Centimeters to Inches (cm -> in)\n";
            cout << "3. Feet to Meters (ft -> m)\n";

            cout << "\nWhich operation do you need: ";
            cin >> choice;

            if (choice == '1')
            {
                double meters;
                cout << "Enter meters: ";
                cin >> meters;
                double kilometer = meters / 1000.0;
                cout << "=> Kilometers = " << kilometer << " km\n";
            }
            else if (choice == '2')
            {
                double cm;
                cout << "Enter cm: ";
                cin >> cm;
                double inches = cm / 2.54;
                cout << "=> Inches = " << inches << " inches\n";
            }
            else if (choice == '3')
            {
                double feets;
                cout << "Enter feet: ";
                cin >> feets;
                double meters = feets * 0.3048;
                cout << "=> Meters = " << meters << " m\n";
            }
            else
            {
                cout << "❌ Invalid choice! Returning to main menu.\n";
            }
            break;

        // ==================== WEIGHT SECTION ====================
        case 'w':
        case 'W':
            cout << "\n***** Weight Conversions *****\n";
            cout << "1. Kilograms to Pounds (kg -> lbs)\n";
            cout << "2. Pounds to Kilograms (lbs -> kg)\n";
            cout << "3. Grams to Ounces (g -> oz)\n";

            cout << "\nWhich operation do you need: ";
            cin >> choice;

            if (choice == '1')
            {
                double kg;
                cout << "Enter kilograms: ";
                cin >> kg;
                double lbs = kg * 2.20462;
                cout << "=> Pounds = " << lbs << " lbs\n";
            }
            else if (choice == '2')
            {
                double lbs;
                cout << "Enter pounds: ";
                cin >> lbs;
                double kg = lbs / 2.20462;
                cout << "=> Kilograms = " << kg << " kg\n";
            }
            else if (choice == '3')
            {
                double grams;
                cout << "Enter grams: ";
                cin >> grams;
                double ounces = grams / 28.3495;
                cout << "=> Ounces = " << ounces << " oz\n";
            }
            else
            {
                cout << "❌ Invalid choice! Returning to main menu.\n";
            }
            break;

        // ==================== TEMPERATURE SECTION ====================
        case 't':
        case 'T':
            cout << "\n***** Temperature Conversions *****\n";
            cout << "1. Celsius to Fahrenheit (°C -> °F)\n";
            cout << "2. Fahrenheit to Celsius (°F -> °C)\n";
            cout << "3. Celsius to Kelvin (°C -> K)\n";

            cout << "\nWhich operation do you need: ";
            cin >> choice;

            if (choice == '1')
            {
                double celsius;
                cout << "Enter Celsius: ";
                cin >> celsius;
                double fahrenheit = (celsius * 9.0 / 5.0) + 32.0;
                cout << "=> Fahrenheit = " << fahrenheit << " °F\n";
            }
            else if (choice == '2')
            {
                double fahrenheit;
                cout << "Enter Fahrenheit: ";
                cin >> fahrenheit;
                double celsius = (fahrenheit - 32.0) * 5.0 / 9.0;
                cout << "=> Celsius = " << celsius << " °C\n";
            }
            else if (choice == '3')
            {
                double celsius;
                cout << "Enter Celsius: ";
                cin >> celsius;
                double kelvin = celsius + 273.15;
                cout << "=> Kelvin = " << kelvin << " K\n";
            }
            else
            {
                cout << "❌ Invalid choice! Returning to main menu.\n";
            }
            break;

        // ==================== ERROR HANDLING ====================
        default:
            cout << "❌ Invalid category selection! Please pick l, w, t, or q.\n";
            break;
        }

    } while (conversion != 'q' && conversion != 'Q');

    cout << "\n==================================\n";
    cout << "Thank you for using Global Unit Converter! Goodbye.\n";

    return 0;
}
