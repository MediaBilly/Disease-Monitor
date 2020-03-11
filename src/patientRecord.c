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

int PatientRecord_Create(patientRecord *record,string recordID,string patientFirstName,string patientLastName,string diseaseID,string country,string entryDate,string exitDate) {
  // Allocate space for patientRecord object
  if ((*record = (patientRecord)malloc(sizeof(struct patient_record))) == NULL) {
    not_enough_memory();
    return FALSE;
  }
  // Copy record contents
  if (((*record)->recordID = CopyString(recordID)) == NULL) {
    not_enough_memory();
    return FALSE;
  }
  if (((*record)->patientFirstName = CopyString(patientFirstName)) == NULL) {
    not_enough_memory();
    return FALSE;
  }
  if (((*record)->patientLastName = CopyString(patientLastName)) == NULL) {
    not_enough_memory();
    return FALSE;
  }
  if (((*record)->diseaseID = CopyString(diseaseID)) == NULL) {
    not_enough_memory();
    return FALSE;
  }
  if (((*record)->country = CopyString(country)) == NULL) {
    not_enough_memory();
    return FALSE;
  }
  struct tm tmpTime;
  if (strptime(entryDate,"%d-%m-%Y",&tmpTime) == NULL) {
    printf("entryDate parsing failed!\n");
    return FALSE;
  } else {
    (*record)->entryDate = mktime(&tmpTime);
  }
  // Check if patient exited the hospital and initialize the corresponding fields properly
  if (strcmp(exitDate,"-")) {
    if (strptime(exitDate,"%d-%m-%Y",&tmpTime) == NULL) {
      printf("exitDate parsing failed!\n");
      return FALSE;
    } else {
      (*record)->exitDate = mktime(&tmpTime);
      (*record)->exited = TRUE;
    }
  } else {
    (*record)->exited = FALSE;
  }
  return TRUE;
}

int PatientRecord_Exited(patientRecord record) {
  return record->exited;
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