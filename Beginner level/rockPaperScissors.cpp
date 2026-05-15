#include <iostream>
#include <ctime>
using namespace std;

char getUserChoice();
char getComputerChoice();
void showChoice(char choice);
void chooseWinner(char player, char computer);

int main()
{
    char player;
    char computer;
    char playAgain;

    srand(time(0));

    cout << "***** ROCK PAPER SCISSORS *****\n";

    do
    {
        player = getUserChoice();

        cout << "\nYour choice: ";
        showChoice(player);

        computer = getComputerChoice();

        cout << "Computer choice: ";
        showChoice(computer);

        cout << "\n";

        chooseWinner(player, computer);

        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;

        cout << "\n";

    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Thanks for playing!\n";

    return 0;
}
char getUserChoice()
{
    char player;

    do
    {
        cout << "\nChoose one:\n";
        cout << "r = Rock\n";
        cout << "p = Paper\n";
        cout << "s = Scissors\n";

        cout << "Enter your choice: ";
        cin >> player;

        player = tolower(player);

        if (player != 'r' && player != 'p' && player != 's')
        {
            cout << "Invalid choice! Try again.\n";
        }

    } while (player != 'r' && player != 'p' && player != 's');

    return player;
}
char getComputerChoice()
{
    int num = rand() % 3 + 1;

    switch (num)
    {
    case 1:
        return 'r';
    case 2:
        return 'p';
    case 3:
        return 's';
    }

    return 'r';
}
void showChoice(char choice)
{
    switch (choice)
    {
    case 'r':
        cout << "Rock\n";
        break;
    case 'p':
        cout << "Paper\n";
        break;
    case 's':
        cout << "Scissors\n";
        break;
    }
}
void chooseWinner(char player, char computer)
{
    if (player == computer)
    {
        cout << "It's a tie!\n";
    }
    else if (player == 'r')
    {
        if (computer == 's')
        {
            cout << "You WIN!\n";
        }
        else
        {
            cout << "You LOSE!\n";
        }
    }
    else if (player == 'p')
    {
        if (computer == 'r')
        {
            cout << "You WIN!\n";
        }
        else
        {
            cout << "You LOSE!\n";
        }
    }
    else if (player == 's')
    {
        if (computer == 'p')
        {
            cout << "You WIN!\n";
        }
        else
        {
            cout << "You LOSE!\n";
        }
    }
}