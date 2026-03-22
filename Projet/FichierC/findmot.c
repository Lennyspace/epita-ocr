#include <stdio.h>
#include <stdlib.h>
#include "additionnal_functions.h"

#include "findmot.h"


SDL_Surface** decoupeListe(SDL_Surface* image, int* nbMot) {
    SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0);

    Uint32 white = SDL_MapRGBA(formattedImage->format, 255, 255, 255, 255);
    Uint32 black = SDL_MapRGBA(formattedImage->format, 0, 0, 0, 255);

    int width =formattedImage->w;
    int height =formattedImage->h;

    SDL_Surface** mots = malloc(sizeof(SDL_Surface*) * height); 
    *nbMot = 0;

    for (int y = 0; y <height; y++) {
        int ligneVide = 1;
        for (int x = 0; x< width; x++) {
            Uint32 pixel = ((Uint32*)formattedImage->pixels)[y * width + x];
            if (pixel == black) {
                ligneVide = 0; 
                break;
            }
        }
        if (!ligneVide) {
            int top = y;
            int bottom = y;

            while (bottom < height) {
                ligneVide = 1;
                for (int x = 0; x < width; x++) {
                    Uint32 pixel = ((Uint32*)formattedImage->pixels)[bottom * width + x];
                    if (pixel == black) {
                        ligneVide = 0;
                        break;
                    }
                }
                if (ligneVide) break;
                bottom++;
            }
            SDL_Rect rect = {0, top, width, bottom - top};
            SDL_Surface* mot = SDL_CreateRGBSurfaceWithFormat(0, rect.w, rect.h, 32, SDL_PIXELFORMAT_RGBA32);
            SDL_BlitSurface(formattedImage, &rect, mot, NULL);

            mots[*nbMot] = mot;
            (*nbMot)++;
            y = bottom;
        }
    }
    SDL_FreeSurface(formattedImage);
    return mots;
}

SDL_Surface** decoupelistemot(SDL_Surface* image,int* nbmot)
{
    SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0);
    image = formattedImage;
    //IMG_SavePNG(image, "res/debug.png");
    return decoupeListe(image,nbmot);
}