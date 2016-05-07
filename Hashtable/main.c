/* ========================================================================= *
 * Main Function
 * ========================================================================= */

#include "Array.h"

#include <stdio.h>

int main(int argc, char** argv)
{
  // Check arguments
  if (argc != 3)
  {
    printf("Usage: %s <File> <File>\n", argv[0]);
    return -1;
  }

  // Load files
  Array* firstSet = createArrayFromFile(argv[1]);
  if (!firstSet)
    return -1;

  Array* secondSet = createArrayFromFile(argv[2]);
  if (!secondSet)
    return -1;

  // Compute intersection
  Array* intersection = getIntersection(firstSet, secondSet);
  if (!intersection)
  {
    printf("An error has occured while computing the insection of files: \n");
    printf("- %s\n", argv[1]);
    printf("- %s\n", argv[2]);
    return 3;
  }

  printArray(intersection);

  // Free memory
  freeArray(firstSet, true);
  freeArray(secondSet, true);
  freeArray(intersection, false);

  return 0;
}
