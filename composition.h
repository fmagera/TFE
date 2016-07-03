#ifndef _COMPO
#define _COMPO

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "lash.h"
#include "lash-diag.h"
#include "lash-auto.h"
#include "auto-io-print.h"
#include "auto-io-dots.h"
#include "lash-auto-operations.h"
#include "auto-minimize.h"
#include "auto.h"
#include "auto-product.h"

automaton * add_composant(automaton* a, int position, int ln, int alph_max);

automaton * compose(automaton* a, int ln_tr_a, automaton* b,int len_tr_b, int alph, int ln);

void add_composant_rec(automaton* a, int position, int count, uint4 curr, int alph_max, uint1* added, int space);


uint1* createLabel(uint1 in, uint1 out);
void copy_transitions(automaton* a, uint4 s, uint4 d);

#endif