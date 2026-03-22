#ifndef READLETTER1_H
#define READLETTER1_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


SDL_Surface** readlettermain(SDL_Surface* image,int* nbcol,int* nbligne,struct Point*** coordonateimage);

#endif