#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    double simpleInterestForYear;
    double simpleInterestForMonth;
    double principalAmount;
    double interestRatePerYear;
    double timeInYears;

    cout << "***** Simple Interest Calculator *****\n";

    cout << "\n";

    cout << "Enter prinicipal amount: Rs.";
    cin >> principalAmount;

    cout << "Enter interest rate per year: ";
    cin >> interestRatePerYear;

    cout << "Enter time in years: ";
    cin >> timeInYears;

    cout << "\n";

    simpleInterestForYear = (principalAmount * interestRatePerYear * timeInYears) / (100);
    simpleInterestForMonth = simpleInterestForYear / 12;

    cout << "Your Interest for month: Rs." << setprecision(2) << fixed << simpleInterestForMonth << "\n";
    cout << "Your Interest for year: Rs." << setprecision(2) << fixed << simpleInterestForYear << "\n";

    cout << "\n";

    cout << "**************************************\n";

    return 0;
}