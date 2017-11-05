/*Abstract Data Type Page*/
#include "buffer.h"
#define PAGE_SIZE 500 /* Default number of lines */
#define Windowsize (LINES - 2) /* Size of window, making room for bottom prompt */
#define NAME_LIMIT 256 /* Max size of a unix filename + 1 */
typedef struct Page {
	char filename[32];
	buffer *text;
	int nol;
	int size;
}Page;
void openpage(Page *p, char *filename, int size);
void closepage(Page *p);
void printpage(const Page *p, int start, int end);
void insertline(Page *p, int pos);
void delline(Page *p, int pos);
void doublepagesize(Page *p);
