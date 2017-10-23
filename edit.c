#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#define CTRL_KEY(k) ((k) & 0x1f)
struct editorConfig {
	int screenrows;
	int screencols;
	struct termios initial;
};
struct editorConfig E;
void die(const char *s) {//printing the error message in case of failure
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	perror(s);
	exit(1);
}
void disableRawMode() {//Leave raw mode
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.initial) == -1)
    		die("tcsetattr");
}
void enableRawMode() {
	if (tcgetattr(STDIN_FILENO, &E.initial) == -1) die("tcgetattr");
	atexit(disableRawMode);
	struct termios raw = E.initial;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);//conto; various signals by combinations of keys
	raw.c_oflag &= ~(OPOST);//stop output processing
 	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);//stop printing, read line by line, stop ctrl-c, ctrl-z
  	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
char editorReadKey() {
	int nread;
	char c;
  	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    		if (nread == -1 && errno != EAGAIN) die("read");
  	}
  	return c;
}
int getCursorPosition(int *rows, int *cols) {
	char buf[32];
  	unsigned int i = 0;
  	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
	while (i < sizeof(buf) - 1) {
    		if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    		if (buf[i] == 'R') break;
    		i++;
	}
  	buf[i] = '\0';
	if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
	return 0;
}
int getWindowSize(int *rows, int *cols) {//get size of terminal window to print proper number of tildes
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    		return getCursorPosition(rows, cols);
  	} 
  	else {
    		*cols = ws.ws_col;
    		*rows = ws.ws_row;
		return 0;
  	}
}
void editorDrawRows() {//printing the tildes to gain vi like interface
	int y;
  	for (y = 0; y < E.screenrows; y++) {
    		write(STDOUT_FILENO, "~", 1);
		if (y < E.screenrows - 1) {
      			write(STDOUT_FILENO, "\r\n", 2);
    		}
  	}
}
void editorRefreshScreen() {//set cursor and print tildes
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	editorDrawRows();
	write(STDOUT_FILENO, "\x1b[H", 3);
}
void editorProcessKeypress() {//handling keypresses. currently only handles ctrl-q for quitting the program
	char c = editorReadKey();
	switch (c) {
    	case CTRL_KEY('q'):
      		write(STDOUT_FILENO, "\x1b[2J", 4);
      		write(STDOUT_FILENO, "\x1b[H", 3);
      		exit(0);
      		break;
  	}
}
void initEditor() {//initializes editor configurations
	if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
}
int main() {
	enableRawMode();//we dont see keypresses currently
	initEditor();
	while (1) {
    		editorRefreshScreen();
    		editorProcessKeypress();
  	}
	return 0;
}

