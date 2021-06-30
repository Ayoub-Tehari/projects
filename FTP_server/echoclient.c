/*
 * echoclient.c - An echo client
 */
#include "csapp.h"
#include "types.h"
#include <time.h>
#include "readcmd.h"
#define CHIFFRES 4

void mettreFin (int flag) {
     if (flag)
	printf("exit\n");
     exit(0);
}

int main(int argc, char **argv)
{
    int clientfd, port;
    char *host;
    rio_t rio;
    data buf;
    
    if (argc != 2) {
        fprintf(stderr, "usage: %s <MASTER_IP>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = 2222;

    /*
     * Note that the 'host' can be a name or an IP address.
     * If necessary, Open_clientfd will perform the name resolution
     * to obtain the IP address.
     */
    clientfd = Open_clientfd(host, port);
    
    /*
     * At this stage, the connection is established between the client
     * and the server OS ... but it is possible that the server application
     * has not yet called "Accept" for this connection
     */
    printf("client connected to server OS\n"); 
    idconnx slaveid;
    Rio_readn(clientfd, &slaveid, sizeof(idconnx));
    close (clientfd);
    clientfd = Open_clientfd(slaveid.ipAddress, slaveid.port);
	struct cmdline *l;
	while(1) {
		printf("FTP> ");
		l = readcmd();

		/* If input stream closed, normal termination */
		if (!l) 
			mettreFin(1);

		if (l->err != NULL) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
			
		}

		Rio_readinitb(&rio, clientfd);
		
		time_t debut, fin;
		
	    	
		debut = time(NULL);
		sizeIfExist req1;
		char *nameOut;
		char string_number[CHIFFRES];
		char *res_pwd;
		char* res_ls;
		
		//ECRIRE LE NOM DANS LA SOCKET
		Rio_writen(clientfd, l, sizeof(struct cmdline));
		switch (l->type) {
			case BYE :        /* the server has prematurely closed the connection */
				Close(clientfd);
    				mettreFin(1);
    			case LS :
    				
    				res_ls= malloc(sizeof(char)*MAXLINE);
    				memset (res_ls, 0, MAXLINE);
    				ssize_t taille_lue;
    				while ((taille_lue = read(clientfd, res_ls, MAXLINE-1))>0){
    					printf("%s\n",res_ls);
    					memset (res_ls, 0, MAXLINE);
    				}
    				free(res_ls);

    				break;
    			case PWD : 
    				
    				res_pwd = malloc(sizeof(char)*MAXLINE);
    				memset (res_pwd, 0, MAXLINE);
    				read(clientfd, res_pwd, MAXLINE-1);
    				printf("%s\n",res_pwd);
    				free(res_pwd);
    				break;
			case GET :

			
			
			//OUVRIRE LE FICHIER EN ECRITURE
			//char string_number[CHIFFRES];
			
			nameOut = malloc (2*MAXNAME);
			int fdOut;

			memset(nameOut, 0, 2*MAXNAME);
			strcpy(nameOut, "clientFiles/");
			strcat(nameOut, l->fichier);
			strcat(nameOut, ".part");			 
			fdOut = Open(nameOut, O_WRONLY|O_CREAT, S_IRWXU|S_IROTH);
			struct stat tailleLue;
			fstat(fdOut, &tailleLue);
			lseek(fdOut, tailleLue.st_size, SEEK_SET);
			req1.st_size = tailleLue.st_size; 
			Rio_writen(clientfd, &req1, sizeof(sizeIfExist));
			
			//LIRE LA TAILLE DU FICHIER DANS a
			sizeIfExist tailleALire;
			Rio_readnb(&rio, &tailleALire, sizeof(sizeIfExist));
			
			if ( tailleALire.exist == 0) {
				fprintf(stderr, "file does not exist in the server\n");
				continue;
			}
			
			
			
		
			
			//COMMENCER A RECEVOIR LES PAQUETS
			ssize_t received;
			ssize_t somme = 0;
			
			printf("taille du fichier est = %ld \n",tailleALire.st_size+tailleLue.st_size);
			
		 	while ((received =Rio_readnb(&rio, &buf, sizeof(data))) > 0) {
		    		Rio_writen(fdOut, buf.donnee, buf.taille);
				somme += buf.taille;
				//printf("taille lue est = %ld \n",somme);
				if (somme == tailleALire.st_size) {
					printf("Transfer successfully complete.\n");
				
					fin = time(NULL) - debut;
					printf("%ld bytes received in %ld seconds ", tailleALire.st_size, fin);
					if (fin == 0) {
							printf ("(+l'infinie Kbytes/s).\n");		
					}else{
						double vitesse = tailleALire.st_size/fin;
						long int kibi = 1;
						kibi <<= 10;
						vitesse = vitesse / kibi;
						printf ("(%lf Kbytes/s).\n", vitesse);
					}
					//TROUVEER LE BON NOM POUR LE FICHIER EN ECRITURE
					int i = 0;
					int fdOut2;
					char *finalNameOut;
					finalNameOut = malloc(2*MAXNAME);
					do {
						memset(finalNameOut, 0, 2*MAXNAME);
						strcpy(finalNameOut, "clientFiles/");
						strcat(finalNameOut, l->fichier);
						
						i++;
						memset(string_number, 0, CHIFFRES);
						snprintf(string_number, CHIFFRES, "%d", i);
						
						strcat(finalNameOut, string_number);
						fdOut2 = open(finalNameOut, O_WRONLY, 0);
					 	if (fdOut2 < 0) {
					 		break;
					 	}
					 }while (i<1000);
					 close (fdOut2);
					 rename(nameOut, finalNameOut);
					 free(nameOut);
					 free(finalNameOut);
					goto termine;
				}
				
			} 
			printf("Transfer failed. \n");
			termine:
			close(fdOut);	
			break;
		default :
			fprintf(stderr, "commande not supported\n");
		}
	}	
 
           
        
    
    
}
