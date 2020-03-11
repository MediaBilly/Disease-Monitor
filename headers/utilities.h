#ifndef UTILITIES_H
#define UTILITIES_H

#define TRUE 1
#define FALSE 0

typedef char* string;

void not_enough_memory();
string CopyString(string);
int DestroyString(string*);

#endif