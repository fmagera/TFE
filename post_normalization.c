#include <stdlib.h>
#include <stdio.h>
#include "pattern.h"


void postNormalize(pattern* p, int order, int* value);
void genRules(int alphMax, int order, int* values);
void expand(pattern* p, int order, int* values, int alph_max);

// input format :  m [ 1 1 1 ] [ 4 2 1 ]"
int main(int argc, char *argv[])
{


	if(argc < 3)
		return 1;
	
	int order = atoi(argv[1]);
	int coefficients[order];
	int values[order];
	int i = 2,j = 0, k = 0;

	for( ; i < argc && *argv[i] != '['; i++);

	j = i+1;
	while(*argv[j]  != ']')
	{			 
		coefficients[k] = atoi(argv[j]);
		j++;
		k++;				
	}

	i = j;
	k = 0;
	for( ; i < argc && *argv[i] != '['; i++);

	j = i+1;
	while(*argv[j]  != ']')
	{	 
		values[k] = atoi(argv[j]);
		j++;
		k++;				
	}
	
	int tab[3] = {1,0,5};	
	pattern* p = malloc(sizeof(pattern));
	p->values = &tab;
	p->size = 3;
	expand(p, order, values, 2);
	postNormalize(p, order,values);
	printPattern(p);

	
	//genRules(1, order, values);
	/*
	
	incPat(p, 1);
	printPattern(p);
	pattern** prefixes = getPrefixes(p);
	
	for(i = 0; i < p->size-1; i++)
	{
		printPattern(prefixes[i]);
		printPattern(getSuffix(p, prefixes[i]));
	}
	*/
	
}

void genRules(int alphMax, int order, int* values)
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
			for(int l = 1; l <= p+2; l++)
			{
				pattern* left;
				left = malloc(sizeof(pattern));
				left->size = order+1;
				left->values = malloc((order+1)*sizeof(int));
				left->values[0] = h;
				for(int j = 1; j < order; j++)
				{
					left->values[j] = p+1+i.values[j-1]; 
				}
				left->values[order] = l;
				//printPattern(left);
				// etats qui en decoulent
				pattern** prefix_left = getPrefixes(left);
				/*			
				printPattern(prefix_left[1]);
				postNormalize(prefix_left[1], order, values);
				printPattern(prefix_left[1]);
				*/
			}
			
			for(int j = 1; j < order; j++) 
			{
				
				pattern* iprime = subPattern(&i, j);
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

						pattern* nuprime = subPattern(nu, order-j);

						pattern* tocomp = malloc(sizeof(pattern));
						tocomp->size = order-j;
						tocomp->values = malloc((order-j)*sizeof(int));
						
						for(int k = 0; k < order-j-1; k++)
						{
							tocomp->values[k]= p+1;
						}
						
						tocomp->values[order-j-1] = 1;

						if(compLexPat(nuprime, tocomp) <= 0)
							continue;
						
						pattern* left2 = malloc(sizeof(pattern));
						left2->size = order+j+1;
						left2->values = malloc((order+j+1)*sizeof(int));
						left2->values[0] = h;
						
						for(int k = 0; k < iprime->size; k++)
						{
							left2->values[k+1]= p+iprime->values[k]+1;
						}
						for(int k = 0; k < nu->size; k++) 
						{
							left2->values[j+k+1] = nu->values[k];	
						}
						printPattern(left2);
						
						pattern** prefixes_st = getPrefixes(left2);
						for(int k = 0; k < left2-> size-1; k++)
						{
							printf("State : ");
							printPattern(prefixes_st[k]);
							pattern* suffix;
							if(prefixes_st[k]->size > order)
								suffix = getSuffix(prefixes_st[k],subPattern(prefixes_st[k], prefixes_st[k]->size-order) );
							else {
								suffix = prefixes_st[k];
							}
							printPattern(suffix);
							postNormalize(suffix, order, values);
							printPattern(suffix);
							
						}
												
						
					}while(incPat(nu, p+2) == 0);
				} 
			}
		}while(incPat(&i, 2) == 0);
	}
		
	
}

void postNormalize(pattern* p, int order, int* value)
{
	int size_pat = p->size;
	if(p->size > order)
	{
		size_pat = order;
	}
			
	int i;
	
	int max = 0;
	for(i = 0; i < p->size; i++ )
	{
		if(p->values[i] > max)
			max = p->values[i];
	}
	
	int val = 0;
	int j = p->size-1;
	i = order - 1;
	while(i >= 0 && (j >= 0 && j >= p->size - order)) 
	{
		val += p->values[j]*value[i];
		i--;
		j--;
	}

	int r = val;
	int max_rep = 0;
	for(int i = 0; i < order; i++)
	{
		max_rep += (max-1)*value[i];
	}
	if(r > max_rep)
	{
		expand(p, order, value, max);
		return;
	}
		

	int* normalized = malloc((order)*sizeof(int));
	if(normalized == NULL)
		return;	
	i = 0;
	while(i < order)
	{
		normalized[i] = 0;
		if(r == value[i])
		{
			normalized[i] = 1;
			r -= value[i];
		}
		if(r > value[i]) 
		{
			int j = 1;
			while(r < (max-j)*value[i])
			{
				j++;
			}

			if(r - (max - j)*value[i] >= 0)
			{
				normalized[i] = (max - j);
				r -= (max - j)*value[i];		
			}
				
		}
		
		i++;
	}
	if(p->size <= order)
	{
		p->values = normalized;
		p->size = order;
	}
	else
	{
		for(int i = p->size - order, j= 0; i < p->size; i++, j++)
			p->values[i] = normalized[j];
	}		
}

void expand(pattern* p, int order, int* values, int alph_max)
{
	int dep = 0;
	if(p->size > order)
		dep = p->size - order;
	int found = 0;
	int r = 0;
	for(int i = dep; i < p->size; i++)
	{
		if(p->values[i] == alph_max && found == 0 && i < p->size-1)
		{
			p->values[i]--;
			found = 1;
			r = values[i-dep];
			continue;
		}
		
		if(found == 1 && r > 0)
		{
			if(values[i-dep] == r)
			{
				r = 0;
				p->values[i]++;
			}
			
			if(r > values[i-dep]) 
			{
				int j = r/values[i-dep];
				
				if(p->values[i] >= alph_max && i < p->size)
					j--;
				p->values[i] +=j;
				r -= j*(values[i-dep]);	
			}
			
		}
		
			
	}
	printPattern(p);
}



