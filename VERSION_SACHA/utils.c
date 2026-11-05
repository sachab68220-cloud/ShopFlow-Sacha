#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void color(int code)
{
    printf("\033[%dm", code);
}

int VerifChiffres(char* texte, int longueur)
{
    if (strlen(texte) != longueur)
    {
        return 0;
    }

    int i;
    for (i = 0; i < longueur; i++)
    {
        if (texte[i] < '0' || texte[i] > '9')
        {
            return 0;
        }
    }

    return 1;
}

int lireEntier(void)
{
    int n;

    while (scanf("%d", &n) != 1)
    {
        while (getchar() != '\n');

        color(31);
        printf("Entrez un nombre : ");
        color(0);
    }

    return n;
}

float lireReel(void)
{
    float n;

    while (scanf("%f", &n) != 1)
    {
        while (getchar() != '\n');

        color(31);
        printf("Entrez un nombre : ");
        color(0);
    }

    return n;
}

int dateValide(int jour, int mois, int annee)
{
    int joursParMois[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (annee < 2000 || annee > 2100)
    {
        return 0;
    }

    if (mois < 1 || mois > 12)
    {
        return 0;
    }

    if (annee % 4 == 0 && (annee % 100 != 0 || annee % 400 == 0))
    {
        joursParMois[1] = 29;
    }

    if (jour < 1 || jour > joursParMois[mois - 1])
    {
        return 0;
    }

    return 1;
}
