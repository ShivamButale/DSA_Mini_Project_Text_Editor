#include "buffer.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void initbuffer(buffer *bf) {
	bf -> string = (char*)malloc(100);
	bf -> size = 100;
	bf -> string[0] = '\0';       /* Initialize first character to NUL*/
}
void doublebuffer(buffer *bf) {
	char *node;
	node = (char*)malloc(bf -> size * 2);  
	strcpy(node, bf -> string);   	/* Initially copy buffer string into another string*/
	bf -> string = node;          	/* Set the buffer pointer pointing to the node*/ 
	bf -> size =  2 * bf -> size;	/* Double the size of the buffer*/
	free(bf -> string);	
}
void insertcharatpos(buffer *bf, char ch, int pos) {
	int i;
	if(strlen(bf -> string) >= bf -> size - 2) {  
		doublebuffer(bf);
	}
	for(i = strlen(bf->string) ; i >= pos; i--) { 
		bf -> string[i + 1] = bf -> string[i];    /* Move all characters after specified position forward by one index */
	}
	bf -> string[pos] = ch;                           /* Insert the character at the required position */
}
void addchar(buffer *bf, char ch) {
	insertcharatpos(bf, ch, strlen(bf -> string));    
}
void delcharfrombuffer(buffer *bf, int pos) {
	int i;
	for(i = pos; i < strlen(bf -> string); i++) {   /*Move all charcters after specified position backward by one index */
		bf -> string[i] = bf -> string[i + 1];
	}
}
