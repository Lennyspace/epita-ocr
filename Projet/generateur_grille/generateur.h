#ifndef GENERATEUR_H
#define GENERATEUR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void print_grid(char** grid, int nb_lines, int nb_columns);
void free_grid(char** grid, int nb_lines);
void fill_grid(char** grid, int nb_lines, int nb_colonnes);
int can_place(char** grille, int nb_lines, int nb_colonnes, const char* mot, int x, int y, int dx, int dy);
void place_word(char** grille, const char* mot, int x, int y, int dx, int dy);
void place_multiple_words(char** grid, char** mots, int nb_mots);
char** random_liste(void);
char* generateur_grille(int nb_lines, int nb_colonnes);
char** final_generate(char*** liste);

#endif // GENERATEUR_H