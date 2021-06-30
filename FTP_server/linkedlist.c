#include "linkedlist.h"


Liste *initialisation()
{
	Liste *liste = malloc(sizeof(Liste));
	Element *element = malloc(sizeof(Element));

	if (liste == NULL || element == NULL)
	{
	exit(EXIT_FAILURE);
	}

	element->nombre = 0;
	element->cmd= NULL;
	element->suivant = NULL;
	liste->premier = element;

	return liste;
}

pid_t getPid(Liste *liste, int position) 
{
	if (liste == NULL || position < 1)
	{
	exit(EXIT_FAILURE);
	}
	Element *tmp = liste->premier;
	int i=1;
	while(i<position && tmp != NULL){
		tmp = tmp->suivant;i++;
	}
	if (tmp == NULL)
	{
	exit(EXIT_FAILURE);
	}
	return tmp->nombre;
}

void insertion(Liste *liste, idconnx id1)
{
	/* Création du nouvel élément */
	Element *nouveau = malloc(sizeof(nouveau));
	if (liste == NULL || nouveau == NULL)
	{
	exit(EXIT_FAILURE);
	}
	nouveau->nombre = nvNombre;
	int j;
	strcpy(nouveau->cmd, nom[0]);
	for (j=1; nom[j]!=0; j++) {
	strcat(nouveau->cmd, " ");
	strcat(nouveau->cmd, nom[j]);
	}

	Element *tmp = liste->premier;j=1;
	if (liste->taille == 0){
	free(liste->premier);
	liste->premier=nouveau;
	}else{

	while (tmp->suivant != NULL)
	 { tmp = tmp->suivant;j++;}

	/* Insertion de l'élément à la fin de la liste */
	nouveau->suivant = tmp->suivant;
	tmp->suivant = nouveau;
	}
	liste->taille++;
	printf("[%d] %d\n", j, nouveau->nombre);
}

void suppression(Liste *liste)
{
	if (liste == NULL)
	{
	exit(EXIT_FAILURE);
	}

	if (liste->premier != NULL)
	{
	Element *aSupprimer = liste->premier;
	liste->premier = liste->premier->suivant;
	free(aSupprimer);
	}
}

void afficherListe(Liste *liste)
{
	if (liste == NULL)
	{
	exit(EXIT_FAILURE);
	}

	Element *actuel = liste->premier;
	int i=1;
	printf("liste des jobs:\n");
	while (actuel != NULL)
	{
	printf("[%d] En cours d'exécution %s &\n", i, actuel->cmd);
	actuel = actuel->suivant;i++;
	}
}
