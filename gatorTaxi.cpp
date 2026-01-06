#include <iostream>
#include <fstream>
#include <string>

#include "MyRBT.h"
#include "stringUtils.h"

using namespace std;

void Print(RBT* rbt, Heap* heap, int* args, string& output) {
    (void)heap;
    output += rbt->range(args[0], args[0]);
}

void PrintRange(RBT* rbt, Heap* heap, int* args, string& output) {
    (void)heap;
    output += rbt->range(args[0], args[1]);
}

void InsertRide(RBT* rbt, Heap* heap, int* args, string& output) {
    int DriveNumber = args[0];
    int DriveCost = args[1];
    int DriveDuration = args[2];

    if (rbt->findNode(DriveNumber, rbt->root)) {
        output += "Duplicate RideNumber\n";
        return;
    }

    // Create nodes
    RBTNode* rnode = new RBTNode(DriveNumber, DriveCost, DriveDuration, RED);
    HeapNode* hnode = new HeapNode(nullptr, DriveNumber, DriveCost, DriveDuration);

    // Cross links
    rnode->heapNode = hnode;
    hnode->rbtNode = rnode;

    // Insert into structures
    rbt->insert(rnode);
    heap->push(hnode);
}

void GetNextRide(RBT* rbt, Heap* heap, string& output) {
    if (heap->getSize() == 0) {
        output += "No active ride requests\n";
        return;
    }

    HeapNode* top = heap->pop();
    top->printHeapNode(output);

    // delete from tree
    RBTNode* removed = rbt->deleteNode(top->DriveNumber);
    (void)removed;
}

void CancelRide(RBT* rbt, Heap* heap, int* args, string& output) {
    (void)output;
    int DriveNumber = args[0];

    RBTNode* removed = rbt->deleteNode(DriveNumber);
    if (!removed) return;

    if (removed->heapNode) heap->remove(removed->heapNode);
}

void UpdateTrip(RBT* rbt, Heap* heap, int* args, string& output) {
    int DriveNumber = args[0];
    int newTD = args[1];

    RBTNode* node = rbt->findNode(DriveNumber, rbt->root);
    if (!node) return;

    int existingTD = node->DriveDuration;

    if (newTD <= existingTD) {
        // Just update duration
        node->DriveDuration = newTD;
        if (node->heapNode) {
            node->heapNode->DriveDuration = newTD;
            heap->update(node->heapNode, node->DriveCost, newTD);
        }
    } else if (existingTD < newTD && newTD <= 2 * existingTD) {
        // cancel + reinsert with cost+10
        int oldCost = node->DriveCost;

        int cancelArgs[1] = {DriveNumber};
        CancelRide(rbt, heap, cancelArgs, output);

        int insertArgs[3] = {DriveNumber, oldCost + 10, newTD};
        InsertRide(rbt, heap, insertArgs, output);
    } else {
        // newTD > 2*existingTD => cancel only
        int cancelArgs[1] = {DriveNumber};
        CancelRide(rbt, heap, cancelArgs, output);
    }
}

static void analyzeInput(RBT* rbt, Heap* heap, const char* path) {
    fstream inputFile;
    inputFile.open(path, ios::in);

    string output;
    if (inputFile.is_open()) {
        string fileLine;
        while (getline(inputFile, fileLine)) {
            if (fileLine.size() < 3) continue;
            string operationPrefix = fileLine.substr(0, 3);
            processOperation(rbt, heap, fileLine, operationPrefix, output);
        }
        inputFile.close();
    }

    ofstream out("output_file.txt");
    out << output;
    out.close();
}

int main(int argc, char** argv) {
    RBT* rbt = new RBT();
    Heap* heap = new Heap();

    if (argc >= 2) {
        analyzeInput(rbt, heap, argv[1]);
        return 0;
    }

    cout << "Please give proper input\n";
    return 0;
}
