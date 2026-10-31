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
