#include "hashtable_r.h"



int hashKey(size_t capacity,pattern* key)
{
	int j = 1, k=0;
	for(int i = 0; i < key->size; i++, j*=2)
	{
		k += (key->values[i]+1)*j;
	}
	return k%capacity;
}




hash_tab* createhash_tab(int capacity)
{

    hash_tab* table = malloc(sizeof(hash_tab));
    table->capacity = capacity;
    table->numberofelements = 0;
    table->tab = malloc(capacity*sizeof(hash_en*));
    

    for(int i = 0; i < capacity ; i++)
    {
        table->tab[i] = NULL;
    }
    return table;
}


void freehash_tab(hash_tab* hashtable)
{
    hashtable->numberofelements = 0;
    free(hashtable->tab);
}

int insertEntry(hash_tab* hashtable, hash_en* e, int key)
{
   hashtable->numberofelements++;
   if(hasKey(hashtable, key))
   {
	   	// add an element to the linked list
	   	e->next = hashtable->tab[key];
	   	hashtable->tab[key] = e;
   }
   else
   {
	   	hashtable->tab[key] = e;
	   	e->next = NULL;
   }
   return key;
}


size_t getNumRules(const hash_tab* hashTable)
{
    return hashTable->numberofelements;
}

bool hasKey(const hash_tab* hashtable, const int key)
{
	return (hashtable->tab[key] != NULL);
}

bool existState(const hash_tab* t, const pattern* p, const int key)
{
	hash_en* e = t->tab[key];
	if(e == NULL)
		return false;
	statef* s =  ((statef*) e->payload);
	if(s == NULL)
		return false;
	pattern* o = s->label;
	if(compLexPat(o,p) == 0)
			return true;
	while(e->next != NULL)
	{
		e = e->next;
		s = ((statef*) e->payload);
		if( s == NULL)
			return false;
		o = s->label;
		if(compLexPat(o,p) == 0)
			return true;
		
	}
	return false;
}

const rule* getRule(const hash_tab *t, const pattern* p, const int key)
{
	hash_en* e = t->tab[key];
	if(e == NULL)
		return NULL;
	rule* r = ((rule*) e->payload);
	pattern* o = (r->left);
	if(compLexPat(o,p) == 0)
			return r;

	while(e->next != NULL)
	{
		e = e->next;
		r = ((rule*) e->payload);
		o = (r->left);
		if(compLexPat(o,p) == 0)
			return r;
		
	}
	return NULL;
}

const statef* getState(const hash_tab *t, const pattern* p, const int key)
{
	hash_en* e = t->tab[key];
	if(e == NULL)
		return NULL;
	statef* r = ((statef*) e->payload);
	pattern* o = (r->label);
	if(compLexPat(o,p) == 0)
			return r;

	while(e->next != NULL)
	{
		e = e->next;
		r = ((statef*) e->payload);
		o = (r->label);
		if(compLexPat(o,p) == 0)
			return r;
		
	}
	return NULL;
}

void printTableRules(const hash_tab* t)
{
	for(int i = 0; i < t->capacity; i++)
	{
		if(t->tab[i] != NULL)
		{
			rule* r = ((rule*) t->tab[i]->payload);
			printf("Rule %i :",  i);
			printRule(r);
			hash_en* e = t->tab[i];
			
			while(e->next != NULL)
			{
				e = e->next;
				r = ((rule*) e->payload);
				printf("Rule %i :",  i);
				printRule(r);
			}
		}
	}
}

void printTablePattern(const hash_tab* t)
{
	for(int i = 0; i < t->capacity; i++)
	{
		if(t->tab[i] != NULL)
		{
			pattern* p = ((pattern*) t->tab[i]->payload);
			printf("Pattern %i :",  i);
			printPattern(p);
			hash_en* e = t->tab[i];
			
			while(e->next != NULL)
			{
				e = e->next;
				p = ((pattern*) e->payload);
				printf("Pattern %i :",  i);
				printPattern(p);
			}
		}
	}
}

void printTableStates(const hash_tab* t)
{
	for(int i = 0; i < t->capacity; i++)
	{
		if(t->tab[i] != NULL)
		{
			statef* p = ((statef*) t->tab[i]->payload);
			printf("State %i :",  i);
			printPattern(p->label);
			if(p->output != NULL)
			{
				printf("\t ");
				printPattern(p->output);
			}
			hash_en* e = t->tab[i];
			
			while(e->next != NULL)
			{
				e = e->next;
				p = ((statef*) e->payload);
				printf("State %i :",  i);
				printPattern(p->label);
				if(p->output != NULL)
				{
					printf("\t ");
					printPattern(p->output);
				}
			}
		}
	}
}

void printTableTransitions(const hash_tab* t)
{
	for(int i = 0; i < t->capacity; i++)
	{
		if(t->tab[i] != NULL)
		{
			transition* p = ((transition*) t->tab[i]->payload);
			printf("Transition %i :",  i);
			printTransition(p);
			hash_en* e = t->tab[i];
			
			while(e->next != NULL)
			{
				e = e->next;
				p = ((transition*) e->payload);
				printf("Transition %i :",  i);
				printTransition(p);
			}
		}
	}
}


