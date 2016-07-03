#include "lash.h"
#include "lash-diag.h"
#include "lash-auto.h"
#include "auto-io-print.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{

  if (lash_init() < 0)
    lash_perror("lash_init");


  automaton* aut = auto_new_empty(1);
  if(aut == NULL)
  	lash_perror("automaton creation");

  uint4 p, q;
  if(auto_add_new_state(aut, &p) != 0)
  		lash_perror("state creation");
  
  if(auto_add_new_state(aut, &q) != 0)
  		lash_perror("state creation");
  		
  uint1 l = 0;
  if(auto_add_new_transition(aut, p, q, 1, &l))
  		lash_perror("transition creation");
  		
	auto_print(aut);

  auto_free(aut);	

  if (lash_end() < 0)
    lash_perror("lash_end");

  printf("### statistics ###\n");
  printf("residual memory : %llu byte(s).\n", lash_get_mem_usage());
  printf("max memory      : %llu byte(s).\n", lash_get_max_mem_usage());
  printf("### end of the test ###\n");   

  exit(0);
 }
