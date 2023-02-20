#include <time.h>
#include <stdio.h>

int main() {

    int matrice[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
    };

    srand(time(0));
    int r = (rand() % (1 - 0 + 1)) + 0;
    printf("The random number is %d!\n\n", r);

    int row, columns;
    for (row=0; row<5; row++)
    {
        for(columns=0; columns<5; columns++)
        {
            printf("-%d-", matrice[row][columns]);
        }
        printf("\n");
    }

    printf("Hello world! The answer to life is %p", *matrice);

    return 0;
}

