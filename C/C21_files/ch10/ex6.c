// 6

#include <stdio.h>
#include <stdlib.h>

char *s1, *s2; // Strings
int count(char str[]); // Func to count chars in strings

int main()
{
    s1 = malloc(81 * sizeof(char));
    s2 = malloc(81 * sizeof(char));

    // Get the strings from user

    puts("Enter string 1: ");
    scanf("%81[^\n]", s1);

    puts("Enter string 2: ");
    scanf(" %81[^\n]", s2);

    if (count(s1) > count(s2))
    {
        printf("String 1 is larger, located in the address %p", s1);
    }

    else if (count(s1) < count(s2))
    {
        printf("String 2 is larger, located in the address %p", s2);
    }

    else
        printf("Strings are the same size");

    printf("\n");

    return 0;
}

int count(char str[])
{
    int i = 0;
    int chars = 0;

    while(str[i] != '\0' && i < 81)
    {
        chars++;
        i++;
    }

    return chars;
}
