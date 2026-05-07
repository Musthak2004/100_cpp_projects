#include <iostream>
using namespace std;

int main()
{
    int size;
    int subjects[size];

    cout << "Enter how many subjects: ";
    cin >> size;

    for (int i = 0; i < size; i++)
    {
        cout << "Enter mark #" << i + 1 << ": ";
    }

    return 0;
}