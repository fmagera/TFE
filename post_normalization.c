#define TEST_AUTO
//#define TEST_COMPO
//#define TEST_TEST_AUTO
//#define TEST_ADD
//#define END_NOR
#define TEST_INTE
//#define TEST_FIB
//#define TEST_MAX_SIZE
#define TEST_TRIB

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


uint1* concat(int in, uint1* output, int sizeOut, int order);

automaton* get_red(int order, int alph_max);
void red_add(automaton* aut, uint4 state, uint1 toRed, bool isRest, uint1 alph, int nbTrans);
automaton* get_un(int order, int alph_max);
void un_add(automaton* aut, uint4 state, int j, int curr, uint1 alph, int order, bool red);
automaton* decalage(automaton* a, int nb_dec, int alph);
void add_identity(automaton* a, int alph, int order);
void final_accepting(automaton* a, int alph_max);
void add_loop(automaton* a, int alph);
automaton* accept_all_size(int alph_max, int size_max);

// input format :  m a [ 1 1 1 ] [ 4 2 1 ]"
int main(int argc, char *argv[])
{

	float tim = clock();
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



#ifdef TEST_INTE

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
	
	// extended normalization part
	automaton* next = auto_copy(base);
	auto_free(base);
	add_loop(next, 3);
	auto_minimize(next);
	new_comp(next, order);
	auto_minimize(next);
	automaton* nc = auto_copy(next);
	nc = auto_unserialize(nc, 2, NULL);
	auto_serialize_write_dot_file(nc, "nc.dot", LASH_EXP_DIGIT);
	auto_free(nc);

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
	//next = auto_seq_projection_separ(next, 3, 1, NULL);
	auto_minimize(next);
	auto_free(en);
	printf("Post nor added %d \n", auto_nb_states(next));


	// Composition of the extended normalization automaton, should be done m times
	
	automaton* result = auto_copy(next);
	for(int i = 0; i < order-1; i++)
	{
		result = compose(result, 2, next, 2, 3, 3 );
		//result = auto_seq_projection_separ(result, 3, 1, NULL);
		auto_minimize(result);
		printf("%d %d \n", i, auto_nb_states(result));
	}
	printf("Composition m times done \n");


	// TEST 
	uint1 word[10] = {0,0, 2,0,2,0,2,0,2,0};
	automaton* normalize_test = test_automata(next, word, 10, order);
	auto_serialize_write_dot_file(normalize_test, "nor_test.dot",LASH_EXP_DIGIT);
	auto_free(next);
	auto_free(normalize_test);

	// Building the final automaton
	result = compose(addit, 3, result, 2, 2*alph_max, 4);
	//result = auto_seq_projection_separ(result, 4, 2, NULL);
	auto_minimize(result);
	// Final normalization
	result = compose(result, 3, normalizer, 2, alph_max, 4);
	//result = auto_seq_projection_separ(result, 4, 2, NULL);
	auto_minimize(result);
	printf("Final number of states : %d \n", auto_nb_states(result));
	auto_free(addit);
	auto_free(normalizer);
	
#ifdef TEST_TRIB
	automaton* fib = fib_addition();
	auto_minimize(fib);
	automaton* trib = trib_add("msd_trib_addition.txt");
	if(trib != NULL)
		auto_serialize_write_dot_file(trib, "trib.dot", LASH_EXP_DIGIT);
	auto_minimize(trib);
	if(auto_equality(trib,result) == 1)
		printf("SUCCEEEESSS\n");
	automaton* dif = auto_difference(result, trib);
	auto_minimize(dif);
	dif = auto_unserialize(dif, 3, NULL);
	auto_minimize(dif);

	auto_serialize_write_dot_file(dif, "dif.dot", LASH_EXP_DIGIT);
	printf("%d \n",auto_nb_states(dif));
	auto_free(fib);
	auto_free(trib);
	auto_free(dif);

#endif
#ifdef TEST_FIB
	automaton* fib = fib_addition();
	auto_minimize(fib);
	if(auto_equality(fib,result) == 1)
		printf("SUCCEEEESSS\n");
	automaton* dif = auto_difference(result,fib);
	auto_minimize(dif);
	dif = auto_unserialize(dif, 3, NULL);
	auto_minimize(dif);

	auto_serialize_write_dot_file(dif, "dif.dot", LASH_EXP_DIGIT);
	printf("%d \n",auto_nb_states(dif));
	auto_free(fib);
	auto_free(dif);
#endif
	// TEST on 2 inputs
	uint1 w1[10] = {0,0, 1,0,1,0,1,0,1,0};
	uint1 w2[10] = {0,0, 1,0,1,0,1,0,1,0};
	//scanf("%6c", word);
	automaton* b = test_automata2(result, w1,10,w2,10, order);
	//automaton* c = test_automata2(trib, w1,10,w2,10, order);

	auto_serialize_write_dot_file(b, "test.dot", LASH_EXP_DIGIT);
	//auto_serialize_write_dot_file(c, "test2.dot", LASH_EXP_DIGIT);
	auto_free(b);
	
	/*
	result = auto_seq_projection_separ(result, 3, 0, NULL);
	result = auto_seq_projection_separ(result, 2, 0, NULL);
	auto_minimize(result);
	*/
	result = auto_unserialize(result, 3, NULL);
	auto_serialize_write_dot_file(result, "addition.dot", LASH_EXP_DIGIT);
	auto_free(result);
	

#endif
	
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


	float tim2 = clock();
	printf("Temps d'execution : %d\n", tim2-tim );
	exit(0);

}


automaton* get_un(int order, int alph_max)
{
	automaton* aut = auto_new_empty(sizeof(char));

	if(aut == NULL)
  		lash_perror("automaton creation");

  	uint4 init = 0;

  	if(auto_add_new_state(aut, &(init)) != 0)
			lash_perror("state creation");

	auto_add_new_i_state(aut, init);

	uint4 second = 0;
	if(auto_add_new_state(aut, &(second)) != 0)
			lash_perror("state creation");	
	if(auto_add_new_transition(aut, init, second, 2, createLabel(0,1)) !=0 )
		lash_perror("T0");

	for(int j = 1 ; j < order; j++)
	{
		un_add(aut, second, j, 1, alph_max*2, order, false);
	}

	return aut;
}

void un_add(automaton* aut, uint4 state, int j, int curr, uint1 alph, int order, bool red)
{
	if(curr ==  j+order+1 )
		return;
	uint4 newState = 0;
	if(auto_add_new_state(aut, &newState) != 0)
			lash_perror("state creation");
	if(j == curr)
	{
		for(uint1 x = 2; x <= alph+1; x ++)
		{
			auto_add_new_transition(aut, state, newState, 2, createLabel(x, x-2));

		}
		un_add(aut, newState, j, curr+1, alph, order, false);
	}
	else if(curr < j)
	{
		for(int x = 1 ; x <= alph+1; x++)
		{
			auto_add_new_transition(aut, state, newState, 2, createLabel(x, x-1));
		}
		un_add(aut, newState, j, curr+1, alph, order, false);
	}
	else if(j < curr && curr <= order )
	{
		if(red)
		{
			for(int x = 0; x <= alph; x++)
			{
				auto_add_new_transition(aut, state, newState, 2, createLabel(x,x));
			}
			un_add(aut, newState, j, curr+1, alph, order, red);
		}
		else
		{
			uint4 newState2 = 0;
			auto_add_new_transition(aut, state, newState, 2, createLabel(alph-2, alph-2));
			un_add(aut, newState, j,curr+1, alph, order, true);

			

			if(curr != order)
			{
				auto_add_new_state(aut, &newState2);
				auto_add_new_transition(aut, state, newState2, 2, createLabel(alph-1, alph-1));
				un_add(aut, newState2, j,curr+1, alph, order, false);
			}
			

		}
	}

	else if(order < curr && curr <= j+order)
	{
		for(uint1 i = 0; i <= alph; i++)
		{
			auto_add_new_transition(aut, state, newState, 2, createLabel(i, i+1));
			
		}
		un_add(aut, newState, j, curr+1, alph, order, red);

	}

	if(curr == j+order)
	{
		auto_mark_accepting_state(aut, newState);
	}

}

automaton* get_red(int order, int alph_max)
{
	automaton* aut = auto_new_empty(sizeof(char));

	if(aut == NULL)
  		lash_perror("automaton creation");

  	uint4 init = 0;

  	if(auto_add_new_state(aut, &(init)) != 0)
			lash_perror("state creation");

	auto_add_new_i_state(aut, init);

  	int red_max = alph_max*2;

  	for(uint1 i = 1; i <= red_max; i++)
  	{
  		uint4 second = 0; 
  		if(auto_add_new_state(aut, &second) != 0)
  			lash_perror("second state creation");

  		auto_add_new_transition(aut, init, second,2, createLabel(0,i));

  		red_add(aut, second, i, false, alph_max*2, order);


  	}
  	return aut;

}
/**
* alph: upper bound of the input alphabet (in general 2)
**/
void red_add(automaton* aut, uint4 state, uint1 toRed, bool isRest, uint1 alph, int nbTrans)
{
	if(nbTrans == 0)
		return;

	uint4 newState = 0;
	auto_add_new_state(aut, &newState);

	if(! isRest)
	{
		auto_add_new_transition(aut, state, newState, 2, createLabel(toRed, 0));
		red_add(aut, newState, toRed, true, alph, nbTrans-1);

		if(nbTrans != 1)
		{
			uint4 newState2 = 0;
			auto_add_new_state(aut, &newState2);

			for(uint1 i = toRed+1; i <= alph + 1; i++)
			{
				auto_add_new_transition(aut, state, newState2, 2, createLabel(i, i-toRed));
			}
			red_add(aut, newState2, toRed, false, alph, nbTrans-1);
		}
	}
	else
	{
		for(uint1 i = toRed; i <= alph  + 1; i++)
		{
			auto_add_new_transition(aut, state, newState, 2, createLabel(i, i-toRed));
		}
		red_add(aut, newState, toRed, isRest, alph, nbTrans-1);
	}

	if(nbTrans == 1)
		auto_mark_accepting_state(aut, newState);

}


uint1* concat(int in, uint1* output, int sizeOut, int order)
{
	uint1* res = calloc(2*order, sizeof(int));
	uint1* k = (uint1*) &in;
	int sizeIn = order*sizeof(int);
	for(int i = 0; i < sizeof(int) ; i++)
	{
		res[sizeIn-1-i] = k[sizeof(int)-1-i];
	}
	if(output == NULL)	
		return res;

	
	for(int i = 0; i < sizeOut; i++)
	{
		res[sizeIn+i] = output[i];
	}
	return res;

}

automaton* decalage(automaton* a, int nb_dec, int alph)
{
	automaton* b = auto_copy(a);
	uint4 i_state = 0;
	auto_i_state(a, 0, &i_state);
	auto_remove_i_states(b);
	uint4 newInit= 0;
	auto_add_new_state(b, &newInit);
	auto_add_new_i_state(b, newInit);
	auto_mark_accepting_state(b, newInit);
	uint4 prev = newInit;
	for(int i = 1; i < nb_dec; i++)
	{
		uint4 newState = 0;
		auto_add_new_state(b, &newState);
		for(int j = 0; j <= alph; j++)
		{
			auto_add_new_transition(b, prev, newState, 2, createLabel(j,j));
		}
		auto_mark_accepting_state(b,newState);
		prev = newState;
	}
	

	for(int i = 0; i <= alph; i++)
	{
		auto_add_new_transition(b, prev, i_state, 2, createLabel(i,i));
	}
	return b;
}

void add_loop(automaton* a, int alph)
{
	uint4 init = 0;
	auto_i_state(a, 0, &init);
	
	for(int i = 0; i < auto_nb_states(a); i++)
	{
		if(auto_accepting_state(a, i))
		{
			auto_unmark_accepting_state(a, i);
			auto_add_new_transition(a, i, init, 0, NULL);
		}
	}
	auto_mark_accepting_state(a, init);

}

void final_accepting(automaton* a, int alph_max)
{
	for(int i = 0; i < auto_nb_states(a); i++)
	{
		if(auto_accepting_state(a, i))
		{
			for(uint1 j = 0; j <= alph_max; j++)
			{
				auto_add_new_transition(a, i, i, 2, createLabel(j,j));
			}
		}
	}


}

void add_identity(automaton* a, int alph, int order)
{
	uint4 puits = 0;
	auto_add_new_state(a, &puits);
	int init = 0;  
	auto_i_state(a, 0, &init); 
	uint4 prev = puits;
	auto_add_new_transition(a, init, puits, 0, NULL );
	for(int j = 1; j <= 2*order; j++)
	{
		uint4 next;
		if(auto_add_new_state(a, &next) != 0)
			lash_perror("state creation");

		for(int i = 0; i <= alph; i++)
		{
			auto_add_new_transition(a, prev, next, 2, createLabel(i,i) );
		}
		auto_mark_accepting_state(a, next);
		prev = next;
	}
	
	auto_mark_accepting_state(a, puits);
	
}

automaton* accept_all_size(int alph_max, int size_max)
{
	automaton* a = auto_new_empty(1);
	uint4 init;
	if(auto_add_new_state(a, &init) != 0)
		lash_perror("state creation");
	auto_add_new_i_state(a, init);
	uint4 prev = init;
	auto_mark_accepting_state(a, init);
	for(int i = 0 ; i < size_max; i++)
	{
		uint4 new;
		if(auto_add_new_state(a, &new) != 0)
			lash_perror("state creation");
		auto_mark_accepting_state(a, new);
		for(int j = 0; j <= alph_max; j++)
		{
			uint1 label = j;
			if(auto_add_new_transition(a, prev, new, 1, & label) != 0 )
				lash_perror("transition creattion");
		}
		prev = new;
	}
	return a;
}
