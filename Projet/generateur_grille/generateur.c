#include "generateur.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int can_place(char** grille, int nb_lines, int nb_colonnes, const char* mot, int x, int y, int dx, int dy) {
    int longueur = strlen(mot);

    for (int i = 0; i < longueur; i++) {
        int nx = x + i * dx; 
        int ny = y + i * dy; 

        //check if the word can be placed in the grid
        if (nx < 0 || nx >= nb_lines || ny < 0 || ny >= nb_colonnes || (dx==0&&dy==0))
            return 0;

        // Check if the placement will not erase a word already placed
        if (grille[nx][ny] != '_' && grille[nx][ny] != mot[i])
            return 0;
    }

    return 1;
}

void place_word(char** grille, const char* mot, int x, int y, int dx, int dy) {
    int longueur = strlen(mot);
    for (int i = 0; i < longueur; i++) {
        int nx = x + i * dx; // Position future en x
        int ny = y + i * dy; // Position future en y
        grille[nx][ny] = mot[i];
    }
}
void free_grid(char** grid, int nb_lines) {
    for (int i = 0; i < nb_lines; i++) {
        free(grid[i]);
    }
    free(grid);
}

char* generateur_grille(int nb_lines,int nb_colonnes)
{
    char* c = (char*)malloc(sizeof(char)*(nb_lines*nb_colonnes+1));
    char* current = c;
    for (int i=0;i<nb_colonnes*nb_lines;i++)
    {
        *current = rand() % (90 - 65 + 1) + 65;

        current++;
    }
    *current = '\0';
    //place_word(c,nb_lines+1,nb_colonnes,"trois");
    return c;
}

char** generate_grille(int nb_lines,int nb_columns)
{
    char** grid = malloc(nb_lines * sizeof(char*));
    for (int i = 0; i < nb_lines; i++) {
        grid[i] = malloc(nb_columns * sizeof(char));
        for (int j = 0; j < nb_columns; j++) {
            grid[i][j] = '_';
        }
    }
    return grid;
}
void print_grid(char** grid,int nb_lines,int nb_columns)
{
    for (int i = 0; i < nb_lines; i++) {
        for (int j = 0; j < nb_columns; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n\n");
    }
}

void place_multiple_words(char** grid, char** mots,int nb_mots)
{
    int x=-1;
    int y=-1;
    int dx=-1;
    int dy=-1;
    int i =0;
    srand(time(NULL));
    while(i<nb_mots) {
        if (can_place(grid,15,15,mots[i],x,y,dx,dy))
        {
            //printf("%s : (%i,%i)->(%i,%i)\n",mots[i],x,y,dx,dy);
            place_word(grid,mots[i],x,y,dx,dy);
            i++;
        }
        
        x = rand() % 15;
        y = rand() % 15;
        dx = rand() % 3 - 1;
        dy = rand() % 3 - 1;
    }
}

void fill_grid(char** grid,int nb_lines,int nb_colonnes)
{
    for (int i=0;i<nb_lines;i++)
    {
        for (int j=0;j<nb_colonnes;j++)
        {
            if (grid[i][j]=='_')
            {
                grid[i][j] = rand() % 26 + 'A';
            }
        }
    }
}


char** final_generate(char*** liste)
{
    srand(time(NULL)); 
    int nb_lines = 15, nb_columns = 15;
    char** grille = generate_grille(nb_lines, nb_columns);

    //char* mots[5] = {"POMME","BANANE","CITRON","RAISIN","POIRE"};

    char* mots[5]; 
    int m = rand() % 3;
    if (m == 0) 
    {
        mots[0] = "POMME";
        mots[1] = "BANANE";
        mots[2] = "CITRON";
        mots[3] = "RAISIN";
        mots[4] = "POIRE";
    } 
    else if (m == 1) 
    {
        mots[0] = "LION";
        mots[1] = "DAUPHIN";
        mots[2] = "PANDA";
        mots[3] = "ELEPHANT";
        mots[4] = "HIBOU";
    } 
    else 
    {
        mots[0] = "DOCTEUR";
        mots[1] = "POMPIER";
        mots[2] = "BOULANGER";
        mots[3] = "JARDINIER";
        mots[4] = "CHIRURGIEN";
    }    
    
    *liste = malloc(5 * sizeof(char*));
    if (!*liste) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les mots\n");
        free_grid(grille, nb_lines);
        return NULL;
    }


    for (int i = 0; i < 5; i++) {
        (*liste)[i] = strdup(mots[i]);
        if (!(*liste)[i]) {
            fprintf(stderr, "Erreur d'allocation mémoire pour le mot %s\n", mots[i]);
            for (int j = 0; j < i; j++) {
                free((*liste)[j]);
            }
            free(*liste);
            free_grid(grille, nb_lines);
            return NULL;
        }
    }


    place_multiple_words(grille,*liste,5);
    fill_grid(grille,15,15);


    return grille;
}