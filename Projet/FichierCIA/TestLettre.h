#ifndef TESTLETTRE_H
#define TESTLETTRE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int reconnaitlettre(SDL_Surface** tablettre,int taillecolone,int tailleligne);

char testlettreunique(SDL_Surface* lettre);
#endif