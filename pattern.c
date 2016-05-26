#include "pattern.h"

void freePattern(pattern* p)
{
	if(p == NULL)
		return;
	free(p->values);
	free(p);
}
void freeRule(rule* r)
{
	if(r == NULL)
		return;

	freePattern(r->left);
	freePattern(r->right);
	free(r);
}
void freeTransition(transition* t)
{
	if(t == NULL)
		return;
	if(t->st_state != NULL)
		freePattern(t->st_state);
	if(t->en_state != NULL)
		freePattern(t->en_state);
	
	freePattern(t->output);
	free(t);
}

void freeStatef(statef* s)
{
	if(s == NULL)
		return;

	freePattern(s->label);
	freePattern(s->output);
	free(s);
}

int incPat(pattern* p, const int alph_max)
{
		for(int i = p->size-1; i >= 0; i--)
		{
			if(p->values[i] < alph_max)
			{
				p->values[i]++;
				if(i < p->size-1)
				{
					for(int j = i+1; j < p->size; j++)
						p->values[j] = 0;
				}
				return 0;
			}
		}
		return 1;
}

pattern* append( const pattern* p, const int i)
{
	pattern* copy = copyPattern(p);
	copy->size++;
	copy->values = (int*) realloc(copy->values, copy->size*sizeof(int));
	if(copy->values == NULL)
	{
		return NULL;
	}
		
	

	copy->values[copy->size -1] = i; 
	return copy;
}

int compLexPat(const pattern* p, const pattern* o)
{
	int m = o->size;
	if(p->size < o->size)
		m = p->size;
	for(int i = 0; i < m; i++) 
	{
		if(p->values[i] > o->values[i])
			return -1;
		else if(p->values[i] < o->values[i])
			return 1;
			
	}
	if(o->size == p->size)
		return 0;
	else return (p->size < o->size)? 1:-1;
}

pattern* subPattern(const pattern* p, const int start, const int end)
{
	if(end >= p->size || start >= p->size)
		return p;
	pattern* res = malloc(sizeof(pattern));
	res->size = end-start + 1;
	res->values = malloc(res->size*sizeof(int));
	for(int i = start, j = 0; j < res->size; i++, j++)
	{
		res->values[j] = p->values[i]; 
	}
	return res;
}

pattern** getPrefixes( const pattern* p)
{
	if(p->size <= 1)
		return NULL;

	pattern** result = malloc((p->size-1)*sizeof(pattern*));
	if(result == NULL)
		return NULL;
	int i = 0; 
	while(i < p->size - 1) 
	{
		result[i] = malloc(sizeof(pattern));
		if(result[i] == NULL)
			return NULL;
			
		result[i]->size = i+1;
		result[i]->values = malloc((i+1)*sizeof(int));
		int j = 0;
		for(;j <= i; j++)
		{
			result[i]->values[j] = p->values[j];
		}
		i++;
	}	
	return result;

}

pattern** getSuffixes(const pattern* p)
{
	if(p->size <= 1)
		return NULL;

	pattern** result = malloc((p->size-1)*sizeof(pattern*));
	if(result == NULL)
		printf("souci \n");

	if(result == NULL)
		return NULL;
	int i = 0; 
	while(i < p->size - 1) 
	{
		result[i] = malloc(sizeof(pattern));
		if(result[i] == NULL)
			return NULL;
			
		result[i]->size = i+1;
		result[i]->values = malloc((i+1)*sizeof(int));
		int j = 0, start = (p->size - i -1);
		for(;j <= i; j++)
		{
			result[i]->values[j] = p->values[j+start];
		}
		i++;

	}	
	return result;

}

pattern* getPrefix(const pattern* p, const int sizeOfSuffix)
{
	int end = p->size - sizeOfSuffix;
	pattern* result = malloc(sizeof(pattern));
	result->values = malloc((end)*sizeof(int));
	result->size = end;
	
	for(int i = 0; i < end; i++)
	{
		result->values[i] = p->values[i];
	}
	
	return result;
}

pattern* getSuffix(const pattern* p, const pattern* prefix)
{
	int j = prefix->size;
	pattern* result = malloc(sizeof(pattern));
	result->values = malloc((p->size - prefix->size)*sizeof(int));
	result->size = p->size - prefix->size;
	int i = 0;
	while(j < p->size)
	{
		result->values[i] = p->values[j];
		j++;
		i++;
	}
	return result;
}

pattern* copyPattern(const pattern* p)
{
	pattern* copy = malloc(sizeof(pattern));
	
	copy->size = p->size;
	copy->values = malloc(copy->size*(sizeof(int)));
	for(int i = 0; i < copy->size; i++)
	{
		copy->values[i] = p->values[i];
	}
	return copy;
}

int getMax(const pattern* p)
{
	int max = 0;
	for(int i = 0; i < p->size; i++ )
	{
		if(p->values[i] > max)
			max = p->values[i];
	}
	return max;
}

int getMin(const pattern* p)
{
	int min = p->values[0];
	for(int i = 1; i < p->size; i++ )
	{
		if(p->values[i] < min)
			min = p->values[i];
	}
	return min;
}

void printPattern(const pattern* p)
{
	int i = 0; 
	printf("Pattern : ");
	for(;i < p->size; i++)
	{
		printf("%i",p->values[i]);
	}
	printf("\n");
}

void printStatef(statef* s)
{
	printf("State : ");
	printPattern(s->label);
}

void printTransition(const transition* t)
{
	printf("From ");
	printPattern(t->st_state);
	printf("to ");
	printPattern(t->en_state);
	printf("on %i / ", t->input);
	if(t->output != NULL)
		printPattern(t->output);
		
	printf("\n");
	
}


void printRule(const rule* r)
{
	printf("Rule : \n");
	printPattern(r->left);
	printf("to :");
	printPattern(r->right);
}

rule* createRule(pattern* l, pattern* r)
{
	rule* new_rule = malloc(sizeof(rule));
	new_rule->left = l; 
	new_rule->right = r;
	return new_rule;
}




