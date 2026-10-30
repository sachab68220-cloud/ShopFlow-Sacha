#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void color(int code)
{
    printf("\033[%dm", code);
}
