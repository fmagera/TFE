#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "rule.h"
#include "pattern.h"

typedef struct _hash_entry{
  struct _hash_entry *next;
  void               *payload;
  int key; 
} hash_entry;

typedef struct {
  int       capacity;
  int numberofelements;
  hash_entry **tab;
} hash_table;


hash_table* createhash_table(int capacity);

void freehash_table(hash_table* hashTable);

int insertEntry(hash_table* hashTable, hash_entry* r, int key);

size_t getNumRules(const hash_table* t);

bool hasKey(const hash_table* t, const int key);

int hashKey(size_t capacity, pattern* p);

void printTableRules(hash_table* t);

void printTablePattern(hash_table* t);

void printTableTransitions(hash_table* t);

bool existPattern(hash_table* t, pattern* p, int key);

rule* getRule(hash_table *t, pattern* p, int key);


#endif