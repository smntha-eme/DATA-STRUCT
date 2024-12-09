#include <iostream>
#include <queue>
#include <string>
#include <map>
using namespace std;

// ANSI color codes for text styling
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Struct for Movie with priority (showtime in minutes from midnight)
struct Movie {
    string name;
    int showtime; // Priority: showtime in minutes from midnight

    // Constructor to initialize Movie with a name and showtime
    Movie(string n, int t) : name(n), showtime(t) {}

    // Overload operator to create a Min-Heap (earliest time comes first)
    bool operator<(const Movie& other) const {
        return showtime > other.showtime; // This ensures the earliest time comes first in the heap
    }
};

// Binary Search Tree Node for Seats
struct SeatNode {
    int seatNumber;  // The number of the seat
    bool isBooked;   // Whether the seat is booked or not
    SeatNode* left;  // Left child (for binary search tree structure)
    SeatNode* right; // Right child (for binary search tree structure)

    // Constructor to initialize a SeatNode with a seat number
    SeatNode(int seat) : seatNumber(seat), isBooked(false), left(nullptr), right(nullptr) {}
};

// Binary Search Tree Class for Managing Seats
class SeatBST {
private:
    SeatNode* root; // Root node of the binary search tree

    // Helper function to insert a seat node into the binary search tree
    SeatNode* insert(SeatNode* node, int seatNumber) {
        if (!node) return new SeatNode(seatNumber);  // Insert the seat if node is empty
        if (seatNumber < node->seatNumber)
            node->left = insert(node->left, seatNumber);  // Insert to the left subtree
        else if (seatNumber > node->seatNumber)
            node->right = insert(node->right, seatNumber); // Insert to the right subtree
        return node;
    }

    // Helper function to perform in-order traversal to view seats
    void inorder(SeatNode* node) {
        if (!node) return; // Base case: if node is null, return
        inorder(node->left); // Visit left subtree
        cout << "  " << YELLOW << "Seat " << node->seatNumber << (node->isBooked ? " (Booked)" : " (Available)") << RESET << endl; // Print seat info
        inorder(node->right); // Visit right subtree
    }

    // Helper function to search for a seat by seat number
    SeatNode* search(SeatNode* node, int seatNumber) {
        if (!node || node->seatNumber == seatNumber) return node; // If seat found or null, return the node
        if (seatNumber < node->seatNumber)
            return search(node->left, seatNumber); // Search in left subtree
        return search(node->right, seatNumber); // Search in right subtree
    }

    // Helper function to delete a seat from the binary search tree
    SeatNode* deleteNode(SeatNode* node, int seatNumber) {
        if (!node) return node;  // If node is empty, return

        // Traverse the tree to find the seat to delete
        if (seatNumber < node->seatNumber)
            node->left = deleteNode(node->left, seatNumber);  // Search in left subtree
        else if (seatNumber > node->seatNumber)
            node->right = deleteNode(node->right, seatNumber); // Search in right subtree
        else { // Seat found
            // Case when the node has only one child or no children
            if (!node->left) {
                SeatNode* temp = node->right;
                delete node;  // Delete the current node
                return temp;  // Return the right child
            } else if (!node->right) {
                SeatNode* temp = node->left;
                delete node;  // Delete the current node
                return temp;  // Return the left child
            }

            // Case when the node has two children:
            // Find the inorder successor (smallest node in the right subtree)
            SeatNode* temp = findMin(node->right);
            node->seatNumber = temp->seatNumber; // Copy the inorder successor's seat number to the current node
            node->right = deleteNode(node->right, temp->seatNumber);  // Delete the inorder successor
        }
        return node;
    }

    // Helper function to find the minimum node (leftmost node)
    SeatNode* findMin(SeatNode* node) {
        SeatNode* current = node;
        while (current && current->left)  // Traverse left subtree until the leftmost node
            current = current->left;
        return current;  // Return the leftmost node
    }

public:
    SeatBST() : root(nullptr) {}  // Constructor to initialize an empty tree

    // Function to add a seat to the binary search tree
    void addSeat(int seatNumber) {
        root = insert(root, seatNumber);
    }

    // Function to view the seats (in-order traversal)
    void viewSeats() {
        if (!root) {
            cout << RED << "No seats available.\n" << RESET;
            return;
        }
        inorder(root);
    }

    // Function to book a seat (mark it as booked)
    bool bookSeat(int seatNumber) {
        SeatNode* seat = search(root, seatNumber);
        if (!seat) {
            cout << RED << "Seat " << seatNumber << " does not exist.\n" << RESET;
            return false;
        }
        if (seat->isBooked) {
            cout << RED << "Seat " << seatNumber << " is already booked.\n" << RESET;
            return false;
        }
        seat->isBooked = true; // Mark the seat as booked
        cout << GREEN << "Seat " << seatNumber << " successfully booked.\n" << RESET;
        return true;
    }

    // Function to delete a seat (remove it from the tree)
    bool deleteSeat(int seatNumber) {
        SeatNode* seat = search(root, seatNumber);
        if (!seat) {
            cout << RED << "Seat " << seatNumber << " does not exist.\n" << RESET;
            return false;
        }
        root = deleteNode(root, seatNumber);  // Delete the seat from the tree
        cout << GREEN << "Seat " << seatNumber << " successfully deleted.\n" << RESET;
        return true;
    }

    // Function to cancel a booking (unbook and delete the seat)
    bool cancelBooking(int seatNumber) {
        SeatNode* seat = search(root, seatNumber);
        if (!seat) {
            cout << RED << "Seat " << seatNumber << " does not exist.\n" << RESET;
            return false;
        }
        if (!seat->isBooked) {
            cout << RED << "Seat " << seatNumber << " is not booked.\n" << RESET;
            return false;
        }
        seat->isBooked = false; // Unmark the seat as booked
        root = deleteNode(root, seatNumber);  // Delete the seat node from the tree
        cout << GREEN << "Booking for Seat " << seatNumber << " successfully canceled and seat deleted.\n" << RESET;
        return true;
    }
};

// Max-Heap for Priority Customers based on age
class PriorityQueue {
private:
    priority_queue<pair<int, string>> heap;  // Max-Heap by default using the first element (age)

public:
    // Function to add a priority request (customer) to the heap
    void addRequest(string customerName, int age) {
        heap.push({age, customerName});  // Add the request to the heap
    }

    // Function to process (serve) the next request in the heap (highest priority)
    void processNextRequest() {
        if (heap.empty()) {
            cout << RED << "No pending requests.\n" << RESET;
            return;
        }
        auto next = heap.top();
        heap.pop();
        cout << GREEN << "Processing request for " << next.second << " (Age: " << next.first << ")\n" << RESET;
    }

    // Function to view all pending priority requests
    void viewRequests() {
        if (heap.empty()) {
            cout << RED << "No pending requests.\n" << RESET;
            return;
        }
        cout << CYAN << "Pending requests:\n" << RESET;
        auto tempHeap = heap; // Copy heap to view contents
        while (!tempHeap.empty()) {
            auto req = tempHeap.top();
            cout << "  " << req.second << " (Age: " << req.first << ")\n";
            tempHeap.pop();
        }
    }
};

int main() {
    // Min-Heap for movies based on showtime
    priority_queue<Movie> movieHeap;
    map<string, SeatBST> movies;  // Map to store movies with their seats

    // Add movies to heap with showtimes
    movieHeap.push(Movie("Hello, Love, Again", 13 * 60)); // 1:00 PM
    movieHeap.push(Movie("Moana 2", 14 * 60 + 30));       // 2:30 PM
    movieHeap.push(Movie("The Wicked", 17 * 60));         // 5:00 PM

    // Add seats for each movie in the map
    for (int i = 1; i <= 10; i++) {
        movies["Hello, Love, Again"].addSeat(i);
        movies["Moana 2"].addSeat(i);
        movies["The Wicked"].addSeat(i);
    }

    string currentMovie;
    int choice;

    cout << CYAN << "=============================================\n";
    cout << "       Welcome to SM Cinema Legazpi!        \n";
    cout << "        Your entertainment destination!      \n";
    cout << "=============================================\n" << RESET;

    PriorityQueue priorityQueue;

    do {
        cout << "\n--- Main Menu ---\n";
        if (currentMovie.empty()) {
            cout << RED << "No movie selected.\n" << RESET;
        } else {
            cout << "Current Movie: " << currentMovie << "\n";
        }
        cout << BLUE << "1. View Movies by Showtimes\n";
        cout << "2. Choose Movie\n";
        cout << "3. View Seats\n";
        cout << "4. Book Seat\n";
        cout << "5. Cancel Booking\n";
        cout << "6. Add Priority Request\n";
        cout << "7. Process Next Priority Request\n";
        cout << "8. View Priority Requests\n";
        cout << "0. Exit\n" << RESET;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "\nMovies by Showtimes:\n";
            auto tempHeap = movieHeap; // Copy heap to view contents
            while (!tempHeap.empty()) {
                auto movie = tempHeap.top();
                tempHeap.pop();
                int hours = movie.showtime / 60;
                int minutes = movie.showtime % 60;

                string am_pm = (hours >= 12) ? "PM" : "AM";
                if (hours > 12) hours -= 12; // Convert to 12-hour format
                if (hours == 0) hours = 12;  // Midnight case (12 AM)

                cout << YELLOW << "  " << movie.name << " - " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << " " << am_pm << RESET << "\n";
            }
            break;
        }
        case 2: {
            cout << "\nEnter the name of the movie you want to select: ";
            cin.ignore();
            getline(cin, currentMovie);

            if (movies.find(currentMovie) == movies.end()) {
                cout << RED << "Invalid movie selection. Please try again.\n" << RESET;
                currentMovie.clear();
            } else {
                cout << "You selected: " << currentMovie << endl;

                auto tempHeap = movieHeap; // Copy heap to view contents
                while (!tempHeap.empty()) {
                    auto movie = tempHeap.top();
                    tempHeap.pop();
                    if (movie.name == currentMovie) {
                        int hours = movie.showtime / 60;
                        int minutes = movie.showtime % 60;

                        string am_pm = (hours >= 12) ? "PM" : "AM";
                        if (hours > 12) hours -= 12;
                        if (hours == 0) hours = 12;

                        cout << GREEN << "Thank you for choosing SM Cinema. Your chosen movie, '" << currentMovie
                             << "', will start at " << hours << ":" << (minutes < 10 ? "0" : "") << minutes << " " << am_pm << "." << RESET << "\n";
                        break;
                    }
                }
            }
            break;
        }

        case 3:
            if (currentMovie.empty()) {
                cout << RED << "Please select a movie first.\n" << RESET;
            } else {
                cout << "\nAvailable Seats for " << currentMovie << ":\n";
                movies[currentMovie].viewSeats();
            }
            break;

        case 4: {
            if (currentMovie.empty()) {
                cout << RED << "Please select a movie first.\n" << RESET;
            } else {
                int seatNumber;
                cout << "Enter seat number to book: ";
                cin >> seatNumber;
                movies[currentMovie].bookSeat(seatNumber);
            }
            break;
        }

        case 5: {
            if (currentMovie.empty()) {
                cout << RED << "Please select a movie first.\n" << RESET;
            } else {
                int seatNumber;
                cout << "Enter seat number to cancel booking: ";
                cin >> seatNumber;
                movies[currentMovie].cancelBooking(seatNumber);
            }
            break;
        }

        case 6: {
            string customerName;
            int age;
            cout << "Enter customer name: ";
            cin.ignore();
            getline(cin, customerName);
            cout << "Enter customer age: ";
            cin >> age;
            priorityQueue.addRequest(customerName, age);
            break;
        }

        case 7:
            priorityQueue.processNextRequest();
            break;

        case 8:
            priorityQueue.viewRequests();
            break;

        case 0:
            cout << CYAN << "Thank you for visiting SM Cinema. Goodbye!" << RESET << endl;
            break;

        default:
            cout << RED << "Invalid choice. Please try again.\n" << RESET;
        }

    } while (choice != 0);

    return 0;
}
