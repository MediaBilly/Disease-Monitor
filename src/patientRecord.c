#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../headers/patientRecord.h"
#include "../headers/utilities.h"

struct patient_record {
  string recordID;
  string patientFirstName;
  string patientLastName;
  string diseaseID;
  string country;
  time_t entryDate;
  time_t exitDate;
  int exited;
};

patientRecord PatientRecord_CreateFromString(string line) {
  // Error check: Ignore record if the line has more than 7 arguments
  if (wordCount(line) != 7) {
    printf("Ignored record due to incorrect number of arguments: %s\n",line);
    return NULL;
  }
  patientRecord record;
  // Allocate space for patientRecord object
  if ((record = (patientRecord)malloc(sizeof(struct patient_record))) == NULL) {
    not_enough_memory();
    return NULL;
  }
  // Create a copy of the given line because strtok modifies strings and in future strtoks unexpected behaviour occurs
  string lineCopy = CopyString(line);
  // Copy record contents
  if ((record->recordID = CopyString(strtok(lineCopy," "))) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    DestroyString(&lineCopy);
    return NULL;
  }
  if ((record->patientFirstName = CopyString(strtok(NULL," "))) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    DestroyString(&lineCopy);
    return NULL;
  }
  if ((record->patientLastName = CopyString(strtok(NULL," "))) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    DestroyString(&lineCopy);
    return NULL;
  }
  if ((record->diseaseID = CopyString(strtok(NULL," "))) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    DestroyString(&lineCopy);
    return NULL;
  }
  if ((record->country = CopyString(strtok(NULL," "))) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    DestroyString(&lineCopy);
    return NULL;
  }
  struct tm tmpTime;
  memset(&tmpTime,0,sizeof(struct tm));
  string entryDate = strtok(NULL," ");
  if (strptime(entryDate,"%d-%m-%Y",&tmpTime) == NULL) {
    printf("entryDate %s parsing failed!\n",entryDate);
    DestroyString(&lineCopy);
    return NULL;
  } else {
    record->entryDate = mktime(&tmpTime);
  }
  // Check if patient exited the hospital and initialize the corresponding fields properly
  string exitDate = strtok(NULL," ");
  memset(&tmpTime,0,sizeof(struct tm));
  if (exitDate[0] != '-') {
    if (strptime(exitDate,"%d-%m-%Y",&tmpTime) == NULL) {
      printf("exitDate %s parsing failed!\n",exitDate);
      DestroyString(&lineCopy);
      return NULL;
    } else {
      record->exitDate = mktime(&tmpTime);
      record->exited = TRUE;
    }
  } else {
    record->exited = FALSE;
  }
  // Check if entryDate is later than exitDate and destroy(ignore) the record if so
  if (record->exited && difftime(record->entryDate,record->exitDate) > 0) {
    char date1[10],date2[10];
    strftime(date1,sizeof(date1),"%d-%m-%Y",localtime(&(record->entryDate)));
    strftime(date2,sizeof(date2),"%d-%m-%Y",localtime(&(record->exitDate)));
    printf("Ignoring record %s %s %s %s %s %s %s because entryDate is later than exitDate\n",record->recordID,record->patientFirstName,record->patientLastName,record->diseaseID,record->country,date1,date2);
    PatientRecord_Destroy(&record);
  }
  DestroyString(&lineCopy);
  return record;
}

patientRecord PatientRecord_Create(string recordID,string patientFirstName,string patientLastName,string diseaseID,string country,string entryDate,string exitDate) {
  patientRecord record;
  // Allocate space for patientRecord object
  if ((record = (patientRecord)malloc(sizeof(struct patient_record))) == NULL) {
    not_enough_memory();
    return NULL;
  }
  // Copy record contents
  if ((record->recordID = CopyString(recordID)) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    return NULL;
  }
  if ((record->patientFirstName = CopyString(patientFirstName)) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    return NULL;
  }
  if ((record->patientLastName = CopyString(patientLastName)) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    return NULL;
  }
  if ((record->diseaseID = CopyString(diseaseID)) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    return NULL;
  }
  if ((record->country = CopyString(country)) == NULL) {
    not_enough_memory();
    PatientRecord_Destroy(&record);
    return NULL;
  }
  struct tm tmpTime;
  memset(&tmpTime,0,sizeof(struct tm));
  if (strptime(entryDate,"%d-%m-%Y",&tmpTime) == NULL) {
    printf("entryDate %s parsing failed!\n",entryDate);
    return NULL;
  } else {
    record->entryDate = mktime(&tmpTime);
  }
  // Check if patient exited the hospital and initialize the corresponding fields properly
  memset(&tmpTime,0,sizeof(struct tm));
  if (exitDate[0] != '-') {
    if (strptime(exitDate,"%d-%m-%Y",&tmpTime) == NULL) {
      printf("exitDate %s parsing failed!\n",exitDate);
      PatientRecord_Destroy(&record);
      return NULL;
    } else {
      record->exitDate = mktime(&tmpTime);
      record->exited = TRUE;
    }
  } else {
    record->exited = FALSE;
  }
  // Check if entryDate is later than exitDate and destroy(ignore) the record if so
  if (record->exited && difftime(record->entryDate,record->exitDate) > 0) {
    char date1[10],date2[10];
    strftime(date1,sizeof(date1),"%d-%m-%Y",localtime(&(record->entryDate)));
    strftime(date2,sizeof(date2),"%d-%m-%Y",localtime(&(record->exitDate)));
    printf("Ignoring record %s %s %s %s %s %s %s because entryDate is later than exitDate\n",record->recordID,record->patientFirstName,record->patientLastName,record->diseaseID,record->country,date1,date2);
    PatientRecord_Destroy(&record);
  }
  printf("Record added\n");
  return record;
}

int PatientRecord_Exited(patientRecord record) {
  return record->exited;
}

string PatientRecord_Get_recordID(patientRecord record) {
  return record->recordID;
}

string PatientRecord_Get_diseaseID(patientRecord record) {
  return record->diseaseID;
}

string PatientRecord_Get_country(patientRecord record) {
  return record->country;
}

time_t PatientRecord_Get_entryDate(patientRecord record) {
  return record->entryDate;
}

time_t PatientRecord_Get_exitDate(patientRecord record) {
  return record->exitDate;
}

int PatientRecord_Exit(patientRecord record,string exitDateStr) {
  struct tm tmpTime;
  memset(&tmpTime,0,sizeof(struct tm));
  time_t exitDate;
  if (strptime(exitDateStr,"%d-%m-%Y",&tmpTime) != NULL) {
    if (difftime((exitDate = mktime(&tmpTime)),record->entryDate) >= 0) {
      record->exitDate = exitDate;
      record->exited = TRUE;
      printf("Record updated\n");
      return TRUE;
    } else {
      printf("The exitDate of the record with id %s is earlier than it's entryDate.Ignoring update.\n",record->recordID);
      return FALSE;
    }
  } else {
    printf("Wrong date format.\n");
    return FALSE;
  }
}

int PatientRecord_Destroy(patientRecord *record) {
  if (*record != NULL) {
    // Destroy the strings first
    DestroyString(&(*record)->recordID);
    DestroyString(&(*record)->patientFirstName);
    DestroyString(&(*record)->patientLastName);
    DestroyString(&(*record)->diseaseID);
    DestroyString(&(*record)->country);
    // Then the record itself
    free(*record);
    *record = NULL;
    return TRUE;
  } else {
    return FALSE;
  }
}