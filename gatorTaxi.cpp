#include <iostream>
#include<vector>
#include<bits/stdc++.h> 
#include "MyRBT.h"
#include "stringUtils.h"

void print(RBT *rbt, Heap *heap, int *args, string &output) {  //definition for a function named "print" that takes pointers to objects of classes RBT and Heap, a pointer to an integer array, and a reference to a string object as arguments.
    cout << "print: " << args[0] << endl;  //output statement that prints the string "print:" followed by the value of the first element in the integer array passed to the function.
    output += rbt -> range(args[0], args[0]);  // Calls the range member function of RBT function
}

void searchinRange(RBT *rbt, Heap *heap, int *args, string &output) { 
    cout << "searchinRange: " << args[0] << " " << args[1] << endl; 
    output += rbt -> range(args[0], args[1]);
}

void inputDrive(RBT *rbt, Heap *heap, int* args, string &output) {
    cout << "insert: " << args[0] << " " << args[1] << " " << args[2] << endl;
    RBTNode* rnode = new RBTNode(nullptr, nullptr, nullptr, nullptr, RED, args[0], args[1], args[2]);
    HeapNode* hnode = new HeapNode(nullptr, args[0], args[1], args[2]);

    rnode -> heapNode = hnode; //assigns the pointer to the Heap node
    hnode -> rbtNode = rnode; // assigns the pointer to rbtnode

    if(rbt -> findNode(rnode -> DriveNumber, rbt -> root)) { //calls the findNode member function of the RBT object
        output += "Duplicate DriveNumber\n"; //appends the string "Duplicate RideNumber\n" to the string
        return;
    }

    rbt -> insert(rnode); // calls the insert member function of the RBT object
    heap -> push(hnode); // calls the push member function of the Heap object

    rbt -> printTree(); //calls the printTree member function of the RBT object
    heap -> printHeap(); //calls the printHeap member function of the Heap object
}

void getNewDrive(RBT *rbt, Heap *heap, string &output) { //definition for a function named "getNewDrive"
    cout << "getNewDrive" << endl; //output statement

    if(heap -> getSize() == 0) { //checks if the size of the Heap object passed to the function = zero
        output += "No active Drive requests\n"; 
        return;
    }
    HeapNode* deletedHeapNode = heap -> pop(); //calls the pop member function of the Heap object
    deletedHeapNode -> printHeapNode(output); // calls the printHeapNode member function of the HeapNode object
    rbt -> deleteNode(deletedHeapNode -> rbtNode -> DriveNumber); // calls the deleteNode member function of the RBT object

    cout << endl;
    heap -> printHeap(); 
    rbt -> printTree();
}

void terminateDrive(RBT *rbt, Heap *heap, int* args, string &output) { //definition for a function named "terminateDrive"
    cout << "terminateDrive: " << args[0] << endl;
    RBTNode* deletedRBTNode = rbt -> deleteNode(args[0]); //calls the deleteNode member function of the RBT object
    heap -> remove(deletedRBTNode -> heapNode);

    heap -> printHeap();
    rbt -> printTree();
}

void upgradeDrive(RBT *rbt, Heap *heap, int* args, string &output) { //definition for a function named "upgradeDrive"
    int newTD = args[1]; //assigns the second element in the integer array
    cout<< "upgradeDrive: " << args[0] << " " << newTD << endl; 
    RBTNode* node = rbt -> findNode(args[0], rbt -> root); // 
    if(node) {
        int existingTD = node -> DriveDuration; // assigns values
        if(newTD <= existingTD) {
            node -> DriveDuration = newTD;
            heap -> update(node -> heapNode , node -> DriveCost, newTD);
            heap -> printHeap();
            rbt -> printTree();
        } else if(existingTD < newTD && newTD <= 2 * existingTD) {
            terminateDrive(rbt, heap, args, output);
            args[1] = node -> DriveCost + 10;
            args[2] = newTD;
            inputDrive(rbt, heap, args, output);
        } else if(newTD > 2 * existingTD) {
            terminateDrive(rbt, heap, args, output);
        }
    } else cout << "No Drive to update" << endl;
}

void analyzeInput(RBT *rbt, Heap *heap, int argc, char **argv) { //definition for a function called "analyzeInput"
    fstream inputFile; // to create, read and write files
    inputFile.open(argv[1], ios::in);
    string output;

    if(inputFile.is_open()){
        string fileLine;
        while(getline(inputFile, fileLine)){ 
            string operationPrefix = fileLine.substr(0, 3);
            processOperation(rbt, heap, fileLine, operationPrefix, output); //calls a function named "processOperation" with all the things listed in the brackets
        }
        inputFile.close();
    }

    ofstream MyFile("output_file.txt"); // create and write files
    MyFile << output;
    MyFile.close();
}

int main(int argc, char **argv) { // terminates the inputfile
    RBT *rbt = new RBT();
    Heap *heap = new Heap();

    if(argc >= 2) {
        analyzeInput(rbt, heap, argc, argv);
        return 0;
    } else cout << "Please give proper input" << endl; //output statement
    return 0;

    // RBTNode *rnode;
    // HeapNode *hnode;

   


  

}

