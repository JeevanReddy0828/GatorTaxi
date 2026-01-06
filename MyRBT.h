#pragma once

#include <iostream>
#include <queue>
#include <string>
#include "MyHeap.h"

using namespace std;

#define RED 0
#define BLACK 1

class RBTNode {
public:
    RBTNode* leftChild;
    RBTNode* rightChild;
    RBTNode* parent;

    HeapNode* heapNode;

    bool color;
    int DriveNumber, DriveCost, DriveDuration;

    RBTNode() {
        leftChild = rightChild = parent = nullptr;
        heapNode = nullptr;
        color = BLACK;
        DriveNumber = DriveCost = DriveDuration = 0;
    }

    RBTNode(int dn, int dc, int dd, bool col = RED) {
        leftChild = rightChild = parent = nullptr;
        heapNode = nullptr;
        color = col;
        DriveNumber = dn;
        DriveCost = dc;
        DriveDuration = dd;
    }
};

class RBT {
public:
    RBTNode* root;
    RBTNode* NIL;
    int treeSize;

    RBT() {
        NIL = new RBTNode();
        NIL->color = BLACK;
        NIL->leftChild = NIL->rightChild = NIL->parent = NIL;
        root = NIL;
        treeSize = 0;
    }

    bool isExternalNode(RBTNode* n) {
        return n == NIL;
    }

    RBTNode* findNode(int key, RBTNode* node) {
        (void)node; // we always search from root in this implementation
        RBTNode* cur = root;
        while (cur != NIL) {
            if (key == cur->DriveNumber) return cur;
            cur = (key < cur->DriveNumber) ? cur->leftChild : cur->rightChild;
        }
        return nullptr;
    }

    void leftRotate(RBTNode* x) {
        RBTNode* y = x->rightChild;
        x->rightChild = y->leftChild;
        if (y->leftChild != NIL) y->leftChild->parent = x;

        y->parent = x->parent;
        if (x->parent == NIL) root = y;
        else if (x == x->parent->leftChild) x->parent->leftChild = y;
        else x->parent->rightChild = y;

        y->leftChild = x;
        x->parent = y;
    }

    void rightRotate(RBTNode* y) {
        RBTNode* x = y->leftChild;
        y->leftChild = x->rightChild;
        if (x->rightChild != NIL) x->rightChild->parent = y;

        x->parent = y->parent;
        if (y->parent == NIL) root = x;
        else if (y == y->parent->rightChild) y->parent->rightChild = x;
        else y->parent->leftChild = x;

        x->rightChild = y;
        y->parent = x;
    }

    void insert(RBTNode* z) {
        // attach NIL children
        z->leftChild = z->rightChild = NIL;

        RBTNode* y = NIL;
        RBTNode* x = root;

        while (x != NIL) {
            y = x;
            if (z->DriveNumber < x->DriveNumber) x = x->leftChild;
            else x = x->rightChild;
        }

        z->parent = y;
        if (y == NIL) root = z;
        else if (z->DriveNumber < y->DriveNumber) y->leftChild = z;
        else y->rightChild = z;

        z->color = RED;
        insertFixup(z);
        treeSize++;
    }

    void insertFixup(RBTNode* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->leftChild) {
                RBTNode* y = z->parent->parent->rightChild; // uncle
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->rightChild) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                RBTNode* y = z->parent->parent->leftChild; // uncle
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->leftChild) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
        root->parent = NIL;
    }

    RBTNode* minimum(RBTNode* x) {
        while (x->leftChild != NIL) x = x->leftChild;
        return x;
    }

    void transplant(RBTNode* u, RBTNode* v) {
        if (u->parent == NIL) root = v;
        else if (u == u->parent->leftChild) u->parent->leftChild = v;
        else u->parent->rightChild = v;
        v->parent = u->parent;
    }

    RBTNode* deleteNode(int key) {
        RBTNode* z = findNode(key, root);
        if (!z) return nullptr;

        RBTNode* y = z;
        bool yOriginalColor = y->color;
        RBTNode* x = NIL;

        if (z->leftChild == NIL) {
            x = z->rightChild;
            transplant(z, z->rightChild);
        } else if (z->rightChild == NIL) {
            x = z->leftChild;
            transplant(z, z->leftChild);
        } else {
            y = minimum(z->rightChild);
            yOriginalColor = y->color;
            x = y->rightChild;

            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->rightChild);
                y->rightChild = z->rightChild;
                y->rightChild->parent = y;
            }

            transplant(z, y);
            y->leftChild = z->leftChild;
            y->leftChild->parent = y;
            y->color = z->color;

            // Keep heap cross-link correct:
            // if we moved y into z's position, y must carry z's heapNode relationship
            // BUT in this project, heap nodes store drive data separately.
            // The simplest consistent approach: keep the node objects (RBTNode*) stable per drive.
            // Therefore we do NOT "copy" z into y; we physically moved y.
            // So after delete, the drive "y" remains the same drive number as before.
            //
            // If you need the exact key removed to match input semantics, you should instead do key-copy delete.
            // Here we are doing standard RB delete (physical move). It's correct BST-wise and remove-by-key works.
        }

        if (yOriginalColor == BLACK) deleteFixup(x);

        // Detach z (so caller can safely use z->heapNode)
        z->leftChild = z->rightChild = z->parent = NIL;
        treeSize--;
        return z;
    }

    void deleteFixup(RBTNode* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->leftChild) {
                RBTNode* w = x->parent->rightChild;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->rightChild;
                }
                if (w->leftChild->color == BLACK && w->rightChild->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->rightChild->color == BLACK) {
                        w->leftChild->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->rightChild;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->rightChild->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                RBTNode* w = x->parent->leftChild;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->leftChild;
                }
                if (w->rightChild->color == BLACK && w->leftChild->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->leftChild->color == BLACK) {
                        w->rightChild->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->leftChild;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->leftChild->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
        root->parent = NIL;
    }

    // Correct range traversal
    void rangeInorder(RBTNode* node, int r1, int r2, string& out) {
        if (node == NIL) return;

        if (node->DriveNumber > r1) rangeInorder(node->leftChild, r1, r2, out);

        if (node->DriveNumber >= r1 && node->DriveNumber <= r2) {
            out += "(" + to_string(node->DriveNumber) + "," +
                   to_string(node->DriveCost) + "," +
                   to_string(node->DriveDuration) + "),";
        }

        if (node->DriveNumber < r2) rangeInorder(node->rightChild, r1, r2, out);
    }

    string range(int r1, int r2) {
        string out;
        rangeInorder(root, r1, r2, out);
        if (!out.empty()) out.pop_back(); // remove trailing comma
        out += "\n";
        return out;
    }

    void printTree() {
        if (root == NIL) {
            cout << "Empty tree\n";
            return;
        }
        queue<RBTNode*> q;
        q.push(root);

        while (!q.empty()) {
            int sz = (int)q.size();
            while (sz--) {
                RBTNode* n = q.front();
                q.pop();
                char c = n->color == BLACK ? 'b' : 'r';
                cout << c << "-" << n->DriveNumber << "\t";
                if (n->leftChild != NIL) q.push(n->leftChild);
                if (n->rightChild != NIL) q.push(n->rightChild);
            }
            cout << "\n";
        }
    }
};
