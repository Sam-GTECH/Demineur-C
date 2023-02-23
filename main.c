#include <time.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>

char matrice[10][10]; // matrice "admin" avec les bombes affichées
char Jeu[10][10];     // matrice joueur "caché"
int row, columns;
int b;

void randomBomb()
{ // générateur de 10 bombes de manières aléatoire
    srand(time(NULL));
    for (b = 0; b < 10; b++)
    {
        int randomR = rand() % 10;
        int randomC = rand() % 10;
        matrice[randomR][randomC] = ' X';
    }
}

void showMatrix()
{ // affichage de la grille de jeu avec pour l'instant GRILLE ADMIN A SUPR !
    printf("  | 1  2  3  4  5  6  7  8  9  10\n");
    printf("  --------------------------------\n");

    for (row = 0; row < 10; row++) // boucle pour les lignes
    {
        if (row < 9) // si ligne < 9 alors écrire " | " et sauté une ligne
            printf(" %d| ", row + 1);
        else
            printf("%d| ", row + 1); // sinon écrire "| " et sauté une ligne

        for (columns = 0; columns < 10; columns++) // boucle pour les colonnes
        {
            printf("%c  ", Jeu[row][columns]); // affiche la grille de jeu
        }
        printf("\n");
    }
    printf("  --------------------------------\n");
    printf("\n");
    printf("  | 1  2  3  4  5  6  7  8  9  10\n");
    printf("  --------------------------------\n");

    for (row = 0; row < 10; row++)
    {
        if (row < 9)
            printf(" %d| ", row + 1);
        else
            printf("%d| ", row + 1);

        for (columns = 0; columns < 10; columns++)
        {
            printf("%c  ", matrice[row][columns]); // affiche la grille admin
        }
        printf("\n");
    }
}

void checkNearbyCases(int r, int c){
    printf("---Called checkNerbyCases function with %d-%d---\n", r, c);
    int aroundCases[9][2] = {
        {r   , c-1}, //top
        {r+1 , c-1}, //top right
        {r-1 , c-1}, //top left
        {r   , c+1}, //bottom
        {r+1 , c-1}, //bottom right
        {r-1 , c-1}, //bottom left
        {r+1 , c  }, //left
        {r-1 , c  }, //right
    };

    int i, j;
    int bombFound = 0;
    char aroundCase;
    int right, left;
    for (i = 0; i < 9; i++)
    {
        if (aroundCases[i][1]<0 || aroundCases[i][1]>10)
            continue;
        if (aroundCases[i][2]<0 || aroundCases[i][2]>10)
            continue;
        aroundCase = matrice[aroundCases[i][1]][aroundCases[i][2]];
        printf("La case en %d-%d est %c!\n", aroundCases[i][1]+1, aroundCases[i][2]+1, aroundCase);
        if (aroundCase == 'X')
            bombFound++;
        else if (aroundCase == '-'){
            Jeu[aroundCases[i][1]][aroundCases[i][2]] = 'O';
            matrice[aroundCases[i][1]][aroundCases[i][2]] = 'O';
            if (bombFound==0)
                checkNearbyCases(aroundCases[i][1], aroundCases[i][2]);
        }
    }
    if (r>0)
        right = matrice[r-1][c];
    if (r<10)
        left = matrice[r+1][c];
}

int main()
{
    setlocale(LC_ALL, "fr-FR");

    for (row = 0; row < 10; row++)
    {
        for (columns = 0; columns < 10; columns++)
        {
            matrice[row][columns] = '-';
            randomBomb();
        }
    }
    randomBomb();
    for (row = 0; row < 10; row++)
    {
        for (columns = 0; columns < 10; columns++)
        {
            Jeu[row][columns] = '*';
        }
    }

    printf("--- DEMINEUR C ---\n");
    printf("\n");
    printf("Bienvenue! Appuyer sur n'importe quel touche pour commencer une partie de démineur\n");
    getchar();

    int selectRow, selectCol;
    int verif = 0;
    int action = -1;
    int flagCase = 0;
    int choix = 0;
    while (1){
        system("cls");
        showMatrix();
        while (verif == 0)
        {
            printf("Sélectionnez le numéro de la ligne et de la colonne que vous voulez modifier: ");
            verif = scanf("%d %d", &selectRow, &selectCol);
            printf("\n[%d]\n", verif);

            if (verif < 2)
            {
                printf("Une des valeurs données n'est pas valide!");
                verif = 0;
            }
        }
            selectRow -= 1;
            selectCol -= 1;
        if (Jeu[selectRow][selectCol] == 3)
            flagCase = 1;

        printf("Case (%d-%d) sélectionné.\n\n", selectRow+1, selectCol+1);
        while (action == -1)
        {
            if (flagCase == 1)
            {
                printf("Que faire?\n1-Enlever un drapeau\n2-Changer de case\nChoix:");
                scanf("%d", &choix);
            }
            else
            {
                printf("Que faire?\n1-Déminer\n2-Poser un drapeau\n3-Changer de case\nChoix:");
                scanf("%d", &choix);
                if (choix == 1)
                {
                    Jeu[selectRow][selectCol] = matrice[selectRow][selectCol];
                    showMatrix();


                }
                else if(choix == 2)
                {
                    if(flagCase == 1)
                        Jeu[selectRow][selectCol] = printf("%d", flagCase);
                    else
                        Jeu[selectRow][selectCol] = printf("%d *", flagCase); // à changer pour supprimer le drapeau posé
                    showMatrix();

                }
            }
            verif = scanf("%d", &action);
            printf("\n[%d]\n", verif);
            if (verif == 0)       // si scanf n'a pas retourné 1, alors l'utilisateur n'a pas retourné un integer
                scanf("%*[^\n]"); // Je sais pas comment ça marche mais ça enlève la mauvaise valeur du buffer, empêchant une boucle infinie
            if (action < 1 || action > 3)
            {
                printf("%d n'est pas un choix valide.\n\n", action);
                action = -1;
            }
        }


        if (action==1){
            if (matrice[selectRow][selectCol] == 1) {
                printf("BOOM!\nIl y avait une bombe, c'est perdu!\n");
                break;
            } else if (matrice[selectRow][selectCol] == 0) {
                matrice[selectRow][selectCol] = 2;
                checkNearbyCases(selectRow, selectCol);
            }
        }
        else if (action == 2)
        {
            matrice[selectRow][selectCol] = 3;
        }
        else if (action == 3)
        {
        }
        verif = 0;
        selectRow = 0;
        selectCol = 0;
        flagCase = 0;
        action = -1;
    }

    return 0;
}