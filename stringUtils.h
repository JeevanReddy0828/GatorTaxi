#include<string>
#include<iostream>

// prototypes functions
void print(RBT *rbt, Heap *heap, int *args, string &output);
void searchinRange(RBT *rbt, Heap *heap, int *args, string &output);
void inputDrive(RBT *rbt, Heap *heap, int* args, string &output);
void getNewDrive(RBT *rbt, Heap *heap, string &output);
void terminateDrive(RBT *rbt, Heap *heap, int* args, string &output);
void upgradeDrive(RBT *rbt, Heap *heap, int* args, string &output);
void analyzeInput(RBT *rbt, Heap *heap, int argc, char **argv, string &output);

// It just parse single arguments like print and cancel of drive
void processArgs3(int *args, string line) {
    int index = line.find_first_of("(") + 1, len = line.size() - index;
    string argStr = line.substr(index, len - 1);

    int firstComma = argStr.find_first_of(",");
    args[0] = stoi(argStr.substr(0, firstComma));

    int secondComma = argStr.find_last_of(",");
    if(secondComma == string::npos) return;
    args[1] = stoi(argStr.substr(firstComma + 1, secondComma - firstComma));
    args[2] = stoi(argStr.substr(secondComma + 1));
}
//it just analyzes the arguments to upgrade command 
void processArgs2(int *args, string line) {
    int index = line.find_first_of("(") + 1, len = line.size() - index;
    string argStr = line.substr(index, len - 1);

    int firstComma = argStr.find_first_of(",");
    args[0] = stoi(argStr.substr(0, firstComma));
    args[1] = stoi(argStr.substr(firstComma + 1));    
}
//it just analyzes the arguments to insert values
void processArgs1(int *args, string line) {
    int index = line.find_first_of("(") + 1, len = line.size() - index;
    args[0] = stoi(line.substr(index, len - 1)); 
}

// parses input to call necessary operation routines and parse arguments.
void processOperation(RBT *rbt, Heap *heap, string line, string operationPrefix, string &output) {
    int args[3];
    if(operationPrefix == "Pri") {
        if(line.find(",") != string::npos) {
            processArgs2(args, line);
            printRange(rbt, heap, args, output);
        } else {
            processArgs1(args, line);
            print(rbt, heap, args, output);
        }
    } else if(operationPrefix == "Ins") {
        processArgs3(args, line);
        insertRide(rbt, heap, args, output);
    } else if(operationPrefix == "Upd") {
        processArgs2(args, line);
        updateTrip(rbt, heap, args, output);
    } else if(operationPrefix == "Can") {
        processArgs1(args, line);
        cancelRide(rbt, heap, args, output);
    } else if(operationPrefix == "Get") {
        getNextRide(rbt, heap, output);
    }
}