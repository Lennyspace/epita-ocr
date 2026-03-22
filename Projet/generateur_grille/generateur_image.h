#ifndef GENERATEUR_IMAGE_H
#define GENERATEUR_IMAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void place_image(SDL_Surface* grid, char* ImagePath, int x, int y);

void quadrillage(SDL_Surface* image, char** grid);

void liste(SDL_Surface* image, char** words, int nb_lines);

void fin_generation();


#endif // GENERATEUR_IMAGE_H