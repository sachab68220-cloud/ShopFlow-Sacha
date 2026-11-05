#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "initialisation.h"
#include "commandes.h"

/* Cree une commande vide (sans lignes) */
Commande creerCommandeTest(int numero, int idClient, int jour, int mois, int annee, char* statut)
{
    Commande c;

    c.numero = numero;
    c.idClient = idClient;
    c.dateCommande.jour = jour;
    c.dateCommande.mois = mois;
    c.dateCommande.annee = annee;
    strcpy(c.statut, statut);

    c.nbLignes = 0;
    c.capaciteLignes = 5;
    c.lignes = malloc(c.capaciteLignes * sizeof(LigneCommande));

    if (c.lignes == NULL)
    {
        printf("Erreur: manque de memoire\n");
        exit(1);
    }

    return c;
}

/* Ajoute une ligne (un produit + une quantite) dans la commande */
void ajouterLigneTest(Commande* c, Produit p, int quantite)
{
    LigneCommande l;

    strcpy(l.refProduit, p.reference);
    strcpy(l.designation, p.designation);
    l.quantite = quantite;
    l.prixUnitaireHT = p.prixUnitaireHT;

    c->lignes[c->nbLignes] = l;
    c->nbLignes = c->nbLignes + 1;
}

/* Copie l'adresse du client dans la commande, puis la range dans sa fiche */
void rattacherCommandeTest(Client* client, Commande c)
{
    c.adresseLivraison = client->adresse;

    if (rattacherCommande(client, c) == 0)
    {
        exit(1);
    }
}


void initialiserDonneesTest(Boutique* b)
{
	int i;

    /* Le f apres le nombre veut dire "float" (sinon C le prend pour un double) */
    Produit modele[8] =
    {
        { "PRD-0001", "Clavier mecanique",  "Informatique",  79.90f,  42, 10, 1 },
        { "PRD-0002", "Souris sans fil",    "Informatique",  24.50f,   8, 15, 1 },
        { "PRD-0003", "Ecran 27 pouces",    "Informatique", 199.00f,   6,  5, 1 },
        { "PRD-0004", "Casque audio",       "Audio",         89.90f,  25, 10, 1 },
        { "PRD-0005", "Enceinte Bluetooth", "Audio",         59.90f,  12,  8, 1 },
        { "PRD-0006", "Cable HDMI 2 m",     "Accessoires",    9.90f, 120, 30, 1 },
        { "PRD-0007", "Hub USB-C 7 ports",  "Accessoires",   49.90f,   4, 10, 1 },
        { "PRD-0008", "Disque SSD 1 To",    "Stockage",     109.00f,  18,  6, 1 }
    };

    b->capaciteProduits = 10;
    b->produits = malloc(b->capaciteProduits * sizeof(Produit));

    if (b->produits == NULL)
    {
        printf("Erreur: manque de memoire\n");
        exit(1);
    }
    b->nbProduits = 0;
    for (i = 0; i < 8; i++)
    {
        b->produits[b->nbProduits] = modele[i];
        b->nbProduits = b->nbProduits + 1;
    }


    Client modeleClients[5] =
    {
        { 1, "Bernard", "Julie", "j.bernard@mail.fr", "0612345678",{ "12 rue de la Paix", "Paris", "75002", "France" }, 0, 0, 0, NULL, 1 },
        { 2, "Martin", "Lucas", "l.martin@mail.fr", "0623456789",{ "5 avenue Foch", "Lyon", "69006", "France" }, 0, 0, 0, NULL, 1 },
        { 3, "Durand", "Emma", "e.durand@mail.fr", "0634567890",{ "8 boulevard Victor", "Marseille", "13008", "France" }, 0, 0, 0, NULL, 1 },
        { 4, "Petit", "Hugo", "h.petit@mail.fr", "0645678901",{ "21 rue Nationale", "Lille", "59000", "France" }, 0, 0, 0, NULL, 1 },
        { 5, "Moreau", "Chloe", "c.moreau@mail.fr", "0656789012",{ "3 place du Capitole", "Toulouse", "31000", "France" }, 0, 0, 0, NULL, 1 },
    };

    b->capaciteClients = 8;
    b->clients = malloc(b->capaciteClients * sizeof(Client));

    if (b->clients == NULL)
    {
        printf("Erreur: manque de memoire\n");
        exit(1);
    }
    b->nbClients = 0;
    for (i = 0; i < 5; i++)
    {
        b->clients[b->nbClients] = modeleClients[i];
        b->nbClients = b->nbClients + 1;
    }

    b->idClientSuivant = 6;


    /* ---------- Commandes de test ----------
       Ce sont des ventes "passees" : les stocks ci-dessus sont deja ceux d'apres ces ventes,
       donc on ne les diminue pas ici. */
    Commande c;

    /* Commande 1001 : Julie Bernard, 3 lignes, LIVREE */
    c = creerCommandeTest(1001, 1, 12, 3, 2026, "LIVREE");
    ajouterLigneTest(&c, b->produits[0], 1);   /* Clavier mecanique x1 */
    ajouterLigneTest(&c, b->produits[1], 2);   /* Souris sans fil x2   */
    ajouterLigneTest(&c, b->produits[5], 3);   /* Cable HDMI x3        */
    rattacherCommandeTest(&b->clients[0], c);

    /* Commande 1002 : Lucas Martin, 3 lignes, LIVREE */
    c = creerCommandeTest(1002, 2, 2, 4, 2026, "LIVREE");
    ajouterLigneTest(&c, b->produits[2], 2);   /* Ecran 27 pouces x2   */
    ajouterLigneTest(&c, b->produits[7], 1);   /* Disque SSD x1        */
    ajouterLigneTest(&c, b->produits[3], 1);   /* Casque audio x1      */
    rattacherCommandeTest(&b->clients[1], c);

    /* Commande 1003 : Emma Durand, 2 lignes, EXPEDIEE (plus de 600 euros HT -> 15 %) */
    c = creerCommandeTest(1003, 3, 15, 5, 2026, "EXPEDIEE");
    ajouterLigneTest(&c, b->produits[2], 3);   /* Ecran 27 pouces x3   */
    ajouterLigneTest(&c, b->produits[7], 1);   /* Disque SSD x1        */
    rattacherCommandeTest(&b->clients[2], c);

    /* Commande 1004 : Julie Bernard, 1 ligne, EN_COURS (moins de 60 euros -> frais de port) */
    c = creerCommandeTest(1004, 1, 1, 6, 2026, "EN_COURS");
    ajouterLigneTest(&c, b->produits[5], 2);   /* Cable HDMI x2        */
    rattacherCommandeTest(&b->clients[0], c);

    /* Commande 1005 : Hugo Petit, 2 lignes, ANNULEE */
    c = creerCommandeTest(1005, 4, 10, 6, 2026, "ANNULEE");
    ajouterLigneTest(&c, b->produits[4], 1);   /* Enceinte Bluetooth x1 */
    ajouterLigneTest(&c, b->produits[6], 1);   /* Hub USB-C x1          */
    rattacherCommandeTest(&b->clients[3], c);

    /* Commande 1006 : Chloe Moreau, 2 lignes, EN_COURS */
    c = creerCommandeTest(1006, 5, 20, 6, 2026, "EN_COURS");
    ajouterLigneTest(&c, b->produits[3], 1);   /* Casque audio x1      */
    ajouterLigneTest(&c, b->produits[1], 1);   /* Souris sans fil x1   */
    rattacherCommandeTest(&b->clients[4], c);

    b->numeroSuivant = 1007;
}


/* Libere toute la memoire, du plus petit au plus grand :
   lignes de chaque commande -> commandes de chaque client -> clients -> produits */
void detruireBoutique(Boutique* b)
{
    int i;
    int j;

    for (i = 0; i < b->nbClients; i++)
    {
        for (j = 0; j < b->clients[i].nbCommandes; j++)
        {
            free(b->clients[i].commandes[j].lignes);
        }
        free(b->clients[i].commandes);
    }

    free(b->clients);
    free(b->produits);

    b->clients = NULL;
    b->produits = NULL;
    b->nbClients = 0;
    b->nbProduits = 0;
}
