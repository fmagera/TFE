#include "HashTable.h"
#DEFINE DELETED = -7;
// TODO: You have to implement the opaque structure
//       and the functions defined in HashTable.h.

typedef struct HashTable_t HashTable
{
	size_t capacity;
	size_t* p = (*hashFunction)(const void* key);
	int* k = (*compareFunction)(const void* a, const void* b);
	int tab[capacity];
	int numberofelements;
};

size_t (*hashFunction)(const void* key)
{
	int  key = chartoint(key);
	return floor(21*((key*0.3)%1);
}

int compareFunction(const void* a, const void* b)
{
	if(*a == *b)
	    return 0;
	else if(*a < *b)
	    return -2;
	else
	    return 2;

 }


HashTable* createHashTable(size_t capacity,
                           size_t (*hashFunction)(const void* key),
                           int (*compareFunction)(const void* a, const void* b))
{

    Hashtable_t Hashtable ;
    Hashtable.capacity = capacity;
    Hashtable.p = &(*hashFunction)(const void* key);
    Hashtable.k = &(*compareFunction)(const void* a, const void* b);
    Hashtable.numberofelements = 0;

    for(int i = 0; i < capacity ; i++)
    {
        Hashtable.tab[i] = DELETED;
    }
    return Hashtable.tab;
}


void freeHashTable(HashTable* hashTable){
    for(i = 0; i < hashtable.capacity; i++){
        hashtable.tab[i]=DELETED;
    }
    hashtable.numberofelement = 0;
}

void* insertElement(HashTable* hashTable, void* key, void* value){
    hashtable.tab[*p(*key)] = *value;
    hashtable.numberofelements++;
    return hashtable.tab[p*(*key)];
}


void* removeElement(HashTable* hashTable, void* key){
    void* k = hashtable.tab[p*(*key)]
    hashtable.tab[p*(*key)] = DELETED;
    return k;
}


size_t getNumElements(const HashTable* hashTable){
    size_t value;
    return hashtable.numberofelements;
}

bool hasKey(const HashTable* hashTable, const void* key){
    if(hashtable.tab[*p(*key)] != DELETED)
        return true;
    return false;
}
