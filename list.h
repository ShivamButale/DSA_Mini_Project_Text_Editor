typedef struct node {
	char array[200];                            
	int count;
	struct node *prev, *next;            
}node;
void addnode(char t[],node *q);
void deletenode(node *p);
void clearlist();
void editnode(node *p);
void printfile();