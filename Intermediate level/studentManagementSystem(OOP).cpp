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
};

int main()
{
    return 0;
}