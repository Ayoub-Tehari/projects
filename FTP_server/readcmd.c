#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "readcmd.h"
#include "types.h"

void memory_error(void)
{
	errno = ENOMEM;
	perror(0);
	exit(1);
}


void *xmalloc(size_t size)
{
	void *p = malloc(size);
	if (!p) memory_error();
	return p;
}


void *xrealloc(void *ptr, size_t size)
{
	void *p = realloc(ptr, size);
	if (!p) memory_error();
	return p;
}


/* Read a line from standard input and put it in a char[] */
char *readline(void)
{
	size_t buf_len = 16;
	char *buf = xmalloc(buf_len * sizeof(char));

	if (fgets(buf, buf_len, stdin) == NULL) {
		free(buf);
		return NULL;
	}
	
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	do {
		size_t l = strlen(buf);
		if ((l > 0) && (buf[l-1] == '\n')) {
			l--;
			buf[l] = 0;
			return buf;
		}
		if (buf_len >= (INT_MAX / 2)) memory_error();
		buf_len *= 2;
		buf = xrealloc(buf, buf_len * sizeof(char));
		if (fgets(buf + l, buf_len - l, stdin) == NULL) return buf;
	} while (1);
}


/* Split the string in words, according to the simple shell grammar. */
char **split_in_words(char *line)
{
	char *cur = line;
	char **tab = 0;
	size_t l = 0;
	char c;

	while ((c = *cur) != 0) {
		char *w = 0;
		char *start;
		switch (c) {
		case ' ':
		case '\t':
			/* Ignore any whitespace */
			cur++;
			break;
		default:
			/* Another word */
			start = cur;
			while (c) {
				c = *++cur;
				switch (c) {
				case 0:
				case ' ':
				case '\t':
					c = 0;
					break;
				default: ;
				}
			}
			w = xmalloc((cur - start + 1) * sizeof(char));
			strncpy(w, start, cur - start);
			w[cur - start] = 0;
		}
		if (w) {
			tab = xrealloc(tab, (l + 1) * sizeof(char *));
			tab[l++] = w;
		}
	}
	tab = xrealloc(tab, (l + 1) * sizeof(char *));
	tab[l++] = 0;
	return tab;
}


/* Free the fields of the structure but not the structure itself */
void freewords(char **words)
{
	int i;

	for (i=0; words[i]!=0; i++) {
		free(words[i]);

	}
	free(words);

}

char *ajoute(char *destination, char *source) {
	if (destination == NULL){
		destination = xmalloc(sizeof(char)*MAXNAME);
	}
	
	strncpy(destination, source, MAXNAME);
	if (strlen(source) >= MAXNAME) {
		destination[MAXNAME -1] = '\0';
	}
	
	return destination ;
}

struct cmdline *readcmd(void)
{
	struct cmdline *s = 0;
	char *line;
	char **words;
	char *w;
	line = readline();
	if (line == NULL) {
		return s = 0;
	}
	
	words = split_in_words(line);
	free(line);
	s = xmalloc(sizeof(struct cmdline));
	
	s->err = NULL;
	//s->fichier = {0};
	s->type = UNKNOWN;
	int i = 0;
	while ((w = words[i++]) != 0) {
		if (i == 1) {
			if (strcmp(w, "get") == 0){
				s->type = GET;
			}else if (strcmp(w, "bye") == 0) {
				s->type = BYE;
				goto end;
			}else if (strcmp(w, "ls") == 0) {
				s->type = LS ;
				goto end;
			}else if (strcmp(w, "pwd") == 0) {
				s->type = PWD ;
				goto end;
			}else {
				s->err = ajoute(s->err, "type de commande non reconnue");
				goto end;
			}
		}else {
			if (words[i] == 0) {
				strcpy (s->fichier, w);
				goto end;
			}else{
				s->err = ajoute(s->err, "only one file supported");
				goto end;
			}
		}	
	}

	if (s->type == UNKNOWN) 
		s->err = ajoute(s->err, "commande missed");
end:
	freewords(words);
	
	return s;
	
}

