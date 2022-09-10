// 5

#include <stdio.h>
#include <stdlib.h>

char *str, *copy;

int main()
{
    str = malloc(81 * sizeof(char));

    scanf("%81[^\n]", str);

    copy = str;

    puts(copy);

    return 0;
}

