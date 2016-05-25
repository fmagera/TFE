#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "pattern.h"

typedef struct _hash_en{
  struct _hash_en *next;
  void               *payload;
  int key; 
} hash_en;

typedef struct {
  int       capacity;
  int numberofelements;
  hash_en **tab;
} hash_tab;


hash_tab* createhash_tab(int capacity);

void freehash_tab(hash_tab* hashTable);

int insertEntry(hash_tab* hashTable, hash_en* r, int key);

size_t getNumRules(const hash_tab* t);

bool hasKey(const hash_tab* t, const int key);

int hashKey(size_t capacity, pattern* p);

void printTableRules(const hash_tab* t);

void printTableStates(const hash_tab* t);

void printTablePattern(const hash_tab* t);

void printTableTransitions(const hash_tab* t);

bool existState(const hash_tab* t,const  pattern* p,const  int key);

const rule* getRule(const hash_tab *t,const  pattern* p,const  int key);

const statef* getState(const hash_tab *t,const  pattern* p,const  int key);


#endif