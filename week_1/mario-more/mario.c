#include <stdio.h>
#include <cs50.h>

void pyramidrow(int n);

int main(void)
{
    int size = get_int("How big would you like your pyramid to be?\n");
    for (int i = 1; i < size+1; i++)
    {
        for (int k = 1; k <= size-i; k++)
        {
            printf(" ");
        }
        pyramidrow(i);
        printf("  ");
        pyramidrow(i);
        printf("\n");
    }
}

void pyramidrow(int n)
{
    for (int j = 1; j < n+1; j++)
    {
        printf("#");
    }
}
