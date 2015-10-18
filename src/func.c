/*
 * func.c
 *
 *  Created on: Oct 18, 2015
 *      Author: thor
 */
 
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 
 /**
  * Grows a buffer to a certain size
  * @param buffer pointer to buffer pointer to grow
  * @param length New length to grow to
  * @return new length of buffer or -1 on fail
  */
int grow_buffer( void **buffer, int length ){
    void *temp;
    temp = realloc( *buffer, length );
    if( temp == NULL ){
        return -1;
    }
    *buffer = temp;
    return length;
}
 
