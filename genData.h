#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "pattern.h"
#include "hashtable_r.h"

void genData(int order, int alph_max, int* values, hash_tab* rules, hash_tab* states, hash_tab* transitions);

void genRules(int alphMax, int order, int* values, hash_tab* t, hash_tab* states );

void expand(pattern* p, int order, int* values);

void outputState(pattern* output, int order, int* value, int max_alph, hash_tab* rules);

void genTransitions(hash_tab* states, hash_tab* rules, hash_tab* transitions, int alph_max, int order, pattern* state);

int applyRules(hash_tab* rules, pattern* p, int order);