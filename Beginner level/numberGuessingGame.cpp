#include <iostream>
#include <ctime>
#include <limits>

using namespace std;

int main()
{
    cout << "********** Number Guessing Game **********\n";
    cout << "Guess the random number between 1 - 100\n";
    cout << "The number is: X\n\n";

    srand(time(NULL));

    int randomNumber = rand() % 100 + 1;
    int guess = 0;
    int attempted = 0;

    while (guess != randomNumber)
    {
        cout << "Enter number: ";

        cin >> guess;

        if (cin.fail())
        {
            cout << "Invalid input! Enter numbers only.\n";

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            continue;
        }

        if (guess < 1 || guess > 100)
        {
            cout << "Enter between 1 and 100 only!\n";
            continue;
        }

        attempted++;

        if (guess > randomNumber)
        {
            cout << "Too high!\n";
        }
        else if (guess < randomNumber)
        {
            cout << "Too low!\n";
        }
        else
        {
            cout << "\nCorrect!\n";
            cout << "You guessed this number in "
                 << attempted << " attempts\n";
        }
    }

    cout << "\n******************************************\n";

    return 0;
}