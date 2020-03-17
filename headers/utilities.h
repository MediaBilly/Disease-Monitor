#ifndef UTILITIES_H
#define UTILITIES_H

#define TRUE 1
#define FALSE 0

//#define MAX(A,B) (((A) > (B)) ? (A) : (B))

typedef char* string;

void not_enough_memory();
string CopyString(string);
int DestroyString(string*);
// Counts # of words in a string
unsigned int wordCount(string);
string readNextWord(int*);
void IgnoreRemainingInput();

#endif