#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/monitor.h"
#include "../headers/utilities.h"
#include "../headers/patientRecord.h"
#include "../headers/list.h"

struct disease_monitor {
  unsigned int totalRecords;
  List recordsList;
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
  // Read file records
  string line = NULL;
  size_t len = 0;
  patientRecord record;
  (*monitor)->totalRecords = 0;
  while (getline(&line,&len,patientRecordsFile) != -1) {
    if ((record = PatientRecord_Create(line)) != NULL) {
      // Check if record with the same id already exists and exit the program if so
      if (!List_Contains((*monitor)->recordsList,PatientRecord_Get_recordID(record))) {
        // Insert record to the data structures(list and binary tree hash tables)
        List_Insert((*monitor)->recordsList,record);
        (*monitor)->totalRecords++;
      } else {
        printf("Record with id was found twice in patientRecordsFile.Exiting...\n");
        return FALSE;
      }
    }
  }
  free(line);
  // For test purposes only. Will be removed later
  printf("Total records:%u\n",(*monitor)->totalRecords);
  return TRUE;
}

int DiseaseMonitor_Run(DiseaseMonitor monitor) {
  // TODO
  return TRUE;
}

int DiseaseMonitror_Destroy(DiseaseMonitor *monitor) {
  if (*monitor != NULL) {
    List_Destroy(&((*monitor)->recordsList));
    free(*monitor);
    *monitor = NULL;
    return TRUE;
  } else {
    return FALSE;
  }
}