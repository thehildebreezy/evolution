/*
 * func.c
 *
 *  Created on: Oct 18, 2015
 *      Author: thor
 */
 
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 
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
 

/**
 * Quickly compare strings without breaking my stuff
 * @param str1 first string to compare
 * @param str2 second string to compare
 * @return -1 on not same, 0 on same
 */
int quick_comp( char *str1, char *str2 )
{
    int i = strlen( str1 );
    int j = strlen( str2 );
    
    if( i != j ){
        return -1;
    }
    
    int k = 0;
    while( k < i ){
        if( str1[k] != str2[k] ) return -1;
        k++;
    }
    
    return 0;
}

/**
 * Quickly copy strings without breaking my stuff. need to dealloc
 * @param str1 first string to compare
 * @param str2 second string to compare
 * @return string on success, null on failure
 */
char *quick_copy( char *str )
{
    if( str == NULL ) return NULL;

    int i = strlen( str );
    
    char *new_str = malloc( i + 1 );
    if( new_str == NULL ){
        return NULL;
    }
    
    
    int k = 0;
    while( k < i ){
        new_str[k] = str[k];
        k++;
    }
    new_str[k] = 0;
    
    return new_str;
}

/**
 * trims the whitespace down so things can look standard
 * @param out output buffer
 * @param len length of input
 * @param str input to trim
 * @return length of output
 */
size_t trimwhitespace(char *out, size_t len, const char *str)
{
  if(len == 0)
    return 0;

  const char *end;
  size_t out_size;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
  {
    *out = 0;
    return 0;
  }

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;
  end++;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len-1 ? (end - str) : len-1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;

  return out_size;
}


