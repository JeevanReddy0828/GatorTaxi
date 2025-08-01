#include<iostream>
#include<math.h>
#include<bits/stdc++.h>

using namespace std;

class RBTNode;

class HeapNode {
public :
    RBTNode *rbtNode; //a pointer to an object of the RBTNode class named rbtNode
    int index, DriveNumber, DriveCost, DriveDuration; //variables
    HeapNode() { // constructor function
        index = -1;
        DriveNumber = 0;
        DriveCost = 0;
        DriveDuration = 0;
        rbtNode = nullptr;
    }

    HeapNode(RBTNode *rbtNode, int DriveNumber, int DriveCost, int DriveDuration) { //constructor function
        this -> index = -1;
        this -> rbtNode = rbtNode; 
        this -> DriveNumber = DriveNumber;
        this -> DriveCost = DriveCost;
        this -> DriveDuration = DriveDuration;
    }

    void printHeapNode() { //print function that prints the information of Heap Node
        cout << "(" << this -> index << ", " << 
        this -> DriveNumber << ", " << 
        this -> DriveCost << ", " << 
        this -> DriveDuration << ")";
    }

    void printHeapNode(string &output) {
        output += "(" + to_string(this -> DriveNumber) + "," + 
        to_string(this -> DriveCost) + "," +  
        to_string(this -> DriveDuration) + ")\n";
    }
};

class Heap {
    int back;
    HeapNode* heap[2000];
public:
    Heap() {
        back = 0;
    }

    int getSize() { // function that returns the value of the back variable
        return back;
    }

    void push(HeapNode *heapNode) {
        heap[back] = heapNode;
        heap[back] -> index = back;
        heapifyUp(back);
        back++;
    }

    void getTopNode () { // function that returns the value of the topHeap node
        heap[0] -> printHeapNode();
    }
    
    HeapNode* top () { //function that returns the HeapNode object
        return heap[0];
    }

    HeapNode* pop() { // removes and then returns the HeapNode object at the top of the heap.
        back--;
        HeapNode *deletedNode = heap[0];

        heap[0] = heap[back];
        heap[0] -> index = 0;
        heapifyDown(0);
        return deletedNode;
    }

    void heapifyUp(int childIndex) { //recursive function
        if(childIndex == 0) return ;
        int parent = childIndex % 2 ? (childIndex - 1)/2 : (childIndex - 1)/2;

        int prc = heap[parent] -> DriveCost, crc = heap[childIndex] -> DriveCost;
        int ptd = heap[parent] -> DriveDuration, ctd = heap[childIndex] -> DriveDuration;
        
        if(prc < crc) return;

        if(prc > crc) {
            swap(heap[parent] -> index, heap[childIndex] -> index); //swaps the index variables 
            swap(heap[parent], heap[childIndex]);
        } else if(ptd > ctd) {
            swap(heap[parent] -> index, heap[childIndex] -> index);
            swap(heap[parent], heap[childIndex]);
        } else return ;

        heapifyUp(parent); //recursively call heapifyUp
    }

    void heapifyDown(int parent) { //recursive function
        int leftChild = parent * 2 + 1, rightChild = parent * 2 + 2;
        if(leftChild >= back && rightChild >= back) return;

        int prc = heap[parent] -> DriveCost, 
            lcrc = heap[leftChild] -> DriveCost;
        int ptd = heap[parent] -> DriveDuration, 
            lctd = heap[leftChild] -> DriveDuration;
        int childToSwap = rightChild;

        //  lc = exists or not
        if(rightChild == back) {
            if(prc > lcrc || ptd > lctd) childToSwap = leftChild;
        } else {
            int rcrc = heap[rightChild] -> DriveCost, 
                rctd = heap[rightChild] -> DriveDuration;

            if(prc < lcrc && prc < rcrc) return;

            if(prc > lcrc || ptd > lctd) childToSwap = leftChild;
            else if(prc > rcrc || ptd > rctd) childToSwap = rightChild;
        }
        
        swap(heap[parent] -> index, heap[childToSwap] -> index);
        swap(heap[parent], heap[childToSwap]);
        heapifyDown(childToSwap);
    }

    void update(HeapNode* heapNode, int newDriveCost, int newDriveDuration) { //updates a node in the heap array 
        if(newDriveCost > heapNode -> DriveCost) {
            heapNode -> DriveCost = newDriveCost;
            heapifyDown(heapNode -> index);
        } else if(newDriveDuration <= heapNode -> DriveDuration) {
            heapNode -> DriveDuration = newDriveDuration;
            heapifyUp(heapNode -> index);
        } else if(newDriveDuration > heapNode -> DriveDuration) {
            heapNode -> DriveDuration = newDriveDuration;
            heapifyDown(heapNode -> index);
        }
    }

    void remove(HeapNode* heapNode) { //removes node from heap array
        int index = heapNode -> index;

        back--;
        heap[index] = heap[back];
        heap[index] -> index = index;
        heapifyDown(index);
    }

    void printHeap() { //prints the heap array.
        int level = 0, limit = 0;

        for(int i = 0; i < back; i++) {
            heap[i] -> printHeapNode();
            cout << ", ";
            if(i == limit) {
                limit += pow(2, ++level);
                cout << endl;
            }
        }
        cout << endl; 
    }
};