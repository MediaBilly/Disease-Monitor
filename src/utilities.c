#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/utilities.h"

void not_enough_memory() {
  printf("Not enough memory!\n");
}

string CopyString(string str) {
  string ret = NULL;
  if ((ret = (string)malloc(strlen(str) + 1)) != NULL) {
    strcpy(ret,str);
  }
  return ret;
}

int DestroyString(string *str) {
  if (*str != NULL) {
    free(*str);
    *str = NULL;
    return TRUE;
  } else {
    return FALSE;
  }
}

unsigned int wordCount(string str) {
  unsigned int count = 0;
  // Ignore 1st potential gaps
  while (*str == ' ') str++;
  while (*str != '\n' && *str != 0) {
    count++;
    // Loop through characters
    while (*str != ' ' && *str != '\n' && *str != 0) str++;
    // Ignore gaps
    while (*str == ' ') str++;
  }
  return count;
}

string IgnoreNewLine(string str) {
  str[strlen(str) - 1] = 0;
  return str;
}

string* SplitString(string str,string delimeter) {
  string *array;
  if ((array = (string*)malloc(wordCount(str)*sizeof(string))) == NULL) {
    not_enough_memory();
    return NULL;
  }
  unsigned int index = 0;
  string tmp = strtok(str,delimeter);
  while (tmp != NULL) {
    array[index++] = tmp;
    tmp = strtok(NULL," ");
  }
  return array;
}