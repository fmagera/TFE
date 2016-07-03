#include "addition.h"

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
/*
*	Creates the normalization automaton for fibonacci numeration system 
*
*/
automaton* l_normalize(int order)
{
	automaton*  a = auto_new_empty(1);
	if(!a)
		lash_perror("automaton creation");

	uint4 init;
	if(auto_add_new_state(a, &init))
		lash_perror("initial state creation");

	auto_add_new_i_state(a, init);
	auto_mark_accepting_state(a,init);
	for(uint1 j = 0; j < 2; j++)
	{
		uint1 label[2] = {j,j};
		auto_add_new_transition(a, init, init, 2, &label);
	}
	uint4 prev = init;
	for(int i = 0; i < order; i++)
	{	
		uint4 next;
		if(auto_add_new_state(a, & next ) != 0)
			lash_perror("state creation");
		uint1 label[2] = {1,0};
		if(auto_add_new_transition(a, prev, next, 2, & label ) != 0)
			lash_perror("transition creation");
		prev = next;
	}
	uint1 l2[2] = {0,1};
	if(auto_add_new_transition(a, prev, init, 2, & l2) != 0)
		lash_perror("transition creation");
	return a;
}

automaton* r_normalize(int order)
{
	automaton* a = auto_new_empty(1);
	if(!a)
		lash_perror("automaton creation");

	uint4 init;
	if(auto_add_new_state(a, &init) != 0)
		lash_perror("initial state creation");

	auto_add_new_i_state(a, init);
	auto_mark_accepting_state(a,init);
	for(uint1 j = 0; j < 2; j++)
	{
		uint1 label[2] = {j,j};
		auto_add_new_transition(a, init, init, 2, &label);
	}

	uint4* next;
	if(auto_add_new_state(a, & next) != 0)
		lash_perror("state creation");
	uint1 l1[2] = {0,1};
	if(auto_add_new_transition(a, init, next, 2, &l1) != 0)
		lash_perror("tran creation");
	uint4* prev = next;
	uint1 label[2] = {1,0};
	for(int i = 0; i < order-1; i++)
	{	
		if(auto_add_new_state(a, & next ) != 0)
			lash_perror("state creation");
		
		if(auto_add_new_transition(a, prev, next, 2, & label ) != 0)
			lash_perror("transition creation");
		prev = next;
	}
	if(auto_add_new_transition(a, prev, init, 2, & label) != 0)
		lash_perror("transition creation");

	return a;

}

