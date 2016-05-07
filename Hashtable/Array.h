/* ========================================================================= *
 * Array
 * An Array is a generic object to store data into a vector.
 *
 * An Array is composed of a vector $values$ and the length of the vector.
 * ========================================================================= */

#ifndef _ARRAY_H_
# define _ARRAY_H_

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

/* Structure */
typedef struct Array_t
{
    void** values;
    size_t length;
} Array;

/* ------------------------------------------------------------------------- *
 * Create an Array object from the content of the file $fileName$. Each line
 * of $fileName$ is stored as an element of the Array object.
 *
 * The Array object must later be deleted by calling freeArray().
 *
 * PARAMETER
 * fileName         Name of a file
 *
 * RETURN
 * Array            A new pointer to an Array object
 * NULL             If an error has occured. The function produces an error
 *                  message on the standard error output.
 * ------------------------------------------------------------------------- */
Array* createArrayFromFile(const char* fileName);

/* ------------------------------------------------------------------------- *
 * Free the allocated memory of the Array object $array$. If the flag
 * $deleteElements$ is true, the function free() is called on each element.
 *
 * PARAMETERS
 * array            An Array object
 * deleteElements   Call the function free() on each element contained in
 *                  $array$
 * ------------------------------------------------------------------------- */
void freeArray(Array* array, bool deleteElements);

/* ------------------------------------------------------------------------- *
 * Print the content of the Array object $array$ on the standard output.
 *
 * PARAMETER
 * array            An Array object
 * ------------------------------------------------------------------------- */
void printArray(const Array* array);

/* ------------------------------------------------------------------------- *
 * Return a new Array object that contains the common elements of two Array
 * objects. To compare elements, the function use the compareFunction()
 * provided by the arrays. Therefore, the elements of the two Array must be
 * compatible in the sense of compareFunction().
 *
 * The new Array object must later be deleted by calling
 * freeArray(array, false). Note that the function copies adresses of common
 * elements. It doesn't clone the content of common elements.
 *
 * PARAMETERS
 * array1       The first container of elements
 * array2       The second container of elements
 *
 * RETURN
 * Array        A new Array object that contains the common elements
 * ------------------------------------------------------------------------- */
Array* getIntersection(const Array* array1, const Array* array2);

/* ------------------------------------------------------------------------- *
 * Compare elements $a$ and $b$. It returns an integer less than,
 * equal to, or greater than zero if $a$ is found, respectively, to be less
 * than, to match, or be greater than $b$.
 *
 * PARAMETERS
 * a            Pointer to the first element
 * b            Pointer to the second element
 *
 * RETURN
 * < 0          If $a$ < $b$
 * = 0          If $a$ == $b$
 * > 0          If $a$ > $b$
 * ------------------------------------------------------------------------- */
int compareElements(const void* a, const void* b);

/* ------------------------------------------------------------------------- *
 * Map an input $element$ to a positive integer value.
 *
 * PARAMETER
 * element      Pointer to the element
 *
 * RETURN
 * hash         The hash value corresponding to $element$
 * ------------------------------------------------------------------------- */
size_t getHashValue(const void* element);

#endif // !_ARRAY_H_
