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
	automaton* result = auto_intersection(aprime, bprime);
	int nb_to_project = (ln_tr_a+ ln_tr_b)-ln;
	if(ln == ln_tr_a || ln == ln_tr_b)
		nb_to_project = 0;
	int l = ln;
	for(int i = 0 ; i < nb_to_project; i ++)
	{
		result = auto_seq_projection_separ(result, l, ln_tr_a -1 -i, NULL);
		l--;
	}
	
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
		auto_add_new_transition(a, d, auto_transition_dest(t), 1, &label); 
	}
}

void new_comp(automaton* a, int order)
{
	hash_tab* path_in = createhash_tab(pow(10,order));
	hash_tab* path_out = createhash_tab(pow(10,order));
	for(int j = 1; j <= order; j++)
	{
		for(int i = 0; i < auto_nb_states(a); i++)
		{

			if(auto_accepting_state(a, i))
			{	
				uint4* path = calloc(2*j,sizeof(uint4));
				uint4* trans = calloc(2*j,sizeof(uint4));
				uint4* path1 = calloc(2*j, sizeof(uint4));
				uint4* trans1 = calloc(2*j, sizeof(uint4));
				uint4* visited = calloc(2*j, sizeof(uint4));
				uint4* visited1 = calloc(2*j, sizeof(uint4));
				new_comp_rec(a, i, 2*j, path, trans, 2*j, path_in, visited);
				get_path_out(a, i, 2*j, path1, trans1, 2*j, path_out, visited1 );
				free(path);
				free(trans);
				free(path1);
				free(trans1);
				free(visited);
				free(visited1);


			}
		}
	}
	
	for(int i = 0; i < path_out->capacity; i++)
	{
		if(path_out->tab[i] == NULL)
			continue;
		path_c* p;
		hash_en* e1 = path_out->tab[i];
		p = ((path_c*) e1->payload);
		
		if(hasKey(path_in,p->key))
		{
			path_c* in;
			hash_en* e2 = path_in->tab[i];
			in = ((path_c*) e2->payload);
			if(in->nb_trans == p->nb_trans)
			{
				
				int equal = 1;
				for(int k = 0; k < p->nb_trans/2 ; k++)
				{
					
					if(in->tran_out[k] != p->tran_in[k])
					{
						equal = 0;
						break;
					}
						
				}
				if(equal)
				{
					uint1* label = malloc(p->nb_trans);
					for(int k = 0; k < p->nb_trans; k++)
					{
						if(k%2 == 0)
							label[k] = in->tran_in[k/2];
						else
						{
							label[k] = p->tran_out[k/2];
						}

					}
					auto_add_new_transition(a, in->state, p->state, p->nb_trans, label);
				}
			}
			while(e2->next != NULL)
			{
				e2 = e2->next;
				in = ((path_c*) e2->payload);
				if(in->nb_trans == p->nb_trans)
				{
					
					int equal = 1;
					for(int k = 0; k < p->nb_trans/2 ; k++)
					{
						if(in->tran_out[k] != p->tran_in[k])
						{
							equal = 0;
							break;
						}
							
					}
					if(equal)
					{
						uint1* label = malloc(p->nb_trans);
						for(int k = 0; k < p->nb_trans; k++)
						{
							if(k%2 == 0)
								label[k] = in->tran_in[k/2];
							else
							{
								label[k] = p->tran_out[k/2];
							}

						}
						auto_add_new_transition(a, in->state, p->state, p->nb_trans, label);
					}
				}
			}

		}
		
		while(e1->next != NULL)
		{
			e1 = e1->next;
			p = ((path_c*) e1->payload);
			if(hasKey(path_in,p->key))
			{
				path_c* in;
				hash_en* e2 = path_in->tab[i];
				in = ((path_c*) e2->payload);
				int equal = 1;
				if(in->nb_trans == p->nb_trans)
				{
					for(int k = 0; k < p->nb_trans/2; k++)
					{
						
						if(in->tran_out[k] != p->tran_in[k])
						{
							equal = 0;
							break;
						}
							
					}
					if(equal)
					{
						uint1* label = malloc(p->nb_trans);
						for(int k = 0; k < 2*p->nb_trans; k++)
						{
							if(k%2 == 0)
								label[k] = in->tran_in[k/2];
							else
							{
								label[k] = p->tran_out[k/2];
							}

						}
						auto_add_new_transition(a, in->state, p->state, p->nb_trans, label);
					}
				}
				while(e2->next != NULL)
				{

					e2 = e2->next;
					in = ((path_c*) e2->payload);
					
					if(in->nb_trans == p->nb_trans)
					{
						
						int equal = 1;
						for(int k = 0; k < p->nb_trans/2 ; k++)
						{
							if(in->tran_out[k] != p->tran_in[k])
							{
								equal = 0;
								break;
							}
								
						}
						if(equal)
						{
							uint1* label = malloc(p->nb_trans);
							for(int k = 0; k < p->nb_trans; k++)
							{
								if(k%2 == 0)
									label[k] = in->tran_in[k/2];
								else
								{
									label[k] = p->tran_out[k/2];
								}

							}
							auto_add_new_transition(a, in->state, p->state, p->nb_trans, label);
						}
					}
				}

			}
			

		}
		
	}
	freehash_tab(path_in);
	freehash_tab(path_out);
	
}

void get_path_out(automaton* a, uint4 state_st, uint4 steps_left, uint4* path, uint4* trans, uint4 ini_nb_st, hash_tab* path_out, uint4* visited)
{
	visited[ini_nb_st-steps_left] = state_st;
	if(steps_left == 0)
	{
		uint1* tab_in = malloc(ini_nb_st/2);
		uint1* tab_out = malloc(ini_nb_st/2);
		int key = 0;
		for(int i = 0 ; i < ini_nb_st; i++)
		{
			tran* t = auto_transition(a, path[i], trans[i]);
			if(i%2)
			{
				tab_out[i/2] = *auto_transition_label_ptr(t,1);
				
			}
			else
			{
				tab_in[i/2] = *auto_transition_label_ptr(t,1);
				
				key += 3* (7* *auto_transition_label_ptr(t,1));
				
			}
		}
		path_c* p = malloc(sizeof(path_c));
		p->nb_trans = ini_nb_st;
		p->tran_in = tab_in;
		p->tran_out = tab_out;
		p->state = state_st;
		p->key = key;

		hash_en* e = malloc(sizeof(hash_en));
		e->payload = p;
		e->key = key;
		e->next = NULL;
				
		insertEntry(path_out,e, e->key);

		return;

	}
	int nb_out = 0;
	auto_nb_out_transitions(a, state_st, &nb_out);
	for(int j = 0; j < nb_out; j++)
	{
		tran* t = auto_transition(a, state_st, j);
		int seen = 1;
		for(int j = 0; j < ini_nb_st-steps_left; j++)
		{
			if(visited[j] == auto_transition_dest(t))
			{
				seen = 0;
				break;
			}
		}
		if(!seen)
			continue;
		
		path[ini_nb_st-steps_left] = state_st;
		trans[ini_nb_st-steps_left] = j;

		get_path_out(a, auto_transition_dest(t), steps_left-1, path,trans, ini_nb_st, path_out, visited );
		
	}
}

void new_comp_rec(automaton *a, uint4 state_end, uint4 steps_left, uint4* path, uint4* trans, uint4 ini_nb_st, hash_tab* path_in, uint4* visited)
{
	visited[ini_nb_st-steps_left] = state_end;
	if(steps_left == 0)
	{
		uint1* tab_in = malloc(ini_nb_st/2);
		uint1* tab_out = malloc(ini_nb_st/2);
		int key = 0;
		int rev = ini_nb_st/2 -1;
		for(int i = 0 ; i < ini_nb_st; i++)
		{
			tran* t = auto_transition(a, path[i], trans[i]);

			if(i%2)
			{
				tab_in[rev-i/2] = *auto_transition_label_ptr(t,1);
				
				
			}
				
			else
			{
				tab_out[rev-i/2] = *auto_transition_label_ptr(t,1);
				key += 3* (7* *auto_transition_label_ptr(t,1));
				
				
			}
		}
		path_c* p = malloc(sizeof(path_c));
		p->nb_trans = ini_nb_st;
		p->tran_in = tab_in;
		p->tran_out = tab_out;
		p->state = state_end;
		p->key = key;

		hash_en* e = malloc(sizeof(hash_en));
		e->payload = p;
		e->key = key;
		e->next = NULL;
				
		insertEntry(path_in,e, e->key);

		return;

		
	}
	for(int i = 0; i < auto_nb_states(a); i++)
	{
		int nb_out = 0;
		auto_nb_out_transitions(a, i, &nb_out);
		int seen = 1;
		for(int j = 0; j < ini_nb_st-steps_left; j++)
		{
			if(visited[j] == i)
			{
				seen = 0;
				break;
			}
		}
		if(!seen)
			continue;
		for(int j = 0; j < nb_out; j++)
		{
			tran* t = auto_transition(a, i, j);
			if(auto_transition_dest(t) == state_end )
			{
				path[ini_nb_st-steps_left] = i;
				trans[ini_nb_st-steps_left] = j;
				new_comp_rec(a, i, steps_left-1, path, trans, ini_nb_st, path_in, visited );
			}
		}
	}
}

