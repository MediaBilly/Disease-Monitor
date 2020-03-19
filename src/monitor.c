#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "../headers/monitor.h"
#include "../headers/utilities.h"
#include "../headers/patientRecord.h"
#include "../headers/list.h"
#include "../headers/hashtable.h"
#include "../headers/avltree.h"

struct disease_monitor {
  unsigned int totalRecords;
  List recordsList;
  HashTable recordsHashTable;
  HashTable diseaseHashTable;
  HashTable countryHashTable;
};

int InsertRecord(DiseaseMonitor monitor, patientRecord record) {
  // Insert into the list
  if (!List_Insert(monitor->recordsList,record)) {
    return FALSE;
  }
  // Insert id into the recordsHashTable
  if (!HashTable_Insert(monitor->recordsHashTable,PatientRecord_Get_recordID(record),record)) {
    return FALSE;
  }
  // Insert into diseaseHashTable and tree
  AvlTree diseaseTree = HashTable_SearchKey(monitor->diseaseHashTable,PatientRecord_Get_diseaseID(record));
  // First record of record's disease in diseaseHashTable
  if (diseaseTree == NULL) {
    // Create new tree
    if (!AvlTree_Create(&diseaseTree)) {
      return FALSE;
    }
    if (!HashTable_Insert(monitor->diseaseHashTable,PatientRecord_Get_diseaseID(record),diseaseTree)) {
      AvlTree_Destroy(&diseaseTree);
      return FALSE;
    }
  }
  if (!AvlTree_Insert(diseaseTree,record)) {
      return FALSE;
  }
  // Insert into countryHashTable and tree
  AvlTree countryTree = HashTable_SearchKey(monitor->countryHashTable,PatientRecord_Get_country(record));
  // First record of record's disease in diseaseHashTable
  if (countryTree == NULL) {
    // Create new tree
    if (!AvlTree_Create(&countryTree)) {
      return FALSE;
    }
    if (!HashTable_Insert(monitor->countryHashTable,PatientRecord_Get_country(record),countryTree)) {
      AvlTree_Destroy(&countryTree);
      return FALSE;
    }
  }
  if (!AvlTree_Insert(countryTree,record)) {
      return FALSE;
  }
  monitor->totalRecords++;
  return TRUE;
}

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
  if (!HashTable_Create(&((*monitor)->diseaseHashTable),diseaseHashtableNumOfEntries + countryHashtableNumOfEntries,bucketSize)) {
    return FALSE;
  }
  if (!HashTable_Create(&((*monitor)->countryHashTable),diseaseHashtableNumOfEntries + countryHashtableNumOfEntries,bucketSize)) {
    return FALSE;
  }
  // Read file records
  string line = NULL;
  size_t len = 0;
  patientRecord record;
  (*monitor)->totalRecords = 0;
  while (getline(&line,&len,patientRecordsFile) != -1) {
    if ((record = PatientRecord_CreateFromString(line)) != NULL) {
      // Check if record with the same id already exists and exit the program if so
      if (HashTable_SearchKey((*monitor)->recordsHashTable,PatientRecord_Get_recordID(record)) == NULL) {
        // Insert record to the data structures(list and binary tree hash tables)
        if (!InsertRecord(*monitor,record)) {
          PatientRecord_Destroy(&record);
          free(line);
          return FALSE;
        }
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

void globalDiseaseStats(string disease,void *tree,int argc,va_list valist) {
  unsigned int cases;
  if (argc == 2) {
    time_t date1 = va_arg(valist,time_t);
    time_t date2 = va_arg(valist,time_t);
    cases = AvlTree_NumRecordsInDateRange((AvlTree)tree,date1,date2,NULL);
  } else {
    cases = AvlTree_NumRecords((AvlTree)tree);
  }
  printf("%s: %u cases\n",disease,cases);
}

int DiseaseMonitor_Run(DiseaseMonitor monitor) {
  int running = TRUE;
  string line = NULL,command;
  string *argv;
  size_t len;
  unsigned int argc;
  // Execute commands until /exit is given
  while (running) {
    // Read command name
    putchar('>');
    getline(&line,&len,stdin);
    IgnoreNewLine(line);
    argc = wordCount(line);
    argv = SplitString(line," ");
    command = argv[0];
    // Determine command type
    if (!strcmp("/globalDiseaseStats",command)) {
      // Usage check
      if (argc == 1 || argc == 3) {
        if (argc == 3) {
          struct tm tmpTime;
          memset(&tmpTime,0,sizeof(struct tm));
          if (strptime(argv[1],"%d-%m-%Y",&tmpTime) != NULL) {
            time_t date1 = mktime(&tmpTime);
            if (strptime(argv[2],"%d-%m-%Y",&tmpTime) != NULL) {
              time_t date2 = mktime(&tmpTime);
              HashTable_ExecuteFunctionForAllKeys(monitor->diseaseHashTable,globalDiseaseStats,2,date1,date2);
            } else {
              printf("date2 parsing failed.\n");
            }
          } else {
            printf("date1 parsing failed.\n");
          }
        } else if (argc == 1) {
          HashTable_ExecuteFunctionForAllKeys(monitor->diseaseHashTable,globalDiseaseStats,0);
        }
      } else {
        printf("Usage:/globalDiseaseStats [date1 date2]\n");
      }
    } else if (!strcmp("/diseaseFrequency",command)) {
      // Usage check
      if (argc == 4 || argc == 5) {
        AvlTree virusTree;
        if ((virusTree = (AvlTree)HashTable_SearchKey(monitor->diseaseHashTable,argv[1])) != NULL) {
          struct tm tmpTime;
          memset(&tmpTime,0,sizeof(struct tm));
          if (strptime(argv[2],"%d-%m-%Y",&tmpTime) != NULL) {
            time_t date1 = mktime(&tmpTime);
            if (strptime(argv[3],"%d-%m-%Y",&tmpTime) != NULL) {
              time_t date2 = mktime(&tmpTime);
              if (argc == 5) {
                printf("%s total cases in %s:%u\n",argv[1],argv[4],AvlTree_NumRecordsInDateRange(virusTree,date1,date2,argv[4]));
              } else {
                printf("%s total cases:%u\n",argv[1],AvlTree_NumRecordsInDateRange(virusTree,date1,date2,NULL));
              }
            } else {
              printf("date2 parsing failed.\n");
            }
          } else {
            printf("date1 parsing failed.\n");
          }
        } else {
          printf("%s total cases:%u\n",argv[1],0);
        }
      } else {
        printf("Usage:/diseaseFrequency virusName date1 date2 [country]\n");
      }
    } else if (!strcmp("/topk-Diseases",command)) {
      // TODO
    } else if (!strcmp("/topk-Countries",command)) {
      // TODO
    } else if (!strcmp("/insertPatientRecord",command)) {
      // Usage check
      if (argc == 7 || argc == 8) {
        patientRecord record;
        if ((record = PatientRecord_Create(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6],argc == 8 ? argv[7] : "-")) == NULL) {
          return FALSE;
        } 
        if (!InsertRecord(monitor,record)) {
          return FALSE;
        }
      } else {
        printf("Usage:/insertPatientRecord recordID patientFirstName patientLastName diseaseID country entryDate [exitDate]\n");
      }
    } else if (!strcmp("/recordPatientExit",command)) {
      // Usage check
      if (argc == 3) {
        // Check if patient with that name exists
        patientRecord record;
        if ((record = (patientRecord)HashTable_SearchKey(monitor->recordsHashTable,argv[1])) != NULL) {
          PatientRecord_Exit(record,argv[2]);
        } else {
          printf("Patient with ID %s does not exist.\n",argv[1]);
        }
      } else {
        printf("Usage:/recordPatientExit recordID exitDate\n");
      }
    } else if (!strcmp("/numCurrentPatients",command)) {
      // TODO
    } else if (!strcmp("/exit",command)) {
      // Usage check
      if (argc == 1) {
        running = FALSE;
      } else {
        printf("Usage:/exit\n");
      }
    } else {
      printf("Command %s not found.\n",command);
    }
    free(argv);
    DestroyString(&line);
  }
  return TRUE;
}

int DiseaseMonitror_Destroy(DiseaseMonitor *monitor) {
  if (*monitor != NULL) {
    // Destroy data structures
    // Destroy function typecast:(int (*)(void**))func_name 
    HashTable_Destroy(&((*monitor)->recordsHashTable),NULL);
    HashTable_Destroy(&((*monitor)->diseaseHashTable),(int (*)(void**))AvlTree_Destroy);
    HashTable_Destroy(&((*monitor)->countryHashTable),(int (*)(void**))AvlTree_Destroy);
    List_Destroy(&((*monitor)->recordsList));
    // Destroy monitor itself
    free(*monitor);
    *monitor = NULL;
    return TRUE;
  } else {
    return FALSE;
  }
}