#ifndef PATIENT_RECORD_H
#define PATIENT_RECORD_H

#include "./utilities.h"

typedef struct patient_record *patientRecord;

// Creates a patientRecord object
int PatientRecord_Create(patientRecord*,string,string,string,string,string,string,string);
// Returns TRUE if patientRecord's patient exited the hospital and FALSE otherwise
int PatientRecord_Exited(patientRecord);
// Destroys a patientRecord object
int PatientRecord_Destroy(patientRecord*);

#endif