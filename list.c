#include"list.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
node *head; 
void addnode(char t[], node *q) {
	node *p = (node*)malloc(sizeof(node));
	node *temp = q -> next;
	strcpy(p -> array, t);
	p -> prev = q;
	p -> next = q -> next;
	if((q -> next) != NULL) {
		((q -> next) -> prev) = p;
		while(temp != NULL) {
			(temp -> count)++;  
			temp = temp -> next;
		}
	}
	q -> next = p;
	p -> count = q -> count + 1;       
}
void deletenode(node *p) {
	node *temp = p -> next;
	((p -> prev -> next)) = p -> next;
	if(p -> next != NULL) {
		((p -> next) -> prev) = p -> prev;
		while(temp != NULL) {
			(temp -> count)--;       
			temp = temp -> next;
		}
	}
	free(p);                     
}
void editnode(node *p) {
	printf("\nThe line is :\n%s",p -> array);
	printf("\nEnter the new line :\n");
	gets(p -> array);                         
	printf("\nLine was edited successfully.\n");
}
void printfile() {
	node *temp = head;
	system("clear");
	while(temp -> next != NULL) {
		temp = temp -> next;
		printf("%d %s\n",temp -> count,temp -> array);            
	}
}

