#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "initialisation.h"

void initialiserDonneesTest(Boutique* b)
{
	int i;

    Produit modele[8] =
    {
        { "PRD-0001", "Clavier mecanique",  "Informatique",  79.90,  42, 10, 1 },
        { "PRD-0002", "Souris sans fil",    "Informatique",  24.50,   8, 15, 1 },
        { "PRD-0003", "Ecran 27 pouces",    "Informatique", 199.00,   6,  5, 1 },
        { "PRD-0004", "Casque audio",       "Audio",         89.90,  25, 10, 1 },
        { "PRD-0005", "Enceinte Bluetooth", "Audio",         59.90,  12,  8, 1 },
        { "PRD-0006", "Cable HDMI 2 m",     "Accessoires",    9.90, 120, 30, 1 },
        { "PRD-0007", "Hub USB-C 7 ports",  "Accessoires",   49.90,   4, 10, 1 },
        { "PRD-0008", "Disque SSD 1 To",    "Stockage",     109.00,  18,  6, 1 }
    };

    b->capaciteProduits = 10;
    b->produits = malloc(b->capaciteProduits * sizeof(Produit));

    if (b->produits == NULL)
    {
        printf("Erreur: manque de memoire\n");
        return 1;
    }
    b->nbProduits = 0;
    for (i = 0; i < 8; i++)
    {
        b->produits[b->nbProduits] = modele[i];
        b->nbProduits = b->nbProduits + 1;
    }
}
