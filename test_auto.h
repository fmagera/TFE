/*************************************************************************************************
*
*		test_auto.h : contains functions to compute the product of automata with
*						transducers, used for testing
*
*
**************************************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


#include "lash.h"
#include "lash-diag.h"
#include "lash-auto.h"
#include "auto-io-print.h"
#include "auto-io-dots.h"
#include "lash-auto-operations.h"
#include "auto-minimize.h"
#include "auto-serialize.h"

#include "composition.h"

automaton* test_automata(automaton* a, uint1* in, int len, int order);

automaton* test_automata2 (automaton* a, uint1* in, int ln, uint1* in2, int ln2, int order);

automaton* create_word_auto(uint1* in, int ln);
