// 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *s1, *s2, *str; //input s1 and s2, concatenate to str

int main()
{
    s1 = malloc(40 * sizeof(char));
    s2 = malloc(40 * sizeof(char));
    str = malloc(81 * sizeof(char));

    // Get the strings from user

    puts("Enter string 1: ");
    scanf("%81[^\n]", s1);

    puts("Enter string 2: ");
    scanf(" %81[^\n]", s2);

    strcat(str, s1);
    strcat(str, s2);

    puts(str);

    return 0;
}
