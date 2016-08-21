

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>


#include "lash.h"
#include "lash-diag.h"
#include "lash-auto.h"
#include "auto-io-print.h"
#include "auto-io-dots.h"
#include "lash-auto-operations.h"
#include "auto-minimize.h"
#include "auto-serialize.h"
#include "auto-quotient.h"



#include "pattern.h"
#include "hashtable_r.h"
#include "genData.h"
#include "composition.h"
#include "test_auto.h"
#include "addition.h"
#include "end_normalize.h"
#include "fib_add.h"
#include "base_automaton.h"




// input format :  m a [ 1 1 1 ] [ 4 2 1 ]"
int main(int argc, char *argv[])
{

	if(argc < 3)
		return 1;
	
	int order = atoi(argv[1]);
	int alph_max = atoi(argv[2]);
	int coefficients[order];
	int values[order];
	int i = 3,j = 0, k = 0;

	for( ; i < argc && *argv[i] != '['; i++);

	j = i+1;
	while(*argv[j]  != ']')
	{			 
		coefficients[k] = atoi(argv[j]);
		j++;
		k++;				
	}

	i = j;
	k = 0;
	for( ; i < argc && *argv[i] != '['; i++);

	j = i+1;
	while(*argv[j]  != ']')
	{	 
		values[k] = atoi(argv[j]);
		j++;
		k++;				
	}

	hash_tab* rules = createhash_tab(pow(10,order+2));
	hash_tab* states = createhash_tab(pow(10,order));
	hash_tab* transitions = createhash_tab(pow(10,order)*alph_max);

	genData(order, alph_max, values, rules, states, transitions);
	printf("Data generated... \n");

	

	
	if (lash_init() < 0)
    lash_perror("lash_init");
	

	automaton* a1 = get_un(order, alph_max);

	automaton* a2 = get_red(order, alph_max);
	automaton* base = auto_product_union(a1, a2);
	auto_minimize(base);

	auto_free(a1);
	auto_free(a2);



	automaton* addit = addition(alph_max);
	automaton* nor1 = normalised(order, alph_max);
	automaton* nor2 = normalised(order, alph_max);
	automaton* nor3 = auto_seq_product_separ(nor1, nor2,1,1,NULL);
	auto_minimize(nor3);
	auto_free(nor1);
	auto_free(nor2);

	addit = compose(nor3, 2, addit, 3, alph_max+1, 3); 
	auto_free(nor3);

	automaton* r_nor = r_normalize(order);
	automaton* l_nor = l_normalize(order);

	auto_minimize(addit);
	auto_minimize(r_nor);
	auto_minimize(l_nor);

	automaton* normalizer = compose(l_nor, 2, r_nor, 2,alph_max,3);
	auto_minimize(normalizer);
	auto_free(r_nor);
	auto_free(l_nor);

	automaton* en = end_normalize(states, alph_max);
	auto_minimize(en);
	automaton* en3 = auto_copy(en);
	en3 = auto_unserialize(en3, 2, NULL);
	auto_serialize_write_dot_file(en3, "Dot\ and\ images/en3.dot",LASH_EXP_DIGIT);
	auto_free(en3);
	
	// extended normalization part
	automaton* next = auto_copy(base);
	auto_free(base);
	add_loop(next, 3);
	auto_minimize(next);
	new_comp(next, order);
	auto_minimize(next);

	uint4 in_st = 0;
	auto_i_state(next,0, & in_st);
	for(int i = 0 ; i < 2*alph_max+2; i++)
	{
		auto_add_new_transition(next, in_st, in_st, 2, createLabel(i,i));
	}
	auto_minimize(next);
	printf("Extended generated \n");
	// extended normalization of the end (2m...0) of the word	
	next = compose(next, 2, en, 2, 3, 3);
	auto_minimize(next);
	auto_free(en);
	printf("Post nor added %d \n", auto_nb_states(next));


	// Composition of the extended normalization automaton, should be done m times
	
	automaton* result = auto_copy(next);
	for(int i = 0; i < order/4; i++)
	{
		result = compose(result, 2, next, 2, 3, 3 );
		auto_minimize(result);
		printf("%d %d \n", i, auto_nb_states(result));
	}
	printf("Composition m times done \n");
	

	automaton* result2 = auto_copy(result);
	result = compose(addit, 3, result, 2, 2*alph_max, 4);
	auto_minimize(result);
	if(order % 2)
		result = compose(result, 3, next, 2, 3, 4 );


	result2 = compose(result2, 2, normalizer, 2, 2*alph_max, 3);
	auto_minimize(result2);
	result = compose(result, 3, result2, 2, 2*alph_max, 4);
	auto_minimize(result);
	printf("Final number of states : %d \n", auto_nb_states(result));
	
	// TEST 
	uint1 word[10] = {0,0, 2,0,2,0,2,0,2,0};
	automaton* normalize_test = test_automata(next, word, 10, order);
	auto_serialize_write_dot_file(normalize_test, "Dot\ and\ images/nor_test.dot",LASH_EXP_DIGIT);
	auto_free(next);
	auto_free(normalize_test);
	auto_free(result2);
	auto_free(addit);
	auto_free(normalizer);
	
	
	if(order == 3)
	{
		automaton* trib = trib_add("msd_trib_addition.txt");
		auto_minimize(trib);
		if(auto_equality(trib,result) == 1)
			printf("SUCCEEEESSS\n");
		auto_free(trib);
	}
	else if(order == 2)
	{
		automaton* fib = fib_addition();
		auto_minimize(fib);
		if(auto_equality(fib,result) == 1)
			printf("SUCCEEEESSS\n");

		auto_free(fib);
	}
	
	// TEST on 2 inputs
	uint1 w1[10] = {0,0, 1,0,1,0,1,0,1,0};
	uint1 w2[10] = {0,0, 1,0,1,0,1,0,1,0};
	automaton* b = test_automata2(result, w1,10,w2,10, order);
	auto_serialize_write_dot_file(b, "Dot\ and\ images/test.dot", LASH_EXP_DIGIT);
	auto_free(b);

	result = auto_unserialize(result, 3, NULL);
	auto_serialize_write_dot_file(result, "Dot\ and\ images/addition.dot", LASH_EXP_DIGIT);
	auto_free(result);
	

	
	if (lash_end() < 0)
	    lash_perror("lash_end");

	printf("### statistics ###\n");
	printf("residual memory : %llu byte(s).\n", lash_get_mem_usage());
	printf("max memory      : %llu byte(s).\n", lash_get_max_mem_usage());
	printf("### end of the test ###\n");   
	
	for(int i = 0; i < states->capacity; i++)
	{
		if(states->tab[i] != NULL)
		{
			freeStatef(states->tab[i]->payload);
		}
	}
	freehash_tab(states);

	for(int i = 0; i < rules->capacity; i++)
	{
		if(rules->tab[i] != NULL)
		{
			freeRule(rules->tab[i]->payload);
		}
	}
	freehash_tab(rules);

	for(int i = 0; i < transitions->capacity; i++)
	{
		if(transitions->tab[i] != NULL)
		{
			freeTransition(transitions->tab[i]->payload);
		}
	}
	freehash_tab(transitions);

	return 0;

}


