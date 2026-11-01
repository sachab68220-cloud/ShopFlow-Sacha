#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commandes.h"

void calculCommande(Commande* c)
{
	c->montantBrutHT = 0;

	int i;
	for (i = 0; i < c->nbLignes; i++)
	{
		c->lignes[i].sousTotalHT = c->lignes[i].quantite * c->lignes[i].prixUnitaireHT;
		c->montantBrutHT = c->montantBrutHT + c->lignes[i].sousTotalHT;
	}

	if (c->montantBrutHT < 100)
	{
		c->tauxRemise = 0;
	}

	else if (c->montantBrutHT < 300)
	{
		c->tauxRemise = 5;
	}

	else if (c->montantBrutHT < 600)
	{
		c->tauxRemise = 10;
	}
	else
	{
		c->tauxRemise = 15;
	}

	c->montantRemise = c->montantBrutHT * c->tauxRemise / 100;
	c->montantHT = c->montantBrutHT - c->montantRemise;
	c->montantTVA = c->montantHT * 20 / 100;

	if (c->montantBrutHT >= 60)
	{
		c->fraisPort = 0;
	}

	else
	{
		c->fraisPort = 5.90;
	}

	c->montantTTC = c->montantHT + c->montantTVA + c->fraisPort;
}
