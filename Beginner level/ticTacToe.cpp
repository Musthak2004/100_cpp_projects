#include <iostream>
#include <ctime>

using namespace std;

// function declarations
void drawBoard(char *spaces);
void playerMove(char *spaces, char player);
void computerMove(char *spaces, char computer);
bool checkWinner(char *spaces, char player, char computer);
bool checkTie(char *spaces);
void showPositions();

int main()
{
    char spaces[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    char player = 'X';
    char computer = 'O';
    bool running = true;

    cout << "******** TIC TAC TOE ********\n";

    showPositions();
    drawBoard(spaces);

    while (running)
    {
        // player move
        playerMove(spaces, player);
        drawBoard(spaces);

        if (checkWinner(spaces, player, computer))
        {
            running = false;
            break;
        }

        if (checkTie(spaces))
        {
            cout << "It's a tie!\n";
            running = false;
            break;
        }

        // computer move
        computerMove(spaces, computer);
        drawBoard(spaces);

        if (checkWinner(spaces, player, computer))
        {
            running = false;
            break;
        }

        if (checkTie(spaces))
        {
            cout << "It's a tie!\n";
            running = false;
            break;
        }
    }

    cout << "\n*****************************\n";

    return 0;
}

// show board positions
void showPositions()
{
    cout << "\nBoard positions:\n\n";

    cout << " 1 | 2 | 3 \n";
    cout << "---|---|---\n";
    cout << " 4 | 5 | 6 \n";
    cout << "---|---|---\n";
    cout << " 7 | 8 | 9 \n\n";
}

// draw actual board
void drawBoard(char *spaces)
{
    cout << "\n";

    cout << " " << spaces[0] << " | " << spaces[1] << " | " << spaces[2] << '\n';
    cout << "---|---|---\n";

    cout << " " << spaces[3] << " | " << spaces[4] << " | " << spaces[5] << '\n';
    cout << "---|---|---\n";

    cout << " " << spaces[6] << " | " << spaces[7] << " | " << spaces[8] << '\n';

    cout << "\n";
}

// player move
void playerMove(char *spaces, char player)
{
    int number;

    while (true)
    {
        cout << "Enter a spot (1-9): ";
        cin >> number;

        // invalid input
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');

            cout << "Invalid input!\n";
            continue;
        }

        // valid range
        if (number < 1 || number > 9)
        {
            cout << "Choose between 1-9 only!\n";
            continue;
        }

        number--;

        // empty space check
        if (spaces[number] == ' ')
        {
            spaces[number] = player;
            break;
        }
        else
        {
            cout << "Spot already taken!\n";
        }
    }
}

// computer move
void computerMove(char *spaces, char computer)
{
    int number;

    while (true)
    {
        number = rand() % 9;

        if (spaces[number] == ' ')
        {
            spaces[number] = computer;

            cout << "Computer placed at position "
                 << number + 1 << "\n";

            break;
        }
    }
}

// winner check
bool checkWinner(char *spaces, char player, char computer)
{
    // rows
    if (spaces[0] != ' ' && spaces[0] == spaces[1] && spaces[1] == spaces[2])
    {
        spaces[0] == player ? cout << "YOU WIN!\n"
                            : cout << "YOU LOSE!\n";
    }
    else if (spaces[3] != ' ' && spaces[3] == spaces[4] && spaces[4] == spaces[5])
    {
        spaces[3] == player ? cout << "YOU WIN!\n"
                            : cout << "YOU LOSE!\n";
    }
    else if (spaces[6] != ' ' && spaces[6] == spaces[7] && spaces[7] == spaces[8])
    {
        spaces[6] == player ? cout << "YOU WIN!\n"
                            : cout << "YOU LOSE!\n";
    }

    // columns
    else if (spaces[0] != ' ' && spaces[0] == spaces[3] && spaces[3] == spaces[6])
    {
        spaces[0] == player ? cout << "YOU WIN!\n"
                            : cout << "YOU LOSE!\n";
    }
    else if (spaces[1] != ' ' && spaces[1] == spaces[4] && spaces[4] == spaces[7])
    {
        spaces[1] == player ? cout << "YOU WIN!\n"
                            : cout << "YOU LOSE!\n";
    }
    else if (spaces[2] != ' ' && spaces[2] == spaces[5] && spaces[5] == spaces[8])
    {
        spaces[2] == player ? cout << "YOU WIN!\n"
                            : cout << "YOU LOSE!\n";
    }

    // diagonals
    else if (spaces[0] != ' ' && spaces[0] == spaces[4] && spaces[4] == spaces[8])
    {
        spaces[0] == player ? cout << "YOU WIN!\n"
                            : cout << "YOU LOSE!\n";
    }
    else if (spaces[2] != ' ' && spaces[2] == spaces[4] && spaces[4] == spaces[6])
    {
        spaces[2] == player ? cout << "YOU WIN!\n"
                            : cout << "YOU LOSE!\n";
    }
    else
    {
        return false;
    }

    return true;
}

// tie check
bool checkTie(char *spaces)
{
    for (int i = 0; i < 9; i++)
    {
        if (spaces[i] == ' ')
        {
            return false;
        }
    }

    return true;
}