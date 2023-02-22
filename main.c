#include <time.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>

char matrice[10][10];
int row, columns;

void showMatrix() {
    printf("  | 1  2  3  4  5  6  7  8  9  10\n");
    printf("  --------------------------------\n");

    for (row = 0; row < 10; row++)
    {
        if (row  < 9)
            printf(" %d| ", row+1 );
        else
            printf("%d| ", row+1 );
        
        for (columns = 0; columns < 10; columns++)
        {
            printf("%c  ", matrice[row][columns] );
        }
        printf("\n");
    }
}

void checkNearbyCases(int r, int c){
    int top          = -1;
    int top_right    = -1;
    int top_left     = -1;
    int bottom       = -1;
    int bottom_right = -1;
    int bottom_left  = -1;
    int right        = -1;
    int left         = -1;
    if (c>0) {
        top = matrice[r][c-1];
        if (r<10)
            top_right = matrice[r+1][c-1];
        if (r>0)
            top_left = matrice[r-1][c-1];
    }
    if (c<10) {
        bottom = matrice[r][c+1];
        if (r<10)
            bottom_right = matrice[r+1][c+1];
        if (r>0)
            bottom_left = matrice[r-1][c+1];
    }
    if (r>0)
        right = matrice[r-1][c];
    if (r<10)
        left = matrice[r+1][c];
}

int main() {
    setlocale( LC_ALL, "fr-FR" );
    srand(time(NULL));

    int r = rand();
    int r2 = rand()+r*50;
    for (row=0; row<10; row++)
    {
        for(columns=0; columns<10; columns++)
        {
            int randomR = rand()% 10;
            int randomC = rand()% 10;
            //printf("The random number is %d!\n\n", r);
            matrice[randomR][randomC] = ' X';
        }
        //printf("\n");
    }

    printf("--- DEMINEUR C ---\n");
    printf("\n");
    printf("Bienvenue! Appuyer sur n'importe quel touche pour commencer une partie de démineur\n");
    getchar();

    int selectRow, selectCol;
    int verif = 0;
    int action = -1;
    int flagCase = 0;
    while (1){
        system("cls");
        showMatrix();
        while (verif==0){
            printf("Sélectionnez le numéro de la ligne et de la colonne que vous voulez modifier: ");
            verif = scanf("%d %d", &selectRow, &selectCol);
            printf("\n[%d]\n", verif);

            if (verif<2) {
                printf("Une des valeurs données n'est pas valide!");
                verif = 0;
            }
        }

        if (matrice[selectRow][selectCol]==3)
            flagCase = 1;

        printf("Case (%d-%d) sélectionné.\n\n", selectRow, selectCol);
        while (action==-1){
            if (flagCase) {
                printf("Que faire?\n1-Enlever un drapeau\n2-Changer de case\nChoix:");
            } else {
                printf("Que faire?\n1-Déminer\n2-Poser un drapeau\n3-Changer de case\nChoix:");
            }
            verif = scanf("%d", &action);
            printf("\n[%d]\n", verif);
            if (verif==0) // si scanf n'a pas retourné 1, alors l'utilisateur n'a pas retourné un integer
                scanf("%*[^\n]"); //Je sais pas comment ça marche mais ça enlève la mauvaise valeur du buffer, empêchant une boucle infinie
            if (action<1 || action>3){
                printf("%d n'est pas un choix valide.\n\n", action);
                action = -1;
            }
        }

        selectRow -= 1;
        selectCol -= 1;

        if (action==1){
            if (matrice[selectRow][selectCol] == 1) {
                printf("BOOM!\nIl y avait une bombe, c'est perdu!\n");
                break;
            } else if (matrice[selectRow][selectCol] == 0) {
                matrice[selectRow][selectCol] = 2;
                checkNearbyCases(selectRow, selectCol);
            }
        } else if (action==2) {
            matrice[selectRow][selectCol] = 3;
        } else if (action==3) {
            
        }
        verif = 0;
        selectRow = 0;
        selectCol = 0;
        flagCase = 0;
        action = -1;
    }

    return 0;
}