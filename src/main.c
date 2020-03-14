#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/utilities.h"
#include "../headers/monitor.h"

void usage() {
  printf("Usage:./diseaseMonitor -p patientRecordsFile –h1 diseaseHashtableNumOfEntries –h2 countryHashtableNumOfEntries –b bucketSize\n");
}

int main(int argc, char const *argv[]) {
  // Usage check
  if (argc != 9) {
    usage();
    return 0;
  }
  // Specify argument variables
  FILE *patientRecordsFile = NULL;
  unsigned int diseaseHashtableNumOfEntries,countryHashtableNumOfEntries,bucketSize;
  // Read arguments
  int args[4] = {FALSE,FALSE,FALSE,FALSE}; // To check later if all parameters were given
  int i;
  for (i = 1; i < argc; i+=2) {
    if (!strcmp(argv[i],"-p")) {
      // Open patientRecordsFile
      if ((patientRecordsFile = fopen(argv[i+1],"r")) == NULL) {
        printf("patientRecordsFile %s could not be opened.\n",argv[i+1]);
        return 0;
      } else {
        args[0] = TRUE;
      }
    }
    else if (!strcmp(argv[i],"-h1")) {
      // Read diseaseHashtableNumOfEntries
      diseaseHashtableNumOfEntries = atoi(argv[i+1]);
      args[1] = TRUE;
    }
    else if (!strcmp(argv[i],"-h2")) {
      // Read countryHashtableNumOfEntries
      countryHashtableNumOfEntries = atoi(argv[i+1]);
      args[2] = TRUE;
    }
    else if (!strcmp(argv[i],"-b")) {
      // Read bucketSize
      bucketSize = atoi(argv[i+1]);
      args[3] = TRUE;
    }
    else {
      // Wrong argument
      usage();
      // Close patientRecordsFile file if it was previously opened
      if (patientRecordsFile != NULL) {
        fclose(patientRecordsFile);
      }
      // Exit due to error
      return 0;
    }
  }
  DiseaseMonitor diseaseMonitor;
  // Check if all parameters were given
  if (args[0] == TRUE && args[0] == args[1] && args[1] == args[2] && args[2] == args[3]) {
    // Run diseaseMonitor
    if (DiseaseMonitor_Init(&diseaseMonitor,patientRecordsFile,diseaseHashtableNumOfEntries,countryHashtableNumOfEntries,bucketSize)) {
      DiseaseMonitor_Run(diseaseMonitor);
    } 
    DiseaseMonitror_Destroy(&diseaseMonitor);
  } else {
    usage();
    // Close diseaseHashtableNumOfEntries file if it was previously opened
    if (patientRecordsFile != NULL) {
      fclose(patientRecordsFile);
    }
    // Exit due to error
    return 0;
  }
  fclose(patientRecordsFile);
  return 0;
}
