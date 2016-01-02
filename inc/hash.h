/*
 * hash.h
 *
 *  Created on: Oct 10, 2015
 *      Author: thor
 */

#ifndef INC_HASH_H_
#define INC_HASH_H_


/* Hash table items
 */
typedef struct hash_table_link_struct {
	unsigned long hash;
	void *key;
	void *data;
	struct hash_table_link_struct *next;
} * HashTableLink;

// hash table structure
typedef struct hash_table_struct {
	int size;
	int num_buckets;
	struct hash_table_link_struct **table;

	// function used to hash:
	unsigned long (*hash_func)( void * );

	// function to check for equality
	// 0 on equal, -1 on not equal
	int (*comp_func)( void *, void * );

	// these two fields are used for iterating
	int bucket;
	struct hash_table_link_struct *link;
} * HashTable;


/* Creates a new hash table, allocating memory for both the struct
 * and for the table with init_size number of buckets
 * sets the compare and hashing functions
 * Returns null on failure
 */
HashTable create_hash_table( int init_size,
		unsigned long hash_func (void *data),
		int com_func( void *data1, void *data2 ) );

/* Frees all the memory in a hash table
 * Returns -1 on failure
 */
int destroy_hash_table( HashTable table );

/* Frees all the memory in a hash table including the data stored with special func
 * Returns -1 on failure
 */
int destroy_hash_table_func( HashTable table, void dest_func (void *) );

/* Frees all the memory in a hash table including the data stored
 * Returns -1 on failure
 */
int destroy_hash_table_data( HashTable table );

/* Resets the iteration over a hash table
 */
void hash_table_reset( HashTable table );

/* gets the data of the next item in an iteration
 * over a hash table.
 * Returns the void *data of that item that needs to be typecast
 */
void *hash_table_next( HashTable table );

/* adds a new item to the hash_table that hasn't already been hashed
 * key is the value that is hashed and used to assign data to a bucket
 * returns 0 on success, -1 on failure
 */
int hash_table_add( HashTable table, void *key, void *data );

/* gets an unhashed item from a hash table
 * retursn the void *data
 */
void *hash_table_get_from( HashTable table, void *key );

/* gets a prehashed item from the hash table
 * returns the void *data
 */
void *hash_table_get_pre( HashTable table, unsigned long hash, void *key );

/* remove an item from the hash table
 * returns the data that has just been removd
 */
void *hash_table_remove_from( HashTable table, void *key );


/* string hashing fucntion
 * returns the hash of a character string
 */
unsigned long string_hash_djb2( void *data );

/* comares 2 strings for the hash */
int string_comp_func( void *data1, void *data2 );


/* integer hashing fucntion
 */
unsigned long int_hash( void *data );

/* comares 2 integers for the hash */
int int_comp_func( void *data1, void *data2 );

#endif /* INC_HASH_H_ */
