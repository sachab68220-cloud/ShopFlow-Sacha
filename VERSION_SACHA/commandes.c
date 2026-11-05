#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commandes.h"
#include "produits.h"
#include "utils.h"

void calculCommande(Commande* c)
{
	c->montantBrutHT = 0;

	int i;
	for (i = 0; i < c->nbLignes; i++)
	{
		c->lignes[i].sousTotalHT = c->lignes[i].quantite * c->lignes[i].prixUnitaireHT;
		c->montantBrutHT = c->montantBrutHT + c->lignes[i].sousTotalHT;
	}

	if (c->montantBrutHT < 100)
	{
		c->tauxRemise = 0;
	}

	else if (c->montantBrutHT < 300)
	{
		c->tauxRemise = 5;
	}

	else if (c->montantBrutHT < 600)
	{
		c->tauxRemise = 10;
	}
	else
	{
		c->tauxRemise = 15;
	}

	c->montantRemise = c->montantBrutHT * c->tauxRemise / 100;
	c->montantHT = c->montantBrutHT - c->montantRemise;
	c->montantTVA = c->montantHT * 20 / 100;

	if (c->montantBrutHT >= 60)
	{
		c->fraisPort = 0;
	}

	else
	{
		c->fraisPort = 5.90f;
	}

	c->montantTTC = c->montantHT + c->montantTVA + c->fraisPort;
}



int positionClient(Boutique* b, int id)
{
	int i;

	for (i = 0; i < b->nbClients; i++)
	{
		if (b->clients[i].idClient == id)
		{
			return i;
		}
	}

	return -1;
}



Commande* trouverCommande(Boutique* b, int numero)
{
	int i;
	int j;

	for (i = 0; i < b->nbClients; i++)
	{
		for (j = 0; j < b->clients[i].nbCommandes; j++)
		{
			if (b->clients[i].commandes[j].numero == numero)
			{
				return &b->clients[i].commandes[j];
			}
		}
	}

	return NULL;
}



int rattacherCommande(Client* client, Commande c)
{
	calculCommande(&c);

	if (client->nbCommandes == client->capaciteCommandes)
	{
		Commande* nouveauTableau;
		nouveauTableau = realloc(client->commandes, (client->capaciteCommandes + 5) * sizeof(Commande));

		if (nouveauTableau == NULL)
		{
			color(31);
			printf("Erreur: plus de memoire\n");
			color(0);
			return 0;
		}

		client->commandes = nouveauTableau;
		client->capaciteCommandes = client->capaciteCommandes + 5;
	}

	client->commandes[client->nbCommandes] = c;
	client->nbCommandes = client->nbCommandes + 1;

	if (strcmp(c.statut, "ANNULEE") != 0)
	{
		client->pointsFidelite = client->pointsFidelite + (int)(c.montantTTC / 10);
	}

	return 1;
}



void remettreStock(Boutique* b, Commande* c)
{
	int i;
	int pos;

	for (i = 0; i < c->nbLignes; i++)
	{
		pos = indexProduit(b, c->lignes[i].refProduit);

		if (pos != -1)
		{
			b->produits[pos].quantiteStock = b->produits[pos].quantiteStock + c->lignes[i].quantite;
		}
	}
}



int saisirLigne(Boutique* b, Commande* c)
{
	char ref[12];
	int pos;
	int quantite;
	int i;
	int ligneExistante = -1;

	printf("\nReference du produit: ");
	scanf("%11s", ref);

	pos = indexProduit(b, ref);

	if (pos == -1)
	{
		color(31);
		printf("Ce produit n'existe pas !\n");
		color(0);
		return 0;
	}

	if (b->produits[pos].actif == 0)
	{
		color(31);
		printf("Ce produit n'est plus vendu !\n");
		color(0);
		return 0;
	}

	if (b->produits[pos].quantiteStock == 0)
	{
		color(31);
		printf("Ce produit est en rupture de stock !\n");
		color(0);
		return 0;
	}

	printf("%s - %.2f euros HT (stock disponible: %d)\n", b->produits[pos].designation, b->produits[pos].prixUnitaireHT, b->produits[pos].quantiteStock);

	do
	{
		printf("Quantite: ");
		quantite = lireEntier();

		if (quantite < 1 || quantite > b->produits[pos].quantiteStock)
		{
			color(31);
			printf("La quantite doit etre entre 1 et %d !\n", b->produits[pos].quantiteStock);
			color(0);
		}
	} while (quantite < 1 || quantite > b->produits[pos].quantiteStock);

	for (i = 0; i < c->nbLignes; i++)
	{
		if (strcmp(c->lignes[i].refProduit, ref) == 0)
		{
			ligneExistante = i;
		}
	}

	if (ligneExistante != -1)
	{
		c->lignes[ligneExistante].quantite = c->lignes[ligneExistante].quantite + quantite;
	}
	else
	{
		if (c->nbLignes == c->capaciteLignes)
		{
			LigneCommande* nouveauTableau;
			nouveauTableau = realloc(c->lignes, (c->capaciteLignes + 5) * sizeof(LigneCommande));

			if (nouveauTableau == NULL)
			{
				color(31);
				printf("Erreur: plus de memoire\n");
				color(0);
				return 0;
			}

			c->lignes = nouveauTableau;
			c->capaciteLignes = c->capaciteLignes + 5;
		}

		strcpy(c->lignes[c->nbLignes].refProduit, b->produits[pos].reference);
		strcpy(c->lignes[c->nbLignes].designation, b->produits[pos].designation);
		c->lignes[c->nbLignes].quantite = quantite;
		c->lignes[c->nbLignes].prixUnitaireHT = b->produits[pos].prixUnitaireHT;
		c->nbLignes = c->nbLignes + 1;
	}

	b->produits[pos].quantiteStock = b->produits[pos].quantiteStock - quantite;

	color(32);
	printf("Ligne ajoutee !\n");
	color(0);
	return 1;
}



int produitDansCommande(Boutique* b, char* ref)
{
	int i;
	int j;
	int k;

	for (i = 0; i < b->nbClients; i++)
	{
		for (j = 0; j < b->clients[i].nbCommandes; j++)
		{
			if (strcmp(b->clients[i].commandes[j].statut, "ANNULEE") == 0)
			{
				continue;
			}

			for (k = 0; k < b->clients[i].commandes[j].nbLignes; k++)
			{
				if (strcmp(b->clients[i].commandes[j].lignes[k].refProduit, ref) == 0)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}



int clientACommandeActive(Client* client)
{
	int i;

	for (i = 0; i < client->nbCommandes; i++)
	{
		if (strcmp(client->commandes[i].statut, "ANNULEE") != 0)
		{
			return 1;
		}
	}

	return 0;
}



void creerCommande(Boutique* b)
{
	Commande c;
	int id;
	int posClient;
	int continuer;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|          CREER UNE COMMANDE          |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("ID du client: ");
	id = lireEntier();
	posClient = positionClient(b, id);

	if (posClient == -1 || b->clients[posClient].actif == 0)
	{
		color(31);
		printf("Ce client n'existe pas !\n");
		color(0);
		return;
	}

	printf("Client: %s %s\n\n", b->clients[posClient].nom, b->clients[posClient].prenom);

	do
	{
		printf("Date de la commande (jour mois annee, ex: 5 11 2026): ");
		c.dateCommande.jour = lireEntier();
		c.dateCommande.mois = lireEntier();
		c.dateCommande.annee = lireEntier();

		if (dateValide(c.dateCommande.jour, c.dateCommande.mois, c.dateCommande.annee) == 0)
		{
			color(31);
			printf("Cette date n'existe pas !\n");
			color(0);
		}
	} while (dateValide(c.dateCommande.jour, c.dateCommande.mois, c.dateCommande.annee) == 0);

	c.numero = b->numeroSuivant;
	c.idClient = id;
	strcpy(c.statut, "EN_COURS");
	c.adresseLivraison = b->clients[posClient].adresse;

	c.nbLignes = 0;
	c.capaciteLignes = 5;
	c.lignes = malloc(c.capaciteLignes * sizeof(LigneCommande));

	if (c.lignes == NULL)
	{
		color(31);
		printf("Erreur: plus de memoire\n");
		color(0);
		return;
	}

	do
	{
		saisirLigne(b, &c);

		printf("\nAjouter un autre produit ? (1 = oui, 0 = non): ");
		continuer = lireEntier();
	} while (continuer == 1);

	if (c.nbLignes == 0)
	{
		free(c.lignes);
		color(31);
		printf("Commande vide : elle n'est pas enregistree.\n");
		color(0);
		return;
	}

	if (rattacherCommande(&b->clients[posClient], c) == 0)
	{
		remettreStock(b, &c);
		free(c.lignes);
		return;
	}

	b->numeroSuivant = b->numeroSuivant + 1;

	afficherDetailCommande(b, &b->clients[posClient].commandes[b->clients[posClient].nbCommandes - 1]);

	color(32);
	printf("Commande creee avec succes !\n\n");
	color(0);
}



void afficherDetailCommande(Boutique* b, Commande* c)
{
	int i;
	int posClient;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|           COMMANDE n%d             |\n", c->numero);
	printf("+--------------------------------------+\n\n");
	color(0);

	posClient = positionClient(b, c->idClient);
	if (posClient != -1)
	{
		printf("Client: %s %s (n%d)\n", b->clients[posClient].nom, b->clients[posClient].prenom, c->idClient);
	}

	printf("Date: %02d/%02d/%d\n", c->dateCommande.jour, c->dateCommande.mois, c->dateCommande.annee);
	printf("Statut: %s\n", c->statut);
	printf("Livraison: %s, %s %s, %s\n\n", c->adresseLivraison.rue, c->adresseLivraison.codePostal, c->adresseLivraison.ville, c->adresseLivraison.pays);

	printf("----------------------------------------------------------\n");
	for (i = 0; i < c->nbLignes; i++)
	{
		printf("%d. %-10s %-20s %3d x %7.2f = %8.2f\n", i + 1,
			c->lignes[i].refProduit,
			c->lignes[i].designation,
			c->lignes[i].quantite,
			c->lignes[i].prixUnitaireHT,
			c->lignes[i].sousTotalHT);
	}
	printf("----------------------------------------------------------\n");

	printf("Montant brut HT:   %8.2f\n", c->montantBrutHT);
	printf("Remise (%2.0f %%):    -%8.2f\n", c->tauxRemise, c->montantRemise);
	printf("Montant HT:        %8.2f\n", c->montantHT);
	printf("TVA (20 %%):        %8.2f\n", c->montantTVA);
	printf("Frais de port:     %8.2f\n", c->fraisPort);
	color(32);
	printf("TOTAL TTC:         %8.2f\n\n", c->montantTTC);
	color(0);
}



void afficherCommande(Boutique* b)
{
	int numero;
	Commande* c;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|         AFFICHER UNE COMMANDE        |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("Numero de la commande: ");
	numero = lireEntier();
	c = trouverCommande(b, numero);

	if (c == NULL)
	{
		color(31);
		printf("Cette commande n'existe pas !\n");
		color(0);
		return;
	}

	afficherDetailCommande(b, c);
}



void ajouterLigneCommande(Boutique* b)
{
	int numero;
	int ancienPoints;
	int posClient;
	Commande* c;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|      AJOUTER UNE LIGNE A UNE CDE     |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("Numero de la commande: ");
	numero = lireEntier();
	c = trouverCommande(b, numero);

	if (c == NULL)
	{
		color(31);
		printf("Cette commande n'existe pas !\n");
		color(0);
		return;
	}

	if (strcmp(c->statut, "EN_COURS") != 0)
	{
		color(31);
		printf("On ne peut modifier qu'une commande EN_COURS (celle-ci est %s) !\n", c->statut);
		color(0);
		return;
	}

	ancienPoints = (int)(c->montantTTC / 10);

	if (saisirLigne(b, c) == 1)
	{
		calculCommande(c);

		posClient = positionClient(b, c->idClient);
		b->clients[posClient].pointsFidelite = b->clients[posClient].pointsFidelite - ancienPoints + (int)(c->montantTTC / 10);

		system("pause");
		afficherDetailCommande(b, c);
	}
}



void supprimerLigneCommande(Boutique* b)
{
	int numero;
	int choix;
	int i;
	int pos;
	int ancienPoints;
	int posClient;
	Commande* c;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|     SUPPRIMER UNE LIGNE D'UNE CDE    |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("Numero de la commande: ");
	numero = lireEntier();
	c = trouverCommande(b, numero);

	if (c == NULL)
	{
		color(31);
		printf("Cette commande n'existe pas !\n");
		color(0);
		return;
	}

	if (strcmp(c->statut, "EN_COURS") != 0)
	{
		color(31);
		printf("On ne peut modifier qu'une commande EN_COURS (celle-ci est %s) !\n", c->statut);
		color(0);
		return;
	}

	if (c->nbLignes == 1)
	{
		color(31);
		printf("La commande n'a qu'une ligne : utilisez plutot 'Annuler une commande'.\n");
		color(0);
		return;
	}

	for (i = 0; i < c->nbLignes; i++)
	{
		printf("%d. %s - %s x%d\n", i + 1, c->lignes[i].refProduit, c->lignes[i].designation, c->lignes[i].quantite);
	}

	do
	{
		printf("\nNumero de la ligne a supprimer: ");
		choix = lireEntier();
	} while (choix < 1 || choix > c->nbLignes);

	choix = choix - 1;

	pos = indexProduit(b, c->lignes[choix].refProduit);
	if (pos != -1)
	{
		b->produits[pos].quantiteStock = b->produits[pos].quantiteStock + c->lignes[choix].quantite;
	}

	for (i = choix; i < c->nbLignes - 1; i++)
	{
		c->lignes[i] = c->lignes[i + 1];
	}
	c->nbLignes = c->nbLignes - 1;

	ancienPoints = (int)(c->montantTTC / 10);
	calculCommande(c);

	posClient = positionClient(b, c->idClient);
	b->clients[posClient].pointsFidelite = b->clients[posClient].pointsFidelite - ancienPoints + (int)(c->montantTTC / 10);

	color(32);
	printf("Ligne supprimee !\n");
	color(0);
	system("pause");
	afficherDetailCommande(b, c);
}



void changerStatut(Boutique* b)
{
	int numero;
	Commande* c;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|          CHANGER LE STATUT           |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("Numero de la commande: ");
	numero = lireEntier();
	c = trouverCommande(b, numero);

	if (c == NULL)
	{
		color(31);
		printf("Cette commande n'existe pas !\n");
		color(0);
		return;
	}

	if (strcmp(c->statut, "EN_COURS") == 0)
	{
		strcpy(c->statut, "EXPEDIEE");
	}
	else if (strcmp(c->statut, "EXPEDIEE") == 0)
	{
		strcpy(c->statut, "LIVREE");
	}
	else
	{
		color(31);
		printf("Impossible : la commande est deja %s.\n", c->statut);
		color(0);
		return;
	}

	color(32);
	printf("La commande n%d est maintenant %s.\n", c->numero, c->statut);
	color(0);
}



void annulerCommande(Boutique* b)
{
	int numero;
	int posClient;
	Commande* c;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|         ANNULER UNE COMMANDE         |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("Numero de la commande: ");
	numero = lireEntier();
	c = trouverCommande(b, numero);

	if (c == NULL)
	{
		color(31);
		printf("Cette commande n'existe pas !\n");
		color(0);
		return;
	}

	if (strcmp(c->statut, "EN_COURS") != 0)
	{
		color(31);
		printf("On ne peut annuler qu'une commande EN_COURS (celle-ci est %s) !\n", c->statut);
		color(0);
		return;
	}

	remettreStock(b, c);

	posClient = positionClient(b, c->idClient);
	b->clients[posClient].pointsFidelite = b->clients[posClient].pointsFidelite - (int)(c->montantTTC / 10);

	if (b->clients[posClient].pointsFidelite < 0)
	{
		b->clients[posClient].pointsFidelite = 0;
	}

	strcpy(c->statut, "ANNULEE");

	color(32);
	printf("Commande n%d annulee, stock remis a jour.\n", c->numero);
	color(0);
}
