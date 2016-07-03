#include "end_normalize.h"

automaton* end_normalize(hash_tab* states, int alph_max)
{
	automaton*  a = auto_new_empty(1);
	if(!a)
		lash_perror("automaton creation");

	uint4 init;
	if(auto_add_new_state(a, &init))
		lash_perror("initial state creation");

	auto_add_new_i_state(a, init);
	for(int i = 0; i < states->capacity; i++)
	{
		if(states->tab[i] == NULL)
			continue;
		statef* p;
		hash_en* e = states->tab[i];
		p = ((statef*) e->payload);
		pattern* s = p->label;
		if(getMax(s) > alph_max)
		{
			uint1 label[2];
			uint4* next, prev = init;
			for(int j = 0; j < s->size; j++ )
			{
				if(auto_add_new_state(a, & next)!= 0)
					lash_perror("state creation");
				label[0] = (uint1*) s->values[j];
				label[1] = (uint1*) p->output->values[j];
				if(auto_add_new_transition(a, prev, next, 2, & label) != 0)
					lash_perror("Tran creation");
				prev = next;

			}
			auto_mark_accepting_state(a, prev);


		}
	}
	auto_minimize(a);
	return a;
}