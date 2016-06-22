#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "lash.h"
#include "lash-diag.h"
#include "lash-auto.h"
#include "auto-io-print.h"
#include "auto-io-dots.h"
#include "lash-auto-operations.h"

#include "pattern.h"
#include "hashtable_r.h"
#include "genData.h"
#include "auto-minimize.h"

uint1* concat(int in, uint1* output, int sizeOut, int order);
uint1* createLabel(uint1 in, uint1 out);
automaton* getRed(int order, int alph_max);
void redAdd(automaton* aut, uint4 state, uint1 toRed, bool isRest, uint1 alph, int nbTrans);
automaton* getUn(int order, int alph_max);
void unAdd(automaton* aut, uint4 state, int j, int curr, uint1 alph, int order, bool red);


// input format :  m a [ 1 1 1 ] [ 4 2 1 ]"
int main(int argc, char *argv[])
{
	uint1 g = 0;
	printf(" byte %u \n", g);
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
	printTableRules(rules);
	printf("Data generated... \n");

	

	
	if (lash_init() < 0)
    lash_perror("lash_init");
	
	printf("a1\n");

	automaton* a1 = getUn(order, alph_max);
	printf("a2 \n");
	automaton* a2 = getRed(order, alph_max);
/*
	// patterns are at most order long, times 4 because of int type. 
  	automaton* aut = auto_new_empty(sizeof(int)*order);
  	if(aut == NULL)
  		lash_perror("automaton creation");


	for(int i = 0; i < states->capacity; i++)
	{

		if(states->tab[i] == NULL)
			continue;
		statef* p;
		hash_en* e = states->tab[i];
		p = ((statef*) e->payload);
		
		if(auto_add_new_state(aut, &(p->nb)) != 0)
			lash_perror("state creation");


		while(e->next != NULL)
		{
			e = e->next;
			p = ((statef*) e->payload);

			if(auto_add_new_state(aut, &(p->nb)) != 0)
				lash_perror("state creation");

		}
	}
	printf("states created... \n");
	for(int i = 0; i < transitions->capacity; i++)
	{

		if(transitions->tab[i] == NULL)
			continue;
		transition* p;
		hash_en* e = transitions->tab[i];
		p = ((transition*) e->payload);

		statef* in = getState(states, p->st_state, hashKey(states->capacity, p->st_state));
		statef* out = getState(states, p->en_state, hashKey(states->capacity, p->en_state));

		int tab_in = p->input;
		int* tab_out = NULL;
		uint1* t_out = NULL;
		uint1* label;
		if(p->output != NULL)
		{
			tab_out = p->output->values;
			t_out =(uint1*) tab_out;
			label = concat(tab_in, t_out, p->output->size*sizeof(int), order);	
		}
		else
		{
			label = concat(tab_in, t_out, 0, order);	
		}
		


		if(auto_add_new_transition(aut, in->nb, out->nb, 2, label))
	  		lash_perror("transition creation");

		while(e->next != NULL)
		{
			e = e->next;
			p = ((transition*) e->payload);
			tab_in = p->input;

			in = getState(states, p->st_state, hashKey(states->capacity, p->st_state));
			out = getState(states, p->en_state, hashKey(states->capacity, p->en_state));
			t_out = NULL;
			if(p->output != NULL)
			{
				tab_out = p->output->values;
				t_out =(uint1*) tab_out;
				label = concat(tab_in, t_out, p->output->size*sizeof(int), order);	
			}
			else
			{
				label = concat(tab_in, t_out, 0, order);	
			}

			if(auto_add_new_transition(aut, in->nb, out->nb, 2, label))
	  			lash_perror("transition creation");


		}
	}

	auto_print(aut);

	auto_free(aut);	

*/
	auto_merge(a1, a2);
	auto_minimize(a1);
	tran * t = auto_transition(a2, 1, 0);

	printf("Contenu label %i \n", *(auto_transition_label_ptr(t, 1)) );
	auto_serialize_write_dot_file(a2, "output.dot", LASH_EXP_DIGIT);
	//auto_print(a);
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


	exit(0);

}


automaton* getUn(int order, int alph_max)
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
		unAdd(aut, second, j, 1, alph_max*2, order, false);
	}

	return aut;
}

void unAdd(automaton* aut, uint4 state, int j, int curr, uint1 alph, int order, bool red)
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
		unAdd(aut, newState, j, curr+1, alph, order, false);
	}
	else if(curr < j)
	{
		for(int x = 1 ; x <= alph+1; x++)
		{
			auto_add_new_transition(aut, state, newState, 2, createLabel(x, x-1));
		}
		unAdd(aut, newState, j, curr+1, alph, order, false);
	}
	else if(j < curr && curr <= order )
	{
		if(red)
		{
			for(int x = 0; x <= alph; x++)
			{
				auto_add_new_transition(aut, state, newState, 2, createLabel(x,x));
			}
			unAdd(aut, newState, j, curr+1, alph, order, red);
		}
		else
		{
			uint4 newState2 = 0;
			auto_add_new_transition(aut, state, newState, 2, createLabel(alph-2, alph-2));
			unAdd(aut, newState, j,curr+1, alph, order, true);

			

			if(curr != order)
			{
				auto_add_new_state(aut, &newState2);
				auto_add_new_transition(aut, state, newState2, 2, createLabel(alph-1, alph-1));
				unAdd(aut, newState2, j,curr+1, alph, order, false);
			}
			

		}
	}

	else if(order < curr && curr <= j+order)
	{
		for(uint1 i = 0; i <= alph; i++)
		{
			auto_add_new_transition(aut, state, newState, 2, createLabel(i, i+1));
			
		}
		unAdd(aut, newState, j, curr+1, alph, order, red);

	}

	if(curr == j+order)
	{
		auto_mark_accepting_state(aut, newState);
	}

}

automaton* getRed(int order, int alph_max)
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

  		redAdd(aut, second, i, false, alph_max*2, order);


  	}
  	return aut;

}
/**
* alph: upper bound of the input alphabet (in general 2)
**/
void redAdd(automaton* aut, uint4 state, uint1 toRed, bool isRest, uint1 alph, int nbTrans)
{
	if(nbTrans == 0)
		return;

	uint4 newState = 0;
	auto_add_new_state(aut, &newState);

	if(! isRest)
	{
		auto_add_new_transition(aut, state, newState, 2, createLabel(toRed, 0));
		redAdd(aut, newState, toRed, true, alph, nbTrans-1);

		if(nbTrans != 1)
		{
			uint4 newState2 = 0;
			auto_add_new_state(aut, &newState2);

			for(uint1 i = toRed+1; i <= alph + 1; i++)
			{
				auto_add_new_transition(aut, state, newState2, 2, createLabel(i, i-toRed));
			}
			redAdd(aut, newState2, toRed, false, alph, nbTrans-1);
		}
	}
	else
	{
		for(uint1 i = toRed; i <= alph  + 1; i++)
		{
			auto_add_new_transition(aut, state, newState, 2, createLabel(i, i-toRed));
		}
		redAdd(aut, newState, toRed, isRest, alph, nbTrans-1);
	}

	if(nbTrans == 1)
		auto_mark_accepting_state(aut, newState);

}

uint1* createLabel(uint1 in, uint1 out)
{
	uint1* res = calloc(2, sizeof(char));
	res[0] = in;
	res[1] = out;
	return res;
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

