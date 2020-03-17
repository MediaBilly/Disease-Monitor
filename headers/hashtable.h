#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "utilities.h"

typedef struct hashtable *HashTable;

int HashTable_Create(HashTable*,unsigned int,unsigned int);
int HashTable_Insert(HashTable,string,void*);
void* HashTable_SearchKey(HashTable,string);
int HashTable_Destroy(HashTable*,int (*DestroyValueStruct)(void**));

#endif