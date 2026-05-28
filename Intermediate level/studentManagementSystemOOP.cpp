#include <iostream>
#include <limits>
using namespace std;

class Student
{
private:
    string name;
    int age;
    double marks;
    char grade;

public:
    // Default Constructor
    Student()
    {
        name = "";
        age = 0;
        marks = 0;
        grade = 'F';
    }

    // Parameterized Constructor
    Student(string n, int a, double m)
    {
        name = n;
        age = a;
        marks = m;

        calculateGrade();
    }

    void calculateGrade()
    {
        if (marks >= 75)
        {
            grade = 'A';
        }
        else if (marks >= 65)
        {
            grade = 'B';
        }
        else if (marks >= 55)
        {
            grade = 'C';
        }
        else if (marks >= 35)
        {
            grade = 'S';
        }
        else
        {
            grade = 'F';
        }
    }

    void display()
    {
        cout << "\n===== STUDENT DETAILS =====\n";

        cout << "Name  : " << name << "\n";
        cout << "Age   : " << age << "\n";
        cout << "Marks : " << marks << "\n";
        cout << "Grade : " << grade << "\n";
    }

    string getName()
    {
        return name;
    }
};

int main()
{
    cout << "===== STUDENT MANAGEMENT SYSTEM =====\n";

    Student students[100];

    int studentCount = 0;
    int choice;

    do
    {
        cout << "\n";
        cout << "1. Add Student\n";
        cout << "2. View Students\n";
        cout << "3. Search Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            if (studentCount >= 100)
            {
                cout << "Storage full!\n";
                break;
            }

            string name;
            int age;
            double marks;

            cout << "\nEnter student name: ";
            getline(cin, name);

            cout << "Enter age: ";
            cin >> age;

            cout << "Enter marks: ";
            cin >> marks;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            students[studentCount] = Student(name, age, marks);

            studentCount++;

            cout << "Student added successfully!\n";

            break;
        }

        case 2:
        {
            if (studentCount == 0)
            {
                cout << "No students available!\n";
                break;
            }

            for (int i = 0; i < studentCount; i++)
            {
                students[i].display();
            }

            break;
        }

        case 3:
        {
            if (studentCount == 0)
            {
                cout << "No students available!\n";
                break;
            }

            string searchName;
            bool found = false;

            cout << "\nEnter student name to search: ";
            getline(cin, searchName);

            for (int i = 0; i < studentCount; i++)
            {
                if (students[i].getName() == searchName)
                {
                    students[i].display();
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                cout << "Student not found!\n";
            }

            break;
        }

        case 4:
        {
            if (studentCount == 0)
            {
                cout << "No students available!\n";
                break;
            }

            string deleteName;
            bool found = false;

            cout << "\nEnter student name to delete: ";
            getline(cin, deleteName);

            for (int i = 0; i < studentCount; i++)
            {
                if (students[i].getName() == deleteName)
                {
                    for (int j = i; j < studentCount - 1; j++)
                    {
                        students[j] = students[j + 1];
                    }

                    studentCount--;

                    found = true;

                    cout << "Student deleted successfully!\n";

                    break;
                }
            }

            if (!found)
            {
                cout << "Student not found!\n";
            }

            break;
        }

        case 5:
        {
            cout << "Thank you!\n";
            break;
        }

        default:
        {
            cout << "Invalid choice!\n";
            break;
        }
        }

    } while (choice != 5);

    cout << "\n=====================================\n";

    return 0;
}