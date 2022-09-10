// Demonstrates extended ASCII use

#include <stdio.h>
#include <stdlib.h>

unsigned char x; // Must be unsigned for extended ASCII

int main()
{
    for (int count = 180; count < 204; count++)
    {
        x = count;

        printf("ASCII code %d is character %c\n", x, x);
    }

    return 0;
}
