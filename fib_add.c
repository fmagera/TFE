#include "fib_add.h"

automaton* trib_add(char* path)
{
	automaton* a = auto_new_empty(1);

	FILE* f = fopen("msd_trib_addition.txt", "r");

	if(f == NULL)
		return NULL;
	char trash[25];
	uint4 tab[150];
	int init = -1;
	while(fgets(trash, 25, f) != NULL)
	{
		int state, accepting;
		char *p = strchr(trash, '\n');
		if((p-trash) < 10 && sscanf(trash, "%d %d \n", & state, & accepting) == 2)
		{
			auto_add_new_state(a, &tab[state] );
			if(init < 0)
			{
				auto_add_new_i_state(a, tab[state]);
				init = tab[state];
				printf("st: %d\n", state);
			}
			if(accepting)
				auto_mark_accepting_state(a,tab[state]);
		}
	}
	rewind(f);
	uint4 state;
	while(fgets(trash, 25, f) != NULL)
	{
		char *p = strchr(trash, '\n');
		int label[3];
		uint1 label2[3];
		int state_en = -1;
		if(p-trash < 10)
		{
			sscanf(trash, "%d", &state);
		}
		else
		{
			if(sscanf(trash, "%d %d %d -> %d",  & label[0], & label[1], &label[2], &state_en))
			{

				label2[2] = (uint1) label[2];
				label2[1] = (uint1) label[1];
				label2[0] = (uint1) label[0];

				auto_add_new_transition(a, tab[state], tab[state_en], 3, label2);
			}
		}
	}

	fclose(f);
	return a;

}

automaton* fib_addition()
{
	automaton* a = auto_new_empty(1);
	uint4 states[16];
	for(int i = 0; i < 16; i++)
	{
		if(auto_add_new_state(a, &states[i] ) != 0)
			lash_perror("state creation"); 
	}

	auto_add_new_i_state(a, states[0]);
	auto_mark_accepting_state(a, states[0]);
	auto_mark_accepting_state(a, states[6]);
	auto_mark_accepting_state(a, states[14]);
	auto_mark_accepting_state(a, states[11]);
	auto_mark_accepting_state(a, states[9]);
	auto_mark_accepting_state(a, states[2]);

	uint1 label[3] = {0,0,0};
	// transition outgoing zero
	if(auto_add_new_transition(a, states[0], states[0], 3,  label ) != 0)
			lash_perror("state creation"); 

	label[2] = 1;
	if(auto_add_new_transition(a, states[0], states[1], 3,  label ) != 0)
			lash_perror("state creation"); 
	label[0] = 1;
	if(auto_add_new_transition(a, states[0], states[2], 3,  label ) != 0)
			lash_perror("state creation"); 
	label[0] = 0;
	label[1] = 1;
	if(auto_add_new_transition(a, states[0], states[2], 3,  label ) != 0)
			lash_perror("state creation"); 

	// 1
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[1], states[3], 3,  label ) != 0)
			lash_perror("state creation"); 
	label[1] = 1;
	if(auto_add_new_transition(a, states[1], states[5], 3,  label ) != 0)
			lash_perror("state creation");  
	label[0] = 1;
	if(auto_add_new_transition(a, states[1], states[6], 3,  label ) != 0)
			lash_perror("state creation"); 
	label[1] = 0;
	if(auto_add_new_transition(a, states[1], states[4], 3,  label ) != 0)
			lash_perror("state creation"); 

	// 2
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[2], states[0], 3,  label ) != 0)
			lash_perror("state creation"); 
	
	// 3

	label[0] = 1;
	label[1] = 1;
	label[2] = 0;
	if(auto_add_new_transition(a, states[3], states[7], 3,  label ) != 0)
			lash_perror("state creation"); 
	// 4
	label[0] = 0;
	label[1] = 1;
	label[2] = 0;
	if(auto_add_new_transition(a, states[4], states[9], 3,  label ) != 0)
			lash_perror("state creation"); 
	label[2] = 1;
	if(auto_add_new_transition(a, states[4], states[10], 3,  label ) != 0)
			lash_perror("state creation"); 
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[4], states[8], 3,  label ) != 0)
			lash_perror("state creation"); 

	// 5
	label[0] = 1;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[5], states[11], 3,  label ) != 0)
		lash_perror("state creation"); 
	label[2] = 1;
	if(auto_add_new_transition(a, states[5], states[12], 3,  label ) != 0)
		lash_perror("state creation"); 
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[5], states[8], 3,  label ) != 0)
		lash_perror("state creation"); 

	//6
	if(auto_add_new_transition(a, states[6], states[13], 3,  label ) != 0)
		lash_perror("state creation"); 
	label[2] = 1;
	if(auto_add_new_transition(a, states[6], states[14], 3,  label ) != 0)
		lash_perror("state creation"); 
	//7
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[7], states[8], 3,  label ) != 0)
		lash_perror("state creation"); 	
	//8 
	if(auto_add_new_transition(a, states[8], states[3], 3,  label ) != 0)
		lash_perror("state creation"); 	
	label[0] = 1;
	label[1] = 1;
	label[2] = 1;
	if(auto_add_new_transition(a, states[8], states[7], 3,  label ) != 0)
		lash_perror("state creation"); 	
	label[0] = 0;
	label[1] = 1;
	label[2] = 0;
	if(auto_add_new_transition(a, states[8], states[5], 3,  label ) != 0)
		lash_perror("state creation"); 	

	label[0] = 1;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[8], states[4], 3,  label ) != 0)
		lash_perror("state creation");
	label[0] = 1;
	label[1] = 1;
	label[2] = 0;
	if(auto_add_new_transition(a, states[8], states[6], 3,  label ) != 0)
		lash_perror("state creation"); 

	//9 
	label[0] = 1;
	label[1] = 0;
	label[2] = 1;
	if(auto_add_new_transition(a, states[9], states[2], 3,  label ) != 0)
		lash_perror("state creation"); 
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[9], states[0], 3,  label ) != 0)
		lash_perror("state creation"); 		
	label[2] = 1;
	if(auto_add_new_transition(a, states[9], states[1], 3,  label ) != 0)
		lash_perror("state creation"); 	
	//10
	label[0] = 1;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[10], states[4], 3,  label ) != 0)
		lash_perror("state creation"); 	
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[10], states[3], 3,  label ) != 0)
		lash_perror("state creation");
	//11
	if(auto_add_new_transition(a, states[11], states[0], 3,  label ) != 0)
		lash_perror("state creation");
	label[2] = 1;
	if(auto_add_new_transition(a, states[11], states[1], 3,  label ) != 0)
		lash_perror("state creation");
	label[1] = 1;
	if(auto_add_new_transition(a, states[11], states[2], 3,  label ) != 0)
		lash_perror("state creation");
	//12
	label[0] = 0;
	label[1] = 1;
	label[2] = 0;
	if(auto_add_new_transition(a, states[12], states[5], 3,  label ) != 0)
		lash_perror("state creation");
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[12], states[3], 3,  label ) != 0)
		lash_perror("state creation");
	//13
	label[0] = 0;
	label[1] = 0;
	label[2] = 1;
	if(auto_add_new_transition(a, states[13], states[2], 3,  label ) != 0)
		lash_perror("state creation");
	//14
	label[0] = 0;
	label[1] = 1;
	label[2] = 0;
	if(auto_add_new_transition(a, states[14], states[6], 3,  label ) != 0)
		lash_perror("state creation");
	label[0] = 1;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[14], states[6], 3,  label ) != 0)
		lash_perror("state creation");	
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[14], states[15], 3,  label ) != 0)
		lash_perror("state creation");
	//15
	label[0] = 1;
	label[1] = 0;
	label[2] = 1;
	if(auto_add_new_transition(a, states[15], states[12], 3,  label ) != 0)
		lash_perror("state creation");
	label[0] = 1;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[15], states[11], 3,  label ) != 0)
		lash_perror("state creation");
	label[0] = 0;
	label[1] = 1;
	label[2] = 1;
	if(auto_add_new_transition(a, states[15], states[10], 3,  label ) != 0)
		lash_perror("state creation");
	label[0] = 1;
	label[1] = 1;
	label[2] = 1;
	if(auto_add_new_transition(a, states[15], states[2], 3,  label ) != 0)
		lash_perror("state creation");
	label[0] = 0;
	label[1] = 1;
	label[2] = 0;
	if(auto_add_new_transition(a, states[15], states[9], 3,  label ) != 0)
		lash_perror("state creation");
	label[0] = 0;
	label[1] = 0;
	label[2] = 0;
	if(auto_add_new_transition(a, states[15], states[8], 3,  label ) != 0)
		lash_perror("state creation");
	return a;

}