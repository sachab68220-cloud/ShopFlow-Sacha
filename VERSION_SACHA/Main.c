#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "produits.h"
#include "clients.h"
#include "commandes.h"
#include "statistiques.h"
#include "initialisation.h"
#include "utils.h"

void modeDemonstration(Boutique* b)
{
    afficherProduits(b);
    system("pause");

    afficherClient(b);
    system("pause");

    afficherDetailCommande(b, trouverCommande(b, 1001));
    system("pause");

    afficherDetailCommande(b, trouverCommande(b, 1003));
    system("pause");

    afficherDetailCommande(b, trouverCommande(b, 1004));
    system("pause");

    palmaresProduits(b);
    system("pause");

    palmaresClients(b);
    system("pause");

    alertesStock(b);
    system("pause");

    statistiquesCategories(b);

    color(32);
    printf("Fin de la demonstration !\n\n");
    color(0);
}


int main(void)
{
    Boutique boutique = { "ShopFlow", NULL, 0, 0, NULL, 0, 0, 1001, 1 };

    initialiserDonneesTest(&boutique);

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
        printf("5. Mode demonstration\n");
        printf("6. Quitter\n\n");
        printf("Votre choix: ");
        choix = lireEntier();

        if (choix == 1)
        {
            do
            {
                system("cls");
                color(96);
                printf("+--------------------------------------+\n");
                printf("|               Sous-Menu              |\n");
                printf("+--------------------------------------+\n\n");
                color(0);
                printf("1. Afficher le catalogue\n");
                printf("2. Ajouter un produit\n");
                printf("3. Modifier un produit\n");
                printf("4. Supprimer un produit\n");
                printf("5. Rechercher un produit (reference)\n");
                printf("6. Rechercher un produit (mot-cle)\n");
                printf("7. Retour\n\n");
                printf("Votre choix: ");
                choix2 = lireEntier();

                if (choix2 < 1 || choix2 > 7)
                {
                    color(31);
                    printf("Valeur non prise en compte !\n");
                    color(0);
                    system("pause");
                }
            } while (choix2 < 1 || choix2 > 7);

            if (choix2 == 1)
            {
                afficherProduits(&boutique);
            }
            else if (choix2 == 2)
            {
                ajouterProduit(&boutique);
            }
            else if (choix2 == 3)
            {
                modifierProduit(&boutique);
            }
            else if (choix2 == 4)
            {
                supprimerProduit(&boutique);
            }
            else if (choix2 == 5)
            {
                char ref[12];
                int position;

                system("cls");
                color(96);
                printf("+--------------------------------------+\n");
                printf("|         RECHERCHER UN PRODUIT        |\n");
                printf("+--------------------------------------+\n\n");
                color(0);

                printf("Reference a chercher: ");
                scanf("%11s", ref);

                position = rechercherProduit(&boutique, ref);

                if (position != -1)
                {
                    color(32);
                    printf("Position trouve !\n\n");
                    color(0);
                    printf("Reference: %s\n", boutique.produits[position].reference);
                    printf("Designation: %s\n", boutique.produits[position].designation);
                    printf("Prix: %.2f\n", boutique.produits[position].prixUnitaireHT);
                    printf("Stock: %d\n\n", boutique.produits[position].quantiteStock);
                }
            }
            else if (choix2 == 6)
            {
                rechercherProduitMotCle(&boutique);
            }
            else if (choix2 == 7)
            {
                continue;
            }
        }

        else if (choix == 2)
        {
            do
            {
                system("cls");
                color(96);
                printf("+--------------------------------------+\n");
                printf("|               Sous-Menu              |\n");
                printf("+--------------------------------------+\n\n");
                color(0);
                printf("1. Afficher la liste des clients\n");
                printf("2. Afficher un client\n");
                printf("3. Ajouter un client\n");
                printf("4. Modifier un client\n");
                printf("5. Supprimer un client\n");
                printf("6. Retour\n\n");
                printf("Votre choix: ");
                choix2 = lireEntier();

                if (choix2 < 1 || choix2 > 6)
                {
                    color(31);
                    printf("Valeur non prise en compte !\n");
                    color(0);
                    system("pause");
                }
            } while (choix2 < 1 || choix2 > 6);

            if (choix2 == 1)
            {
                afficherClient(&boutique);
            }
            else if (choix2 == 2)
            {
                afficherUnClient(&boutique);
            }
            else if (choix2 == 3)
            {
                ajouterClient(&boutique);
            }
            else if (choix2 == 4)
            {
                modifierClient(&boutique);
            }
            else if (choix2 == 5)
            {
                supprimerClient(&boutique);
            }
            else if (choix2 == 6)
            {
                continue;
            }
        }

        else if (choix == 3)
        {
            do
            {
                system("cls");
                color(96);
                printf("+--------------------------------------+\n");
                printf("|               Sous-Menu              |\n");
                printf("+--------------------------------------+\n\n");
                color(0);
                printf("1. Creer une commande\n");
                printf("2. Afficher une commande\n");
                printf("3. Ajouter une ligne\n");
                printf("4. Supprimer une ligne\n");
                printf("5. Changer le statut\n");
                printf("6. Annuler une commande\n");
                printf("7. Retour\n\n");
                printf("Votre choix: ");
                choix2 = lireEntier();

                if (choix2 < 1 || choix2 > 7)
                {
                    color(31);
                    printf("Valeur non prise en compte !\n");
                    color(0);
                    system("pause");
                }
            } while (choix2 < 1 || choix2 > 7);

            if (choix2 == 1)
            {
                creerCommande(&boutique);
            }
            else if (choix2 == 2)
            {
                afficherCommande(&boutique);
            }
            else if (choix2 == 3)
            {
                ajouterLigneCommande(&boutique);
            }
            else if (choix2 == 4)
            {
                supprimerLigneCommande(&boutique);
            }
            else if (choix2 == 5)
            {
                changerStatut(&boutique);
            }
            else if (choix2 == 6)
            {
                annulerCommande(&boutique);
            }
            else if (choix2 == 7)
            {
                continue;
            }
        }

        else if (choix == 4)
        {
            do
            {
                system("cls");
                color(96);
                printf("+--------------------------------------+\n");
                printf("|               Sous-Menu              |\n");
                printf("+--------------------------------------+\n\n");
                color(0);
                printf("1. Chiffre d'affaires\n");
                printf("2. Palmares produits\n");
                printf("3. Palmares clients\n");
                printf("4. Alertes de stock\n");
                printf("5. Statistiques par categorie\n");
                printf("6. Retour\n\n");
                printf("Votre choix: ");
                choix2 = lireEntier();

                if (choix2 < 1 || choix2 > 6)
                {
                    color(31);
                    printf("Valeur non prise en compte !\n");
                    color(0);
                    system("pause");
                }
            } while (choix2 < 1 || choix2 > 6);

            if (choix2 == 1)
            {
                chiffreAffaires(&boutique);
            }
            else if (choix2 == 2)
            {
                palmaresProduits(&boutique);
            }
            else if (choix2 == 3)
            {
                palmaresClients(&boutique);
            }
            else if (choix2 == 4)
            {
                alertesStock(&boutique);
            }
            else if (choix2 == 5)
            {
                statistiquesCategories(&boutique);
            }
            else if (choix2 == 6)
            {
                continue;
            }
        }

        else if (choix == 5)
        {
            modeDemonstration(&boutique);
        }

        else if (choix == 6)
        {
            printf("Au revoir !\n");
        }

        else
        {
            system("cls");
            printf("Valeur non prise en compte !\n");
        }

        if (choix != 6)
        {
            system("pause");
        }

    } while (choix != 6);

    detruireBoutique(&boutique);
    return 0;
}
