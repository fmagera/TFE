#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "pattern.h"
#include "hashtable_r.h"

void genData(const int order, const int alph_max, const int* values, hash_tab* rules, hash_tab* states, hash_tab* transitions);

void genRules(const int alphMax,const int order, const int* values, hash_tab* t, hash_tab* states );

void expand(pattern* p,const int order, const int* values);

void outputState(pattern* output, const int order, const int* value,const int max_alph, const hash_tab* rules);

void genTransitions(const hash_tab* states, const hash_tab* rules, hash_tab* transitions,const  int alph_max,
	const int order, const pattern* stat);

int applyRules( const hash_tab* rules, pattern* p, const int order);