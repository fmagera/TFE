/* ========================================================================= *
 * StringArray
 * StringArray is an implementation of Array in which elements are strings.
 * ========================================================================= */

#include "Array.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* Constant */
static const size_t MAX_LINE_SIZE = 128;

/* ------------------------------------------------------------------------- *
 * Read a line of characters from the file stream $file$ and returns the data
 * as an array of char. A line is defined as a sequence of characters followed
 * by a line feed \n. Lines containing more than $MAX_LINE_SIZE$ characters
 * are splitted into smaller lines of size of at most $MAX_LINE_SIZE$. Empty
 * lines are skipped.
 *
 * The array must later be deleted by calling free().
 *
 * PARAMETER
 * file         File stream
 *
 * RETURN
 * string       A new pointer to an array of characters that represents the
 *              line read from the file stream $file$.
 * NULL         The function returns NULL when it has reached the end of the
 *              file. On error, the function also returns NULL.
 * ------------------------------------------------------------------------- */
static char* readLine(FILE* file);

Array* createArrayFromFile(const char* fileName)
{
  // Open file
  FILE* file = fopen(fileName, "r");
  if (!file)
  {
    fprintf(stderr, "Error while loading file '%s': %s\n",
            fileName, strerror(errno));
    return NULL;
  }

  // Create the Array
  Array* result = (Array*)malloc(sizeof(Array));
  if (!result)
  {
    fprintf(stderr, "Memory allocation error while reading file %s\n", fileName);
    return NULL;
  }
  result->values = NULL;
  result->length = 0;

  // Read file line by line
  char* line = readLine(file);
  while (line != NULL)
  {
    result->length++;
    result->values = realloc(result->values, result->length * sizeof(void*));
    if (!result->values)
    {
      fprintf(stderr, "Memory allocation error while reading file %s\n", fileName);
      free(result);
      return NULL;
    }

    result->values[result->length - 1] = line;
    line = readLine(file);
  }

  fclose(file);
  return result;
}

static char* readLine(FILE* file)
{
  // Read a line
  char* line = (char*)calloc(MAX_LINE_SIZE, sizeof(char));
  if (!line)
    return NULL;

  if (!fgets(line, MAX_LINE_SIZE - 1, file))
  {
    free(line);
    return NULL;
  }

  // Remove tailing newline
  size_t lastCharIndex = strlen(line) - 1;
  if (line[lastCharIndex] == '\n')
    line[lastCharIndex--] = '\0';

  // Skip empty line
  if (lastCharIndex == (size_t)-1)
  {
    free(line);
    return readLine(file);
  }

  // Adjust memory size
  line = realloc(line, (lastCharIndex + 2) * sizeof(char));
  return line;
}

void freeArray(Array* array, bool deleteElements)
{
  if (deleteElements)
  {
    for (size_t i = 0; i < array->length; ++i)
      free((char*)array->values[i]);
  }

  free(array->values);
  free(array);
}

void printArray(const Array* array)
{
  for (size_t i = 0; i < array->length; ++i)
    printf("%s\n", (char*)array->values[i]);
}
