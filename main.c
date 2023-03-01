#include "time.h"
#include "stdio.h"
#include "locale.h"
#include "wchar.h"
#include "stdlib.h"

char matrice[10][10]; // matrice "admin" avec les bombes affichées
char Jeu[10][10];     // matrice joueur " bombes caché"
int row, columns;  // lignes et colonnes
int b;
int countVictory; // Compteur de case vide pour condition de victoire

// générateur de 10 bombes de manières aléatoire
void randomBomb()
{
    srand(time(NULL));
    int randomR, randomC;
    for (b = 0; b < 10; b++)
    {
        randomR = rand() % 10;
        randomC = rand() % 10;
        while (matrice[randomR][randomC] == 'X') // quand il y a déja une bombe replacer une bombe ailleurs
        {
            randomR = rand() % 10;
            randomC = rand() % 10;
        }
        matrice[randomR][randomC] = ' X';
    }
}

void showJeu() // grille de jeu
{
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

void showMatrix() // grille admin (où l'on voit les bombes)
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
void revealCasesAround(int r, int c)
{
    printf("---Called revealCasesAround function with %d-%d---\n", r + 1, c + 1);
    if (r < 0 || r > 9 || c < 0 || c > 9) // Si la case est en dehors de la grille, on ne vérifie pas
        return;
    if (Jeu[r][c] != '*') // Si la case a déja été révélé, on ne révèle pas une seconde fois
        return;
    printf("\n[Pass]\n");
    int aroundCases[8][2] = {
        {r, c - 1},     // top
        {r + 1, c - 1}, // top right
        {r + 1, c},     // right
        {r + 1, c + 1}, // bottom right
        {r, c + 1},     // bottom
        {r - 1, c + 1}, // bottom left
        {r - 1, c},     // left
        {r - 1, c - 1}, // top left
    }; // Cases autours de l'élément choisi

    int i;
    int bombFound = 0;
    countVictory++; // ajoute 1 au compteur de victoire à chaque révélation
    for (i = 0; i < 8; i++)
    {
        if (aroundCases[i][0] < 0 || aroundCases[i][0] > 9 || aroundCases[i][1] < 0 || aroundCases[i][1] > 9) // Localise la recherche de bombe unisuement dans la grille
            continue;
        if (matrice[aroundCases[i][0]][aroundCases[i][1]] == 'X') // compte le nombre de bombe autour de la case
        {
            bombFound++;
            continue;
        }
    }
    if (bombFound == 0)
    {
        Jeu[r][c] = matrice[r][c];
        for (i = 0; i < 8; i++)
        {
            revealCasesAround(aroundCases[i][0], aroundCases[i][1]); // Révèle les cases autours du déminage
        }
    }
    else
        Jeu[r][c] = bombFound + '0';

}

int main()
{
    for (row = 0; row < 10; row++)
    {
        for (columns = 0; columns < 10; columns++)
        {
            matrice[row][columns] = '-'; // affiche '-' dans la matrice admin
        }
    }
    randomBomb(); // Fonctions pour placer les bombes
    for (row = 0; row < 10; row++)
    {
        for (columns = 0; columns < 10; columns++)
        {
            Jeu[row][columns] = '*'; // affiche '*' dans la matrice joueur
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

    while (1) // Commencement du jeu
    {
        system("cls"); //nettoyage du terminale
        showJeu(); // affiche la grille du joueur
        showMatrix(); // affiche la grille admin
        while (verif == 0)
        {
            printf("Selectionnez le numero de la ligne et de la colonne que vous voulez modifier: ");
            verif = scanf_s("%d %d", &selectRow, &selectCol);

            if (verif < 2 || Jeu[selectRow - 1][selectCol - 1] < 11 || Jeu[selectCol - 1][selectRow - 1] < 11) // Si la valeur n'est pas entre 1 et 10 
            {
                scanf_s("%*[^\n]");
                printf("Une des valeurs donnees n'est pas valide!\n\n"); // afficher erreur
                verif = 0;
            }
        }
        selectRow -= 1;
        selectCol -= 1;

        printf("Case (%d-%d) selectionne.\n\n", selectRow + 1, selectCol + 1); // Affiche les coordonées de la case séléctionné
        while (action == -1)
        {
            if (Jeu[selectRow][selectCol] == 'P') // si il y a un drapeau
            {
                printf("Que faire?\n1-Enlever un drapeau\n2-Changer de case\nChoix:");
                scanf_s("%d", &action);
                if (action == 1) // supprimer le drapeau posé si il y a un drapeau
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
            else if (Jeu[selectRow][selectCol] != 'P') // Si il n'y a pas de drapeau
            {
                printf("Que faire?\n1-Deminer\n2-Poser un drapeau\n3-Changer de case\nChoix:"); // Choix 1 déminer, 2 Poser un drapeau et 3 Changer de case
                scanf_s("%d", &action);
                if (action == 1) // si choix est 1 déminer
                {
                    revealCasesAround(selectRow, selectCol); // révélé la case choisi
                    showJeu();
                    showMatrix();
                }
                else if (action == 2) // si choix est 2 poser drapeau
                {
                    Jeu[selectRow][selectCol] = 'P'; // Place le drapeau
                }
                else if (action == 3)
                {
                    verif = 0; // renvoie a choix de la case
                }
            }

            if (action < 1 || action > 3) // Si action n'est pas 1, 2 ou 3 
            {
                printf("%d n'est pas un choix valide.\n\n", action); // Afficher erreur
                action = -1; // renvoi au choix 1, 2 ou 3
            }
        }

        if (action == 1) // Si l'action est 1
        {
            if (matrice[selectRow][selectCol] == 'X') // Si la case déminer est une bombe
            {
                printf("BOOM!\nIl y avait une bombe, c'est perdu!\n"); // Afficher Perdu !
                showMatrix();
                break;
            }
            else if (countVictory == 90) // Si toute les cases sans bombes sont révélé
            {
                printf("Bravo vous avez reussi"); // Afficher Bravo !
                printf("\n");
                break;
            }
        }
        verif = 0;
        selectRow = 0;
        selectCol = 0;
        action = -1;
    }

    return 0;
}