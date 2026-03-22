#include <stdio.h>
#include <stdlib.h>
#include "additionnal_functions.h"
#include "detectermot.h"

#define ADJACENT_PIXEL_COUNT 9
#define MAX_LINES 1000


void processImagev2(SDL_Surface* source, int countmaxblackpixel) {
    int width = source->w;
    int height = source->h;
    Uint32* pixels = (Uint32*)source->pixels;
    Uint32 black = SDL_MapRGB(source->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(source->format, 255, 255, 255);
    Uint32* tempPixels = (Uint32*)malloc(width * height * sizeof(Uint32));
    if (tempPixels == NULL) {
        fprintf(stderr, "Erreur \n");
        return;
    }
    memcpy(tempPixels, pixels, width * height * sizeof(Uint32));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tempPixels[y * width + x] == black) {
                int blackCount = 0;

                for (int dy = -4; dy<= 4; ++dy) {
                    for (int dx = -4;dx <= 4; ++dx) {
                        int nx = x +dx;
                        int ny = y +dy;
                        if (nx >= 0 && nx <  width && ny >= 0&&  ny <height) {
                            if (tempPixels[ny * width + nx] == black) {
                                blackCount++;
                            }
                        }
                    }
                }
                
                if (blackCount<countmaxblackpixel) {
                    for (int dy= -4; dy<= 4; ++dy) {
                        for (int dx = -4; dx <= 4; ++dx) {
                            int nx = x +dx;
                            int ny = y+ dy;

                            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                                if (tempPixels[ny * width+nx] == black) {
                                    pixels[ny *width +nx] = white;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    free(tempPixels);
}


void processImagev22(SDL_Surface* source) {
    int width = source->w;
    int height = source->h;
    int squareSize = 70;
    int threshold = 60; 
    Uint32* pixels = (Uint32*)source->pixels;
    Uint32 black = SDL_MapRGB(source->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(source->format, 255, 255, 255);

    
    for (int y = 0; y < height; y += squareSize) {
        for (int x = 0; x < width; x += squareSize) {
            int blackCount = 0;

            
            for (int j = 0; j < squareSize && (y + j) < height; j++) {
                for (int i = 0; i < squareSize && (x + i) < width; i++) {
                    if (pixels[(y + j) * width + (x + i)] == black) {
                        blackCount++;
                    }
                }
            }
            if (blackCount < threshold) {
                for (int j = 0; j < squareSize && (y + j) < height; j++) {
                    for (int i = 0; i < squareSize && (x + i) < width; i++) {
                        pixels[(y + j) * width + (x + i)] = white;
                    }
                }
            }
        }
    }
}
   

SDL_Surface* zoomimage(SDL_Surface* image, SDL_Surface* imagedebase) {
    //IMG_SavePNG(imagedebase, "imagedebase.png");
    SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(image);
    image = formattedImage;

    int x_min = 0, y_min = 0, x_max = 0, y_max = 0;
    //IMG_SavePNG(imagedebase, "imagedebase2.png");
    processImagev2(image, 5);
    //IMG_SavePNG(imagedebase, "imagedebase3.png");
    processImagev22(image);
    //IMG_SavePNG(imagedebase, "imagedebase4.png");

    // Récupérer les coordonnées de la zone d'intérêt
    first_black_pixel_from_each(image, &x_min, &y_min, &x_max, &y_max);

    // Appliquer les coordonnées sur l'image de base
    SDL_Surface* zoomedBaseImage = create_new_surface(imagedebase, x_min - 5, y_min - 5, x_max + 5, y_max + 5);
    //IMG_SavePNG(zoomedBaseImage, "imagedebase5.png");
    return zoomedBaseImage;
}