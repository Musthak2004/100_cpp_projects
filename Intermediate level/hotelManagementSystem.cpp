#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::find

using namespace std;

// --- 1. Guest Class (Represents the customer) ---
class Guest
{
private:
    string name;
    string contact;

public:
    // Constructor
    Guest(string n, string c) : name(n), contact(c) {}

    // Getters
    string getName() const { return name; }
    string getContact() const { return contact; }

    // Display method
    void displayGuestInfo() const
    {
        cout << "\n--- Guest Information ---" << endl;
        cout << "Name: " << name << endl;
        cout << "Contact: " << contact << endl;
    }
};

// --- 2. Room Class (Represents a single room) ---
class Room
{
private:
    int roomNumber;
    string roomType;
    double price;
    bool isBookedStatus; // Changed name slightly to avoid conflict with method name
    Guest *currentGuest; // Pointer to the guest currently occupying the room

public:
    // Constructor
    Room(int num, string type, double p) : roomNumber(num), roomType(type), price(p), isBookedStatus(false), currentGuest(nullptr) {}

    // Getters
    int getRoomNumber() const { return roomNumber; }
    string getRoomType() const { return roomType; }
    double getPrice() const { return price; }
    bool isBooked() const { return isBookedStatus; }

    // Setter/Mutator
    void setGuest(Guest *guest)
    {
        currentGuest = guest;
        isBookedStatus = true;
    }

    void clearBooking()
    {
        currentGuest = nullptr;
        isBookedStatus = false;
    }

    // Display method
    void displayDetails() const
    {
        cout << "\n=======================================" << endl;
        cout << "Room Number: " << roomNumber << endl;
        cout << "Type: " << roomType << endl;
        cout << "Price per Night: $" << price << endl;
        cout << "Status: " << (isBookedStatus ? "Booked" : "Vacant") << endl;
        if (isBookedStatus && currentGuest != nullptr)
        {
            cout << "Occupied by: " << currentGuest->getName() << endl;
        }
        else
        {
            cout << "Occupied by: None" << endl;
        }
        cout << "=======================================" << endl;
    }
};

// --- 3. Hotel Class (Manages all Rooms and operations) ---
class Hotel
{
private:
    // Using a vector to store all Room objects (Composition)
    vector<Room> rooms;

public:
    // Method to add a new room to the hotel
    void addRoom(const Room &room)
    {
        rooms.push_back(room);
        cout << "\n[SUCCESS] Room " << room.getRoomNumber() << " added to the hotel." << endl;
    }

    // Search function to find a room by number
    Room *findRoom(int roomNum)
    {
        for (auto &room : rooms)
        {
            if (room.getRoomNumber() == roomNum)
            {
                return &room; // Return a pointer to the found room
            }
        }
        return nullptr; // Room not found
    }

    // Method to book a room
    bool bookRoom(int roomNum, Guest *guest)
    {
        Room *room = findRoom(roomNum);

        if (room == nullptr)
        {
            cout << "\n[ERROR] Room " << roomNum << " does not exist." << endl;
            return false;
        }

        if (room->isBooked())
        {
            cout << "\n[ERROR] Room " << roomNum << " is already booked." << endl;
            return false;
        }

        // Successful Booking
        room->setGuest(guest);
        cout << "\n[SUCCESS] Room " << roomNum << " successfully booked for " << guest->getName() << "." << endl;
        return true;
    }

    // Method to check in a guest safely using a pointer to a persistent Guest object
    void checkIn(int roomNum, Guest *guest)
    {
        Room *room = findRoom(roomNum);
        if (room == nullptr)
        {
            cout << "\n[ERROR] Check-in failed: Room " << roomNum << " not found." << endl;
            return;
        }

        if (guest == nullptr)
        {
            cout << "\n[ERROR] Check-in failed: Invalid Guest." << endl;
            return;
        }

        room->setGuest(guest);
        cout << "\n[CHECK-IN SUCCESS] Welcome, " << guest->getName() << "! You are now checked into Room " << roomNum << "." << endl;
    }

    // Method to check out a guest
    void checkOut(int roomNum)
    {
        Room *room = findRoom(roomNum);

        if (room == nullptr)
        {
            cout << "\n[ERROR] Check-out failed: Room " << roomNum << " not found." << endl;
            return;
        }

        if (!room->isBooked())
        {
            cout << "\n[ERROR] Check-out failed: Room " << roomNum << " is currently vacant." << endl;
            return;
        }

        // Update status
        room->clearBooking();
        cout << "\n[CHECK-OUT SUCCESS] Room " << roomNum << " successfully checked out." << endl;
    }

    // Display all rooms
    void displayAllRooms() const
    {
        cout << "\n\n=======================================" << endl;
        cout << "        CURRENT HOTEL ROOMS" << endl;
        cout << "=======================================" << endl;
        for (const auto &room : rooms)
        {
            room.displayDetails();
        }
        cout << "=======================================" << endl;
    }
};

// --- Main Execution Function ---
int main()
{
    // 1. Initialize the Hotel
    Hotel myHotel;

    // 2. Add Rooms to the Hotel
    myHotel.addRoom(Room(101, "Standard", 50.00));
    myHotel.addRoom(Room(102, "Deluxe", 85.00));
    myHotel.addRoom(Room(201, "Suite", 150.00));

    // Display initial state
    myHotel.displayAllRooms();

    // 3. Define Guests
    Guest alice("Alice Smith", "alice@example.com");
    Guest bob("Bob Johnson", "bob@example.com");

    cout << "\n\n--------------------------------------------" << endl;
    cout << "         STARTING BOOKING PROCESS" << endl;
    cout << "--------------------------------------------" << endl;

    // 4. Attempt to Book a Room
    cout << "\nAttempting to book Room 101 for Alice..." << endl;
    myHotel.bookRoom(101, &alice);

    cout << "\nAttempting to book Room 101 for Bob (Should Fail)..." << endl;
    myHotel.bookRoom(101, &bob);

    // 5. Check-In Process (Passing address of persistent guest object)
    cout << "\n--- Simulating Check-In for Alice in Room 101 ---" << endl;
    myHotel.checkIn(101, &alice);

    // 6. Display updated room details
    myHotel.displayAllRooms();

    // 7. Check-Out Process
    cout << "\n--- Simulating Check-Out for Alice from Room 101 ---" << endl;
    myHotel.checkOut(101);

    // 8. Display final state
    myHotel.displayAllRooms();

    return 0;
}