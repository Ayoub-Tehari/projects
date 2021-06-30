#ifndef __READCMD_H
#define __READCMD_H

/* Read a command line from input stream. Return null when input closed.
Display an error and call exit() in case of memory exhaustion. */

enum commands {UNKNOWN, GET, BYE, LS, PWD};

/* Structure returned by readcmd() */
struct cmdline {
	char *err;	/* If not null, it is an error message that should be
			   displayed. The other fields are null. */
	enum commands type;
	char fichier[256];	
};


void memory_error(void);

void *xmalloc(size_t size);

void *xrealloc(void *ptr, size_t size);

char *readline(void);
char **split_in_words(char *line);
void freewords(char **words);
char *ajoute(char *destination, char *source) ;

struct cmdline *readcmd(void);
#endif

