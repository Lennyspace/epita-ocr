#ifndef NET_H
#define NET_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <math.h>


void drawLine2(SDL_Surface* surface, int x0, int y0, int x1, int y1);


void drawPathsBetweenAllBlackPixels(SDL_Surface* surface);


void processImage(const SDL_Surface* source, SDL_Surface* target, int nbnoirmax);


void redrawAdjacentPixels(SDL_Surface* surface);


int countIsolatedBlackPixels(SDL_Surface* source);


void removeSparseBlackPixels(SDL_Surface* source);
void removeSparseBlackPixelsmot(SDL_Surface* source);

void removeSparseBlackPixelsmotv2(SDL_Surface* source);

SDL_Surface* net(SDL_Surface* sourceImage);

SDL_Surface* netmot(SDL_Surface* sourceImage);

#endif // NET_H