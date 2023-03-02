#include "time.h"
#include "stdio.h"
#include "wchar.h"
#include "stdlib.h"
#include "Windows.h"
#include "conio.h"

char matrice[10][10]; // matrice "admin" avec les bombes affichées
char Jeu[10][10];     // matrice joueur " bombes caché"
int row, columns;  // lignes et colonnes
int b;
int countVictory; // Compteur de case vide pour condition de victoire

// générateur de 10 bombes de manières aléatoire
// @param refused Une table qui donne la position des 9 cases sur lesquelles aucune bombe ne doit être placée
void randomBomb(int refused[9][2])
{
    srand(time(NULL));
    int randomR, randomC;
    int i;
    for (i = 0; i < 9; i++)
    {
        // On pose temp. une bombe sur la case du joueur et autour pour que la boucle while après empêche une vraie bombe d'être placée sur ces cases.
        matrice[refused[i][0]][refused[i][1]] = 'X';
    }
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
    for (i = 0; i < 9; i++)
    {
        // Une fois les vrais bombes placées, on enlève celles sur la case du joueur et ses alentours, les certifiant "safe"
        matrice[refused[i][0]][refused[i][1]] = '-';
    }
}

// Affiche la grille de jeu
void showJeu(int x, int y)
{
    printf("\033[90m"); // Change la couleur du texte de la console à gris foncé
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
            if (Jeu[row][columns]=='1')
                printf("\033[94m"); // Change la couleur du texte de la console à bleu
            else if (Jeu[row][columns]=='2')
                printf("\033[32m"); // Change la couleur du texte de la console à vert
            else if (Jeu[row][columns]=='3')
                printf("\033[31m"); // Change la couleur du texte de la console à rouge
            else if (Jeu[row][columns]=='4')
                printf("\033[36m"); // Change la couleur du texte de la console à bleu clair
            else if (Jeu[row][columns]=='5')
                printf("\033[33m"); // Change la couleur du texte de la console à jaune
            else if (Jeu[row][columns]=='6')
                printf("\033[96m"); // Change la couleur du texte de la console à cyan
            else if (Jeu[row][columns]=='7')
                printf("\033[35m"); // Change la couleur du texte de la console à violet
            else if (Jeu[row][columns]=='8')
                printf("\033[37m"); // Change la couleur du texte de la console à gris clair (mais ça semble impossible à voir)
            else if (Jeu[row][columns]=='P')
                printf("\033[7m");
            else
                printf("\033[0m");
            if (row==y && columns==x)
                printf("\033[7m");
            printf("%c\033[0m  ", Jeu[row][columns]); // affiche la grille de jeu
        }
        printf("\033[90m"); // Change la couleur du texte de la console à gris foncé
        printf("\n");
    }
    printf("  --------------------------------\n");
    printf("\n");
    printf("\033[0m"); // Change la couleur du texte de la console au défaut (blanc)
}

// Affiche la grille admin avec les bombes
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

// Révèle une case et ses alentours
// @param r la rangée de la matrice où est la case
// @param c la colonne de la matrice où est la case
void revealCasesAround(int r, int c)
{
    if (r < 0 || r > 9 || c < 0 || c > 9) // Si la case est en dehors de la grille, on ne vérifie pas
        return;
    if (Jeu[r][c] != '*') // Si la case a déja été révélé, on ne révèle pas une seconde fois
        return;
    // Cases autours de l'élément choisi
    int aroundCases[8][2] = {
        {r, c - 1},     // top
        {r + 1, c - 1}, // top right
        {r + 1, c},     // right
        {r + 1, c + 1}, // bottom right
        {r, c + 1},     // bottom
        {r - 1, c + 1}, // bottom left
        {r - 1, c},     // left
        {r - 1, c - 1}, // top left
    };

    int i;
    int bombFound = 0; // Le nombre de bombes trouvés autour de la case concernée
    countVictory++; // ajoute 1 au compteur de victoire à chaque révélation
    for (i = 0; i < 8; i++)
    {
        if (aroundCases[i][0] < 0 || aroundCases[i][0] > 9 || aroundCases[i][1] < 0 || aroundCases[i][1] > 9) // Si c'est hors-limite, on passe
            continue;
        if (matrice[aroundCases[i][0]][aroundCases[i][1]] == 'X') // compte le nombre de bombe autour de la case
        {
            bombFound++;
            continue;
        }
    }
    if (bombFound == 0) // Si y'a aucune bombe au alentours
    {
        Jeu[r][c] = matrice[r][c];
        for (i = 0; i < 8; i++)
        {
            revealCasesAround(aroundCases[i][0], aroundCases[i][1]); // Révèle les cases autours du déminage
        }
    }
    else
        Jeu[r][c] = bombFound + '0'; //La case affiche le nombre de bombes autour

}

int main()
{
    // Passe la console en UTF-8, permettant de print des accents
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int firstTry = 1; // Détermine si c'est le premier coup du joueur ou pas

    for (row = 0; row < 10; row++)
    {
        for (columns = 0; columns < 10; columns++)
        {
            matrice[row][columns] = '-'; // affiche '-' dans la matrice admin
        }
    }
    
    for (row = 0; row < 10; row++)
    {
        for (columns = 0; columns < 10; columns++)
        {
            Jeu[row][columns] = '*'; // affiche '*' dans la matrice joueur
        }
    }

    printf("--- DEMINEUR C ---\n");
    printf("\n");
    printf("Bienvenue! Appuyez sur n'importe quel touche pour commencer une partie de démineur\n");
    getchar();

    int selectRow, selectCol;
    int verif = 0;
    int action = -1;
    int choix = 0;

    //Variables du pointer
    int posX = 4;
    int posY = 4;
    int canMove = 1;

    while (1) // Commencement du jeu
    {
        system("cls"); //nettoyage du terminale
        showJeu(posX, posY); // affiche la grille du joueur
        //showMatrix(); // affiche la grille admin

        while (1){
            if (_kbhit()){
                int key = _getch();
                printf("The key hitted is %i", key);
                switch (key)
                {
                    //Déplacement
                    case 72: // Flèche haut
                        posY-=1;
                        break;
                    case 80: // Flèche bas
                        posY+=1;
                        break;
                    case 75: // Flèche gauche
                        posX-=1;
                        break;
                    case 77: // Flèche droite
                        posX+=1;
                        break;

                    //Actions
                    case 13:
                    case 32: //Entrée ou Espace: démine une case
                        action = 1;
                        break;
                    case 102:
                    case 70: //F (minuscule et majuscule because why not?): pose/retire un drapeau
                        action = 2;
                        break;
                    default:
                        break;
                }

                if (posX<0)
                    posX = 0;
                else if (posX>9)
                    posX = 9;
                if (posY<0)
                    posY = 0;
                else if (posY>9)
                    posY = 9;

                break;
            }
        }

        selectRow = posY;
        selectCol = posX;

        if (Jeu[selectRow][selectRow]!='*'){
            action = -1;
        }

        //TODO: Comprendre pourquoi les cases ne répondent plus parfois et pourquoi les flags agissent n'importe comment
        if (action == 1) {
            if (Jeu[selectRow][selectCol] != 'P') {
                if (firstTry==1) { // Si c'est le premier coup du joueur, on met en place les bombes
                    int refusedCases[9][2] = {
                        {selectRow, selectCol},
                        {selectRow   , selectCol-1}, //top
                        {selectRow+1 , selectCol-1}, //top right
                        {selectRow+1 , selectCol  }, //right
                        {selectRow+1 , selectCol+1}, //bottom right
                        {selectRow   , selectCol+1}, //bottom
                        {selectRow-1 , selectCol+1}, //bottom left
                        {selectRow-1 , selectCol  }, //left
                        {selectRow-1 , selectCol-1}, //top left
                    };
                    randomBomb(refusedCases);
                    firstTry = 0;
                }
                revealCasesAround(selectRow, selectCol);
            }
        } else if (action == 2) {
            if (Jeu[selectRow][selectCol] == 'P') // si il y a un drapeau
                Jeu[selectRow][selectCol] = '*'; // Place le drapeau
            else
                Jeu[selectRow][selectCol] = 'P';
        }

        selectRow = 0;
        selectCol = 0;
        action = -1;

        /*
        while (verif == 0)
        {
            printf("Sélectionnez le numéro de la ligne et de la colonne que vous voulez modifier: ");
            verif = scanf_s("%d %d", &selectRow, &selectCol);

            if (verif < 2 || Jeu[selectRow - 1][selectCol - 1] < 11 || Jeu[selectCol - 1][selectRow - 1] < 11) // Si la valeur n'est pas entre 1 et 10 
            {
                scanf_s("%*[^\n]"); // Vide le buffer si la valeur n'est pas un integer
                printf("Une des valeurs données n'est pas valide!\n\n"); // afficher erreur
                verif = 0;
            }
            else if (Jeu[selectRow-1][selectCol-1] != '*')
            {
                printf("La case %d-%d a deja ete deminee!\n\n", selectRow, selectCol);
                verif = 0;
            }
        }

        selectRow -= 1;
        selectCol -= 1;

        printf("Case (%d-%d) sélectionnée.\n\n", selectRow + 1, selectCol + 1); // Affiche les coordonées de la case séléctionné
        while (action == -1)
        {
            if (Jeu[selectRow][selectCol] == 'P') // si il y a un drapeau
            {
                printf("Que faire?\n1-Enlever un drapeau\n2-Changer de case\nChoix:");
                scanf_s("%d", &action);
                if (action == 1) // supprimer le drapeau posé si il y a un drapeau
                {
                    Jeu[selectRow][selectCol] = '*';
                }
                else
                {
                    verif = 0; // renvoie au choix d'une case
                }
            }
            else if (Jeu[selectRow][selectCol] != 'P') // Si il n'y a pas de drapeau
            {
                printf("Que faire?\n1-Déminer\n2-Poser un drapeau\n3-Changer de case\nChoix:"); // Choix 1 déminer, 2 Poser un drapeau et 3 Changer de case
                scanf_s("%d", &action);
                if (action == 1) // si choix est 1 déminer
                {
                    if (firstTry==1) { // Si c'est le premier coup du joueur, on met en place les bombes
                        int refusedCases[9][2] = {
                            {selectRow, selectCol},
                            {selectRow   , selectCol-1}, //top
                            {selectRow+1 , selectCol-1}, //top right
                            {selectRow+1 , selectCol  }, //right
                            {selectRow+1 , selectCol+1}, //bottom right
                            {selectRow   , selectCol+1}, //bottom
                            {selectRow-1 , selectCol+1}, //bottom left
                            {selectRow-1 , selectCol  }, //left
                            {selectRow-1 , selectCol-1}, //top left
                        };
                        randomBomb(refusedCases);
                        firstTry = 0;
                    }
                    revealCasesAround(selectRow, selectCol);
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
                printf("\033[31m"); // Change la couleur du texte de la console à rouge
                printf("BOOM!\nIl y avait une bombe, c'est perdu!\n"); // Afficher Perdu !
                showMatrix();
                break;
            }
            else if (countVictory == 90) // Si toute les cases sans bombes sont révélé
            {
                printf("Bravo vous avez réussi"); // Afficher Bravo !
                printf("\n");
                break;
            }
        }
        verif = 0;
        selectRow = 0;
        selectCol = 0;
        action = -1;
        */
    }

    return 0;
}