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
automaton* r_normalize(int order)
{
	automaton*  a = auto_new_empty(1);
	if(!a)
		lash_perror("automaton creation");

	uint4 init;
	if(auto_add_new_state(a, &init))
		lash_perror("initial state creation");

	auto_add_new_i_state(a, init);
	auto_mark_accepting_state(a,init);
	uint1 label0[2] = {0,0};
	uint1 label1[2] = {1,1};
	uint1 label3[2] = {1,0};
	auto_add_new_transition(a, init, init, 2, &label0);

	uint4 prev = init;
	uint4 second;
	
	for(int i = 0; i < order; i++)
	{	
		uint4 next;

		if(auto_add_new_state(a, & next ) != 0)
			lash_perror("state creation");
		if(i == 0)
			second = next;

		uint1 label[2] = {0,1};
		if(auto_add_new_transition(a, prev, next, 2, & label ) != 0)
			lash_perror("transition creation");
		if(i == order-1)
		{
			if(auto_add_new_transition(a, next, second, 2, & label0 ) != 0)
			lash_perror("transition creation");	
		}
		prev = next;
	}
	uint1 l2[2] = {1,0};
	if(auto_add_new_transition(a, prev, init, 2, & l2) != 0)
		lash_perror("transition creation");

	prev = init;
	a = auto_reverse(a);
	for(int i = 0; i < order-1; i++)
	{	
		uint4 next;
		if(auto_add_new_state(a, & next ) != 0)
			lash_perror("state creation");
		
		if(auto_add_new_transition(a, prev, next, 2, & label1 ) != 0)
			lash_perror("transition creation");
		if(auto_add_new_transition(a, next, init, 2, & label0 ) != 0)
			lash_perror("transition creation");
		if(i != order-2)
		{
			if(auto_add_new_transition(a, next, second, 2, & label3 ) != 0)
				lash_perror("transition creation");
		}
		auto_mark_accepting_state(a,next);
		prev = next;
	}

	return a;
}

automaton* l_normalize(int order)
{
	automaton* a = auto_new_empty(1);
	if(!a)
		lash_perror("automaton creation");

	uint4 init;
	if(auto_add_new_state(a, &init) != 0)
		lash_perror("initial state creation");

	auto_add_new_i_state(a, init);
	auto_mark_accepting_state(a,init);
	uint1 label0[2] = {0,0};
	auto_add_new_transition(a, init, init, 2, &label0);


	uint4* second;
	if(auto_add_new_state(a, &second) != 0)
		lash_perror("state creation");
	uint1 l1[2] = {0,1};
	if(auto_add_new_transition(a, init, second, 2, &l1) != 0)
		lash_perror("tran creation");
	uint4* prev = second;
	uint1 label[2] = {1,0};
	uint1 label1[2] = {1,1};
	for(int i = 0; i < order-1; i++)
	{	
		uint4 next;
		if(auto_add_new_state(a, & next ) != 0)
			lash_perror("state creation");
		
		if(auto_add_new_transition(a, prev, next, 2, & label ) != 0)
			lash_perror("transition creation");
		if(i == order-2)
		{
			if(auto_add_new_transition(a, next, second, 2, & label1 ) != 0)
			lash_perror("transition creation");	
		}
		prev = next;
	}
	if(auto_add_new_transition(a, prev, init, 2, & label) != 0)
		lash_perror("transition creation");
	prev = init;
	for(int i = 0; i < order-1; i++)
	{	
		uint4 next;
		if(auto_add_new_state(a, & next ) != 0)
			lash_perror("state creation");
		
		if(auto_add_new_transition(a, prev, next, 2, & label1 ) != 0)
			lash_perror("transition creation");
		if(auto_add_new_transition(a, next, init, 2, & label0 ) != 0)
			lash_perror("transition creation");
		if(auto_add_new_transition(a, next, second, 2, &l1 ) != 0)
			lash_perror("transition creation");
		auto_mark_accepting_state(a,next);
		prev = next;
	}

	return a;

}

