#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clients.h"
#include "utils.h"

void afficherClient(Boutique* b)
{
	system("cls");
	int i;

	color(96);
	printf("+--------------------------------------+\n");
	printf("|         CATALOGUE DES CLIENTS        |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	for (i = 0; i < b->nbClients; i++)
	{
		if (b->clients[i].actif == 0)
		{
			continue;
		}

		printf("----------------------------------------------------------\n");
		printf("Client n%d: %s %s\n", b->clients[i].idClient, b->clients[i].nom, b->clients[i].prenom);
		printf("Email: %s\n", b->clients[i].adresse);
		printf("Telephone: %s\n", b->clients[i].telephone);
		printf("Adresse: %s, %s, %s, %s\n", b->clients[i].adresse.rue, b->clients[i].adresse.ville, b->clients[i].adresse.codePostal, b->clients[i].adresse.pays);
		printf("Points de fidelites: %d\n", b->clients[i].pointsFidelite);
		printf("Nombre de commande: %d\n", b->clients[i].nbCommandes);
	}
}



void ajouterClient(Boutique* b)
{
	Client c;
	int i;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|         CATALOGUE DES CLIENTS        |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("Mail du nouveau client: ");
	scanf("%79s", c.email);

	for (i = 0, i < b->nbClients; i++)
	{
		if (strcmp(b->clients[i].email, c.email) == 0)
		{
			color(31);
			printf("Ce client existe deja !\n");
			color(0);
			return 0;
		}
	}

	printf("Nom: ");
	scanf("%s", c.nom);

	printf("Prenom: ");
	scanf("%s", c.prenom);

	printf("Telephone: ");
	scanf("%s", c.telephone);


}
