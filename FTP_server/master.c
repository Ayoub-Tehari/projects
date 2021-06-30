/*
 * echoserveri.c - An iterative echo server
 */

#include "csapp.h"
#include "types.h"
#define MAX_NAME_LEN 256
int *fini;
int Pipe (int *tube) {
    int resultat;
    if ((resultat = pipe(tube)) == -1) {
        fprintf(stderr, "Erreur de création du tube\n");
        exit(1);
    }
    return resultat;
}
void tictoc(int sig) {
	close(*fini);
	exit(0);
}
/* 
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supported)
 */
int main(int argc, char **argv)
{
    int connfd;

    int listenfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    
    if (argc != 1) {
	fprintf(stderr, "usage: %s\n", argv[0]);
	exit(0);
    }
    int *taille;
    taille = malloc(sizeof(int));
    int tube[2];
   Pipe(tube);
    
	if (Fork()!=0){
		close (tube[0]);
		fini = &(tube[1]);
		signal(SIGALRM, tictoc);
	    port = 3333;
	    
	    clientlen = (socklen_t)sizeof(clientaddr);
	    
	    listenfd = Open_listenfd(port);
	    
	    (*taille)= 0;
	    while (1) {
		alarm(10);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		alarm(0);
		/* determine the name of the client */
		Getnameinfo((SA *) &clientaddr, clientlen,
		            client_hostname, MAX_NAME_LEN, 0, 0, 0);
		
		/* determine the textual representation of the client's IP address */
		Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
		          INET_ADDRSTRLEN);
		
		printf("server connected to %s (%s)\n", client_hostname,
		       client_ip_string);
		(*taille) = 1 + (*taille);
		idconnx id1;
		strcpy(id1.ipAddress, client_ip_string);
		Rio_readn(connfd, &(id1.port), sizeof(int));
		Rio_writen(tube[1], &id1, sizeof(idconnx));
		Close(connfd);
	    }
	    
	    close(tube[1]);
	    exit(0);
	} else {
		signal(SIGPIPE, SIG_IGN);
		close(tube[1]);
		idconnx id2;
		idconnx tab[12];
		int taille = 0;
		int lue;
		while ((lue = rio_readn(tube[0], &id2, sizeof(idconnx))) >0) {
			tab[taille++]=id2; 
		}
		close(tube[0]);
	    port = 2222;
	    if (taille ==0 ) return 10;
	    clientlen = (socklen_t)sizeof(clientaddr);
	    
	    listenfd = Open_listenfd(port);
	    int choix=0;
	    
	    while (1) {
		
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

		/* determine the name of the client */
		Getnameinfo((SA *) &clientaddr, clientlen,
		            client_hostname, MAX_NAME_LEN, 0, 0, 0);
		
		/* determine the textual representation of the client's IP address */
		Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
		          INET_ADDRSTRLEN);
		
		printf("server connected to %s (%s)\n", client_hostname,
		       client_ip_string);
		
		Rio_writen(connfd, &(tab[choix]), sizeof(idconnx));
		choix = (choix + 1) % taille;
		
		Close(connfd);
	    }
	    exit(0);	
	}
}

