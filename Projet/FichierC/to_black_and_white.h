#ifndef MODIFIER_IMAGE_H
#define MODIFIER_IMAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int is_black(Uint32 pixel, SDL_PixelFormat* format);


float count_black_pixelstb(SDL_Surface* image);

SDL_Surface* modifierImage(const char* imagePath);


SDL_Surface* toblack(char* adresse);

SDL_Surface* toblackmot(SDL_Surface*  image);

#endif 
