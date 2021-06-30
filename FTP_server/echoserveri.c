/*
 * echoserveri.c - An iterative echo server
 */

#include "csapp.h"

#define MAX_NAME_LEN 256
#define NBRP 5
#define PORT_ECOUTE 2121

int *connfd;

pid_t pids[NBRP] ;

void echo(int connfd);

void sendToMaster(int *port, char *ip) {
    
    int slaveMasterfd, Masterport;
    char* masterIP = ip;
   
    Masterport = 3333;

    slaveMasterfd = Open_clientfd(masterIP, Masterport);
    printf("slave connected to master\n"); 
    
    Rio_writen(slaveMasterfd, port, sizeof(int));
    close(slaveMasterfd);
}    


int isFather(){
	int i;
	for(i=0; i<NBRP ; i++ ){
		if ( pids[i] == 0 )
			return 0;
	}
	return 1;
}

void pool_processes () {
	int i;
	for (i=0; i< NBRP ; i++) {
		pids [i] = Fork();
		if (pids[i] == 0) {
			break;
		}		
	}
	

}


void arret (int signal) {
	int i;
	for (i=0; i<NBRP ; i++) {
		kill(pids[i], SIGINT);
	}
	exit(0);
}

int distinct(){
	int i, c=0;
	for(i=0; i<NBRP ; i++ ){
		if( pids[i] == 0){
			c++;
		}
	}
	return c;
}
/* 
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supported)
 */
int main(int argc, char **argv)
{
	
	
	Signal (SIGINT, arret);
	
    int listenfd, port;
    char *host;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    connfd = malloc (sizeof(int));
    if (argc != 2) {
        fprintf(stderr, "usage: %s <MASTER_IP>\n", argv[0]);
        exit(0);
    }
    port = PORT_ECOUTE;
    host = argv[1];
    clientlen = (socklen_t)sizeof(clientaddr);
    listenfd = Open_listenfd(port);
    
    pool_processes();
    
    sendToMaster(&port, host);
    
    signal(SIGPIPE, SIG_IGN);
    while (1) {
        
        (*connfd) = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        /* determine the name of the client */
        Getnameinfo((SA *) &clientaddr, clientlen,
                    client_hostname, MAX_NAME_LEN, 0, 0, 0);
        
        /* determine the textual representation of the client's IP address */
        Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                  INET_ADDRSTRLEN);
        
        printf("server connected to %s (%s)\n", client_hostname,
               client_ip_string);
	printf("connfd = %d \n",*connfd);
	echo(*connfd);
        
        Close(*connfd);
    }
    exit(0);
}

