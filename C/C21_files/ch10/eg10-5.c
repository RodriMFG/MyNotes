// Demonstrates the use of gets()

#include <stdio.h>
#include <string.h>

// Allocate a array to hold input
char input[81];

char *mygets(char *buf, size_t size);

int main()
{
    puts("Enter some text, then press Enter: ");
    mygets(input, sizeof(input));
    printf("You entered: %s\n", input);

    return 0;
}

char *mygets(char *buf, size_t size)
{
    if (buf != NULL && size > 0) {
        if (fgets(buf, size, stdin)) {
            buf[strcspn(buf, "\n")] = '\0';
            return buf;
        }
        *buf = '\0';  /* clear buffer at end of file */
    }
    return NULL;
}
