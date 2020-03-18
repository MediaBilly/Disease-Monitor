#ifndef LIST_H
#define LIST_H

#include "utilities.h"
#include "patientRecord.h"

typedef struct list *List;

int List_Initialize(List*);
int List_Insert(List,patientRecord);
int List_Destroy(List*);

#endif