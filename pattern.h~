#ifndef _PATTERN
#define _PATTERN

#include <stdlib.h>
#include <stdio.h>

typedef struct pattern
{
	int* values;
	int size;
}pattern;

int incPat(pattern* p, int alph_max);

pattern* append(pattern* p, int i);

int compLexPat(pattern* p, pattern* o);

pattern* subPattern(pattern* p, int start, int end);

pattern** getPrefixes(pattern* p);

pattern** getSuffixes(pattern* p);

pattern* getPrefix(pattern* p, int sizeOfSuffix);

pattern* getSuffix(pattern* p, pattern* prefix);

void printPattern(pattern* p);

pattern* copyPattern(pattern* p);

int getMax(pattern* p);

int getMin(pattern* p);

typedef struct transition
{
	pattern* st_state;
	pattern* en_state;
	int input;
	pattern* output;
}transition;

void printTransition(transition* t);

#endif
