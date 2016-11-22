/*************************************************************************************************
*
*		addition.c : contains functions to create the simple bit by bit addition automaton 
*		author : floriane magera
**************************************************************************************************/
#include "addition.h"
automaton* normalised(int order, int alph_max)
{
	automaton* a = auto_new_empty(1);
	if(!a)
		lash_perror("automaton creation");

	uint4 init;
	if(auto_add_new_state(a, &init))
		lash_perror("initial state creation");
	auto_add_new_i_state(a, init);
	auto_mark_accepting_state(a,init);

	uint4 prev = init;
	uint1 zero = 0;

	if(auto_add_new_transition(a, init, init, 1, &zero ))
			lash_perror(" tran creation");

	for(int i = 0 ; i < order-1; i++)
	{
		uint4 new;
		if(auto_add_new_state(a, &new))
			lash_perror("state creation");
		auto_mark_accepting_state(a,new);
		
		for(uint1 j = 1; j <= alph_max;j++)
		{
			if(auto_add_new_transition(a, prev, new, 1, &j ))
				lash_perror(" tran creation");
		}
		if(auto_add_new_transition(a, new, init, 1, &zero ))
			lash_perror(" tran creation");
		prev = new;
	}
	return a;
}

// petit 



automaton* addition(uint1 alph_max)
{
	automaton* a = auto_new_empty(1);
	if(!a)
		lash_perror("automaton creation");

	uint4 init;
	if(auto_add_new_state(a, &init))
		lash_perror("initial state creation");

	auto_add_new_i_state(a, init);
	auto_mark_accepting_state(a,init);

	for(uint1 i = 0; i <= alph_max; i++)
	{
		for(uint1 j = 0; j <= alph_max; j++)
		{
			uint1 label[3] = {i,j, i+j};
			auto_add_new_transition(a, init, init, 3, &label);
		}
	}
	return a;

}

