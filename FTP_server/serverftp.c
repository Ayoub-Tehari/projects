#include <csapp.h>
#define NBRP 5


pid_t pids[NBRP] ;

void pool_processes () {
	int i;
	for (i=0; i< NBRP ; i++) {
		pids [i] = Fork();
		if (pids[i] != 0) {
			kill(SIGSTOP, pids[i]);
		}
	}
}

int main (int argc, char **argv) {
	pool_processes();	
}

