#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statistiques.h"
#include "clients.h"
#include "utils.h"

int quantiteVendue(Boutique* b, char* ref)
{
	int i;
	int j;
	int k;
	int total = 0;

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
					total = total + b->clients[i].commandes[j].lignes[k].quantite;
				}
			}
		}
	}

	return total;
}


void chiffreAffaires(Boutique* b)
{
	int i;
	int j;
	int annee;
	int nbCommandes = 0;
	float totalHT = 0;
	float totalTTC = 0;
	float moisHT[12] = { 0 };
	float moisTTC[12] = { 0 };
	float anneeHT = 0;
	float anneeTTC = 0;
	Commande* c;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|          CHIFFRE D'AFFAIRES          |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	for (i = 0; i < b->nbClients; i++)
	{
		for (j = 0; j < b->clients[i].nbCommandes; j++)
		{
			c = &b->clients[i].commandes[j];

			if (strcmp(c->statut, "ANNULEE") != 0)
			{
				totalHT = totalHT + c->montantHT;
				totalTTC = totalTTC + c->montantTTC;
				nbCommandes = nbCommandes + 1;
			}
		}
	}

	printf("Commandes prises en compte: %d\n", nbCommandes);
	color(32);
	printf("CA total HT:  %10.2f euros\n", totalHT);
	printf("CA total TTC: %10.2f euros\n\n", totalTTC);
	color(0);

	printf("Voir le detail de quelle annee ? ");
	annee = lireEntier();

	for (i = 0; i < b->nbClients; i++)
	{
		for (j = 0; j < b->clients[i].nbCommandes; j++)
		{
			c = &b->clients[i].commandes[j];

			if (strcmp(c->statut, "ANNULEE") != 0 && c->dateCommande.annee == annee)
			{
				moisHT[c->dateCommande.mois - 1] = moisHT[c->dateCommande.mois - 1] + c->montantHT;
				moisTTC[c->dateCommande.mois - 1] = moisTTC[c->dateCommande.mois - 1] + c->montantTTC;
				anneeHT = anneeHT + c->montantHT;
				anneeTTC = anneeTTC + c->montantTTC;
			}
		}
	}

	printf("\nMois        CA HT       CA TTC\n");
	printf("----------------------------------\n");
	for (i = 0; i < 12; i++)
	{
		printf("%02d/%d  %10.2f  %10.2f\n", i + 1, annee, moisHT[i], moisTTC[i]);
	}
	printf("----------------------------------\n");
	color(32);
	printf("Annee %d  %10.2f  %10.2f\n\n", annee, anneeHT, anneeTTC);
	color(0);
}



void palmaresProduits(Boutique* b)
{
	int i;
	int rang;
	int meilleur;
	int* quantites;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|      TOP 5 DES PRODUITS VENDUS       |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	quantites = malloc(b->nbProduits * sizeof(int));
	if (quantites == NULL)
	{
		color(31);
		printf("Erreur: plus de memoire\n");
		color(0);
		return;
	}

	for (i = 0; i < b->nbProduits; i++)
	{
		quantites[i] = quantiteVendue(b, b->produits[i].reference);
	}

	for (rang = 1; rang <= 5; rang++)
	{
		meilleur = -1;

		for (i = 0; i < b->nbProduits; i++)
		{
			if (quantites[i] > 0 && (meilleur == -1 || quantites[i] > quantites[meilleur]))
			{
				meilleur = i;
			}
		}

		if (meilleur == -1)
		{
			break;
		}

		printf("%d. %s - %-20s %d vendu(s)\n", rang, b->produits[meilleur].reference, b->produits[meilleur].designation, quantites[meilleur]);
		quantites[meilleur] = -1;
	}

	printf("\n");
	free(quantites);
}



void palmaresClients(Boutique* b)
{
	int i;
	int rang;
	int meilleur;
	float* ca;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|         CLASSEMENT DES CLIENTS       |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	ca = malloc(b->nbClients * sizeof(float));
	if (ca == NULL)
	{
		color(31);
		printf("Erreur: plus de memoire\n");
		color(0);
		return;
	}

	for (i = 0; i < b->nbClients; i++)
	{
		ca[i] = chiffreAffairesClient(&b->clients[i]);
	}

	for (rang = 1; rang <= b->nbClients; rang++)
	{
		meilleur = -1;

		for (i = 0; i < b->nbClients; i++)
		{
			if (ca[i] >= 0 && (meilleur == -1 || ca[i] > ca[meilleur]))
			{
				meilleur = i;
			}
		}

		printf("%d. %-10s %-10s %d commande(s)  %10.2f euros HT\n", rang, b->clients[meilleur].nom, b->clients[meilleur].prenom, b->clients[meilleur].nbCommandes, ca[meilleur]);
		ca[meilleur] = -1;
	}

	printf("\n");
	free(ca);
}



void alertesStock(Boutique* b)
{
	int i;
	int nbAlertes = 0;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|           ALERTES DE STOCK           |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	for (i = 0; i < b->nbProduits; i++)
	{
		if (b->produits[i].actif == 1 && b->produits[i].quantiteStock <= b->produits[i].seuilAlerte)
		{
			if (b->produits[i].quantiteStock == 0)
			{
				color(31);
			}
			else
			{
				color(33);
			}
			printf("%s - %-20s stock: %3d  (seuil: %d)\n", b->produits[i].reference, b->produits[i].designation, b->produits[i].quantiteStock, b->produits[i].seuilAlerte);
			color(0);
			nbAlertes = nbAlertes + 1;
		}
	}

	if (nbAlertes == 0)
	{
		color(32);
		printf("Aucun produit en alerte.\n");
		color(0);
	}
	printf("\n");
}
