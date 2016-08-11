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

automaton* normalised(int order, int alph_max);

automaton* addition(uint1 alph_max);

automaton* l_normalize(int order);

automaton* r_normalize(int order);