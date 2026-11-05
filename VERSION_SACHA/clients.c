#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clients.h"
#include "commandes.h"
#include "utils.h"

/* Chiffre d'affaires HT genere par un client (commandes non annulees) */
float chiffreAffairesClient(Client* client)
{
	int i;
	float total = 0;

	for (i = 0; i < client->nbCommandes; i++)
	{
		if (strcmp(client->commandes[i].statut, "ANNULEE") != 0)
		{
			total = total + client->commandes[i].montantHT;
		}
	}

	return total;
}

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
		printf("Chiffre d'affaires genere: %.2f euros HT\n", chiffreAffairesClient(&b->clients[i]));
	}
	printf("----------------------------------------------------------\n\n");
}



/* Fiche detaillee d'un client avec l'historique de ses commandes */
void afficherUnClient(Boutique* b)
{
	int id;
	int position;
	int i;
	Commande* c;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|         RECHERCHER DES CLIENTS       |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("Entrez l'ID du client: ");
	id = lireEntier();

	position = rechercheClient(b, id);

	if (position == -1)
	{
		return;
	}

	color(32);
	printf("Client trouve !\n\n");
	color(0);
	printf("Client n%d: %s %s", b->clients[position].idClient, b->clients[position].nom, b->clients[position].prenom);
	if (b->clients[position].actif == 0)
	{
		color(31);
		printf(" (supprime)");
		color(0);
	}
	printf("\n");
	printf("Email: %s\n", b->clients[position].email);
	printf("Telephone: %s\n", b->clients[position].telephone);
	printf("Rue: %s\n", b->clients[position].adresse.rue);
	printf("Ville: %s\n", b->clients[position].adresse.ville);
	printf("Code Postal: %s\n", b->clients[position].adresse.codePostal);
	printf("Pays: %s\n", b->clients[position].adresse.pays);
	color(32);
	printf("Points de fidelite: %d\n", b->clients[position].pointsFidelite);
	color(0);
	printf("Chiffre d'affaires genere: %.2f euros HT\n\n", chiffreAffairesClient(&b->clients[position]));

	/* Historique des commandes */
	printf("Historique des commandes (%d):\n", b->clients[position].nbCommandes);
	printf("----------------------------------------------------------\n");

	if (b->clients[position].nbCommandes == 0)
	{
		printf("Aucune commande.\n");
	}

	for (i = 0; i < b->clients[position].nbCommandes; i++)
	{
		c = &b->clients[position].commandes[i];
		printf("n%d  %02d/%02d/%d  %-9s  %d ligne(s)  %8.2f euros TTC\n", c->numero, c->dateCommande.jour, c->dateCommande.mois, c->dateCommande.annee, c->statut, c->nbLignes, c->montantTTC);
	}
	printf("----------------------------------------------------------\n\n");
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
	scanf(" %49[^\n]", c.nom);

	printf("Prenom: ");
	scanf(" %49[^\n]", c.prenom);

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
	id = lireEntier();
	
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
	scanf(" %49[^\n]", b->clients[position].nom);

	printf("Nouveau prenom: ");
	scanf(" %49[^\n]", b->clients[position].prenom);

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
	id = lireEntier();

	position = rechercheClient(b, id);

	if (position == -1)
	{
		return;
	}

	if (b->clients[position].actif == 0)
	{
		color(31);
		printf("Ce client est deja supprime !\n\n");
		color(0);
		return;
	}

	/* Refuse si le client a des commandes non annulees */
	if (clientACommandeActive(&b->clients[position]) == 1)
	{
		color(31);
		printf("Impossible : ce client a des commandes non annulees !\n\n");
		color(0);
		return;
	}

	b->clients[position].actif = 0;
	color(32);
	printf("Client supprime avec succes !\n\n");
	color(0);
}
