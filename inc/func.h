/*
 * func.h
 *
 *  Created on: Oct 18, 2015
 *      Author: thor
 */

#ifndef INC_FUNC_H_
#define INC_FUNC_H_


#define NODE_ELEMENT 1
#define NODE_ENDELEMENT 15
#define NODE_TEXT 3

int grow_buffer( void **, int );

int quick_comp( char *, char * );

char *quick_copy( char * );

size_t trimwhitespace(char *out, size_t len, const char *str);

#endif /* INC_FUNC_H_ */
