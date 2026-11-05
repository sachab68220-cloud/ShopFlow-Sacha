#pragma once

#include "types.h"

float chiffreAffairesClient(Client* client);
void afficherClient(Boutique* b);
void afficherUnClient(Boutique* b);
int ajouterClient(Boutique* b);
int rechercheClient(Boutique* b, int id);
void modifierClient(Boutique* b);
void supprimerClient(Boutique* b);
