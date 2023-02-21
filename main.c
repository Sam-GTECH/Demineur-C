#include <time.h>
#include <stdio.h>
char matrice[10][10];
int row, columns;

int main()
{

    // int matrice[10][10] = {
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // };
    
    printf("  | 1  2  3  4  5  6  7  8  9  10\n");
    printf("  --------------------------------\n");

    for (row = 0; row < 10; row++)
    {
        if (row  < 9)
            printf(" %d | ", row+1 );
        else
            printf("%d | ", row+1 );
        
        for (columns = 0; columns < 10; columns++)
    {
        printf("%c 0 ", matrice[row][columns] );
    }
        printf("\n");

    };

    srand(time(0));
    int r = (rand() % (1 - 0 + 1)) + 0;
    printf("The random number is %d!\n\n", r);

    int row, columns;
    for (row=0; row<10; row++)
    {
        for(columns=0; columns<10; columns++)
        {
            printf("-%d-", matrice[row][columns]);
        }
        printf("\n");
    }

    printf("Hello world! The answer to life is %p", *matrice);

    return 0;
}

