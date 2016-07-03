#include "test_auto.h"

automaton* test_automata(automaton* a, uint1* in, int ln, int order)
{
	if(in == NULL)
		return NULL;


	automaton* b = create_word_auto(in, ln);
	automaton* res;
	for(int i = 0; i < order; i++)
	{
		res = compose(b,1,a, 2, 3, 2);
		res = auto_seq_projection_separ(res, 2, 0, NULL);
		auto_minimize(res);
	}
	
	auto_serialize_write_dot_file(res, "test.dot", LASH_EXP_DIGIT);
	return res;
}

automaton* create_word_auto(uint1* in, int ln)
{

	automaton* a = auto_new_empty(1);
	if(!a)
		lash_perror("auto creation");
	uint4 init = 0;
	if(auto_add_new_state(a, &init)!=0)
		lash_perror("init state creation");
	if(auto_add_new_i_state(a, init) != 0)
		lash_perror("init state marking");
	uint4* prev = init;
	for(int i = 0; i < ln; i++)
	{
		uint4* next = 0;
		if(auto_add_new_state(a, &next) != 0)
			lash_perror("state creation");
		if(auto_add_new_transition(a, prev, next, 1, &in[i]) != 0)
			lash_perror("transition adding");
		prev = next;
	}
	auto_mark_accepting_state(a, prev);
	auto_serialize_write_dot_file(a, "word.dot", LASH_EXP_DIGIT);
	return a;

}