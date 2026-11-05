#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produits.h"
#include "commandes.h"
#include "utils.h"

void afficherProduits(Boutique* b)
{
    system("cls");
	int i;
    float valeurStock = 0;

	color(96);
	printf("+--------------------------------------+\n");
	printf("|        CATALOGUE DES PRODUITS        |\n");
	printf("+--------------------------------------+\n\n");
	color(0);

    for (i = 0; i < b->nbProduits; i++)
    {
        if (b->produits[i].actif == 0)
        {
            continue;
        }

        /* Valorisation du stock = prix x quantite, pour chaque produit */
        valeurStock = valeurStock + b->produits[i].prixUnitaireHT * b->produits[i].quantiteStock;

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

    printf("----------------------------------------------------------\n");
    color(96);
    printf("Valeur totale du stock: %.2f euros HT\n\n", valeurStock);
    color(0);
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

    do
    {
        printf("Prix: ");
        p.prixUnitaireHT = lireReel();
    } while (p.prixUnitaireHT <= 0);

    do
    {
        printf("Stock: ");
        p.quantiteStock = lireEntier();
    } while (p.quantiteStock < 0);

    do
    {
        printf("Seuil d'alerte: ");
        p.seuilAlerte = lireEntier();
    } while (p.seuilAlerte < 0);

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

    color(31);
    printf("Ce produit n'existe pas encore\n");
    color(0);
    return -1;
}


/* Comme rechercherProduit, mais n'affiche rien : sert dans les commandes */
int indexProduit(Boutique* b, char* ref)
{
    int i;

    for (i = 0; i < b->nbProduits; i++)
    {
        if (strcmp(b->produits[i].reference, ref) == 0)
        {
            return i;
        }
    }

    return -1;
}


/* Affiche tous les produits dont la designation contient le mot tape.
   strstr(texte, mot) renvoie NULL si le mot n'est pas dans le texte. */
void rechercherProduitMotCle(Boutique* b)
{
    char mot[60];
    int i;
    int trouve = 0;

    system("cls");

    color(96);
    printf("+--------------------------------------+\n");
    printf("|      RECHERCHER PAR MOT-CLE          |\n");
    printf("+--------------------------------------+\n\n");
    color(0);

    printf("Mot-cle (ex: Casque, USB, Ecran): ");
    scanf(" %59[^\n]", mot);
    printf("\n");

    for (i = 0; i < b->nbProduits; i++)
    {
        if (b->produits[i].actif == 1 && strstr(b->produits[i].designation, mot) != NULL)
        {
            printf("%s - %s - %.2f euros - stock: %d\n", b->produits[i].reference, b->produits[i].designation, b->produits[i].prixUnitaireHT, b->produits[i].quantiteStock);
            trouve = 1;
        }
    }

    if (trouve == 0)
    {
        color(31);
        printf("Aucun produit ne contient \"%s\".\n", mot);
        color(0);
    }
    printf("\n");
}


void modifierProduit(Boutique* b)
{
    char ref[12];
    int position;

    system("cls");

    color(96);
    printf("+--------------------------------------+\n");
    printf("|         RECHERCHER UN PRODUIT        |\n");
    printf("+--------------------------------------+\n\n");
    color(0);

    printf("Entrez la reference du produit que vous voulez modifie: ");
    scanf("%11s", ref);
    position = rechercherProduit(b, ref);

    system("cls");

    color(96);
    printf("+--------------------------------------+\n");
    printf("|           MODIFIER UN PRODUIT        |\n");
    printf("+--------------------------------------+\n\n");
    color(0);

    if (position == -1)
    {
        return;
    }

    printf("Produit: %s - %s\n", b->produits[position].reference, b->produits[position].designation);
    printf("Prix actuel: %.2f\n", b->produits[position].prixUnitaireHT);
    printf("Stock actuel: %d\n", b->produits[position].quantiteStock);
    printf("Seuil actuel: %d\n\n", b->produits[position].seuilAlerte);

    do
    {
        printf("Nouveau prix: ");
        b->produits[position].prixUnitaireHT = lireReel();
    } while (b->produits[position].prixUnitaireHT <= 0);

    do
    {
        printf("Nouveau stock: ");
        b->produits[position].quantiteStock = lireEntier();
    } while (b->produits[position].quantiteStock < 0);

    do
    {
        printf("Nouveau seuil d'alerte: ");
        b->produits[position].seuilAlerte = lireEntier();
    } while (b->produits[position].seuilAlerte < 0);

    color(32);
    printf("Produit modifie avec succee !\n");
    color(0);
}


void supprimerProduit(Boutique* b)
{
    char ref[12];
    int position;

    system("cls");

    color(96);
    printf("+--------------------------------------+\n");
    printf("|         RECHERCHER UN PRODUIT        |\n");
    printf("+--------------------------------------+\n\n");
    color(0);

    printf("Entrez la reference du produit que vous voulez modifie: ");
    scanf("%11s", ref);
    position = rechercherProduit(b, ref);

    system("cls");

    color(96);
    printf("+--------------------------------------+\n");
    printf("|          SUPPRIMER UN PRODUIT        |\n");
    printf("+--------------------------------------+\n\n");
    color(0);

    if (position == -1)
    {
        return;
    }

    if (b->produits[position].actif == 0)
    {
        color(31);
        printf("Ce produit est deja supprime !\n");
        color(0);
        return;
    }

    /* Refuse si le produit est dans une commande non annulee */
    if (produitDansCommande(b, b->produits[position].reference) == 1)
    {
        color(31);
        printf("Impossible : ce produit est dans une commande non annulee !\n");
        color(0);
        return;
    }

    b->produits[position].actif = 0;

    color(32);
    printf("Produit supprime avec succes !\n");
    color(0);
}

