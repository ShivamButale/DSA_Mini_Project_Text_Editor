#include<stdio.h>            
#include<string.h>                            
#include<stdlib.h>                            
#include<termios.h>                           
#include<unistd.h>                            
#include"list.h"
void editcmd();                   
void addline(node *temp);
void openorcreate();
void closefile();
void editfile();
void savefile();
void deletefile();
node *head;                       
char file[20];   
FILE *fp = NULL;  
void red () {
	printf("\033[1;31m");
}
void yellow() {
	printf("\033[1;33m");
}
void reset () {
	printf("\033[0m");
}
void BoldBlue() {
	printf("\033[1;34m");	
}
void Blue() {
	printf("\033[0;34m");	
}
void green() {
	printf("\033[0;32m");	
}
int getch() {
	struct termios terminitial, termfinal;
	int ch;
	tcgetattr( STDIN_FILENO, &terminitial );
	termfinal = terminitial;
	termfinal.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &termfinal );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &terminitial );
	return ch;
}                      
int main() {
	int pos = 0;
	char c;
	head = (node*)malloc(sizeof(node));           
	(head -> next) = (head -> prev) = NULL;               
	(head -> count) = 0;
	while(1) {
		system("clear");  
		BoldBlue();                      
		printf("\t\t\t\t************TEXT EDITOR************");
		yellow();
		printf("\nOPTIONS\n");	
		printf("~ O :Open a file and read it into a buffer\n   If file does not exist creates a new file for editing\n");
		printf("~ E :Edit the currently open file\n");
		printf("~ X :Close the current file and open another file\n");
		printf("~ Q :Quit discarding any unsaved changes\n");
		reset();
		c = getch();    
		switch(c) {
			case 'o' :
			case 'O' :
				openorcreate();
				break;
			case 'e' :
			case 'E' :
				editfile();
				break;
			case 'x' :
			case 'X' :
				closefile();
				break;
			case 'q' :
			case 'Q' :
				system("clear");
				exit(1);
				break;
		}
	}
	return 0;
}
void addline(node *temp) {
	char buffer[200];
	printf("\nEnter the new line :\n");
	gets(buffer);                   
	addnode(buffer,temp);      
}                           
void openorcreate() {
	node *buffer = head;                       
	char c;
	char buf[200];                            
	if(fp != NULL) {
		printf("\nAnother file is open and it will be closed\nDo you want to proceed?(Y/N):");
		c = getch();
		if(c == 'n' || c == 'N')
			return;
		else
			closefile();
	}
	fflush(stdin);
	printf("\nEnter the name of the file :");
	scanf("%s", file);
	getchar();
	fflush(stdin);
	deletefile();
	fp = fopen(file,"r");                    
	if(fp == NULL) {
		printf("\nThis file does not exist.Do you want to create one?(Y/N) :");
		c = getchar();
		getchar();
		if(c =='N'||c =='n')
			return;
		else {
			fp = fopen(file,"w");              
			editfile();
			return;
		}
	}
	if(feof(fp))
		return;
	while((fgets(buf,201,fp)) != NULL) {
		addnode(buf,buffer);
		buffer = buffer -> next;
	}
	editfile();                          
}
void editfile() {
	node *temp = head -> next;          
	char c, d;
	system("clear");                   
	if(fp == NULL) {
		printf("\nNo file is open\n");
		return;
	}
	printf("\nThe file contents will be displayed along with the line number\npress any key\n");
	getch();
	system("clear");
	printfile();                          
	if(temp != NULL) {
		BoldBlue();
		printf("This is line number %d",temp -> count);
		reset();
	}    
	else
		temp = head;
	editcmd();               
	while(1) {
		c = getch();
		switch(c) {
			case 'a' :
			case 'A' :           
				addline(temp);              
				temp = temp -> next;
				printfile();
				BoldBlue();
				printf("\nThis is line number %d",temp -> count);
				reset();
				break;	
			case 'c' :
			case 'C' :
				editnode(temp); 
				break;
			case 'D' :
			case 'd' :              
				if(temp == head) {
					printf("\nFile is empty\n");
					break;
				}
				temp = temp -> prev;
				deletenode(temp -> next);         
				printf("\nLine deleted\n");
				printfile();             
				if(temp -> count) {
					BoldBlue();
					printf("\nThis is line number %d",temp -> count);
					reset();
				}
				if(((temp -> prev) == NULL) && (temp -> next) != NULL)
					temp = temp -> next;
				else if((temp == head) && ((temp -> next) == NULL))
					printf("\nFile is empty");       
				break;
			case 'h' :
			case 'H' :                  
				system("clear");
				editcmd();               
				system("clear");
			case 'n' :
			case 'N' :                    
				if(temp -> next != NULL) {
					temp = temp -> next;
					BoldBlue();
					printf("\nThis is line number %d",temp -> count);
					reset();
				}
				else if(temp == head)
					printf("\nFile is empty");        
				else
					printf("\nThis is the last line");
				break;
			case 'p' :
			case 'P' :           
				if(temp == head) {
					printf("\nFile empty");    
					break;
				}
				if(temp -> prev != head) {
					temp = temp -> prev;
					BoldBlue();
					printf("\nThis is line number %d",temp -> count);
					reset();
				}
				else {           
					BoldBlue();   
					printf("\nThis is the first line");
					reset();
				}	
				break;
				
			case 's' :
			case 'S' :                    
				savefile();
				closefile();
				return;
				break;
			case 'v' :
			case 'V' :                   
				printfile();
				BoldBlue();
				printf("\nThis is line number %d",temp -> count);
				reset();
				break;
			case 'X' :
			case 'x' :               
				printf("\nSave the file before exiting?(y/n) :");
				d = getch();               
				if(d == 'y' || d == 'Y')
					savefile();
				closefile();
				return;
				break;
		}
	}
}
void savefile() {
	node *temp = head -> next;
	fclose(fp);
	fp = fopen(file,"w");                     
	while(temp != NULL) {
		fprintf(fp,"%s",temp -> array);                  
		temp = temp -> next;
	}
}
void closefile() {
	if(fp == NULL)
		return;
	fclose(fp);
	fp = NULL;
	deletefile();                    
}
void editcmd() {
	printf("\nEditor commands\n");
	printf("Following Options are available \n");
	printf("P :Move one line up\n");
	printf("N :Move one line down\n");
	printf("C :Edit the current line\n");
	printf("A :Add a line after the current line\n");
	printf("D :Delete the current line\n");
	printf("V :Display the contents of the buffer\n");
	printf("S :Save the changes and exit to main menu\n");
	printf("X :Exit discarding any changes\n");
	printf("H :Display the list of commands\n");
	getch();
}
void deletefile() {
	while(head -> next != NULL)
		deletenode(head -> next);                 
}

