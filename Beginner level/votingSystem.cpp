#include <iostream>
using namespace std;

void castVote(string candidates[], int votes[], int size)
{
    int candidateNumber;

    cout << "Candidates:\n";

    for (int i = 0; i < size; i++)
    {
        cout << i + 1 << ". " << candidates[i] << "\n";
    }

    cout << "\nChoose candidate number: ";
    cin >> candidateNumber;

    if (candidateNumber >= 1 && candidateNumber <= size)
    {
        votes[candidateNumber - 1]++;
        cout << "Vote cast successfully!\n";
    }
    else
    {
        cout << "Invalid candidate!\n";
    }
}
void showResults(string candidates[], int votes[], int size)
{
    cout << "===== RESULTS =====\n";

    for (int i = 0; i < size; i++)
    {
        cout << candidates[i] << ": " << votes[i] << " votes\n";
    }
}
void showWinner(string candidates[], int votes[], int size)
{
    int maxVotes = votes[0];
    int winnerIndex = 0;

    for (int i = 1; i < size; i++)
    {
        if (votes[i] > maxVotes)
        {
            maxVotes = votes[i];
            winnerIndex = i;
        }
    }

    cout << "Winner: " << candidates[winnerIndex] << " with " << maxVotes << " votes!\n";
}

int main()
{
    cout << "===== VOTING SYSTEM =====\n";

    string candidates[] = {"Alice", "Bob", "Charlie"};
    int votes[] = {0, 0, 0};

    int size = sizeof(candidates) / sizeof(candidates[0]);
    int choice;

    do
    {
        cout << "\n";
        cout << "1. Cast Vote\n";
        cout << "2. Show Results\n";
        cout << "3. Show Winner\n";
        cout << "4. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        cout << "\n";

        switch (choice)
        {
        case 1:
            castVote(candidates, votes, size);
            break;
        case 2:
            showResults(candidates, votes, size);
            break;
        case 3:
            showWinner(candidates, votes, size);
            break;
        case 4:
            cout << "Voting ended!\n";
            break;

        default:
            cout << "Invalid choice!\n";
            break;
        }

    } while (choice != 4);

    cout
        << "\n=========================\n";

    return 0;
}