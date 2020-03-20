#ifndef UTILITIES_H
#define UTILITIES_H

#define TRUE 1
#define FALSE 0

typedef char* string;

void not_enough_memory();
string CopyString(string);
int DestroyString(string*);
// Counts # of words in a string
unsigned int wordCount(string);
string IgnoreNewLine(string);
string* SplitString(string,string);

#endif