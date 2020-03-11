#ifndef MONITOR_H
#define MONITOR_H

#include <stdio.h>

typedef struct disease_monitor *DiseaseMonitor;

int DiseaseMonitor_Init(DiseaseMonitor*,FILE*,unsigned int,unsigned int,unsigned int);
int DiseaseMonitor_Run(DiseaseMonitor);
int DiseaseMonitror_Destroy(DiseaseMonitor*);

#endif