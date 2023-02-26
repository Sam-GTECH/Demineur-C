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

void showJeu()
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

}

void showMatrix()
{
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

//Fuck this shit
void revealCasesAround(int r, int c){
    printf("---Called revealCasesAround function with %d-%d---\n", r+1, c+1);
    int aroundCases[8][2] = {
        {r   , c-1}, //top
        {r+1 , c-1}, //top right
        {r+1 , c  }, //right
        {r+1 , c+1}, //bottom right
        {r   , c+1}, //bottom
        {r-1 , c+1}, //bottom left
        {r-1 , c  }, //left
        {r-1 , c-1}, //top left
    };

    int i;
    int bombFound = 0;
    for (i = 0; i < 8; i++)
    {
        printf("\n[Cheking case %d-%d of value %c]\n", aroundCases[i][0]+1, aroundCases[i][1]+1, matrice[aroundCases[i][0]][aroundCases[i][1]]);
        if (aroundCases[i][0]<0 || aroundCases[i][1]>9)
            continue;   
        if (aroundCases[i][0]<0 || aroundCases[i][1]>9)
            continue;
        printf("[yes]\n");
        if (matrice[aroundCases[i][0]][aroundCases[i][1]] == 'X'){
            bombFound++;
            printf("bomb? you want it? -Morshu");
            continue;
        }
        Jeu[aroundCases[i][0]][aroundCases[i][1]] = matrice[aroundCases[i][0]][aroundCases[i][1]];
    }

    if (bombFound==0)
        Jeu[r][c] = matrice[r][c];
    else
        Jeu[r][c] = bombFound + '0';
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
    int choix = 0;
    while (1){
        system("cls");
        showJeu();
        showMatrix();
        while (verif == 0)
        {
            printf("Sélectionnez le numéro de la ligne et de la colonne que vous voulez modifier: ");
            verif = scanf("%d %d", &selectRow, &selectCol);

            if (verif < 2)
            {
                scanf("%*[^\n]");
                printf("Une des valeurs données n'est pas valide!\n\n");
                verif = 0;
            }
        }
            selectRow -= 1;
            selectCol -= 1;

        printf("Case (%d-%d) sélectionné.\n\n", selectRow+1, selectCol+1);
        while (action == -1)
        {
            if (Jeu[selectRow][selectCol] == 'P')
            {
                printf("Que faire?\n1-Enlever un drapeau\n2-Changer de case\nChoix:");
                scanf("%d", &action);
                if (action == 1)
                {
                    Jeu[selectRow][selectCol] = 0 ; // supprimer le drapeau posé si il y a un drapeau
                    Jeu[selectRow][selectCol] = "*";
                    showJeu();
                    showMatrix();
                }
                else
                { 
                    verif = 0; // renvoie au choix d'une case
                }
            }
            else if (Jeu[selectRow][selectCol] != 'P')
            {
                printf("Que faire?\n1-Déminer\n2-Poser un drapeau\n3-Changer de case\nChoix:");
                scanf("%d", &action);
                if (action == 1) // si choix est 1 déminer
                {
                    revealCasesAround(selectRow, selectCol);
                    showJeu();
                    showMatrix();


                }
                else if(action == 2)    //si choix est 2 poser drapeau
                    {  
                        Jeu[selectRow][selectCol] = 'P';
                    }
                else if(action == 3)
                {
                    verif = 0;
                }
            }

            // verif = scanf("%d", &action);
            // printf("\n[%d]\n", verif);
            // if (verif == 0)       // si scanf n'a pas retourné 1, alors l'utilisateur n'a pas retourné un integer
            //     scanf("%*[^\n]"); // Je sais pas comment ça marche mais ça enlève la mauvaise valeur du buffer, empêchant une boucle infinie
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
                revealCasesAround(selectRow, selectCol);
            }
        }
        verif = 0;
        selectRow = 0;
        selectCol = 0;
        action = -1;
    }

    return 0;
}