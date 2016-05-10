#include "rule.h"

void printRule(rule* r)
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

