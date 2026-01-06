# GatorTaxi – Ride Dispatch System

## Overview
**GatorTaxi** is a ride dispatch simulation system that efficiently manages ride requests using a combination of:

- **Min Heap** → selects the next ride to serve based on priority  
- **Red-Black Tree (RBT)** → supports fast search, deletion, and range queries by `RideNumber`

The system processes commands from an input file and produces an output file following the given specification.

---

## Priority Rules
Rides are prioritized using the following rules:

1. **Lower RideCost** has higher priority  
2. If RideCost is equal, **lower TripDuration** has higher priority  

This ordering is enforced consistently inside the Min Heap.

---

## Supported Operations

| Command | Description |
|------|------------|
| `Insert(rideNumber, rideCost, tripDuration)` | Inserts a new ride |
| `GetNextRide()` | Returns and removes the highest-priority ride |
| `CancelRide(rideNumber)` | Cancels an existing ride |
| `UpdateTrip(rideNumber, newDuration)` | Updates a ride based on duration rules |
| `Print(rideNumber)` | Prints details of a specific ride |
| `Print(low, high)` | Prints rides in the given ride number range |

---

## UpdateTrip Rules
Given `existingDuration` and `newDuration`:

- `newDuration <= existingDuration`  
  → Update duration only
- `existingDuration < newDuration <= 2 × existingDuration`  
  → Cancel ride and reinsert with **cost + 10**
- `newDuration > 2 × existingDuration`  
  → Cancel ride

---

## Data Structures Used

### Min Heap
- Stores active rides ordered by `(RideCost, TripDuration)`
- Supports insert, delete, update, and extract-min operations
- Each `HeapNode` stores its index for **O(log n)** updates

### Red-Black Tree
- Stores rides ordered by `RideNumber`
- Guarantees **O(log n)** operations
- Supports fast range queries for `Print` commands

### Cross-Linking
Each ride is represented by:
- One `HeapNode`
- One `RBTNode`

These nodes maintain pointers to each other to keep both data structures consistent.

---

## File Structure
```
.
├── gatorTaxi.cpp
├── MyHeap.h
├── MyRBT.h
├── stringUtils.h
├── Makefile
├── Input.txt
├── output_file.txt
└── README.md
```

---

## Build Instructions
Ensure `g++` is installed.

```bash
make
```

---

## Run Instructions
```bash
./gatorTaxi Input.txt
```

The output is written to:
```
output_file.txt
```

---

## Design Highlights
- Uses complementary data structures for optimal performance
- All major operations run in **O(log n)** time
- Clean separation of parsing, heap, and tree logic
- Deterministic behavior through strict priority rules

---

## Notes
- Duplicate ride numbers are rejected
- Input is assumed to follow the given specification
- Designed for correctness, efficiency, and clarity
