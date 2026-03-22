#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define PI 3.14159265


void sobelFilter(SDL_Surface *surface, Uint8 threshold) {
    int width = surface->w;
    int height = surface->h;

    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 *newPixels = malloc(width * height * sizeof(Uint32));

    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1;x < width - 1; x++) {
            int sumX = 0;
            int sumY = 0;
            for (int ky = 0; ky < 3; ky++) {
                for (int kx = 0; kx < 3; kx++) {
                    int px = x + kx - 1;
                    int py = y + ky - 1;

                    Uint32 pixel = pixels[py * width + px];
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, surface->format,&r, &g, &b);

                    int intensity = (r + g + b) / 3; 
                    sumX += intensity * Gx[ky][kx];
                    sumY += intensity * Gy[ky][kx];
                }
            }

            
            int magnitude = (int)sqrt(sumX * sumX + sumY * sumY);
            if (magnitude > threshold) magnitude = 255;
            else magnitude = 0;

            newPixels[y *width + x] = SDL_MapRGBA(surface->format, magnitude, magnitude, magnitude, 255);
        }
    }

    
    memcpy(pixels, newPixels, width * height * sizeof(Uint32));
    free(newPixels);
}


double houghTransform(SDL_Surface *surface) {
    int width = surface->w;
    int height = surface->h;
    Uint32 *pixels = (Uint32 *)surface->pixels;

    int thetaBins = 180; 
    int rMax = (int)sqrt(width * width + height * height);
    int *accumulator = calloc(rMax *thetaBins, sizeof(int));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint32 pixel = pixels[y * width + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);

            
            if (r > 128) {
                for (int theta = 0; theta < thetaBins; theta++) {
                    double rad = theta * PI / 180.0;
                    int r = (int)(x * cos(rad) + y * sin(rad));
                    if (r >= 0 && r < rMax) {
                        accumulator[r * thetaBins + theta]++;
                    }
                }
            }
        }
    }

    
    int maxVotes = 0;
    int bestTheta = 0;
    for (int theta = 0; theta < thetaBins; theta++) {
        for (int r = 0; r < rMax; r++) {
            int votes = accumulator[r * thetaBins + theta];
            if (votes > maxVotes) {
                maxVotes = votes;
                bestTheta = theta;
            }
        }
    }

    free(accumulator);
    return bestTheta - 90; 
}


SDL_Surface *rotateSurface(SDL_Surface *surface, double angle) {
    SDL_Surface *rotated = SDL_CreateRGBSurfaceWithFormat(
        0, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->format);

    if (!rotated) {
        printf("Erreur de création de surface : %s\n", SDL_GetError());
        return NULL;
    }

    SDL_LockSurface(rotated);
    SDL_LockSurface(surface);

    Uint32 *srcPixels = (Uint32 *)surface->pixels;
    Uint32 *dstPixels = (Uint32 *)rotated->pixels;
    int width = surface->w;
    int height = surface->h;

    double rad = angle * PI / 180.0;
    double cosA = cos(rad);
    double sinA = sin(rad);
    int cx = width / 2;
    int cy = height / 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int nx = cosA * (x - cx) - sinA * (y - cy) + cx;
            int ny = sinA * (x - cx) + cosA * (y - cy) + cy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {

                dstPixels[y * width + x] = srcPixels[ny * width + nx];
            } else {

                dstPixels[y * width + x] = SDL_MapRGBA(rotated->format, 255, 255, 255, 255);
            }
        }
    }

    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(rotated);

    return rotated;
}

double rotate_auto_stylee (char* adresse, char* sauvegarde,SDL_Surface** imagedebase) {
    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface *image = IMG_Load(adresse);
    SDL_Surface *image2 = IMG_Load(adresse);

    sobelFilter(image, 128);
    double angle = houghTransform(image);
    if(angle==0) angle=-1;

    SDL_Surface *rotated = rotateSurface(image2, angle+1);
    *imagedebase=rotateSurface(*imagedebase, angle+1);
    IMG_SavePNG(rotated, sauvegarde);

    


    SDL_FreeSurface(image);
    SDL_FreeSurface(rotated);

    IMG_Quit();
    SDL_Quit();

    return angle+1;
}