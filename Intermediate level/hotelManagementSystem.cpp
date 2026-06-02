#include <iostream>
#include <string>
#include <vector>
#include <limits> // Required for numeric_limits

using namespace std;

// --- Function to handle input safely ---
double get_double_input(const string &prompt)
{
    double value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            return value;
        }
        cout << "Invalid input. Please enter a valid number.\n";
        cin.clear();                                         // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
    }
}

// --- Main Logic ---
int main()
{
    cout << "========================================\n";
    cout << "     Hotel Room Management System\n";
    cout << "========================================\n\n";

    // Using vectors to store rooms dynamically
    vector<string> room_names;
    vector<int> room_numbers;
    vector<string> room_statuses; // To track availability

    int num_rooms;
    cout << "How many rooms do you want to manage? ";
    if (!(cin >> num_rooms) || num_rooms <= 0)
    {
        cerr << "Invalid number of rooms entered. Exiting." << endl;
        return 1;
    }

    // 1. Room Setup Phase
    cout << "\n--- Room Setup ---\n";
    for (int i = 1; i <= num_rooms; ++i)
    {
        string name;
        int room_num;
        string status = "Available"; // Start all rooms as available

        cout << "\n--- Room " << i << " ---\n";
        cout << "Enter Room Name: ";
        getline(cin >> ws, name); // Use ws to consume any leftover whitespace

        cout << "Enter Room Number (e.g., 101): ";
        if (!(cin >> room_num))
        {
            cerr << "Invalid room number. Skipping this room setup.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        room_names.push_back(name);
        room_numbers.push_back(room_num);
        room_statuses.push_back(status); // Storing initial status
    }

    // 2. Booking Phase
    cout << "\n--- Booking System ---\n";

    int room_index;
    string customer_name;
    int room_to_book;

    while (true)
    {
        cout << "\nWhat would you like to do?\n";
        cout << "1. View Rooms\n";
        cout << "2. Make a Reservation\n";
        cout << "3. Exit\n";
        cout << "Choice: ";

        if (!(cin >> room_index))
        {
            cout << "Invalid choice. Please enter a number (1, 2, or 3).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (room_index == 3)
        {
            cout << "\nExiting system. Goodbye!\n";
            break;
        }

        if (room_index == 1)
        {
            cout << "\n--- Current Room List ---\n";
            if (room_names.empty())
            {
                cout << "No rooms have been set up.\n";
            }
            else
            {
                for (size_t i = 0; i < room_names.size(); ++i)
                {
                    cout << "Room #" << room_numbers[i] << ": " << room_names[i]
                         << " (Status: " << room_statuses[i] << ")\n";
                }
            }
        }
        else if (room_index == 2)
        {
            cout << "\n--- Make a Reservation ---\n";

            // Simple selection for booking
            cout << "Enter the room number you wish to book: ";
            if (!(cin >> room_to_book))
            {
                cout << "Invalid room number validation failed.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            // Find the index corresponding to the room number
            int found_index = -1;
            for (size_t i = 0; i < room_names.size(); ++i)
            {
                if (room_numbers[i] == room_to_book)
                {
                    found_index = i;
                    break;
                }
            }

            if (found_index != -1)
            {
                string current_status = room_statuses[found_index];
                if (current_status == "Available")
                {
                    cout << "Enter Customer Name: ";
                    getline(cin >> ws, customer_name);

                    // Update status
                    room_statuses[found_index] = "Booked by " + customer_name;
                    cout << "\nSUCCESS! Room " << room_to_book << " (" << room_names[found_index]
                         << ") is now booked for " << customer_name << ".\n";
                }
                else
                {
                    cout << "\nERROR! Room " << room_to_book << " is currently " << current_status << ".\n";
                }
            }
            else
            {
                cout << "\nERROR! Room number " << room_to_book << " does not exist.\n";
            }
        }
        else
        {
            cout << "\nInvalid choice. Please select 1, 2, or 3.\n";
        }
    }

    return 0;
}