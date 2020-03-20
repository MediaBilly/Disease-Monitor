#ifndef MAXHEAP_H
#define MAXHEAP_H

#include "utilities.h"

// Datatype pointer definitions
typedef struct maxheap *MaxHeap;
typedef struct heapdata *HeapData;

// Heap Functions
int MaxHeap_Create(MaxHeap*);
int MaxHeap_Insert(MaxHeap,string,unsigned int);
HeapData MaxHeap_ExtractMax(MaxHeap);
int MaxHeap_Destroy(MaxHeap*);

// Data functions
string MaxHeapData_GetKey(HeapData);
unsigned int MaxHeapData_GetValue(HeapData);
int MaxHeapData_Destroy(HeapData*);

#endif