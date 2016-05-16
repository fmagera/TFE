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




hash_table* createhash_table(int capacity)
{

    hash_table* table = malloc(sizeof(hash_table));
    table->capacity = capacity;
    table->numberofelements = 0;
    table->tab = malloc(capacity*sizeof(hash_entry*));
    

    for(int i = 0; i < capacity ; i++)
    {
        table->tab[i] = NULL;
    }
    return table;
}


void freehash_table(hash_table* hashtable)
{
    hashtable->numberofelements = 0;
    free(hashtable->tab);
}

int insertEntry(hash_table* hashtable, hash_entry* e, int key)
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


size_t getNumRules(const hash_table* hashTable)
{
    return hashTable->numberofelements;
}

bool hasKey(const hash_table* hashtable, const int key)
{
	return (hashtable->tab[key] != NULL);
}

bool existPattern(hash_table* t, pattern* p, int key)
{
	hash_entry* e = t->tab[key];
	if(e == NULL)
		return false;
	while(e->next != NULL)
	{
		pattern* o = ((pattern*) e->payload);
		if(compLexPat(o,p) == 0)
			return true;
		e = e->next;
	}
	return false;
}

rule* getRule(hash_table *t, pattern* p, int key)
{
	hash_entry* e = t->tab[key];
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

void printTableRules(hash_table* t)
{
	for(int i = 0; i < t->capacity; i++)
	{
		if(t->tab[i] != NULL)
		{
			rule* r = ((rule*) t->tab[i]->payload);
			printf("Rule %i :",  i);
			printRule(r);
			hash_entry* e = t->tab[i];
			
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

void printTablePattern(hash_table* t)
{
	for(int i = 0; i < t->capacity; i++)
	{
		if(t->tab[i] != NULL)
		{
			pattern* p = ((pattern*) t->tab[i]->payload);
			printf("Pattern %i :",  i);
			printPattern(p);
			hash_entry* e = t->tab[i];
			
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

void printTableTransitions(hash_table* t)
{
	for(int i = 0; i < t->capacity; i++)
	{
		if(t->tab[i] != NULL)
		{
			transition* p = ((transition*) t->tab[i]->payload);
			printf("Transition %i :",  i);
			printTransition(p);
			hash_entry* e = t->tab[i];
			
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

