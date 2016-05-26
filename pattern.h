#ifndef _PATTERN
#define _PATTERN

#include <stdlib.h>
#include <stdio.h>

typedef struct pattern
{
	int* values;
	int size;
}pattern;

typedef struct statef
{
	pattern* label;
	pattern* output;
	int nb;
}statef;

int incPat(pattern* p, const int alph_max);

pattern* append(const pattern* p, const int i);

int compLexPat(const pattern* p, const pattern* o);

pattern* subPattern(const pattern* p, const int start,const  int end);

pattern** getPrefixes(const pattern* p);

pattern** getSuffixes(const pattern* p);

pattern* getPrefix(const pattern* p, const int sizeOfSuffix);

pattern* getSuffix(const pattern* p, const pattern* prefix);

void printPattern(const pattern* p);

pattern* copyPattern(const pattern* p);

int getMax(const pattern* p);

int getMin(const pattern* p);

typedef struct rule
{
	pattern* left;
	pattern* right;
}rule;

void printRule(const rule* r);

rule* createRule(pattern* l, pattern* r);


typedef struct transition
{
	pattern* st_state;
	pattern* en_state;
	int input;
	pattern* output;
}transition;

void printTransition(const transition* t);

void printStatef(statef* s);


void freePattern(pattern* p);
void freeRule(rule* r);
void freeTransition(transition* t);
void freeStatef(statef* s);

#endif
