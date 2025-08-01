#include "MyHeap.h"
#include <queue>
#include<bits/stdc++.h>
// #include<iostream>
// #include<string>

#define RED 0
#define BLACK 1

#define L 0
#define R 1

class RBTNode {
public :
    RBTNode *leftChild, *rightChild, *parent;
    HeapNode *heapNode;
    bool color;
    int DriveNumber, DriveCost, DriveDuration;

    // Constructor
    RBTNode() {
        this -> leftChild = nullptr;
        this -> rightChild = nullptr;
        this -> parent = nullptr;
        this -> heapNode = nullptr;
        this -> color = BLACK;
        this -> DriveNumber = 0;
        this -> DriveCost = 0;
        this -> DriveDuration = 0;
    }

    // Parameterised constructor
    RBTNode(RBTNode *leftChild, RBTNode *rightChild, RBTNode *parent, HeapNode* heapNode,
    bool color, int DriveNumber, int DriveCost, int DriveDuration) {
        this -> leftChild = leftChild != nullptr ?
            leftChild : new RBTNode();
        this -> rightChild = rightChild != nullptr ?
            rightChild : new RBTNode();
        this -> parent = parent;
        this -> heapNode = heapNode;
        this -> color = color;
        this -> DriveNumber = DriveNumber;
        this -> DriveCost = DriveCost;
        this -> DriveDuration = DriveDuration;
    }

    RBTNode(RBTNode *node) {
        this -> leftChild = node -> leftChild != nullptr ?
            node -> leftChild : new RBTNode();
        this -> rightChild = node -> rightChild != nullptr ?
            node -> rightChild : new RBTNode();
        this -> parent = node -> parent;
        this -> heapNode = node -> heapNode;
        this -> color = node -> color;
        this -> DriveNumber = node -> DriveNumber;
        this -> DriveCost = node -> DriveCost;
        this -> DriveDuration = node -> DriveDuration;
    }

    void copyHeapNodeExceptData(RBTNode *node) {
        this -> leftChild = node -> leftChild;
        this -> rightChild = node -> rightChild;
        this -> parent = node -> parent;
        this -> heapNode = node -> heapNode;
        this -> color = node -> color;
    }
};

class RBT {
public:
    RBTNode *root;
    int treeSize;

    RBT() {
        root = nullptr;
        treeSize = 0;
    }

    RBT(RBTNode *rbtNode) {
        root = rbtNode;
        treeSize = 1;
    }

    int getTreeSize(RBTNode *root) {
        if(!isExternalNode(root)) 
            return getTreeSize(root -> leftChild) + getTreeSize(root -> rightChild) + 1;
        return 0;
    }

    bool isExternalNode(RBTNode* node) {
        return node -> DriveNumber == 0 && node -> DriveDuration == 0 && node -> DriveCost == 0;
    }

    // Search a node in the tree
    RBTNode* findNode(int key, RBTNode* node) {
        if(!node || isExternalNode(node)) return nullptr;

        int currentDriveNumber = node -> DriveNumber;
        if(key == currentDriveNumber) return node;

        if(key > currentDriveNumber) return findNode(key, node -> rightChild);
        return findNode(key, node -> leftChild);
    }

    // Get the parent of a node
    RBTNode* findParent(int key, RBTNode* node) {
        if(!node) return node;
        if(key > (node -> DriveNumber)) {
            if(isExternalNode(node -> rightChild)) return node;
            node = node -> rightChild;
        } else {
            if(isExternalNode(node -> leftChild)) return node;
            node = node -> leftChild;
        }
        return findParent(key, node);
    }

    void insert(RBTNode *p) {
        int rn = p -> DriveNumber;
        treeSize++;

        RBTNode *pp = findParent(rn, root);

        if(pp == nullptr) {
            root = p;
            p -> color = BLACK;
            return;
        } 

        p -> parent = pp;
        if(rn < pp -> DriveNumber) pp -> leftChild = p;
        else pp -> rightChild = p;

        adjustRBT(p);
    }

    RBTNode *findCommonAncestor(int r1, int r2, RBTNode *root) {
        if(isExternalNode(root)) return root;
        if(root -> DriveNumber < r1) return findCommonAncestor(r1, r2, root -> rightChild);
        if(root -> DriveNumber > r2) return findCommonAncestor(r1, r2, root -> leftChild);
        return root;
    }

    // Search in a range and return a string formatted answer.
    string range(int r1, int r2) {
        RBTNode *n1 = findNode(r1, root), *n2 = findNode(r2, root);
        RBTNode *ancestor = findCommonAncestor(r1, r2, root);

        string output;
        if(!isExternalNode(ancestor))
            printInorder(r1, r2, output, ancestor);
        else output += "(" + to_string(ancestor -> DriveNumber) + "," + to_string(ancestor -> DriveCost) + ","
            + to_string(ancestor -> DriveDuration) + ")," ;

        return output.substr(0, output.length() - 1) + "\n";
    }

    // Inorder traversal for viewing the tree nodes.
    void printInorder(int r1, int r2, string &output, RBTNode* node) {
        int rn = node -> DriveNumber;
        if(rn < r1 || rn > r2 || isExternalNode(node)) return;

        printInorder(r1, r2, output, node -> leftChild);
        output += "(" + to_string(node -> DriveNumber) + "," + to_string(node -> DriveCost) + ","
            + to_string(node -> DriveDuration) + "),";
        printInorder(r1, r2, output, node -> rightChild);
    }

    int getChildType(RBTNode* pp, RBTNode* p) {
        if(pp -> rightChild == p) return R;
        return L;
    }

    RBTNode* getMaxNode(RBTNode *p, RBTNode *currMax) {
        if(isExternalNode(p)) return currMax;
        else return getMaxNode(p -> rightChild, p);
    }

    // Delete a node from the tree
    RBTNode* deleteNode(int DriveNumber) {
        RBTNode *p = findNode(DriveNumber, root);
        if(!p) return p;

        RBTNode *pp = p -> parent, *lc = p -> leftChild, *rc = p -> rightChild, *y;
        int pColor = p -> color;
        int noOfChildren = isExternalNode(lc) && isExternalNode(rc) ? 0 : 
                            !isExternalNode(lc) && !isExternalNode(rc) ? 2 : 1;

        if(noOfChildren == 0) {
            RBTNode *extNode = p -> rightChild;
            if(p == root) {
                root = nullptr;
                return p;
            } else {
                if(getChildType(pp, p) == R) pp -> rightChild = extNode;
                else pp -> leftChild = extNode;
                y = extNode;
            }
        } else if(noOfChildren == 1) {
            RBTNode *lc = p -> leftChild, *rc = p -> rightChild;
            if(p == root) {
                root = isExternalNode(lc) ? rc : lc;
            } else {
                if(!isExternalNode(rc) && getChildType(pp, p) == R) {
                    pp -> rightChild = rc;
                } else if(!isExternalNode(lc) && getChildType(pp, p) == R) {
                    pp -> rightChild = lc;
                } else if(!isExternalNode(rc)) pp -> leftChild = rc;
                else pp -> leftChild = rc;
            }
            y = isExternalNode(lc) ? rc : lc;
        } else {
            RBTNode *LSTMaxNode = getMaxNode(p -> leftChild, p);
            if(LSTMaxNode != p -> leftChild) {
                LSTMaxNode -> parent -> rightChild = LSTMaxNode -> leftChild;
                y = LSTMaxNode -> parent -> rightChild;
            } else {
                LSTMaxNode -> parent -> leftChild = LSTMaxNode -> leftChild;
                y = LSTMaxNode -> parent -> leftChild;
            }
            
            pp = LSTMaxNode -> parent != p ? LSTMaxNode -> parent : LSTMaxNode;
            // LSTMaxNode -> copyHeapNodeExceptData(p);
            if(p == root) root = LSTMaxNode;
        }

        // cout << y -> DriveNumber << " " << p -> DriveNumber << " " << pp -> DriveNumber << endl;

        if(y == root || y -> color == RED) {
            y -> color = BLACK;
            return p;
        }
        y -> parent = pp;
        
        if(pColor == BLACK) {
            adjustRBTAfterDelete(y, pp);
        }
        treeSize--;
        return p;
    }

    // Readjust colours of the tree nodes after deleting
    void adjustRBTAfterDelete(RBTNode *y, RBTNode* py) {
        if(y == root) {
            y -> color = BLACK;
            return;
        }

        RBTNode *v = (getChildType(py, y) == L) ? py -> rightChild : py -> leftChild;
        if(isExternalNode(v)) {
            if(!isExternalNode(y))
                y -> color = RED;
            return;
        }

        RBTNode *a = v -> leftChild, *b = v -> rightChild;
        int X = getChildType(py, y), c = v -> color, 
        n = a -> color == RED ? (b -> color == RED ? 2 : 1) : b -> color == RED ? 1 : 0;

        //------------------------------------ Different rotations -----------------------------------------------------------
        if(X == R && c == BLACK && n == 0) {
            cout << "RB0 case" << endl;
            RB0(y, py);
        } else if(X == R && c == BLACK && n == 1) {
            RB1Cases(y, py, a);
        } else if(X == R && c == BLACK && n == 2) {
            RB2(y, py);
        } else if(X == R && c == RED) {
            RRCases(py);
        }

        if(X == L && c == BLACK && n == 0) {
            cout << "LB0 case" << endl;
            LB0(y, py);
        } else if(X == L && c == BLACK && n == 1) {
            LB1Cases(y, py, b);
        } else if(X == L && c == BLACK && n == 2) {
            LB2(y, py);
        } else if(X == L && c == RED) {
            LRCases(py);
        }
    }

    void RB0(RBTNode *y, RBTNode *py) {
        if(y == root) {
            y -> color = BLACK;
            return;
        }

        RBTNode *v = py -> leftChild;

        if(py -> color == BLACK) {
            v -> color = RED;
        } else {
            py -> color = BLACK;
            v -> color = RED;
            return;
        }
        adjustRBTAfterDelete(py, py -> parent);
    }

    void RB1Cases(RBTNode *y, RBTNode *py, RBTNode *a) {
        if(a == RED) {
            cout << "RB11 case" << endl;
            RB11(y, py);
        } else {
            cout << "RB12 case" << endl;
            RB12(y, py);
        }
    }

    void RB11(RBTNode *y, RBTNode *py) {
        RBTNode *v = py -> leftChild, *a = v -> leftChild;
        a -> color = BLACK;
        v -> color = py -> color ;
        py -> color = BLACK;
        LLRotation(py);
    }

    void RB12(RBTNode *y, RBTNode *py) {
        RBTNode *v = py -> leftChild, *w = v -> rightChild;
        w -> color = py -> color ;
        py -> color = BLACK;
        LRRotation(v, py);
    }

    void RB2(RBTNode *y, RBTNode *py) {
        RBTNode* w = py -> leftChild -> rightChild;
        w -> color = py -> color;
        py -> color = BLACK;
        LRRotation(py -> leftChild, py);
    }

    void RRCases(RBTNode *py) {
        RBTNode *v = py -> leftChild, *w = v -> rightChild;
        RBTNode *b = w -> leftChild, *c = w -> rightChild;
        int n = c -> color == RED ? (b -> color == RED ? 2 : 1) : b -> color == RED ? 1 : 0;

        if(n == 0) {
            RR0(py, v, w);
        } else if(n == 1) {
            if(b -> color == RED) RR11(v, py, b);
            else RR12(py, v, w, c);
        } else {
            RR2(py, v, w, c);
        }
    }

    void RR0(RBTNode *py, RBTNode *v, RBTNode *b) {
        LLRotation(py);
        v -> color = BLACK;
        b -> color = RED;
    }

    void RR11(RBTNode *y, RBTNode *py, RBTNode *b) {
        LRRotation(y, py);
        b -> color = BLACK;
    }

    void RR12(RBTNode *py, RBTNode *v, RBTNode *w, RBTNode *x) {
        RRRotation(w);
        LRRotation(v, py);
        x -> color = BLACK;
    }

    void RR2(RBTNode *py, RBTNode *v, RBTNode *w, RBTNode *x) {
        RRRotation(w);
        LRRotation(v, py);
        x -> color = BLACK;
    }

    void LB0(RBTNode *y, RBTNode *py) {
        if(y == root) {
            y -> color = BLACK;
            return;
        }
        RBTNode *v = py -> rightChild;

        if(py -> color == BLACK) {
            v -> color = RED;
        } else {
            py -> color = BLACK;
            v -> color = RED;
            return;
        }
        adjustRBTAfterDelete(py, py -> parent);
    }

    void LB1Cases(RBTNode *y, RBTNode *py, RBTNode *b) {
        if(b == RED) {
            cout << "LB11 case" << endl;
            LB11(y, py);
        } else {
            cout << "LB12 case" << endl;
            LB12(y, py);
        }
    }

    void LB11(RBTNode *y, RBTNode *py) {
        RBTNode *v = py -> rightChild, *b = v -> rightChild;
        b -> color = BLACK;
        v -> color = py -> color ;
        py -> color = BLACK;
        RRRotation(py);
    }

    void LB12(RBTNode *y, RBTNode *py) {
        RBTNode *v = py -> rightChild, *a = v -> leftChild;
        a -> color = py -> color ;
        py -> color = BLACK;
        RLRotation(v, py);
    }

    void LB2(RBTNode *y, RBTNode *py) {
        RBTNode* a = py -> rightChild -> leftChild;
        a -> color = py -> color;
        py -> color = BLACK;
        LRRotation(py -> rightChild, py);
    }

    void LRCases(RBTNode *py) {
        RBTNode *v = py -> rightChild, *a = v -> leftChild;
        RBTNode *b = a -> leftChild, *c = a -> rightChild;
        int n = c -> color == RED ? (b -> color == RED ? 2 : 1) : b -> color == RED ? 1 : 0;

        if(n == 0) {
            LR0(py, v, a);
        } else if(n == 1) {
            if(c -> color == RED) LR11(v, py, c);
            else LR12(py, v, a, b);
        } else {
            LR2(py, v, a, b);
        }
    }

    void LR0(RBTNode *py, RBTNode *v, RBTNode *a) {
        RRRotation(py);
        v -> color = BLACK;
        a -> color = RED;
    }

    void LR11(RBTNode *v, RBTNode *py, RBTNode *c) {
        RLRotation(v, py);
        c -> color = BLACK;
    }

    void LR12(RBTNode *py, RBTNode *v, RBTNode *w, RBTNode *b) {
        LLRotation(w);
        RLRotation(v, py);
        b -> color = BLACK;
    }

    void LR2(RBTNode *py, RBTNode *v, RBTNode *w, RBTNode *b) {
        LLRotation(w);
        RLRotation(v, py);
        b -> color = BLACK;
    }

    // ------------------------------------------ END OF ALL ROTATION CASES ---------------------------------------------------
    void adjustRBT(RBTNode* p) {
        if(!p || !p -> parent || !p -> parent -> parent) return;

        RBTNode *pp = p -> parent, *gp = pp -> parent;
        if(pp -> color == BLACK) return;

        int X, Y, r;

        X = (gp -> leftChild == pp) ? L : R;
        Y = (pp -> leftChild == p) ? L : R;
        r = (X == L) ? gp -> rightChild -> color : gp -> leftChild -> color;

        if(r == RED) {
            XYr(p);
            return;
        }

        if(X == L && Y == L) {
            LLRotation(gp);
            pp -> color = BLACK;
            gp -> color = RED;  
        } else if(X == R && Y == R) {
            RRRotation(gp);
            pp -> color = BLACK;
            gp -> color = RED;  
        } else if(X == L && Y == R) {
            LRRotation(pp, gp);
            p -> color = BLACK;
            gp -> color = RED;  
        } else {
            RLRotation(pp, gp);
            p -> color = BLACK;
            gp -> color = RED; 
        }
        
    }

    void XYr(RBTNode* p) {
        if(p == root) {
            p -> color = BLACK;
            return;
        }

        RBTNode *pp = p -> parent;
        if(p -> color == BLACK || pp -> color == BLACK) return;
        
        int X = (pp -> parent -> leftChild == pp) ? L : R;
        RBTNode *gp = pp -> parent, *d = (X == L) ? 
            gp -> rightChild : gp -> leftChild;

        d -> color = BLACK;
        pp -> color = BLACK;
        gp -> color = RED;

        if(gp == root) gp -> color = BLACK;
        adjustRBT(gp);
    }

    void LLRotation(RBTNode* gp) {
        RBTNode *ggp = gp -> parent, *pp = gp -> leftChild, *p = pp -> leftChild;
        RBTNode *c = pp -> rightChild;
        
        pp -> rightChild = gp;
        gp -> leftChild = c;

        pp -> parent = ggp;
        gp -> parent = pp;

        if(!ggp) {
            root = pp;
            return;
        }

        if(ggp -> rightChild == gp) ggp -> rightChild = pp;
        else ggp -> leftChild = pp;    

        pp -> parent = ggp;   
    }

    void RRRotation(RBTNode* gp) {
        RBTNode *ggp = gp -> parent, *pp = gp -> rightChild, *p = pp -> rightChild;
        RBTNode *c = pp -> leftChild, *b = p -> leftChild;
        
        pp -> leftChild = gp;
        gp -> rightChild = c;

        pp -> parent = ggp;
        gp -> parent = pp;

        if(!ggp) {
            root = pp;
            return;
        }

        if(ggp -> leftChild == gp) ggp -> leftChild = pp;
        else ggp -> rightChild = pp; 

        pp -> parent = ggp;
    }

    void LRRotation(RBTNode* pp, RBTNode *gp) {
        RRRotation(pp);
        LLRotation(gp);
    }

    void RLRotation(RBTNode* pp, RBTNode *gp) {
        LLRotation(pp);
        RRRotation(gp);
    }

    // Print tree for debugging purpose.
    void printTree() {
        queue<RBTNode*> q;
        q.push(root);

        while(!q.empty()) {
            int size = q.size();
            while(size) {
                RBTNode *node = q.front();
                q.pop();
                if(!node) {
                    cout << "Empty tree" << endl;
                    return;
                }
                char color = node -> color ? 'b' : 'r';

                if(node -> DriveNumber > 0) 
                    cout << color << "-" << node -> DriveNumber << "\t";
                if(node -> leftChild) q.push(node -> leftChild);
                if(node -> rightChild) q.push(node -> rightChild);
                size--;
            }
            cout << endl;
        }
    }
};