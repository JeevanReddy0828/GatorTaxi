#pragma once

#include <string>
#include <iostream>

using namespace std;

class RBT;
class Heap;

// prototypes
void Print(RBT* rbt, Heap* heap, int* args, string& output);
void PrintRange(RBT* rbt, Heap* heap, int* args, string& output);
void InsertRide(RBT* rbt, Heap* heap, int* args, string& output);
void GetNextRide(RBT* rbt, Heap* heap, string& output);
void CancelRide(RBT* rbt, Heap* heap, int* args, string& output);
void UpdateTrip(RBT* rbt, Heap* heap, int* args, string& output);

static inline void processArgs3(int* args, const string& line) {
    int index = (int)line.find_first_of("(") + 1;
    int len = (int)line.size() - index;
    string argStr = line.substr(index, len - 1);

    int firstComma = (int)argStr.find_first_of(",");
    args[0] = stoi(argStr.substr(0, firstComma));

    int secondComma = (int)argStr.find_last_of(",");
    if (secondComma == (int)string::npos) return;

    args[1] = stoi(argStr.substr(firstComma + 1, secondComma - firstComma - 1));
    args[2] = stoi(argStr.substr(secondComma + 1));
}

static inline void processArgs2(int* args, const string& line) {
    int index = (int)line.find_first_of("(") + 1;
    int len = (int)line.size() - index;
    string argStr = line.substr(index, len - 1);

    int firstComma = (int)argStr.find_first_of(",");
    args[0] = stoi(argStr.substr(0, firstComma));
    args[1] = stoi(argStr.substr(firstComma + 1));
}

static inline void processArgs1(int* args, const string& line) {
    int index = (int)line.find_first_of("(") + 1;
    int len = (int)line.size() - index;
    args[0] = stoi(line.substr(index, len - 1));
}

static inline void processOperation(RBT* rbt, Heap* heap, const string& line, const string& operationPrefix, string& output) {
    int args[3] = {0, 0, 0};

    if (operationPrefix == "Pri") {
        if (line.find(",") != string::npos) {
            processArgs2(args, line);
            PrintRange(rbt, heap, args, output);
        } else {
            processArgs1(args, line);
            Print(rbt, heap, args, output);
        }
    } else if (operationPrefix == "Ins") {
        processArgs3(args, line);
        InsertRide(rbt, heap, args, output);
    } else if (operationPrefix == "Upd") {
        processArgs2(args, line);
        UpdateTrip(rbt, heap, args, output);
    } else if (operationPrefix == "Can") {
        processArgs1(args, line);
        CancelRide(rbt, heap, args, output);
    } else if (operationPrefix == "Get") {
        GetNextRide(rbt, heap, output);
    }
}
