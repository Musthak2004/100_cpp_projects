#include <iostream>
using namespace std;

int main()
{
    string questions[5] = {
        "1. What is the capital of Japan?",
        "2. Which language is used for in web pages?",
        "3. How many days are there in a week?",
        "4. Which planet is known as the Red Planet?",
        "5. What is 5 + 5?"};

    string options[][4] =
        {
            {"A. Seoul", "B. Tokyo", "C. Beijing", "D. Bangkok"},

            {"A. HTML", "B. C++", "C. Python", "D. Java"},

            {"A. 5", "B. 6", "C. 7", "D. 8"},

            {"A. Earth", "B. Jupiter", "C. Mars", "D. Venus"},

            {"A. 8", "B. 9", "C. 10", "D. 11"}};

    char answers[] = {'B', 'A', 'C', 'C', 'C'};

    int size = sizeof(questions) / sizeof(questions[0]);

    char guess;
    int score = 0;

    cout << "===== QUIZ GAME =====\n";

    for (int i = 0; i < size; i++)
    {
        cout << "\n";
        cout << questions[i] << "\n\n";

        for (int j = 0; j < 4; j++)
        {
            cout << options[i][j] << "\n";
        }

        cout << "\nEnter your answer (A/B/C/D): ";
        cin >> guess;

        guess = toupper(guess);

        if (guess == answers[i])
        {
            cout << "Correct!\n";
            score++;
        }
        else
        {
            cout << "Wrong!\n";
            cout << "Correct answer was: " << answers[i] << "\n";
        }
    }

    cout << "\n===== RESULT =====\n";

    cout << "Your score: " << score << "/" << size << "\n";

    double percentage = ((double)score / size) * 100;

    cout << "Percentage: " << percentage << "%\n";

    cout << "\n===================\n";

    return 0;
}