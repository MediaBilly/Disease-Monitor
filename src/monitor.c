#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../headers/monitor.h"
#include "../headers/utilities.h"
#include "../headers/patientRecord.h"
#include "../headers/list.h"
#include "../headers/hashtable.h"

struct disease_monitor {
  unsigned int totalRecords;
  List recordsList;
  HashTable recordsHashTable;
};

int DiseaseMonitor_Init(DiseaseMonitor *monitor,FILE *patientRecordsFile,unsigned int diseaseHashtableNumOfEntries,unsigned int countryHashtableNumOfEntries,unsigned int bucketSize) {
  // Allocate space for DiseaseMonitor
  if ((*monitor = (DiseaseMonitor)malloc(sizeof(struct disease_monitor))) == NULL) {
    not_enough_memory();
    *monitor = NULL;
    return FALSE;
  }
  // Initialize data structures
  if (!List_Initialize(&((*monitor)->recordsList))) {
    return FALSE;
  }
  if (!HashTable_Create(&((*monitor)->recordsHashTable),diseaseHashtableNumOfEntries + countryHashtableNumOfEntries,bucketSize)) {
    return FALSE;
  }
  // Read file records
  string line = NULL;
  size_t len = 0;
  patientRecord record;
  (*monitor)->totalRecords = 0;
  while (getline(&line,&len,patientRecordsFile) != -1) {
    //printf("%ld %s %ld %d\n",len,line,strlen(line),wordCount(line));
    if ((record = PatientRecord_Create(line)) != NULL) {
      // Check if record with the same id already exists and exit the program if so
      if (HashTable_SearchKey((*monitor)->recordsHashTable,PatientRecord_Get_recordID(record)) == NULL) {
        // Insert record to the data structures(list and binary tree hash tables)
        List_Insert((*monitor)->recordsList,record);
        HashTable_Insert((*monitor)->recordsHashTable,PatientRecord_Get_recordID(record),record);
        (*monitor)->totalRecords++;
        //printf("%s\n",PatientRecord_Get_recordID(record));
      } else {
        printf("Record with id %s was found twice in patientRecordsFile.Exiting...\n",PatientRecord_Get_recordID(record));
        PatientRecord_Destroy(&record);
        free(line);
        return FALSE;
      }
    }
  }
  free(line);
  // For test purposes only. Will be removed later
  return TRUE;
}

int DiseaseMonitor_Run(DiseaseMonitor monitor) {
  int running = TRUE,lastword;
  string argument;
  // Execute commands until /exit is given
  while (running) {
    // Read command name
    argument = readNextWord(&lastword);
    // Determine command type
    if (!strcmp("/globalDiseaseStats",argument)) {
      // TODO
    } else if (!strcmp("/diseaseFrequency",argument)) {
      // TODO
    } else if (!strcmp("/topk-Diseases",argument)) {
      // TODO
    } else if (!strcmp("/topk-Countries",argument)) {
      // TODO
    } else if (!strcmp("/insertPatientRecord",argument)) {
      // TODO
    } else if (!strcmp("/recordPatientExit",argument)) {
      // TODO
    } else if (!strcmp("/numCurrentPatients",argument)) {
      // TODO
    } else if (!strcmp("/exit",argument)) {
      running = FALSE;
      DestroyString(&argument);
    } else {
      printf("Command %s not found.\n",argument);
    }
  }
  return TRUE;
}

int DiseaseMonitror_Destroy(DiseaseMonitor *monitor) {
  if (*monitor != NULL) {
    // Destroy data structures
    // Destroy function typecast:(int (*)(void**))func_name 
    HashTable_Destroy(&((*monitor)->recordsHashTable),NULL);
    List_Destroy(&((*monitor)->recordsList));
    // Destroy monitor itself
    free(*monitor);
    *monitor = NULL;
    return TRUE;
  } else {
    return FALSE;
  }
}