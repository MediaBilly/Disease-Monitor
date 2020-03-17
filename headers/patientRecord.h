#ifndef PATIENT_RECORD_H
#define PATIENT_RECORD_H

#include <time.h>
#include "./utilities.h"

typedef struct patient_record *patientRecord;

// Creates a patientRecord object
patientRecord PatientRecord_Create(string);
// Returns TRUE if patientRecord's patient exited the hospital and FALSE otherwise
int PatientRecord_Exited(patientRecord);
// Getters
string PatientRecord_Get_recordID(patientRecord);
string PatientRecord_Get_diseaseID(patientRecord);
string PatientRecord_Get_country(patientRecord);
time_t PatientRecord_Get_entryDate(patientRecord);
time_t PatientRecord_Get_exitDate(patientRecord);
// Exit date updater
int PatientRecord_Exit(patientRecord,string);
// Destroys a patientRecord object
int PatientRecord_Destroy(patientRecord*);

#endif