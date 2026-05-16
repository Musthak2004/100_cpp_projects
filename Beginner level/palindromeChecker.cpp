#include <iostream>
#include <string>
using namespace std;

int main()
{
    string word;
    string reversed = "";

    cout << "***** Palindrome Checker *****\n\n";

    cout << "Enter a word: ";
    cin >> word;

    for (int i = word.length() - 1; i >= 0; i--)
    {
        reversed += word[i];
    }

    cout << "\nReversed word: " << reversed << "\n";

    if (word == reversed)
    {
        cout << "It is a palindrome!\n";
    }
    else
    {
        cout << "It is NOT a palindrome!\n";
    }

    cout << "\n*********************************\n";

    return 0;
}