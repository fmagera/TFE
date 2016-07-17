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

#include "hashtable_r.h"

typedef struct 
{
	uint4 nb_trans;
	uint1* tran_in;
	uint1* tran_out;
	uint4 state; 
	int key;
}path_c;

automaton * add_composant(automaton* a, int position, int ln, int alph_max);

automaton * compose(automaton* a, int ln_tr_a, automaton* b,int len_tr_b, int alph, int ln);

void add_composant_rec(automaton* a, int position, int count, uint4 curr, int alph_max, uint1* added, int space);


uint1* createLabel(uint1 in, uint1 out);

void copy_transitions(automaton* a, uint4 s, uint4 d);

void new_comp(automaton* a, int order);

void new_comp_rec(automaton *a, uint4 state_end, uint4 steps_left, uint4* path, uint4* trans, uint4 ini_nb_st, hash_tab* path_in,  uint4* visited);

void get_path_out(automaton* a, uint4 state_st, uint4 steps_left, uint4* path, uint4* trans, uint4 ini_nb_st, hash_tab* path_out, uint4* visited);

#endif