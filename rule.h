#ifndef _RULE
#define _RULE

#include <stdlib.h>
#include <stdio.h>

#include "pattern.h"


typedef struct rule
{
	pattern* left;
	pattern* right;
}rule;

void printRule(rule* r);

rule* createRule(pattern* l, pattern* r);


#endif