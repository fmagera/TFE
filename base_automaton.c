/*************************************************************************************************
*
*		base_automaton.c : contains functions to create the base automaton and 
*						to operate on it.
*
*
**************************************************************************************************/

#include "base_automaton.h"




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
