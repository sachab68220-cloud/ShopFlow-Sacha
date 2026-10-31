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
		printf("Email: %s\n", b->clients[i].email);
		printf("Telephone: %s\n", b->clients[i].telephone);
		printf("Adresse: %s, %s, %s, %s\n", b->clients[i].adresse.rue, b->clients[i].adresse.ville, b->clients[i].adresse.codePostal, b->clients[i].adresse.pays);
		printf("Points de fidelites: %d\n", b->clients[i].pointsFidelite);
		printf("Nombre de commande: %d\n", b->clients[i].nbCommandes);
	}
}



int ajouterClient(Boutique* b)
{
	Client c;
	int i;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|          AJOUTER DES CLIENTS         |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	do
	{
		printf("Mail du nouveau client: ");
		scanf("%79s", c.email);

		if (strchr(c.email, '@') == NULL)
		{
			color(31);
			printf("L'email doit contenir un @ !\n");
			color(0);
		}
	} while (strchr(c.email, '@') == NULL);

	for (i = 0; i < b->nbClients; i++)
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

	do
	{
		printf("Telephone: ");
		scanf("%14s", c.telephone);
	} while (VerifChiffres(c.telephone, 10) == 0);

	printf("Rue: ");
	scanf(" %79[^\n]", c.adresse.rue);

	printf("Ville: ");
	scanf(" %49[^\n]", c.adresse.ville);

	do
	{
		printf("Code postal: ");
		scanf("%5s", c.adresse.codePostal);
	} while (VerifChiffres(c.adresse.codePostal, 5) == 0);

	printf("Pays: ");
	scanf(" %49[^\n]", c.adresse.pays);

	c.idClient = b->idClientSuivant;
	b->idClientSuivant = b->idClientSuivant + 1;
	c.pointsFidelite = 0;
	c.nbCommandes = 0;
	c.capaciteCommandes = 0;
	c.commandes = 0;
	c.actif = 1;

	if (b->nbClients == b->capaciteClients)
	{
		Client* NouveauTableau;
		NouveauTableau = realloc(b->clients, (b->capaciteClients + 5) * sizeof(Client));

		if (NouveauTableau == NULL)
		{
			color(31);
			printf("Erreur: plus de memoire");
			color(0);
			return 0;
		}

		b->clients = NouveauTableau;
		b->capaciteClients = b->capaciteClients + 5;
	}

	b->clients[b->nbClients] = c;
	b->nbClients = b->nbClients + 1;

	color(32);
	printf("Client ajoute avec succee !\n\n");
	color(0);
	return 1;

}





int rechercheClient(Boutique* b, int id)
{
	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|         RECHERCHER DES CLIENTS       |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	int choix;
	printf("1. Chercher par ID\n");
	printf("2. Chercher par Nom/Prenom");

	if (choix == 1)
	{
		int i;
		for (i = 0; i < b->nbClients; i++)
		{
			if (b->clients[i].idClient == id)
			{
				return id;
			}
		}
		color(31);
		printf("Ce client n'existe pas !");
		color(0);
		return -1;
	}
	else if (choix == 2)
	{
		int i;
		for (i = 0; i < b->nbClients; i++)
		{
			if (strcmp(b->clients[i].telephone, id) == O)
			{
				return id;
			}
		}
		color(31);
		printf("Ce client n'existe pas !");
		color(0);
		return -1;
	}
	else
	{
		return;
	}
}
