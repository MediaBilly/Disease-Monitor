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
  // Parameter checking
  if (bucketSize < sizeof(string) + 2*sizeof(void*)) {
    printf("Minimum bucket size required:%ld bytes\n",sizeof(string) + 2*sizeof(void*));
    *monitor = NULL;
    return FALSE;
  }
  if (patientRecordsFile == NULL) {
    printf("patientRecordsFile is not opened.\n");
    *monitor = NULL;
    return FALSE;
  }
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
  string command;
  // Execute commands until /exit is given
  while (running) {
    // Read command name
    putchar('>');
    command = readNextWord(&lastword);
    // Determine command type
    if (!strcmp("/globalDiseaseStats",command)) {
      // TODO
    } else if (!strcmp("/diseaseFrequency",command)) {
      // TODO
    } else if (!strcmp("/topk-Diseases",command)) {
      // TODO
    } else if (!strcmp("/topk-Countries",command)) {
      // TODO
    } else if (!strcmp("/insertPatientRecord",command)) {
      // TODO
    } else if (!strcmp("/recordPatientExit",command)) {
      // Usage check
      if (!lastword) {
        // Read recordID
        string recordID = readNextWord(&lastword);
        // Usage check
        if (!lastword) {
          string exitDate = readNextWord(&lastword);
          // Usage check
          if (lastword) {
            // Check if patient with that name exists
            patientRecord record;
            if ((record = (patientRecord)HashTable_SearchKey(monitor->recordsHashTable,recordID)) != NULL) {
              PatientRecord_Exit(record,exitDate);
            } else {
              printf("Patient with ID %s does not exist.\n",recordID);
            }
          } else {
            printf("Usage:/recordPatientExit recordID exitDate\n");
            IgnoreRemainingInput();
          }
          DestroyString(&exitDate);
        } else {
          printf("Usage:/recordPatientExit recordID exitDate\n");
        }
        DestroyString(&recordID);
      } else {
        printf("Usage:/recordPatientExit recordID exitDate\n");
      }
    } else if (!strcmp("/numCurrentPatients",command)) {
      // TODO
    } else if (!strcmp("/exit",command)) {
      running = FALSE;
    } else {
      printf("Command %s not found.\n",command);
      if (!lastword) {
        IgnoreRemainingInput();
      }
    }
    DestroyString(&command);
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