/*
 * echo - read and echo text lines until client closes connection
 */
#include "csapp.h"
#include "types.h"
#include "readcmd.h"

void echo(int connfd)
{
    size_t n;
    struct cmdline l ;
    char *dossier;
    pid_t pid ;
    //int status;
    //int tube [2];
    while (1) {
    	n = Rio_readn(connfd, &l, sizeof(struct cmdline));
        printf("server received %u bytes\n", (unsigned int)n);
        switch (l.type) {
        	case BYE: printf("bye : type de commande\n"); return;
        	case LS :
        		/*if (pipe(tube) == -1 ){
				perror("pipe error");
				continue;
			}*/
        		pid = Fork();
        		if (pid == 0) {
        			//close (tube[0]);
	  			char *argv[2];
   				argv[0] = "ls"; argv[1] = NULL;
   				dup2(connfd, STDOUT_FILENO);
	  			execvp(argv[0], argv);
	  			
	  		}
	  		
	  		break;
	  	case PWD :
        		pid= Fork();
        		if (pid == 0) {
	  			char *argv[2];
   				argv[0] = "pwd"; argv[1] = NULL;
   				dup2(connfd, STDOUT_FILENO);
	  			execvp(argv[0], argv);
	  			
	  		}
	  		break;
        	case GET:
        	printf("get : type de commande\n");
        	printf("%s : nom du fichier\n", l.fichier);
        	dossier = calloc(2*MAXNAME, sizeof(char));
    		strcpy (dossier, "serverFiles/");
        	strcat(dossier, l.fichier);
		int fdIn = open (dossier, O_RDONLY, 0);
		free(dossier);
		sizeIfExist msg0;
		
		Rio_readn(connfd, &msg0, sizeof(sizeIfExist));
		
		lseek(fdIn, msg0.st_size, SEEK_SET);
		sizeIfExist msg1;
		struct stat total;
		if (fdIn < 0 ) {
			fprintf(stderr, "ERREUR OUVERTURE DE FICHIER\n");
			msg1.exist=0;
			msg1.st_size=0;
			
		
		}else {		
			fstat(fdIn, &total);
			msg1.exist = 1;
			msg1.st_size = total.st_size - msg0.st_size;
		}

		rio_writen(connfd, &msg1, sizeof(sizeIfExist));

		if ( fdIn < 0 ) 
			goto returning;
		
		data buf ;
		ssize_t lue;
		ssize_t resultat;
		while ( (lue = Rio_readn(fdIn, buf.donnee, MAXDATA)) > 0 ) {
			buf.taille = lue;
			resultat = rio_writen(connfd, &buf, sizeof(data));
			if ( resultat == -1 ) {
				if (errno == EPIPE) {
					close(fdIn);
					return ;
				}
			}
			
		}
	returning:
		close(fdIn);
		break;
	default : fprintf(stderr, "command not found");
	}
    }
}

