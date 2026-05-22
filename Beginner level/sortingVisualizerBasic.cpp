#include <iostream>
using namespace std;

void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << "\n";
}

int main()
{

    cout << "===== SORTING VISUALIZER =====\n\n";

    int size;

    cout << "Enter array size: ";
    cin >> size;

    int arr[size];

    cout << "\nEnter " << size << " numbers:\n";

    for (int i = 0; i < size; i++)
    {
        cout << "Number: " << i + 1 << ": ";
        cin >> arr[i];
    }

    cout << "\nOriginal Array: ";
    printArray(arr, size);

    cout << "\nSorting Process:\n";

    int swapCount = 0;

    for (int i = 0; i < size - 1; i++)
    {
        bool swapped = false;

        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                swapCount++;
                swapped = true;
            }
        }

        cout << "Pass " << i + 1 << ": ";
        printArray(arr, size);

        if (!swapped)
        {
            break;
        }
    }

    cout << "\nFinal Sorted Array:\n";
    printArray(arr, size);

    cout << "\nTotal Swaps: " << swapCount << "\n";

    cout << "\n===============================\n";

    return 0;
}