#include <iostream>
#include <limits>
using namespace std;

void addTask(string tasks[], int &taskCount)
{
    if (taskCount >= 100)
    {
        cout << "To-Do List storage full!\n";
        return;
    }

    cout << "\nEnter task: ";
    getline(cin, tasks[taskCount]);

    taskCount++;

    cout << "Task added successfully!\n";
}

void viewTask(string tasks[], int taskCount)
{
    if (taskCount == 0)
    {
        cout << "No tasks available!\n";
        return;
    }

    cout << "\n===== TASK LIST =====\n";

    for (int i = 0; i < taskCount; i++)
    {
        cout << i + 1 << ". " << tasks[i] << "\n";
    }
}

void deleteTask(string tasks[], int &taskCount)
{
    if (taskCount == 0)
    {
        cout << "No tasks available!\n";
        return;
    }

    string task;
    bool found = false;

    cout << "\nEnter task name to delete: ";
    getline(cin, task);

    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i] == task)
        {
            // Shift elements left
            for (int j = i; j < taskCount - 1; j++)
            {
                tasks[j] = tasks[j + 1];
            }

            taskCount--;
            found = true;

            cout << "Task deleted successfully!\n";
            break;
        }
    }

    if (!found)
    {
        cout << "Task not found!\n";
    }
}

int main()
{
    cout << "===== TO-DO LIST =====\n";

    string tasks[100];
    int taskCount = 0;

    int choice;

    do
    {
        cout << "\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Delete Task\n";
        cout << "4. Exit\n";

        cout << "\nEnter your action (1-4): ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            addTask(tasks, taskCount);
            break;

        case 2:
            viewTask(tasks, taskCount);
            break;

        case 3:
            deleteTask(tasks, taskCount);
            break;

        case 4:
            cout << "Thank you! Exiting...\n";
            break;

        default:
            cout << "Invalid action!\n";
            break;
        }

    } while (choice != 4);

    cout << "\n======================\n";

    return 0;
}