#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "lash.h"
#include "lash-diag.h"
#include "lash-auto.h"
#include "auto-io-print.h"

#include "pattern.h"
#include "hashtable_r.h"
#include "genData.h"

uint1* concat(int in, uint1* output, int sizeOut, int order);

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

	// patterns are at most 2*order long, times 4 because of int type. 
  	automaton* aut = auto_new_empty(8*order);
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

uint1* concat(int in, uint1* output, int sizeOut, int order)
{
	uint1* res = calloc(4*order, sizeof(int));
	uint1* k = (uint1*) &in;
	int st = 2*order*sizeof(int)-(sizeof(int));
	for(int i = 0; i < sizeof(int) ; i++)
	{
		res[st+i] = k[i];
		printf("[%X] \n", k[i]);
	}
	if(output == NULL)	
		return res;
	int sizeIn = 2*order*sizeof(int);
	for(int i = sizeIn-1; i > sizeIn - sizeOut; i--)
	{
		res[sizeIn+i] = output[i];
	}
	return res;

}

