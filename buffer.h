/* Abstract Data Type Buffer */
#define LINE_SIZE 100
#define TAB_WIDTH 8
typedef struct buffer{            
	char *string;
	int size;
}buffer;
void initbuffer(buffer *bf);
void doublebuffer(buffer *bf);
void insertcharatpos(buffer *bf, char ch, int pos);
void addchar(buffer *bf, char ch);
void delcharfrombuffer(buffer *bf, int pos);
