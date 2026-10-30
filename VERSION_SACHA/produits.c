#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produits.h"
#include "utils.h"

void afficherProduits(Boutique* b)
{
    system("cls");
	int i;

	color(96);
	printf("+--------------------------------------+\n");
	printf("|        CATALOGUE DES PRODUITS        |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

    for (i = 0; i < b->nbProduits; i++)
    {
        printf("----------------------------------------------------------\n");
        printf("Reference: %s\nDesignation: %s\nPrix unitaire: %.2f\n",
            b->produits[i].reference,
            b->produits[i].designation,
            b->produits[i].prixUnitaireHT);

        if (b->produits[i].quantiteStock == 0)
        {
            color(31);
            printf("Stock: %d\n\n", b->produits[i].quantiteStock);
            color(0);
        }

        else if (b->produits[i].quantiteStock < 15)
        {
            color(33);
            printf("Stock: %d\n\n", b->produits[i].quantiteStock);
            color(0);
        }

        else
        {
            color(32);
            printf("Stock: %d\n\n", b->produits[i].quantiteStock);
            color(0);
        }
    }
}


int ajouterProduit(Boutique* b)
{
    system("cls");

    color(96);
    printf("+--------------------------------------+\n");
    printf("|          AJOUTER UN PRODUIT          |\n");
    printf("+--------------------------------------+\n\n");
    color(0);

    Produit p;
    int i;
    printf("Reference: ");
    scanf("%11s", p.reference);

    for (i = 0; i < b->nbProduits; i++)
    {
        if (strcmp(b->produits[i].reference, p.reference) == 0)
        {
            color(31);
            printf("Ce produit existe deja en boutique.\n\n\n");
            color(0);
            return 0;
        }
    }

    printf("Designation: ");
    scanf(" %59[^\n]", p.designation);

    printf("Categorie: ");
    scanf(" %29[^\n]", p.categorie);

    printf("Prix: ");
    scanf("%f", &p.prixUnitaireHT);

    printf("Stock: ");
    scanf("%d", &p.quantiteStock);

    printf("Seuil d'alerte: ");
    scanf("%d", &p.seuilAlerte);

    p.actif = 1;

    if (b->nbProduits == b->capaciteProduits)
    {
        Produit* nouveauTableau;
        nouveauTableau = realloc(b->produits, (b->capaciteProduits + 5) * sizeof(Produit));

        if (nouveauTableau == NULL)
        {
            color(31);
            printf("Erreur: plus de memoire");
            color(0);
            return 0;
        }

        b->produits = nouveauTableau;
        b->capaciteProduits = b->capaciteProduits + 5;
    }

    b->produits[b->nbProduits] = p;
    b->nbProduits = b->nbProduits + 1;

    color(32);
    printf("Produit ajoute !\n\n");
    color(0);

    return 1;
}


int rechercherProduit(Boutique* b, char* ref)
{
    system("cls");

    color(96);
    printf("+--------------------------------------+\n");
    printf("|         RECHERCHER UN PRODUIT        |\n");
    printf("+--------------------------------------+\n\n");
    color(0);

    int i;

    for (i = 0; i < b->nbProduits; i++)
    {
        if (strcmp(b->produits[i].reference, ref) == 0)
        {
            return i;         
        }
    }

    printf("Ce produit n'existe pas encore\n");
    return -1;
}

