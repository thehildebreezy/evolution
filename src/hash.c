/*
 * hash.c
 *
 *  Created on: Oct 10, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../inc/hash.h"

// hash tabling
HashTable create_hash_table( int init_size,
		unsigned long hash_func (void *data),
		int com_func( void *data1, void *data2 ) )
{
	// begin allocating data
	HashTable table = (HashTable)malloc( sizeof(
							struct hash_table_struct
						) );
	if( table == NULL ){
		perror("Allocating hash_table");
		return NULL;
	}

	// attempt to allocate buckets
	table->table = calloc( init_size, sizeof(struct hash_table_link_struct *) );
	if( table->table == NULL ){
		perror("Allocating hash buckets");
		free( table );
		return NULL;
	}

	// initialize buckets
	int i = 0;
	for( ; i<init_size; i++ ){
		table->table[i] = NULL;
	}

	table->num_buckets = init_size;

	table->size   = 0;
	table->bucket = 0;
	table->link   = NULL;

	// assign hashing and comparing functions
	table->hash_func = hash_func;
	table->comp_func = com_func;

	return table;
}

// destroy a hash table
// 0 on success, -1 failure
int destroy_hash_table( HashTable table )
{
	int i = 0;
	for( ; i < table->num_buckets; i++ ){

		// loop over all buckets and destroy links
		HashTableLink next = table->table[i];
		while( next != NULL ){
			HashTableLink destroy = next;
			next = next->next;
			free( destroy );
		}
	}
	
	// free buckets table
	free(table->table);

    // free table struct
	free( table );

	return 0;
}

// destroy a hash table
// 0 on success, -1 failure
int destroy_hash_table_data( HashTable table )
{
	int i = 0;
	for( ; i < table->num_buckets; i++ ){
		// loop over all buckets and destroy links
		HashTableLink next = table->table[i];
		while( next != NULL ){
			HashTableLink destroy = next;
			next = next->next;
			if( destroy->data ){
			    free( destroy->data );
			}
			free( destroy );
		}
	}

	// free buckets table
	free(table->table);
	
	free( table );

	return 0;
}

// resets a hash table iterator
void hash_table_reset( HashTable table )
{
	table->bucket = 0;
	table->link   = NULL;
}

// gets the next item to be iterated over
void *hash_table_net( HashTable table )
{
	int i = table->bucket;
	HashTableLink next = NULL;
	void *data = NULL;
	for( ; i < table->num_buckets; i++ ){
		if( table->link == NULL ){
			continue;
		}
		next = table->link->next;
		if( next == NULL ){
			continue;
		}

		data = next->data;
		break;
	}
	table->bucket = i;
	table->link = next;

	return data;
}

// add a new item to the hash table
int hash_table_add( HashTable table, void *key, void *data )
{
	// allocate memory space for new link
	HashTableLink link = malloc( sizeof( struct hash_table_link_struct ) );
	if( link == NULL ){
		perror( "Allocating link memory" );
		return -1;
	}

	unsigned long hash = table->hash_func( key );
	unsigned bucket = hash % table->num_buckets;

	link->key  = key;
	link->data = data;
	link->hash = hash;
	link->next = NULL;

	// check for bucket membership
	if( table->table[bucket] == NULL ){
		table->table[bucket] = link;
	}else{
		// find a place in the list for this value

		HashTableLink next = table->table[bucket];
		while( 1 ){
			// add to the list
			if( next->next == NULL ){
				next->next = link;
				break;
			}

			next = next->next;
		}
	}

	table->size++;
	return 0;
}

// get an item from a table
void *hash_table_get_from( HashTable table, void *key )
{
	unsigned long hash = table->hash_func( key );
	return hash_table_get_pre( table, hash, key );
}

// get a pre hashed value from a hash table
void *hash_table_get_pre( HashTable table, unsigned long hash, void *key )
{
	unsigned bucket = hash % table->num_buckets;

	HashTableLink next = table->table[bucket];
	while( next != NULL ){
		// test for exact match
		/*if( next->hash == hash &&
			table->comp_func( next->key, key ) == 0 )*/
		if( next->hash == hash )
		{
			return next->data;
		}
		next = next->next;
	}

	return NULL;
}

// remove an item from the hash table
void *hash_table_remove_from( HashTable table, void *key )
{
	unsigned long hash = table->hash_func( key );

	unsigned bucket = hash % table->num_buckets;

	if( table->table[bucket] == NULL ){
		return NULL;
	}

	HashTableLink next = table->table[bucket];
	HashTableLink prev = NULL;
	while( next != NULL ){
		// test for exact match
		if( next->hash == hash &&
			table->comp_func( next->key, key ) == 0 )
		{
			if( prev == NULL ){
				table->table[bucket] = next->next;
			} else {
				prev->next = next->next;
			}

			// free this link and return its data
			void *data = next->data;
			free( next );
			return data;
		}
		prev = next;
		next = next->next;
	}

	return NULL;
}


// a function to hash strings
unsigned long string_hash_djb2( void *data )
{
	if( data == NULL ){
		return 0;
	}

	unsigned long hash = 5381;
	const char *string = (const char *)data;

	int c = string[0], i = 0;
	while( c != 0 ){
		hash = ((hash << 5) + hash) + c;

		i++;
		c = string[i];
	}

	return hash;
}

// str_cmp_function
int string_comp_func( void *data1, void *data2 )
{
	return strcmp( (const char *)data1, (const char *)data2 );
}
