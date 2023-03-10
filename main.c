#include "SDL.h"
#include <SDL_mixer.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 450
#define TEXTURES_LEN 17

// générateur de 10 bombes de manières aléatoire
// @param refused Une table qui donne la position des 9 cases sur lesquelles aucune bombe ne doit être placée
void randomBomb(char matrice[10][10], int refused[9][2])
{
    srand(time(NULL));
    int randomR, randomC;
    int i;
    for (i = 0; i < 9; i++)
    {
        // On pose temp. une bombe sur la case du joueur et autour pour que la boucle while après empêche une vraie bombe d'être placée sur ces cases.
        if (refused[i][0] >= 0 && refused[i][0] <= 9 && refused[i][1] >= 0 && refused[i][1] <= 9) {
            matrice[refused[i][0]][refused[i][1]] = 'X';
        }
    }
    for (i = 0; i < 10; i++)
    {
        randomR = rand() % 10;
        randomC = rand() % 10;
        while (matrice[randomR][randomC] == 'X') // quand il y a déja une bombe replacer une bombe ailleurs
        {
            randomR = rand() % 10;
            randomC = rand() % 10;
        }
        matrice[randomR][randomC] = 'X';
    }
    for (i = 0; i < 9; i++)
    {
        // Une fois les vrais bombes placées, on enlève celles sur la case du joueur et ses alentours, les certifiant "safe"
        if (refused[i][0] >= 0 && refused[i][0] <= 9 && refused[i][1] >= 0 && refused[i][1] <= 9)
            matrice[refused[i][0]][refused[i][1]] = '-';
    }
}

// Affiche la grille de jeu
void debugJeu(char Jeu[][10])
{
    printf("  | 1  2  3  4  5  6  7  8  9  10\n");
    printf("  --------------------------------\n");

    int row, columns;  // lignes et colonnes
    for (row = 0; row < 10; row++) // boucle pour les lignes
    {
        if (row < 9) // si ligne < 9 alors écrire " | " et sauté une ligne
            printf(" %d| ", row + 1);
        else
            printf("%d| ", row + 1); // sinon écrire "| " et sauté une ligne

        for (columns = 0; columns < 10; columns++) // boucle pour les colonnes
        {
            /*if (Jeu[row][columns] == '1')
                printf("\033[94m"); // Change la couleur du texte de la console à bleu
            else if (Jeu[row][columns] == '2')
                printf("\033[32m"); // Change la couleur du texte de la console à vert
            else if (Jeu[row][columns] == '3')
                printf("\033[31m"); // Change la couleur du texte de la console à rouge
            else if (Jeu[row][columns] == '4')
                printf("\033[36m"); // Change la couleur du texte de la console à bleu clair
            else if (Jeu[row][columns] == '5')
                printf("\033[33m"); // Change la couleur du texte de la console à jaune
            else if (Jeu[row][columns] == '6')
                printf("\033[96m"); // Change la couleur du texte de la console à cyan
            else if (Jeu[row][columns] == '7')
                printf("\033[35m"); // Change la couleur du texte de la console à violet
            else if (Jeu[row][columns] == '8')
                printf("\033[37m"); // Change la couleur du texte de la console à gris clair (mais ça semble impossible à voir)
            else if (Jeu[row][columns] == 'P')
                printf("\033[0;7m");
            else
                printf("\033[0m");
            */
            printf("%c  ", Jeu[row][columns]); // affiche la grille de jeu
        }
        //printf("\033[0m"); // Change la couleur du texte de la console à gris foncé
        printf("\n");
    }
    printf("  --------------------------------\n");
    printf("\n");
}

void debugMatrice(char matrice[][10])
{
    printf("  | 0  1  2  3  4  5  6  7  8  9\n");
    printf("  --------------------------------\n");

    int row, columns;  // lignes et colonnes
    for (row = 0; row < 10; row++)
    {
        if (row < 9)
            printf("%d| ", row);
        else
            printf("%d| ", row);

        for (columns = 0; columns < 10; columns++)
        {
            printf("%c  ", matrice[row][columns]); // affiche la grille admin
        }
        printf("\n");
    }
}

void drawSmile(SDL_Renderer* renderer, SDL_Texture* textures[TEXTURES_LEN], int gameState, SDL_Rect upper, SDL_Rect game) {
    if (SDL_RenderSetViewport(renderer, &upper) < 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "A SDL2 error has occurred!", SDL_GetError(), NULL);
    }
    SDL_Rect dimensions = {(WINDOW_WIDTH-48)/2, 0, 48, 48};
    if (gameState==0) {
        SDL_RenderCopy(renderer, textures[14], NULL, &dimensions);
    }
    else if (gameState==1) {
        SDL_RenderCopy(renderer, textures[16], NULL, &dimensions);
    }
    else if (gameState==2) {
        SDL_RenderCopy(renderer, textures[15], NULL, &dimensions);
    }
    SDL_RenderSetViewport(renderer, &game);
}

void drawGrid(SDL_Renderer * renderer, char Jeu[10][10], SDL_Texture* textures[TEXTURES_LEN]) {
    int i, j;
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            SDL_Rect dimensions = { 0 + (40 * j), 0 + (40 * i),40,40 };
            int x;
            if (Jeu[i][j] == '*') // Case caché
                SDL_RenderCopy(renderer, textures[9], NULL, &dimensions);
            else if (Jeu[i][j] == '-') // Case révélé
                SDL_RenderCopy(renderer, textures[0], NULL, &dimensions);
            else if (Jeu[i][j] == '1') // Case avec 1 bombe autour
                SDL_RenderCopy(renderer, textures[1], NULL, &dimensions);
            else if (Jeu[i][j] == '2') // Case avec 2 bombes autour
                SDL_RenderCopy(renderer, textures[2], NULL, &dimensions);
            else if (Jeu[i][j] == '3') // Case avec 3 bombes autour
                SDL_RenderCopy(renderer, textures[3], NULL, &dimensions);
            else if (Jeu[i][j] == '4') // Case avec 4 bombes autour
                SDL_RenderCopy(renderer, textures[4], NULL, &dimensions);
            else if (Jeu[i][j] == '5') // Case avec 5 bombes autour
                SDL_RenderCopy(renderer, textures[5], NULL, &dimensions);
            else if (Jeu[i][j] == '6') // Case avec 6 bombes autour
                SDL_RenderCopy(renderer, textures[6], NULL, &dimensions);
            else if (Jeu[i][j] == '7') // Case avec 7 bombes autour
                SDL_RenderCopy(renderer, textures[7], NULL, &dimensions);
            else if (Jeu[i][j] == '8') // Case avec 8 bombes autour
                SDL_RenderCopy(renderer, textures[8], NULL, &dimensions);
            else if (Jeu[i][j] == 'P') // Case flaggé
                SDL_RenderCopy(renderer, textures[11], NULL, &dimensions);
            else if (Jeu[i][j] == 'W') // Case flaggé sans bombe
                SDL_RenderCopy(renderer, textures[13], NULL, &dimensions);
            else if (Jeu[i][j] == 'X') // Case bombe
                SDL_RenderCopy(renderer, textures[10], NULL, &dimensions);
            else if (Jeu[i][j] == '!') // Case bombe sélectionné par le joueur
                SDL_RenderCopy(renderer, textures[12], NULL, &dimensions);
        }
    }
    SDL_RenderPresent(renderer);
}

// Révèle une case et ses alentours
// @param r la rangée de la matrice où est la case
// @param c la colonne de la matrice où est la case
void revealCasesAround(char matrice[10][10], char Jeu[10][10], int r, int c, int *countVictory)
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
    *countVictory+=1; // ajoute 1 au compteur de victoire à chaque révélation
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
            revealCasesAround(matrice, Jeu, aroundCases[i][0], aroundCases[i][1], countVictory); // Révèle les cases autours du déminage
        }
    }
    else
        Jeu[r][c] = bombFound + '0'; //La case affiche le nombre de bombes autour

}

void showAllBomb(char Jeu[10][10], char matrice[10][10], int r, int c){
    for (int r2 = 0; r2 < 10; ++r2)
    {
        for (int c2 = 0; c2 < 10; ++c2)
        {
            if (r2 != r || c2 != c) {
                if (matrice[r2][c2] == 'X' && Jeu[r2][c2] != 'P') {
                    Jeu[r2][c2] = 'X';
                }
                if (Jeu[r2][c2] == 'P' && matrice[r2][c2] != 'X') {
                    Jeu[r2][c2] = 'W';
                }
            }
        }
    }
}

void resetMatrixes(char matrice[10][10], char Jeu[10][10]) {
    int row, columns;  // lignes et colonnes
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
}

int main(int argc, char *argv[]) {
    char Jeu[10][10];
    char matrice[10][10];

    int firstTry = 1; // Détermine si c'est le premier coup du joueur ou pas
    int countVictory = 0; // Compte le nombre de cases révélés

    resetMatrixes(matrice, Jeu);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Erreur en initiant SDL2: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) < 0) {
        printf("Erreur en initiant la fenêtre: %s", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window Error", SDL_GetError(), NULL);
        return EXIT_FAILURE;
    }
    SDL_SetWindowTitle(window, "Démineur SDL2");

    SDL_Surface* icon = SDL_LoadBMP("img/icon.bmp");
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);

    SDL_Event event;

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) {
        printf("Erreur en initiant SDL2_mixer: %s", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL2_mixer Error", SDL_GetError(), NULL);
        return EXIT_FAILURE;
    }

    Mix_AllocateChannels(3);
    Mix_Chunk *tick;
    Mix_Chunk *bomb;
    Mix_Chunk *win;

    tick = Mix_LoadWAV("aud/tick.wav");
    bomb = Mix_LoadWAV("aud/lose.wav");
    win = Mix_LoadWAV("aud/win.wav");

    int isRunning = SDL_TRUE;
    int gameState = 0; //0: Rien, 1: Perdu, 2: Gagné

    SDL_SetRenderDrawColor(renderer, 192, 192, 192, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
    SDL_Rect rect_upper = { 0, 0, WINDOW_WIDTH, 50 };
    SDL_Rect rect_game = { 0, 50, 400, 400 };
    SDL_RenderFillRect(renderer, &rect_upper);
    SDL_SetRenderDrawColor(renderer, 155, 155, 155, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect_game);

    SDL_RenderSetViewport(renderer, &rect_game);

    // Créer les surfaces et charge les images
    SDL_Surface* s_caseN = SDL_LoadBMP("img/case.bmp");
    SDL_Surface* s_caseR = SDL_LoadBMP("img/caseR0.bmp");
    SDL_Surface* s_caseB = SDL_LoadBMP("img/caseB.bmp");
    SDL_Surface* s_caseF = SDL_LoadBMP("img/caseF.bmp");
    SDL_Surface* s_caseB2 = SDL_LoadBMP("img/caseB2.bmp");
    SDL_Surface* s_caseBF = SDL_LoadBMP("img/caseBF.bmp");

    SDL_Surface* s_case1 = SDL_LoadBMP("img/caseR1.bmp");
    SDL_Surface* s_case2 = SDL_LoadBMP("img/caseR2.bmp");
    SDL_Surface* s_case3 = SDL_LoadBMP("img/caseR3.bmp");
    SDL_Surface* s_case4 = SDL_LoadBMP("img/caseR4.bmp");
    SDL_Surface* s_case5 = SDL_LoadBMP("img/caseR5.bmp");
    SDL_Surface* s_case6 = SDL_LoadBMP("img/caseR6.bmp");
    SDL_Surface* s_case7 = SDL_LoadBMP("img/caseR7.bmp");
    SDL_Surface* s_case8 = SDL_LoadBMP("img/caseR8.bmp");

    SDL_Surface* s_smile = SDL_LoadBMP("img/smile.bmp");
    SDL_Surface* s_smileW = SDL_LoadBMP("img/smileW.bmp");
    SDL_Surface* s_smileL = SDL_LoadBMP("img/smileL.bmp");

    if (!s_caseN || !s_caseR || !s_caseB || !s_caseF || !s_caseB2 || !s_caseBF || !s_case1 || !s_case2 || !s_case3 || !s_case4 || !s_case5 || !s_case6 || !s_case7 || !s_case8 || !s_smile || !s_smileL || !s_smileW)
    {
        printf("Erreur de chargement d'une image : %s", SDL_GetError());
        return -1;
    }

    // Créer les textures
    SDL_Texture* textures[TEXTURES_LEN] = {
        SDL_CreateTextureFromSurface(renderer, s_caseR),  // Revealed
        SDL_CreateTextureFromSurface(renderer, s_case1),  // 1 bomb
        SDL_CreateTextureFromSurface(renderer, s_case2),  // 2 bombs
        SDL_CreateTextureFromSurface(renderer, s_case3),  // 3 bombs
        SDL_CreateTextureFromSurface(renderer, s_case4),  // 4 bombs
        SDL_CreateTextureFromSurface(renderer, s_case5),  // 5 bombs
        SDL_CreateTextureFromSurface(renderer, s_case6),  // 6 bombs
        SDL_CreateTextureFromSurface(renderer, s_case7),  // 7 bombs
        SDL_CreateTextureFromSurface(renderer, s_case8),  // 8 bombs
        SDL_CreateTextureFromSurface(renderer, s_caseN),  // Neutral
        SDL_CreateTextureFromSurface(renderer, s_caseB),  // Bomb
        SDL_CreateTextureFromSurface(renderer, s_caseF),  // Flag
        SDL_CreateTextureFromSurface(renderer, s_caseB2), // BombRed
        SDL_CreateTextureFromSurface(renderer, s_caseBF), // WrongFlag
        SDL_CreateTextureFromSurface(renderer, s_smile),  // Smile Neutral
        SDL_CreateTextureFromSurface(renderer, s_smileW), // Smile Win
        SDL_CreateTextureFromSurface(renderer, s_smileL), // Smile Lose
    };

    // Enlève les surfaces
    SDL_FreeSurface(s_caseN);
    SDL_FreeSurface(s_caseR);
    SDL_FreeSurface(s_caseB);
    SDL_FreeSurface(s_caseB2);
    SDL_FreeSurface(s_caseBF);
    SDL_FreeSurface(s_caseF);
    SDL_FreeSurface(s_case1);
    SDL_FreeSurface(s_case2);
    SDL_FreeSurface(s_case3);
    SDL_FreeSurface(s_case4);
    SDL_FreeSurface(s_case5);
    SDL_FreeSurface(s_case6);
    SDL_FreeSurface(s_case7);
    SDL_FreeSurface(s_case8);
    SDL_FreeSurface(s_smile);
    SDL_FreeSurface(s_smileW);
    SDL_FreeSurface(s_smileL);

    drawSmile(renderer, textures, gameState, rect_upper, rect_game);
    drawGrid(renderer, Jeu, textures);
    int i, j;
    while (isRunning) {
        if (SDL_PollEvent(&event) && gameState==0) {
            switch (event.type)
            {
                case (SDL_QUIT):
                    isRunning = SDL_FALSE;
                    break;

                case (SDL_MOUSEBUTTONDOWN):
                    printf("Mouse: %i-%i\n", event.button.x, event.button.y);
                    int mouseX = event.button.x;
                    int mouseY = event.button.y-50;
                    for (i = 0; i < 10; i++)
                    {
                        for (j = 0; j < 10; j++)
                        {
                            if ((mouseX >= 40 * j && mouseX <= (40 * j) + 40) && (mouseY >= 40 * i && mouseY <= (40 * i) + 40)) {
                                printf("Clicked on case %i-%i\n", i, j);
                                if (event.button.button == SDL_BUTTON_LEFT) {
                                    if (Jeu[i][j]!='P') {
                                        if (firstTry) {
                                            int refusedCases[9][2] = {
                                                {i     , j    },
                                                {i     , j - 1}, //top
                                                {i + 1 , j - 1}, //top right
                                                {i + 1 , j    }, //right
                                                {i + 1 , j + 1}, //bottom right
                                                {i     , j + 1}, //bottom
                                                {i - 1 , j + 1}, //bottom left
                                                {i - 1 , j    }, //left
                                                {i - 1 , j - 1}, //top left
                                            };
                                            printf("countVictory: %i\n", countVictory);
                                            printf("A\n");
                                            randomBomb(matrice, refusedCases);
                                            printf("countVictory: %i\n", countVictory);
                                            printf("B\n");
                                            firstTry = 0;
                                        }
                                        if (matrice[i][j]=='X') {
                                            Jeu[i][j]='!';
                                            Mix_PlayChannel(-1, bomb, 0);
                                            gameState = 1;
                                            showAllBomb(Jeu, matrice, i, j);
                                        } else {
                                            printf("C\n");
                                            printf("countVictory: %i\n", countVictory);
                                            if (Jeu[i][j]=='*')
                                                Mix_PlayChannel(-1, tick, 0);
                                            revealCasesAround(matrice, Jeu, i, j, &countVictory);
                                            printf("D\n");
                                            printf("countVictory: %i\n", countVictory);
                                            if (countVictory == 90) {
                                                Mix_PlayChannel(-1, win, 0);
                                                gameState = 2;
                                            }
                                        }
                                    }
                                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                                    if (Jeu[i][j] == 'P') {
                                        Jeu[i][j] = '*';
                                    }
                                    else if (Jeu[i][j] == '*') {
                                        Jeu[i][j] = 'P'; // Place le drapeau
                                    }
                                }
                                printf("F1\n");
                                drawSmile(renderer, textures, gameState, rect_upper, rect_game);
                                printf("F2\n");
                                drawGrid(renderer, Jeu, textures);
                                printf("F3\n");
                            }
                        }
                    };
                    if (event.button.x > (WINDOW_WIDTH-48)/2 && event.button.x < ((WINDOW_WIDTH-48)/2)+48 && event.button.y < 48) {
                        gameState = 0;
                        resetMatrixes(matrice, Jeu);
                        firstTry = 1;
                        countVictory = 0;
                        drawSmile(renderer, textures, gameState, rect_upper, rect_game);
                        drawGrid(renderer, Jeu, textures);
                    }
                break;

            case (SDL_KEYDOWN):
                switch (event.key.keysym.sym){
                    case (SDLK_F5):
                        debugJeu(Jeu);
                        debugMatrice(matrice);
                        break;
                }
                break;
            }
        }

        if (gameState != 0) {
            while(SDL_TRUE) {
                if (SDL_PollEvent(&event)) {
                    switch (event.type)
                    {
                        case (SDL_QUIT):
                            isRunning = SDL_FALSE;
                            break;

                        case (SDL_MOUSEBUTTONDOWN):
                            if (event.button.x > (WINDOW_WIDTH-48)/2 && event.button.x < ((WINDOW_WIDTH-48)/2)+48 && event.button.y < 48) {
                                gameState = 0;
                                resetMatrixes(matrice, Jeu);
                                firstTry = 1;
                                countVictory = 0;
                                drawSmile(renderer, textures, gameState, rect_upper, rect_game);
                                drawGrid(renderer, Jeu, textures);
                            }
                            break;
                    }
                }
                if (gameState!=1 || !isRunning)
                    break;
            }
        }
    }

    // Libère les sons de la mémoire et décharge SDL_mixer
    Mix_FreeChunk(tick);
    Mix_FreeChunk(bomb);
    Mix_FreeChunk(win);
    Mix_CloseAudio();

    // Libère tout les graphismes de la mémoire
    for (int i = 0; i < TEXTURES_LEN; ++i)
    {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}