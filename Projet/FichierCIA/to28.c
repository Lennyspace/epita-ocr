#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include "to28.h"

void calculate_coordonates(SDL_Surface* image, int* x_min, int* x_max, int*y_min, int* y_max) {
    if (!image) return;


    Uint32* pixels = (Uint32*)image->pixels;
    *x_min = image->w;
    *x_max = 0;
    *y_min = image->h;
    *y_max = 0;

    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            Uint8 r, g, b;

            SDL_GetRGB(pixels[y *image->w + x], image->format, &r, &g, &b);
            if (r == 0 && g == 0 && b == 0) {
                if (x < *x_min) *x_min = x;
                if (x > *x_max) *x_max = x;
                if (y < *y_min) *y_min = y;
                if (y > *y_max) *y_max =y;
            }
        }
    }
}

SDL_Surface* create_new_surfaceto28(SDL_Surface* image, int y_min, int x_min, int y_max, int x_max) {
    int width = x_max -x_min + 1;
    int height =y_max -y_min +1;

    SDL_Surface* new_image = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);


    Uint32* src_pixels = (Uint32*)image->pixels;
    Uint32* dest_pixels = (Uint32*)new_image->pixels;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint32 pixel = src_pixels[(y + y_min) * image->w + (x+ x_min)];
            dest_pixels[y * new_image->w + x] = pixel;
        }
    }
    return new_image;
}

SDL_Surface* centralize_letter(SDL_Surface* image) {
    SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0);
    int y_min, y_max, x_min, x_max;
    calculate_coordonates(formattedImage, &x_min, &x_max, &y_min, &y_max);
    if (x_min > x_max || y_min > y_max) {
        return formattedImage;


    }

    SDL_Surface* new_image = create_new_surfaceto28(formattedImage, y_min, x_min, y_max, x_max);
    SDL_FreeSurface(formattedImage);
    if (new_image) {
        return new_image;
    } else {
        return image;


    }
}


SDL_Surface* resizeImageTo28x28(SDL_Surface* src) {
    SDL_Surface* convertedSrc = SDL_ConvertSurfaceFormat(src, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_Surface* resized = SDL_CreateRGBSurface(0, 28, 28, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    SDL_BlitScaled(convertedSrc, NULL, resized, NULL);
    return resized;
}

int to28centerized(SDL_Surface** letters,int nbletter) {

    for (int i = 0; i < nbletter; i++) {
        //printf("nadadaze\n");
        
        letters[i]=centralize_letter(letters[i]);

        letters[i]=resizeImageTo28x28(letters[i]);

    }
    return 1;
}