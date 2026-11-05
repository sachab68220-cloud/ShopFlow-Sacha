#pragma once

#include "types.h"

void calculCommande(Commande* c);

int positionClient(Boutique* b, int id);
Commande* trouverCommande(Boutique* b, int numero);
int rattacherCommande(Client* client, Commande c);
void remettreStock(Boutique* b, Commande* c);
int saisirLigne(Boutique* b, Commande* c);

int produitDansCommande(Boutique* b, char* ref);
int clientACommandeActive(Client* client);

void creerCommande(Boutique* b);
void afficherDetailCommande(Boutique* b, Commande* c);
void afficherCommande(Boutique* b);
void ajouterLigneCommande(Boutique* b);
void supprimerLigneCommande(Boutique* b);
void changerStatut(Boutique* b);
void annulerCommande(Boutique* b);
