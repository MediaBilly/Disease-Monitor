#ifndef AVLTREE_H
#define AVLTREE_H

#include "patientRecord.h"

typedef struct avltree *AvlTree;

int AvlTree_Create(AvlTree*);
int AvlTree_Insert(AvlTree,patientRecord);
unsigned int AvlTree_NumRecords(AvlTree);
int AvlTree_Destroy(AvlTree*);

#endif