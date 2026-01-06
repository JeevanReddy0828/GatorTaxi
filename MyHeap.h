#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class RBTNode;

class HeapNode {
public:
    RBTNode* rbtNode;
    int index, DriveNumber, DriveCost, DriveDuration;

    HeapNode() {
        index = -1;
        DriveNumber = 0;
        DriveCost = 0;
        DriveDuration = 0;
        rbtNode = nullptr;
    }

    HeapNode(RBTNode* rbtNode, int DriveNumber, int DriveCost, int DriveDuration) {
        this->index = -1;
        this->rbtNode = rbtNode;
        this->DriveNumber = DriveNumber;
        this->DriveCost = DriveCost;
        this->DriveDuration = DriveDuration;
    }

    void printHeapNode() {
        cout << "(" << this->index << ", "
             << this->DriveNumber << ", "
             << this->DriveCost << ", "
             << this->DriveDuration << ")";
    }

    void printHeapNode(string& output) {
        output += "(" + to_string(this->DriveNumber) + "," +
                  to_string(this->DriveCost) + "," +
                  to_string(this->DriveDuration) + ")\n";
    }
};

class Heap {
    vector<HeapNode*> heap;

    static bool lessNode(const HeapNode* a, const HeapNode* b) {
        if (a->DriveCost != b->DriveCost) return a->DriveCost < b->DriveCost;
        return a->DriveDuration < b->DriveDuration;
    }

    void swapAt(int i, int j) {
        std::swap(heap[i], heap[j]);
        heap[i]->index = i;
        heap[j]->index = j;
    }

    void heapifyUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (!lessNode(heap[i], heap[p])) break;
            swapAt(i, p);
            i = p;
        }
    }

    void heapifyDown(int i) {
        int n = (int)heap.size();
        while (true) {
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            int best = i;

            if (l < n && lessNode(heap[l], heap[best])) best = l;
            if (r < n && lessNode(heap[r], heap[best])) best = r;

            if (best == i) break;
            swapAt(i, best);
            i = best;
        }
    }

public:
    Heap() = default;

    int getSize() const {
        return (int)heap.size();
    }

    void push(HeapNode* node) {
        node->index = (int)heap.size();
        heap.push_back(node);
        heapifyUp(node->index);
    }

    HeapNode* top() {
        if (heap.empty()) return nullptr;
        return heap[0];
    }

    void getTopNode() {
        if (!heap.empty()) heap[0]->printHeapNode();
    }

    HeapNode* pop() {
        if (heap.empty()) return nullptr;

        HeapNode* deleted = heap[0];
        int last = (int)heap.size() - 1;

        if (last == 0) {
            heap.pop_back();
            deleted->index = -1;
            return deleted;
        }

        heap[0] = heap[last];
        heap[0]->index = 0;
        heap.pop_back();

        deleted->index = -1;
        heapifyDown(0);
        return deleted;
    }

    void update(HeapNode* node, int newCost, int newDuration) {
        if (!node) return;
        int idx = node->index;
        if (idx < 0 || idx >= (int)heap.size()) return;

        int oldCost = node->DriveCost;
        int oldDur  = node->DriveDuration;

        node->DriveCost = newCost;
        node->DriveDuration = newDuration;

        // decide direction safely
        int p = (idx - 1) / 2;
        if (idx > 0 && lessNode(heap[idx], heap[p])) {
            heapifyUp(idx);
        } else {
            heapifyDown(idx);
        }

        (void)oldCost;
        (void)oldDur;
    }

    void remove(HeapNode* node) {
        if (!node) return;
        int idx = node->index;
        if (idx < 0 || idx >= (int)heap.size()) return;

        int last = (int)heap.size() - 1;
        node->index = -1;

        if (idx == last) {
            heap.pop_back();
            return;
        }

        heap[idx] = heap[last];
        heap[idx]->index = idx;
        heap.pop_back();

        // could go up or down
        if (idx > 0) {
            int p = (idx - 1) / 2;
            if (lessNode(heap[idx], heap[p])) heapifyUp(idx);
            else heapifyDown(idx);
        } else {
            heapifyDown(idx);
        }
    }

    void printHeap() {
        for (int i = 0; i < (int)heap.size(); i++) {
            heap[i]->printHeapNode();
            cout << (i + 1 < (int)heap.size() ? ", " : "");
        }
        cout << "\n";
    }
};
