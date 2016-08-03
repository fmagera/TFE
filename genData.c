#include "genData.h"

void genData(const int order, const int alph_max, const int* values, hash_tab* rules, hash_tab* states, hash_tab* transitions)
{
	genRules(alph_max, order, values, rules, states);


	for(int i = 0; i < states->capacity; i++)
	{
		if(states->tab[i] == NULL)
			continue;
		statef* p;
		hash_en* e = states->tab[i];
		p = ((statef*) e->payload);
		pattern* s = p->label;
		

		genTransitions(states, rules, transitions, alph_max, order, s);
		while(e->next != NULL)
		{
			e = e->next;
			p = ((statef*) e->payload);
			s = p->label;

			genTransitions(states, rules, transitions, alph_max, order, s);
		}

	}
	
	for(int i = 0; i < states->capacity; i++)
	{

		if(states->tab[i] == NULL)
			continue;
		hash_en* e = states->tab[i];
		statef* s = ((statef*) e->payload);
		pattern* o = copyPattern(s->label);
		outputState(o, order, values, alph_max, rules);
		s->output = o;
		while(e->next != NULL)
		{
			e = e->next;
			statef* p1 = ((statef*) e->payload);
			pattern* o1 = copyPattern(p1->label);
			outputState(o1, order, values, alph_max, rules);
			p1->output = o1;
		}

	}


	
	
}

void genRules(const int alphMax, const int order, const int* values, hash_tab* t, hash_tab* states )
{
	
	int p = alphMax*2 - 2;
	for(int h = 0; h <= p; h++)
	{
		pattern i;
		i.size = order-1;
		i.values = malloc(i.size*sizeof(int));
		for(int j = 0; j < i.size; j++) 
		{
			i.values[j] = 0; 
		}
		
		do
		{
			for(int l = 1; l <= p+3; l++)
			{
				pattern* left;
				left = malloc(sizeof(pattern));
				left->size = order+1;
				left->values = malloc((order+1)*sizeof(int));
				
				pattern* right;
				right = malloc(sizeof(pattern));
				right->size = order+1;
				right->values = malloc((order+1)*sizeof(int));
				
				left->values[0] = h;
				right->values[0] = h+1;
				for(int j = 1; j < order; j++)
				{
					left->values[j] = p+1+i.values[j-1];
					right->values[j] = p+i.values[j-1]; 
				}
				left->values[order] = l;
				right->values[order] = l-1;
				
				int ret = getMin(subPattern(left, 1, left->size -1));
				if(ret > 1)
				{
					right->values[0] += (ret-1);
					for(int j = 1; j < right->size; j++)
					{
						right->values[j] -= (ret-1);
					}
					
				}				
				
				rule* new_rule = malloc(sizeof(rule));
				new_rule->left = left;
				new_rule->right = right;
				
				
			
				
				hash_en* e = malloc(sizeof(hash_en));
				e->payload = new_rule;
				e->key = hashKey(t->capacity, new_rule->left);
				e->next = NULL;
				
				insertEntry(t,e, e->key);
				
				
				// etats qui en decoulent
				pattern** prefix_left = getPrefixes(left);
				for(int n = 0; n < left->size-1; n++)
				{
					// collisions et unicite !!
					statef* st = malloc(sizeof(statef));
					st->label = prefix_left[n];
					st->output = NULL;
					st->nb = -1;


					hash_en* entry = malloc(sizeof(hash_en));
					entry->payload = st;
					entry->key = hashKey(states->capacity, prefix_left[n]);
					entry->next = NULL;
					if(! existState(states,prefix_left[n], entry->key ))
						insertEntry(states, entry, entry->key );
				}
				
			}
			
			for(int j = 1; j < order; j++) 
			{
				
				pattern* iprime = subPattern(&i,0, j-1);
				if(p+ iprime->values[j-1] -1 >= 0)
				{
					pattern* nu = malloc(sizeof(pattern));
					nu->size = order;
					nu->values = malloc(nu->size*sizeof(int));
					for(int l = 0; l < nu->size; l++) 
					{
						nu->values[l] = 0; 
					}
	
					do {

						pattern* nuprime = subPattern(nu,0, order-j-1);

						pattern* tocomp = malloc(sizeof(pattern));
						tocomp->size = order-j;
						tocomp->values = malloc((order-j)*sizeof(int));
						
						for(int k = 0; k < order-j-1; k++)
						{
							tocomp->values[k]= p+1;
						}
						
						tocomp->values[order-j-1] = 1;

						if(compLexPat(nuprime, tocomp) <= 0)
						{
							free(nuprime);
							freePattern(tocomp);
							continue;
						}
						freePattern(tocomp);
						
						pattern* left2 = malloc(sizeof(pattern));
						left2->size = order+j+1;
						left2->values = malloc((order+j+1)*sizeof(int));
						
						pattern* right2 = malloc(sizeof(pattern));
						right2->size = order+j+1;
						right2->values = malloc((order+j+1)*sizeof(int));
						
						left2->values[0] = h;
						right2->values[0] = h+1;
						
						for(int k = 0; k < iprime->size; k++)
						{
							left2->values[k+1]= p+iprime->values[k]+1;
							right2->values[k+1]= p+iprime->values[k];
							if(k == j-1)
							{
								right2->values[k+1]--;
							}
						}
						for(int k = 0; k < nu->size; k++) 
						{
							left2->values[j+k+1] = nu->values[k];
							right2->values[j+k+1] = nu->values[k];
							if(k >= order-j)
							{
								right2->values[j+k+1]++;
							}	
						}
						
						rule* new_rule2 = malloc(sizeof(rule));
						new_rule2->left = left2;
						new_rule2->right = right2;
						
						hash_en* e = malloc(sizeof(hash_en));
						e->payload = new_rule2;
						e->key = hashKey(t->capacity, new_rule2->left);
						e->next = NULL;
				
						insertEntry(t, e, e->key);
										
						
						pattern** prefixes_st = getPrefixes(left2);
						
						for(int k = 0; k < left2->size - 1; k++)
						{
							statef* st = malloc(sizeof(statef));
							st->label = prefixes_st[k];
							st->output = NULL;
							st->nb = -1;

							hash_en* entry = malloc(sizeof(hash_en));
							entry->payload = st;
							entry->key = hashKey(states->capacity, prefixes_st[k]);
							entry->next = NULL;
							if(!existState(states, prefixes_st[k], entry->key))
								insertEntry(states, entry, entry->key);
							
						}
						freePattern(nuprime);				
						
					}while(incPat(nu, p+2) == 0);
					freePattern(nu);
				}
				freePattern(iprime); 
			}
		}while(incPat(&i, 2) == 0);
		free(i.values);
	}
	
}


void expand(pattern* p,const int order, const int* values)
{
	int sum = 0;
	int val = 0;
	int dep = 0;
	if(p->size > order)
		dep = p->size - order;
		
	for(int i = 0, j  = dep; j < p->size; i++, j++)
	{
		sum += values[i];
		val += values[i]*p->values[j];
		 
	}
	int min = val/sum;

	

	for(int i = dep; i < p->size; i++)
		p->values[i] = min;	
	


	int r = val - min*sum;
	for(int i = dep; i < p->size; i++)
	{
				
			if(values[i-dep] == r)
			{
				r = 0;
				p->values[i]++;
			}
			
			if(r > values[i-dep]) 
			{
				
				p->values[i] += 1;
				r -= (values[i-dep]);	
			}
			
	}
}

int applyRules( const hash_tab* rules, pattern* p, const int order)
{
	
	if(p->size <= order)
		return 0;
	
		
	// need to generate prefixes of size m+1 - 2m-1
	for(int i = order+1; i <= 2*order-1 && i <= p->size; i++)
	{
		for(int j = 0; j <= p->size - i; j++)
		{
			
			if(p->values[j] == 0)
			{

				pattern* sub = subPattern(p,j, i+j-1);

				int key = hashKey(rules->capacity, sub );
				rule* r = getRule(rules, sub, key);
				if(r != NULL)
				{
					freePattern(sub);
					sub = r->right;
					for(int k = 0; k < sub->size; k++)
					{
						p->values[j+k] = sub->values[k];
					}
					return 1;
				}
				freePattern(sub);
			}
		}
	}	
	
	return 0;
	
}


void outputState(pattern* output, const int order, const int* value,const int max_alph, const hash_tab* rules)
{
	
	int max = getMax(output);
	if(max <= max_alph)
		return;
	expand(output, order, value);
	while( applyRules(rules, output, order) == 1  )
	{
		expand(output, order, value);
	}
}


//check state representation is gone from patter to state, so check everything coming from states

void genTransitions(const hash_tab* states, const hash_tab* rules, hash_tab* transitions,const  int alph_max,const int order,
 const pattern* stat)
{
	for(int i = 0; i <= alph_max*2; i ++)
	{
		pattern* current = append(stat, i);

		rule *r = getRule(rules, current, hashKey(rules->capacity, current));
		if(r == NULL)
		{
			if(existState(states, current,hashKey(states->capacity, current) ))
			{

				transition* t = malloc(sizeof(transition));
				statef* s = getState(states, current, hashKey(states->capacity, current));

				t->st_state = copyPattern(stat);
				t->en_state = copyPattern(s->label);
				t->input = i;
				t->output = NULL;
				
				hash_en* e = malloc(sizeof(hash_en));

				e->payload = t;
				e->key = hashKey(transitions->capacity, stat);
				e->next = NULL;
				insertEntry(transitions, e, e->key);
			}
			else 
			{
				pattern** suffixes = getSuffixes(current);

				for(int j = current->size-2; j >= 0; j--)
				{
					pattern* out = getPrefix(current, suffixes[j]->size);
					if(existState(states, suffixes[j],hashKey(states->capacity, suffixes[j]) ) && getMax(out) <= alph_max*2 )
					{
						transition* t = malloc(sizeof(transition));
						pattern* copy = copyPattern(suffixes[j]);
						t->st_state = copyPattern(stat);
						t->en_state = copy;
						t->input = i;
						t->output = copyPattern(out);
				
						hash_en* e = malloc(sizeof(hash_en));
						e->payload = t;
						e->key = hashKey(transitions->capacity, stat);
						e->next = NULL;
						insertEntry(transitions, e, e->key);
						freePattern(out);
						break;
					}
					freePattern(out);
				}

				for(int j = current->size - 2; j >= 0 ; j-- )
				{
					freePattern(suffixes[j]);
				}
				free(suffixes);
			}
		}
		else
		{
			pattern* next = r->right;
			pattern** suffixes = getSuffixes(next);

			if(suffixes == NULL)
				printf("Aille \n");
			bool done = false;
			for(int j = next->size - 2; j >= 0  && !done; j-- )
			{
				pattern* out = getPrefix(next, suffixes[j]->size);
				if(existState(states, suffixes[j],hashKey(states->capacity, suffixes[j]) ) && getMax(out) <= alph_max*2 )
				{
						pattern* copy = copyPattern(suffixes[j]);
						transition* t = malloc(sizeof(transition));
						t->st_state = copyPattern(stat);
						t->en_state = copy;
						t->input = i;
						t->output = copyPattern(out);
				
						hash_en* e = malloc(sizeof(hash_en));
						e->payload = t;
						e->key = hashKey(transitions->capacity, stat);
						e->next = NULL;
						insertEntry(transitions, e, e->key);
						done = true;
				}
				freePattern(out);
			}
			for(int j = next->size - 2; j >= 0; j-- )
			{
				freePattern(suffixes[j]);
			}
			free(suffixes);
			if(!done)
			{
				if(existState(states, next,hashKey(states->capacity, next) ))
				{
					transition* t = malloc(sizeof(transition));
					t->st_state = copyPattern(stat);
					t->en_state = copyPattern(next);
					t->input = i;
					t->output = NULL;
				
					hash_en* e = malloc(sizeof(hash_en));
					e->payload = t;
					e->key = hashKey(transitions->capacity, stat);
					e->next = NULL;
					insertEntry(transitions, e, e->key);
				}
				
			}

		}
		freePattern(current);
	}
}



