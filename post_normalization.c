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
/*
	if (lash_init() < 0)
    lash_perror("lash_init");


  	automaton* aut = auto_new_empty(1);
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
	
			
	  
	/*
	uint1 l = 0;
	if(auto_add_new_transition(aut, p, q, 1, &l))
	  		lash_perror("transition creation");
	
	auto_print(aut);

	auto_free(aut);	

	if (lash_end() < 0)
	    lash_perror("lash_end");

	printf("### statistics ###\n");
	printf("residual memory : %llu byte(s).\n", lash_get_mem_usage());
	printf("max memory      : %llu byte(s).\n", lash_get_max_mem_usage());
	printf("### end of the test ###\n");   
*/
	exit(0);

}

