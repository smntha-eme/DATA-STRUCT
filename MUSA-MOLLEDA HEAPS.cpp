#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <unistd.h> // for sleep()
#include <algorithm> // for find()

using namespace std;

// ANSI escape codes for text formatting (colors)
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

class MaxHeap {
private:
    vector<int> heap;

    // Helper function to maintain the Max-Heap property
    void heapify(int index) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heap.size() && heap[left] > heap[largest]) {
            largest = left;
        }
        if (right < heap.size() && heap[right] > heap[largest]) {
            largest = right;
        }
        if (largest != index) {
            swap(heap[index], heap[largest]);
            heapify(largest);
        }
    }

public:
    // Insert a new ornament (number) into the Max-Heap
    void insert(int ornament) {
        heap.push_back(ornament);
        int index = heap.size() - 1;
        while (index > 0 && heap[(index - 1) / 2] < heap[index]) {
            swap(heap[index], heap[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    }

    // Check if an ornament is already in the heap
    bool isDuplicate(int ornament) {
        return find(heap.begin(), heap.end(), ornament) != heap.end();
    }

    // Remove the root (largest) ornament and reheapify
    void removeRoot() {
        if (heap.size() == 0) {
            cout << RED << "The heap is empty!\n" << RESET;
            return;
        }
        heap[0] = heap.back();
        heap.pop_back();
        heapify(0);
    }

    // Visualize the Max-Heap as a tree structure
    void visualizeHeap() {
        cout << CYAN << "\n ChristMax Tree Design! \n" << RESET;
        int height = log2(heap.size()) + 1;
        int index = 0;
        for (int i = 0; i < height; ++i) {
            int nodesInLevel = pow(2, i);
            int spacing = pow(2, height - i - 1);
            for (int j = 0; j < nodesInLevel && index < heap.size(); ++j) {
                cout << setw(spacing) << heap[index++] << setw(spacing) << " ";
            }
            cout << endl;
        }
    }

    // Display the current heap (array representation of tree)
    void displayHeap() {
        cout << YELLOW << "Current ChristMax Tree: " << RESET;
        for (int i : heap) {
            cout << MAGENTA << i << " " << RESET;
        }
        cout << endl;
    }
};

void printLogo() {
    // Print a fun Christmas logo to add some design
    cout << BOLD << CYAN << "  *** Welcome to ***" << RESET << endl;
    cout << BOLD << GREEN << "  Let's Decorate Your ChristMax Tree!" << RESET << endl;
    cout << YELLOW << "   --- Christmas Fun with Heaps! ---\n\n" << RESET;
}

void printChristmasDecor() {
    // Print a small tree as a decoration
    cout << GREEN << "        *" << RESET << endl;
    cout << GREEN << "       ***" << RESET << endl;
    cout << GREEN << "      *****" << RESET << endl;
    cout << GREEN << "     *******" << RESET << endl;
    cout << GREEN << "    *********" << RESET << endl;
    cout << GREEN << "   ***********" << RESET << endl;
    cout << CYAN << "  [ChristMax Tree] " << RESET << endl;
}

int main() {
    MaxHeap christMaxTree;
    int ornament, choice;

    printLogo();
    printChristmasDecor();

    cout << GREEN << "In this game, you will decorate your tree by answering Christmas-themed questions." << RESET << endl;
    cout << "Each of your answers will add an ornament to the tree, and the largest ornaments will be placed at the top!" << endl;
    cout << YELLOW << "Let's begin the fun!\n" << RESET;

    // A series of Christmas-themed questions to gather inputs for decorating the tree
    for (int i = 1; i <= 5; ++i) {
        cout << BLUE << "Question " << i << ": ";
        if (i == 1) {
            cout << "On a scale of 1 to 10, how excited are you to celebrate Christmas this year? ";
        } else if (i == 2) {
            cout << "On a scale of 1 to 10, how much do you love decorating the Christmas tree? ";
        } else if (i == 3) {
            cout << "On a scale of 1 to 10, how much do you enjoy Christmas music? ";
        } else if (i == 4) {
            cout << "On a scale of 1 to 10, how excited are you for Christmas gifts? ";
        } else {
            cout << "On a scale of 1 to 10, how much do you love Christmas food? ";
        }

        while (true) {
            cin >> ornament;
            if (christMaxTree.isDuplicate(ornament)) {
                cout << RED << "This ornament has already been added! Please choose a different number.\n" << RESET;
            } else {
                christMaxTree.insert(ornament);
                cout << GREEN << "You added an ornament with excitement level " << ornament << "!\n" << RESET;
                break;
            }
        }
        christMaxTree.visualizeHeap();

        sleep(1); // Sleep to give time for user to see each question's response
    }

    // Allow the user to see the tree and make changes
    cout << "\n" << YELLOW << "Now, let's interact with your ChristMax Tree!" << RESET << endl;
    cout << "Would you like to remove the top ornament (root)? (1 for Yes, 0 for No): ";
    cin >> choice;

    if (choice == 1) {
        christMaxTree.removeRoot();
        cout << GREEN << "After removing the top ornament (root), your tree looks like this:\n" << RESET;
        christMaxTree.visualizeHeap();
    }

    // Show final heap state
    cout << "\n" << YELLOW << "Final ChristMax Tree:" << RESET << endl;
    christMaxTree.visualizeHeap();

    return 0;
}
