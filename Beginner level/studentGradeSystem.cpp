#include <iostream>
using namespace std;

int main()
{
    int size;
    int sum = 0;
    double avg;

    cout << "***** Student Grade System *****\n\n";

    cout << "Enter how many subjects: ";
    cin >> size;

    // ✅ validation
    if (size <= 0)
    {
        cout << "Invalid subject count!\n";
        return 0;
    }

    cout << "\n";

    int *subjects = new int[size];

    for (int i = 0; i < size; i++)
    {
        cout << "Enter mark #" << i + 1 << ": ";
        cin >> subjects[i];

        // ✅ mark validation
        if (subjects[i] < 0 || subjects[i] > 100)
        {
            cout << "Invalid mark! Enter between 0-100\n";
            i--;
            continue;
        }

        sum += subjects[i];
    }

    avg = (double)sum / size;

    cout << "\nTotal marks: " << sum << endl;
    cout << "Average mark: " << avg << endl;

    cout << "\n";

    if (avg >= 75)
    {
        cout << "Your grade A\n";
    }
    else if (avg >= 65)
    {
        cout << "Your grade B\n";
    }
    else if (avg >= 55)
    {
        cout << "Your grade C\n";
    }
    else if (avg >= 35)
    {
        cout << "Your grade S\n";
    }
    else
    {
        cout << "Your grade F\n";
    }

    cout << "\n********************************\n";

    delete[] subjects;

    return 0;
}