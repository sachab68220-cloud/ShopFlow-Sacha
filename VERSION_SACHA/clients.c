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

	int i;

	for (i = 0; i < b->nbClients; i++)
	{
		if (b->clients[i].idClient == id)
		{
			return i;
		}
	}
	color(31);
	printf("Ce client n'existe pas !\n");
	color(0);
	return -1;
}




void modifierClient(Boutique* b)
{
	int id;
	int position;
	char email[80];

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|         RECHERCHER DES CLIENTS       |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("Entrez l'ID du client que vous voulez modifie: ");
	scanf("%d", &id);
	
	position = rechercheClient(b, id);

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|          MODIFIER DES CLIENTS        |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	if (position == -1)
	{
		return;
	}

	if (b->clients[position].actif == 0)
	{
		color(31);
		printf("Ce client a ete supprime !\n\n");
		color(0);
		return;
	}

	color(32);
	printf("Client trouve !\n\n");
	color(0);

	printf("Nom actuel: %s\n", b->clients[position].nom);
	printf("Prenom actuel: %s\n", b->clients[position].prenom);
	printf("Email actuel: %s\n", b->clients[position].email);
	printf("Telephone actuel: %s\n", b->clients[position].telephone);
	printf("Rue actuelle: %s\n", b->clients[position].adresse.rue);
	printf("Ville actuelle: %s\n", b->clients[position].adresse.ville);
	printf("Code postal actuel: %s\n", b->clients[position].adresse.codePostal);
	printf("Pays actuel: %s\n\n", b->clients[position].adresse.pays);

	printf("----------------------------------------------------------\n\n");

	printf("Nouveau nom: ");
	scanf("%s", &b->clients[position].nom);

	printf("Nouveau prenom: ");
	scanf("%s", &b->clients[position].prenom);

	do
	{
		printf("Nouvelle email: ");
		scanf("%79s", email);

		if (strchr(email, '@') == NULL)
		{
			color(31);
			printf("L'email doit contenir un @ !\n");
			color(0);
		}
	} while (strchr(email, '@') == NULL);

	int i;
	for (i = 0; i < b->nbClients; i++)
	{
		if (i != position && strcmp(b->clients[i].email, email) == 0)
		{
			color(31);
			printf("Cet email est deja utilise !\n\n");
			color(0);
			return;
		}
	}
	strcpy(b->clients[position].email, email);

	do
	{
		printf("Nouveau telephone: ");
		scanf("%14s", b->clients[position].telephone);
	} while (VerifChiffres(b->clients[position].telephone, 10) == 0);

	printf("Nouvelle Rue: ");
	scanf(" %79[^\n]", b->clients[position].adresse.rue);

	printf("Nouvelle ville: ");
	scanf(" %49[^\n]", b->clients[position].adresse.ville);

	do
	{
		printf("Nouveau code postal: ");
		scanf("%5s", b->clients[position].adresse.codePostal);
	} while (VerifChiffres(b->clients[position].adresse.codePostal, 5) == 0);

	printf("Nouveau pays: ");
	scanf(" %49[^\n]", b->clients[position].adresse.pays);

	color(32);
	printf("Client modifie avec succes !\n\n");
	color(0);
	
}



void supprimerClient(Boutique* b)
{
	int id;
	int position;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|         RECHERCHER DES CLIENTS       |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("Entrez l'ID du client a supprime: ");
	scanf("%d", &id);

	position = rechercheClient(b, id);

	if (position == -1)
	{
		color(32);
		printf("Ce client n'existe pas !\n\n");
		color(0);
		return -1;
	}

	if (b->clients[position].actif == 0)
	{
		color(31);
		printf("Ce client est deja supprime !\n\n");
		color(0);
		return;
	}

	/*A faire plus tard mais on peut pas supp un client avec une commande en cours*/

	b->clients[position].actif = 0;
	color(32);
	printf("Client supprime avec succes !\n\n");
	color(0);
}
