#pragma once

/* types.h - structures partagees par tous les modules de ShopFlow */

typedef struct {
    int jour;   /* 1 a 31, selon le mois */
    int mois;   /* 1 a 12 */
    int annee;  /* 2000 a 2100 */
} Date;

typedef struct {
    char rue[80];        /* non vide */
    char ville[50];      /* non vide */
    char codePostal[6];  /* 5 chiffres */
    char pays[50];       /* non vide */
} Adresse;

typedef struct {
    char  reference[12];    /* unique, non vide */
    char  designation[60];  /* non vide */
    char  categorie[30];    /* non vide */
    float prixUnitaireHT;   /* > 0 */
    int   quantiteStock;    /* >= 0 */
    int   seuilAlerte;      /* >= 0 */
    int   actif;            /* 0 ou 1 */
} Produit;

typedef struct {
    char  refProduit[12];   /* doit exister au catalogue */
    char  designation[60];  /* figee au moment de la vente */
    int   quantite;         /* 1 a stock disponible */
    float prixUnitaireHT;   /* fige au moment de la vente */
    float sousTotalHT;      /* calcule */
} LigneCommande;

typedef struct {
    int            numero;            /* unique, > 0 */
    int            idClient;          /* doit exister */
    Date           dateCommande;
    char           statut[20];        /* EN_COURS, EXPEDIEE, LIVREE ou ANNULEE */
    int            nbLignes;
    int            capaciteLignes;
    LigneCommande *lignes;            /* tableau dynamique */
    float          montantBrutHT;
    float          tauxRemise;        /* 0, 5, 10 ou 15 */
    float          montantRemise;
    float          montantHT;
    float          montantTVA;
    float          montantTTC;
    float          fraisPort;         /* 0 ou 5.90 */
    Adresse        adresseLivraison;  /* recopiee a la creation */
} Commande;

typedef struct {
    int       idClient;        /* unique, > 0 */
    char      nom[50];
    char      prenom[50];
    char      email[80];       /* contient @, unique */
    char      telephone[15];   /* 10 chiffres */
    Adresse   adresse;
    int       pointsFidelite;
    int       nbCommandes;
    int       capaciteCommandes;
    Commande *commandes;       /* tableau dynamique */
    int       actif;           /* 0 ou 1 */
} Client;

typedef struct {
    char     nom[50];
    Produit *produits;         /* tableau dynamique */
    int      nbProduits;
    int      capaciteProduits;
    Client  *clients;          /* tableau dynamique */
    int      nbClients;
    int      capaciteClients;
    int      numeroSuivant;    /* prochain numero de commande */
    int      idClientSuivant;  /* prochain identifiant client */
} Boutique;
