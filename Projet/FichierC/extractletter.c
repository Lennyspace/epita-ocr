#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "extractletter.h"

typedef struct {
    int minX, minY, maxX, maxY;
} BoundingBox;

int isPixelBlack(SDL_Surface* surface, int x, int y) {
    Uint32* pixels = (Uint32*)((Uint8*)surface->pixels + y * surface->pitch);
    Uint32 pixel = pixels[x];
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g,  &b);
    return (r == 0 && g == 0 && b == 0);
}

void exploreRegion(SDL_Surface* surface, int x, int y, BoundingBox* box, Uint8* visited) {
    if (x < 0 || x>= surface->w || y < 0 || y >= surface->h) return;
    if (visited[y* surface->w + x]) return;
    if (!isPixelBlack(surface, x, y)) return;

    visited[y * surface->w +  x] = 1;

    if (x < box->minX) box->minX = x;
    if (x > box->maxX) box->maxX = x;
    if (y < box->minY) box->minY = y;
    if (y > box->maxY) box->maxY = y;

    exploreRegion(surface,x + 1, y, box,visited);
    exploreRegion(surface,x - 1, y, box,visited);
    exploreRegion(surface,x, y + 1, box,visited);
    exploreRegion(surface,x, y - 1, box, visited);

}


void sortLetters(SDL_Surface** letters, BoundingBox* boxes, int nbletter) {
    for (int i = 0; i < nbletter - 1; i++) {
        for (int j = i + 1; j < nbletter; j++) {
            if (boxes[i].minX > boxes[j].minX) {
                SDL_Surface* tempSurface = letters[i];
                letters[i] = letters[j];
                letters[j] = tempSurface;
                BoundingBox tempBox = boxes[i];
                boxes[i] = boxes[j];
                boxes[j] = tempBox;
            }
        }
    }
}

SDL_Surface** extractLetters(SDL_Surface* mot, int* nbletter) {

    Uint8* visited = calloc(mot->w * mot->h, sizeof(Uint8));
    SDL_Surface** letters = malloc(sizeof(SDL_Surface*) * mot->w);
    BoundingBox* boxes = malloc(sizeof(BoundingBox) * mot->w);
    *nbletter = 0;

    for (int y = 0; y < mot->h; y++) {
        for (int x = 0; x < mot->w; x++) {
            if (!visited[y * mot->w + x] && isPixelBlack(mot, x, y)) {
                BoundingBox box = {x, y, x, y};
                exploreRegion(mot, x, y, &box, visited);

                int width = box.maxX - box.minX+ 1;
                int height = box.maxY - box.minY+ 1;
                SDL_Surface* letterSurface = SDL_CreateRGBSurface(0, width, height, 32,mot->format->Rmask,mot->format->Gmask,
                                                                  mot->format->Bmask,mot->format->Amask);

                SDL_Rect srcRect = {box.minX, box.minY, width, height};
                SDL_BlitSurface(mot, &srcRect, letterSurface, NULL);
                letters[*nbletter] = letterSurface;
                boxes[*nbletter] = box;
                (*nbletter)++;
            }
        }
    }

    sortLetters(letters, boxes, *nbletter);
    free(visited);
    free(boxes);
    letters = realloc(letters, sizeof(SDL_Surface*) * (*nbletter));
    return letters;
}


SDL_Surface* addMargin(SDL_Surface* letter, int margin, SDL_PixelFormat* format) {
    int newWidth = letter->w + 2 * margin;
    int newHeight = letter->h + 2 * margin;
    SDL_Surface* newSurface = SDL_CreateRGBSurface(0, newWidth, newHeight, 32,
                                                    format->Rmask, format->Gmask,
                                                    format->Bmask, format->Amask);

    SDL_FillRect(newSurface, NULL, SDL_MapRGB(format, 255, 255, 255));
    SDL_Rect destRect = {margin, margin, letter->w, letter->h};
    SDL_BlitSurface(letter, NULL, newSurface, &destRect);
    return newSurface;
}

SDL_Surface** extractlettermain(SDL_Surface* mot, int* nbletter) {
    SDL_Surface** letters = extractLetters(mot, nbletter);
    for (int j = 0; j < *nbletter; j++) {
        letters[j] = addMargin(letters[j], 7,letters[j]->format);
    }
    return letters;
}