#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "produits.h"
#include "clients.h"
#include "commandes.h"
#include "statistiques.h"
#include "initialisation.h"
#include "utils.h"

int main(void)
{
    Boutique boutique = { "ShopFlow", NULL, 0, 0, NULL, 0, 0, 1001, 1 };

    color(96);
    printf("                                      ==============================================\n");
    printf("                                      ||                                          ||\n");
    printf("                                      ||             S H O P F L O W              ||\n");
    printf("                                      ||                                          ||\n");
    printf("                                      ==============================================\n\n\n\n");
    color(0);

    int choix;
    int choix2;

    do
    {
        system("cls");

        color(96);
        printf("+--------------------------------------+\n");
        printf("|                 Menu                 |\n");
        printf("+--------------------------------------+\n\n");
        color(0);

        printf("1. Gestion des produits\n");
        printf("2. Gestion des clients\n");
        printf("3. Gestion des commandes\n");
        printf("4. Statistiques de la boutique\n");
        printf("5. Quitter\n\n");
        printf("Votre choix: ");
        scanf("%d", &choix);

        if (choix == 1)
        {
            system("cls");
            printf("1. Afficher le catalogue\n");
            printf("2. Ajouter un produit\n");
            printf("3. Modifier un produit\n");
            printf("4. Supprimer un produit\n");
            printf("5. Rechercher un produit\n");
            printf("6. Retour\n\n");
            printf("Votre choix: ");
            scanf("%d", &choix2);

            if (choix2 == 1)
            {
                afficherProduits(&boutique);
            }
            else if (choix2 == 2)
            {
                ajouterProduit(&boutique);
            }
        }

        else if (choix == 2)
        {
            system("cls");
            printf("1. Afficher la liste des clients\n");
            printf("2. Afficher un client\n");
            printf("3. Ajouter un client\n");
            printf("4. Modifier un client\n");
            printf("5. Supprimer un client\n");
            printf("6. Retour\n\n");
            printf("Votre choix: ");
            scanf("%d", &choix2);
        }

        else if (choix == 3)
        {
            system("cls");
            printf("1. Creer une commande\n");
            printf("2. Afficher une commande\n");
            printf("3. Ajouter une ligne\n");
            printf("4. Supprimer une ligne\n");
            printf("5. Changer le statut\n");
            printf("6. Annuler une commande\n");
            printf("7. Retour\n\n");
            printf("Votre choix: ");
            scanf("%d", &choix2);
        }

        else if (choix == 4)
        {
            system("cls");
            printf("1. Chiffre d'affaires\n");
            printf("2. Palmares produits\n");
            printf("3. Palmares clients\n");
            printf("4. Alertes de stock\n");
            printf("5. Retour\n\n");
            printf("Votre choix: ");
            scanf("%d", &choix2);
        }

        else if (choix == 5)
        {
            printf("Au revoir !\n");
        }

        else
        {
            system("cls");
            printf("Valeur non prise en compte !\n");
        }

        if (choix != 5)
        {
            system("pause");
        }

    } while (choix != 5);

    free(boutique.produits); 
    return 0;
}
