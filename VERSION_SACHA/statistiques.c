#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statistiques.h"
#include "clients.h"
#include "utils.h"

/* Nombre d'exemplaires vendus d'un produit (commandes non annulees) */
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



/* Montant HT (avant remise) des ventes d'un produit (commandes non annulees) */
float montantVendu(Boutique* b, char* ref)
{
	int i;
	int j;
	int k;
	float total = 0;

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
					total = total + b->clients[i].commandes[j].lignes[k].sousTotalHT;
				}
			}
		}
	}

	return total;
}



/* CA total de la boutique, puis CA mois par mois pour une annee choisie */
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

	/* 1. Total de toutes les commandes non annulees */
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

	printf("Commandes prises en compte: %d (les annulees sont exclues)\n", nbCommandes);
	color(32);
	printf("CA total HT:  %10.2f euros\n", totalHT);
	printf("CA total TTC: %10.2f euros\n\n", totalTTC);
	color(0);

	/* 2. Par periode : une case par mois (janvier = case 0) */
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



/* Top 5 des produits les plus vendus (en quantite).
   Methode : on calcule la quantite de chaque produit dans un tableau,
   puis 5 fois de suite on cherche le plus grand, on l'affiche et on le met a -1
   pour ne pas le reprendre au tour suivant. */
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

		if (meilleur == -1)   /* plus aucun produit vendu */
		{
			break;
		}

		printf("%d. %s - %-20s %d vendu(s)\n", rang, b->produits[meilleur].reference, b->produits[meilleur].designation, quantites[meilleur]);
		quantites[meilleur] = -1;
	}

	printf("\n");
	free(quantites);
}



/* Classement de tous les clients par chiffre d'affaires HT (meme methode) */
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



/* Produits dont le stock est inferieur ou egal au seuil d'alerte */
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



/* FONCTIONNALITE CREATIVE n1 : statistiques par categorie.
   Pour chaque categorie : nombre de produits, articles vendus, ventes HT et valeur du stock.
   Pour ne traiter chaque categorie qu'une fois, on saute un produit si un produit
   AVANT lui a deja la meme categorie. */
void statistiquesCategories(Boutique* b)
{
	int i;
	int j;
	int dejaVue;
	int nbProduits;
	int vendus;
	float ventes;
	float valeurStock;

	system("cls");
	color(96);
	printf("+--------------------------------------+\n");
	printf("|      STATISTIQUES PAR CATEGORIE      |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

	printf("%-14s %8s %8s %12s %12s\n", "Categorie", "Produits", "Vendus", "Ventes HT", "Val. stock");
	printf("----------------------------------------------------------\n");

	for (i = 0; i < b->nbProduits; i++)
	{
		/* Categorie deja affichee ? */
		dejaVue = 0;
		for (j = 0; j < i; j++)
		{
			if (strcmp(b->produits[j].categorie, b->produits[i].categorie) == 0)
			{
				dejaVue = 1;
			}
		}

		if (dejaVue == 1)
		{
			continue;
		}

		/* On additionne tous les produits de cette categorie */
		nbProduits = 0;
		vendus = 0;
		ventes = 0;
		valeurStock = 0;

		for (j = i; j < b->nbProduits; j++)
		{
			if (strcmp(b->produits[j].categorie, b->produits[i].categorie) == 0)
			{
				nbProduits = nbProduits + 1;
				vendus = vendus + quantiteVendue(b, b->produits[j].reference);
				ventes = ventes + montantVendu(b, b->produits[j].reference);
				valeurStock = valeurStock + b->produits[j].prixUnitaireHT * b->produits[j].quantiteStock;
			}
		}

		printf("%-14s %8d %8d %12.2f %12.2f\n", b->produits[i].categorie, nbProduits, vendus, ventes, valeurStock);
	}
	printf("----------------------------------------------------------\n\n");
}
