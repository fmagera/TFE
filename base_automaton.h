/*************************************************************************************************
*
*		base_automaton.h : contains functions to create the base automaton and 
*						to operate on it.
*
*
**************************************************************************************************/


#ifndef BASE_AUTO
#define BASE_AUTO
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


automaton* get_red(int order, int alph_max);
void red_add(automaton* aut, uint4 state, uint1 toRed, bool isRest, uint1 alph, int nbTrans);


automaton* get_un(int order, int alph_max);
void un_add(automaton* aut, uint4 state, int j, int curr, uint1 alph, int order, bool red);


automaton* decalage(automaton* a, int nb_dec, int alph);

void add_identity(automaton* a, int alph, int order);

void final_accepting(automaton* a, int alph_max);

void add_loop(automaton* a, int alph);

automaton* accept_all_size(int alph_max, int size_max);

#endif