#include "composition.h"

uint1* createLabel(uint1 in, uint1 out)
{
	uint1* res = calloc(2, sizeof(char));
	res[0] = in;
	res[1] = out;
	return res;
}


/*
*	This function modifies all the transitions of the automata a, it inserts a composant at a certain position 
*	and gives it all possible values. ln is the aimed length of the transitions after composition
*
*/
automaton * add_composant(automaton* a, int position, int ln, int alph_max)
{
	int init = 0;  
	
	
	//uint1* added_state = calloc(auto_nb_states(a),sizeof(uint1));
	//add_composant_rec(a, position, 0, init, alph_max, added_state, auto_nb_states(a));
	automaton* uni = auto_new_empty(1);
	if(!uni)
		lash_perror("automaton creation");
	auto_add_new_state(uni, &init);
	auto_add_new_i_state(uni, init);
	auto_mark_accepting_state(uni, init);
	for(uint1 i = 0; i <= alph_max; i++)
	{
		auto_add_new_transition(uni, init, init, 1, &i);
	}

	if(position == 0)
	{
		a = auto_seq_product_separ(uni, a, 1, ln-1, NULL);
	}
	else
	{
		a = auto_seq_product_separ(a, uni, ln-1, 1, NULL);
	}
	return a;	
}

void add_composant_rec(automaton* a, int position, int count, uint4 curr, int alph_max, uint1* added, int space)
{
	int nb_out;
	uint4 init;
	auto_i_state(a, 0, &init);

	if(count % 3 == position && added[curr] == 0)
	{
		uint4 new = 0;
		auto_add_new_state(a, &new);
		copy_transitions(a, curr, new);
		auto_remove_trans(a, curr);
		for(uint1 i = 0; i <= alph_max; i++)
		{
			auto_add_new_transition(a, curr, new, 1, &i) ;
		}
		if(auto_accepting_state(a, curr) && position != 0)
		{
			auto_unmark_accepting_state(a, curr);
			auto_mark_accepting_state(a, new);
		}
		if(curr == init && position != 0)
		{
			auto_remove_i_states(a);
			auto_add_new_i_state(a, new);
		}
		added[curr] = 1;

		curr = new;
		count++;
	}
	auto_nb_out_transitions(a, curr, &nb_out);
	if(nb_out == 0)
		return;
	if(count != 0 && auto_accepting_state(a, curr) )
		return;
	for(int i = 0; i < nb_out; i++)
	{
		tran* t = auto_transition(a, curr, i);
		add_composant_rec(a, position, count+1, auto_transition_dest(t), alph_max, added, space);
	}

}

automaton * compose(automaton* a, int ln_tr_a, automaton* b,int ln_tr_b, int alph, int ln)
{
	int l_a = ln_tr_a;
	automaton* aprime = a;
	while(l_a < ln)
	{
		aprime = add_composant(aprime, l_a, l_a+1, alph);
		l_a++;
	}
	
	int l_b = ln_tr_b;
	automaton* bprime = b;
	while(l_b < ln)
	{
		bprime = add_composant(bprime, 0, l_b+1, alph);
		l_b++;
	}	
	auto_serialize_write_dot_file(aprime, "o1.dot", LASH_EXP_DIGIT);
	auto_serialize_write_dot_file(bprime, "o2.dot", LASH_EXP_DIGIT);
	automaton* result = auto_intersection(aprime, bprime);
	return result;
}

void copy_transitions(automaton* a, uint4 s, uint4 d)
{
	uint4 nb;
	auto_nb_out_transitions(a, s, &nb);
	for(int i = 0 ; i < nb; i++)
	{
		tran* t = auto_transition(a, s, i);
		uint1 label = *auto_transition_label_ptr(t,1);
		printf("\t %d", label);
		auto_add_new_transition(a, d, auto_transition_dest(t), 1, &label); 
	}
}
