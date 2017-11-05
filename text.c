#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 
#include <ncurses.h> 
#include "page.h"

/*Function Prototypes */
void updatestatus(char *info);				/*Function to Update the Status Bar  */
int countlines(FILE *fp);				/*Function to count the total number of lines */
void loadfile(Page *p, char* filename);			/*Function to load a previously saved file */
void savefile(Page *p);					/*Function to Save a File */
void moveleft(int *x, int *y);				/*Functon to navigate towards the left */
void moveright(Page *p, int *x, int *y);		/*Functon to navigate towards the right */
void moveup(Page *p, int *x, int *y);			/*Functon to navigate upwards */
void movedown(Page *p, int *x, int *y);			/*Functon to navigate downwards */
void paste(Page *p, int x3, int y3, char *z);		/*Function to paste copied text */
void insertchar(buffer *s, char c, int index);		/*Function to insert a character in a buffer */
void copy(Page *p, int x1, int y1, int x2, int y2, char *z);/*Function to copy a piece of text */
void cut(Page *p, int x4, int y4, int x5, int y5, char *w, char *z); /*Function to cut a piece of text */
void stringreplace(Page *p, char *searchstring, char *replacestring);/*Function to search a string replace with another string */
void stringsearch(Page *p, char *searchstring);		/*Function to search a string*/
int blanklines = 0; 					/*To count the number of blanklines */
int blanktabs = 0;					/*To count the number of blanktabs */

int main(int argc, char *argv[]) {			/*      MAIN      */
	FILE *fp;	
	Page p;
	char *searchstring, *replacestring, *z, *w, *u;
	searchstring = (char*)malloc(40);
	replacestring = (char*)malloc(40);
	z = (char*)malloc(1024);
	w = (char*)malloc(1024);
	u = (char*)malloc(1024);
	int cpyline;
	if(fp = fopen(argv[1], "r")) {
		    loadfile(&p, argv[1]);
        }
        else {
        	openpage(&p, argv[1], PAGE_SIZE);
            	p.nol = 1;
        }		
	initscr(); 		 	/* Start Curses Mode */
	noecho();		 	/* Stop displaying on screen while we do getch() i.e user input*/
	keypad(stdscr, true);    	/*To be able to read special keys like F1,F2...*/
	int beginning = 0;		/*Starting point to print the page */
	int ending = Windowsize; 		/* Predefined Value */
	int y, x, y2, x2, x1, y1, x3, y3, x4, x5, y4, y5, x6, y6, x7, y7, x8, ck;
  	int i, d, a = 0, b = 0, l = 0;
	updatestatus("F2=Exit F3=save F5=Search F6=Cut F7=Paste F8=SearchandReplace F9=Copy F10=PgUp F12=PgDown"); 
	printpage(&p, beginning, ending); 	/* Print the starting screen */
	getyx(stdscr, y, x);			/* Get the present location of the cursor */
	char status[100];        		/* To be printed at the bottom of the screen */
	while(1) {
		beginning = 0 + blanklines;
		ending = Windowsize + blanklines;
		int ch = getch();
		updatestatus("F2=Exit F3=save F5=Search F6=Cut F7=Paste F8=SearchandReplace F9=Copy F10=PgUp F12=PgDown");  
		switch(ch) {
			case KEY_F(10):                                      /*       F10 = Page Up  */
				getyx(stdscr, y, x);
				move(y - 40, 0);
				if(y < 40) {
					for(ck = 0; ck < 40; ck++)
						moveup(&p, &x, &y);
					refresh();
				}
				break;
			case KEY_F(12):					     /* F12 = Page Down */
				getyx(stdscr, y, x);				
				move(y + 40, 0);
				if(p.nol - y < 40) {
					for(ck = 0; ck < 40; ck++)
						movedown(&p, &x, &y);
					refresh();
				}
				break;
			case KEY_F(2):                                   	/* F2 = exit */
				goto endnc;
	            		printpage(&p, beginning, ending);
				break;
			case KEY_F(3):						/*  F3 = Save File */
				savefile(&p);
            			sprintf(status, "File Saved as \'%s\'", p.filename);
				updatestatus(status);
				break;				
			case KEY_F(9):         					/* F9 = Copy Text   */
				getyx(stdscr, y, x);
				y1 = y, x1 = x - 1;
				sprintf(status, "Press F2 at end location");
				updatestatus(status);
				while(1) {
					int ch1 = getch();					
					if(ch1 == KEY_F(2)) {     		/*  F2 at second location  */
							getyx(stdscr, y2, x2);
							copy(&p, x1, y1, x2, y2, z);
							break;
					}
					if(ch1 == KEY_UP) {  
						moveup(&p, &x, &y);
					}
					if(ch1 == KEY_DOWN) {  
						movedown(&p, &x, &y);
					}	
					if(ch1 == KEY_LEFT) {  
						moveleft(&x, &y);
					}	
					if(ch1 == KEY_RIGHT) {  
						moveright(&p, &x, &y);
					}					
				}
				break;
				case KEY_F(5):                                      /* F5 =  Search */
					sprintf(status, "Enter the string to be searched ");
					getyx(stdscr, y6, x6);
					getstr(searchstring);
					sprintf(status, "Search results for \'%s\'", searchstring);
					updatestatus(status);
					stringsearch(&p, searchstring);
					break;	
				case KEY_F(6):                       		    /*  F6 = Cut   */
					getyx(stdscr, y, x);
					y4 = y, x4 = x - 1;
					sprintf(status, "Press F2 at end location");
					updatestatus(status);
					while(1) {
						int ch1 = getch();					
						if(ch1 == KEY_F(2)) {               /*  F2 at second location */
							getyx(stdscr, y5, x5);
							cut(&p, x4, y4, x5, y5, w, z);
							break;
						}
						if(ch1 == KEY_UP) {  
							moveup(&p, &x, &y);
							
						}
						if(ch1 == KEY_DOWN) {  
							movedown(&p, &x, &y);
							
						}	
						if(ch1 == KEY_LEFT) {  
							moveleft(&x, &y);
						}	
						if(ch1 == KEY_RIGHT) {  
							moveright(&p, &x, &y);
						}					
					}
					printpage(&p, beginning, ending);
					break;
				case KEY_F(7):                      			/*  F7 = Paste */
					getyx(stdscr, y, x);
					y3 = y, x3 = x;
					paste(&p, x3, y3, z);
					refresh();
					printpage(&p, beginning, ending);
					break;	
				case KEY_F(8):                                      /* F8 =  Search and Replace */
					getyx(stdscr, y, x);
					sprintf(status, "Enter the string to be searched: ");
					updatestatus(status);
					getstr(searchstring);
					sprintf(status, "Searched for \'%s\', Enter the string to replace", searchstring);
					updatestatus(status);
					getstr(replacestring);
					stringreplace(&p, searchstring, replacestring);
					printpage(&p, beginning, ending);
					break;	
				case KEY_UP:
					moveup(&p, &x, &y);
					break;
				case KEY_DOWN:
					movedown(&p, &x, &y);
					break;
				case KEY_LEFT:
					moveleft(&x, &y);
					break;
				case KEY_RIGHT:
					moveright(&p, &x, &y);
					break;
				case KEY_DC:     						
				case KEY_BACKSPACE:
				case 127:
					if(strlen(p.text[y + blanklines].string) == 0) {      /* If string is blank */	
						delline(&p, y + blanklines);
						moveup(&p, &x, &y);
					}
					else if( x > 1 ) {				/* Non blank lines */
						delcharfrombuffer(&p.text[y + blanklines], x - 2); 
						moveleft(&x, &y);				 
					}
					printpage(&p, beginning, ending);
					move(y, x);
					break;
            			case '\t':						/* TAB */
                			for(i = 0; i < TAB_WIDTH; i++) {
                    				insertcharatpos(&p.text[y + blanklines], ' ', x - 1);
                    				printpage(&p, beginning, ending);
                    				moveright(&p, &x, &y);
                			}
                			break;
				case '\n':						/* Newline */
					a = 0;
					b = 0;
					getyx(stdscr, y7, x7);
					x8 = x7;
					while(p.text[y7].string[x7] != '\0') {
						u[a] = p.text[y7].string[x7];
						x7++;
						a++;
					}
					u[a] = '\0';
					insertline(&p, y + blanklines + 1);
					for(l = 0; l < a; l++) {
						insertcharatpos(&p.text[y+blanklines + 1], u[l], b);
						b++;
					}
					for(i=0; p.text[y7].string[i] != '\0'; i++) 
						delcharfrombuffer(&(p.text[y7]), x8);	
					printpage(&p, beginning, ending);
					movedown(&p, &x, &y);
					break;
				default: 					/* Remaining characters */
					if( isprint(ch) ) {
						insertcharatpos(&p.text[y + blanklines], ch, x - 1);
						printpage(&p, beginning, ending);
						moveright(&p, &x, &y);
					}
			}
		}
endnc:	
	endwin();							/* End Curses Mode */
	closepage(&p);
	return EXIT_SUCCESS;
}
void updatestatus(char *status) {                                      /* Status Update*/
	int initialy, initialx; 
	getyx(stdscr, initialy, initialx);
	attron(A_REVERSE);					       /* To get the required style for status */
	move(LINES - 1, 0);					
	clrtoeol();
	printw(status);
	attroff(A_REVERSE);
	move(initialy, initialx);
} 
void moveleft(int *x, int *y) { 					/* Move left on pressing left-arrow key */
	if(*x - 1 > 0) 
		move(*y, --(*x));
}
void moveright(Page *p, int *x, int *y) {				/* Move right on pressing the right-arrow key */
	if(*x <= strlen(p -> text[*y + blanklines].string)) {
        	if(p -> text[*y + blanklines].string[*x + blanktabs] == '\t') {
            		move(*y, ++(*x));
        	} 
		else {
	        	move(*y, ++(*x));
        	}
    	}
}
void moveup(Page *p, int *x, int *y) {					/*Move up on pressing the up-arrow key */
	if( *y > 0 ) {          					/* Move one string upward if possible*/
		--(*y);
	}
	else if (blanklines > 0) {					/* If there are blank lines */
		--(blanklines);
		printpage(p, blanklines, Windowsize + blanklines);	
	}
	if( *x > strlen(p -> text[*y + blanklines].string) + 1 ) 
		*x = strlen(p -> text[*y + blanklines].string) + 1;	/* Set cursor at end of string */  
	move(*y, *x);
}
void movedown(Page *p, int *x, int *y) {				/* Move down on pressing the down-arrow key */
	if( *y < p -> nol  - 1 && *y < Windowsize - 1) {      /* If space available at the bottom of the screen */
		 ++(*y);						/* Move one string downward */
	}
	else if ( *y + blanklines < p->nol - 1 ) {           /*If there is enough space */  
		++(blanklines);
		printpage(p, 0 + blanklines, Windowsize + blanklines); 	
	}
	if( *x > strlen(p->text[*y + blanklines].string) + 1 )
		*x = strlen(p->text[*y + blanklines].string) + 1;	/*Set Cursor at proper position */
	move(*y, *x);
}
int countlines(FILE *fp) {						/*Function to count the total number of lines */
	char ch = '\0';
	int count = 0;
	while((ch = fgetc(fp)) != EOF)
		if( ch == '\n' )
			count++;
	fseek(fp, 0, SEEK_SET);
	return count;
} 
void loadfile(Page *p, char *filename) {				/*Loading the file */
	FILE *fp = fopen(filename, "r");
	int size = countlines(fp) * 2;
	char ch = '\0';
	int string;
	if(size < PAGE_SIZE)
        	size = PAGE_SIZE;
	openpage(p, filename, size);
	if(fp == NULL) {
        	p->nol = 1;
        	return;
    	}
	for(string = 0; string < size && ch != EOF; string++) {
        	ch = fgetc(fp);
        	while(ch != '\n' && ch != EOF) {
            		buffer *currline = &(p->text[string]);
            		if(ch != '\t') {
                		addchar(currline, ch);
            		}
            		else {
                		int i;
                		for(i = 0; i < TAB_WIDTH; i++) {
                    			addchar(currline, ' ');
                		}
            		}
            		ch = fgetc(fp);
        	}
        	p->nol++;
    	}
	fclose(fp);
}
void savefile(Page *p) {						/*Function to save the file */
	FILE *fp = fopen(p -> filename, "w");
	int rows, columns;						/*Variables for rows and columns */
	for(rows = 0; rows < p -> nol; rows++) {
		columns = 0;
		while(p -> text[rows].string[columns] != '\0') {
			fputc(p -> text[rows].string[columns], fp);
			columns++;
		}
		fputc('\n', fp);
	}
	fclose(fp);
}
void stringreplace(Page *p, char *searchstring, char *replacestring) {	 	/* String Replace*/
	int y, x;	
	getyx(stdscr, y, x);
	move(38,0);
	move(y, x);
	char c[128];
	char *ptr;
	int pos, pl = 0, q = 0, k, j, len1, len2, m, count = 0, i = 0;
	len1 = strlen(searchstring);
	len2 = strlen(replacestring); 
	for(i = 0; i < p->nol; i++) {
		while(1) {
			ptr = NULL;
			pos = 0, pl = 0;
			ptr = strstr(p->text[i].string, searchstring);
			if(ptr == NULL)	{
				break;
			}
			pos = ptr - p->text[i].string;
			count = count + 1;
			if(len1 == len2) {
				for (k = pos; k < (pos+len1); k++) {
					p->text[i].string[k] = replacestring[pl];
					pl++;
				}
			}
			if(len1 < len2) {
				q = 0;
				for(j = pos + len1; p->text[i].string[j] != '\0'; j++) {
					c[q] = p->text[i].string[j];
					q++;	
				}
				c[q] = '\0';
				pl = 0;
				for(j = pos; replacestring[pl] != '\0'; j++) {
					p->text[i].string[j] = replacestring[pl];
					pl++;
				}
				q = 0;
				for(; c[q] != '\0'; j++) {
					p->text[i].string[j] = c[q];	
					q++;
				}
			}
			if(len1 > len2) {
				q = 0;
				for(j = pos + len1; p->text[i].string[j] != '\0'; j++) {
					c[q] = p->text[i].string[j];
					q++;	
				}
				c[q] = '\0';
				pl = 0;	
				for(j = pos; replacestring[pl] != '\0'; j++) {
					p->text[i].string[j] = replacestring[pl];
					pl++;
				}
				for(q = 0; c[q] != '\0'; j++) {
					p->text[i].string[j] = c[q];
					q++;
				}
				p->text[i].string[j]='\0';
			}
		}
	}	
}
void stringsearch(Page *p, char *searchstring) {                                   /* Search */
	int arr[100][100];
	int q = 0, j = 0, l1, count = 0, i = 0, h = 0, g = 0, r, l2, k;
	l1 = strlen(searchstring); 
	for(i = 0; i < p->nol; i++) {
		for(q = 0; q < strlen(p->text[i].string);) {
			j = 0;
			count = 0;
			while((p->text[i].string[q] == searchstring[j]) && (count < l1)) {
				count++;
				q++;
				j++;
			}
			if(count == l1) {
				arr[g][0] = q - l1;
				arr[g][1] = i;
				g++;
			}
				q++;
		}
	}
	move(arr[0][1], arr[0][0]);
	for(r = 1; r < g; r++) {
		int ch2 = getch();
		if(ch2 == KEY_DOWN) {
			move(arr[r][1], arr[r][0]);
		}
	}
}	
void copy(Page *p, int x1, int y1, int x2, int y2, char *z) {			      /* Copy */
	int i = 0, ct = 0, k = 0, j = 0, m = 0, hc = 0, l = 0, b = 0, a = 0;
	char *t;	
	if(y1 == y2) {
		while(x1 != x2) {
			z[i] = p->text[y2].string[x1];
			x1++;
			i++;
		}
		z[i]='\0';
		refresh();
	}
	z[i]='\0';	
}	
void cut(Page *p, int x4, int y4, int x5, int y5, char *w, char *z) {				/* Cut */
	int i = 0;
	int d = x4;
	int hc, x1, y1, x2, j, b, y2;
	if(y4 == y5) {	
		while(x5 != x4) {
			w[i] = p->text[y4].string[x4];
			x4++;
			i++;
		}
		w[i] = '\0';
	}
	strcpy(z, w);
	while(x5 != d-1) {
		delcharfrombuffer(&(p->text[y4]), x5);
		x5--;
	} 
}
void paste(Page *p, int x3, int y3, char *z) {					    /* Paste */
	int i, a, l, b = 0;	
	a = strlen(z);
	if(strlen(p->text[y3].string) == 0) {
		insertline(p, y3 + blanklines);
			for(l = 0; l < a; l++) {
				insertcharatpos(&(p->text[y3 + blanklines]), z[l], b);
				b++;
			}
	}
	else {
		for(i = strlen(z); i >= 0; i--) { 
			insertcharatpos(&(p->text[y3]), z[i], x3);
		}
		
	}
}
