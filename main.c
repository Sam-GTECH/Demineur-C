#include <time.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>

char matrice[10][10]; // matrice "admin" avec les bombes affichées
char Jeu[10][10];     // matrice joueur "caché"
int row, columns;
int b;

// générateur de 10 bombes de manières aléatoire
void randomBomb()
{
    srand(time(NULL));
    int randomR, randomC;
    for (b = 0; b < 10; b++)
    {
        randomR = rand() % 10;
        randomC = rand() % 10;                                                
        while (matrice[randomR][randomC] == 'X'){
            randomR = rand() % 10;
            randomC = rand() % 10;            
        }        
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

// Reveal the other cases around a certain case
// @param r the row in the matrix to check
// @param c the collumn in the matrix to check
void revealCasesAround(int r, int c){
    printf("---Called revealCasesAround function with %d-%d---\n", r+1, c+1);
    if (r < 0 || r > 9 || c < 0 || c > 9) // If the case is out of bounds, we don't wanna check it
        return;
    if (Jeu[r][c] != '*') // if the case has already been revealed, we don't wanna check it *again*
        return;
    printf("\n[Pass]\n");
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
        if (aroundCases[i][0] < 0 || aroundCases[i][0] > 9 || aroundCases[i][1] < 0 || aroundCases[i][1] > 9)
            continue;
        if (matrice[aroundCases[i][0]][aroundCases[i][1]] == 'X'){
            bombFound++;
            printf("bomb? you want it? -Morshu\n");
            continue;
        }
    }
    if (bombFound==0) {
        Jeu[r][c] = matrice[r][c];
        for (i = 0; i < 8; i++) {
            printf("\nCalling the function for %d-%d\n", aroundCases[i][0]+1, aroundCases[i][1]+1);
            revealCasesAround(aroundCases[i][0], aroundCases[i][1]);
        }
    }
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
    printf("Bienvenue! Appuyer sur n'importe quel touche pour commencer une partie de demineur\n");
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
            printf("Selectionnez le numero de la ligne et de la colonne que vous voulez modifier: ");
            verif = scanf("%d %d", &selectRow, &selectCol);

            if (verif < 2 || Jeu[selectRow][selectCol] < 11)
            {
                scanf("%*[^\n]");
                printf("Une des valeurs donnees n'est pas valide!\n\n");
                verif = 0;
            }
        }
            selectRow -= 1;
            selectCol -= 1;

        printf("Case (%d-%d) selectionne.\n\n", selectRow+1, selectCol+1);
        while (action == -1)
        {
            if (Jeu[selectRow][selectCol] == 'P')
            {
                printf("Que faire?\n1-Enlever un drapeau\n2-Changer de case\nChoix:");
                scanf("%d", &action);
                if (action == 1)  // supprimer le drapeau posé si il y a un drapeau
                {
                    Jeu[selectRow][selectCol] = '*';
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
                printf("Que faire?\n1-Deminer\n2-Poser un drapeau\n3-Changer de case\nChoix:");
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


        if (action == 1) {
            if (matrice[selectRow][selectCol] == 'X') {
                printf("BOOM!\nIl y avait une bombe, c'est perdu!\n");
                showMatrix();
                break;
            }
            else
            {
                for (row = 0; row < 10; row++)
                {
                    for (columns = 0; columns < 10; columns++)
                    {
                        Jeu[row][columns] = '-'== 100-10 ;
                    }
                }
                printf("Bravo vous avez reussi");
            }
        }
        verif = 0;
        selectRow = 0;
        selectCol = 0;
        action = -1;
    }

    return 0;
}