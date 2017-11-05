#include "page.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
void openpage(Page *p, char *filename, int size) {
	p -> text = (buffer*)malloc(size * sizeof(buffer));
	int i;	
	strcpy(p -> filename, filename);                /* Associate entered filename with the page*/
	p -> nol = 0;
	p -> size = size;	
	for(i = 0; i < size; i++) {
		initbuffer(p->text + i);
	}	
}
void closepage(Page *p) {
	int i;
	for(i = 0; i < p -> nol; i++) {           /* Free all characters from all the lines */
		free(p -> text[i].string);
	}
	free(p -> text);
}
void printpage(const Page *p, int start, int end) {
	int i, line;
	for(i = start, line = 0; i < p->nol && i < end; i++, line++) {
		move(line, 0);                     	  
		clrtoeol();
		printw(" %s", p->text[i].string);
	}
	if(start < end) {
		move(line, 0);
		clrtoeol();
		move(line - 1, 1);
	}
	refresh();       /* commands like printw and move make the changes only on an imaginary window. To apply them onto our screen we use 				 the refresh() command */ 
}
void insertline(Page *p, int pos) {
	if(p -> nol >= p -> size)
		doublepagesize(p);
	buffer newbuffer;
	initbuffer(&newbuffer);
	newbuffer.string[0] = '\0';
	int i;
	for(i = p -> nol - 1; i >= pos; i--) {
		p -> text[i + 1] = p -> text[i];
	}
	p -> text[pos] = newbuffer;
	(p -> nol)++;
}
void delline(Page *p, int pos) {
	if(p -> nol > 1) {
		free(p -> text[pos].string);			/* Free the string malloced at the given position*/
		int i;
		for(i = pos; i < p -> nol - 1; i++) { /* Shift all lines one line upward */
			p -> text[i] = p -> text[i+1];
		}
		(p->nol)--;  				/*Decrease the number of lines by 1 */
	}
}
void doublepagesize(Page *p){
	int newsize = p -> size * 2;
	buffer *newbuffer = malloc(newsize * sizeof(buffer));
	int i;
	for(i = 0; i < p -> size; i++) 
		newbuffer[i] = p -> text[i];                
	for(i = p -> size; i < newsize; i++)
		initbuffer(newbuffer + i);
	p -> text = newbuffer;
	p -> size = newsize;
}

