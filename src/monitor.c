#include <stdio.h>
#include <stdlib.h>
#include "../headers/monitor.h"
#include "../headers/utilities.h"

struct disease_monitor {
  unsigned int totalRecords;
};

int DiseaseMonitor_Init(DiseaseMonitor *monitor,FILE *patientRecordsFile,unsigned int diseaseHashtableNumOfEntries,unsigned int countryHashtableNumOfEntries,unsigned int bucketSize) {
  // Allocate space for DiseaseMonitor
  if ((*monitor = (DiseaseMonitor)malloc(sizeof(struct disease_monitor))) == NULL) {
    not_enough_memory();
    return FALSE;
  }
  // Allocate data structures
  // Read file records
}

int DiseaseMonitor_Run(DiseaseMonitor monitor) {
  // TODO
}

int DiseaseMonitror_Destroy(DiseaseMonitor *monitor) {
  if (*monitor != NULL) {
    free(*monitor);
    *monitor = NULL;
    return TRUE;
  } else {
    return FALSE;
  }
}