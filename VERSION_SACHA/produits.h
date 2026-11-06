#pragma once

#include "types.h"

void afficherProduits(Boutique* b);
int ajouterProduit(Boutique* b);
int rechercherProduit(Boutique* b, char* ref);
int indexProduit(Boutique* b, char* ref);
void rechercherProduitMotCle(Boutique* b);
void rechercheMultiCriteres(Boutique* b);
void reapprovisionner(Boutique* b);
void modifierProduit(Boutique* b);
void supprimerProduit(Boutique* b);
