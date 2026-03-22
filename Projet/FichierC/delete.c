#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

void calculate_coordonates(SDL_Surface*  image,  int* x_min, int*  x_max, int* y_min, int* y_max) {
    Uint32* pixels = (Uint32*)image->pixels;
    *x_min = image->w;
    *x_max= 0;
    *y_min =image->h;
    *y_max= 0;
    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[y * image->w + x], image->format, &r, &g, &b);
            if (r == 0 && g == 0 && b == 0) {
                if (x < *x_min) *x_min = x;
                if (x > *x_max) *x_max = x;
                if (y < *y_min) *y_min = y;
                if (y > *y_max) *y_max = y;
            }
        }
    }
}


SDL_Surface* create_new_surface(SDL_Surface* image, int y_min, int x_min, int y_max, int x_max) {
    int width = x_max - x_min + 1;
    int height = y_max - y_min + 1;

    SDL_Surface* new_image = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);

    Uint32* src_pixels = (Uint32*)image->pixels;
    Uint32* dest_pixels = (Uint32*)new_image->pixels;
 
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint32 pixel = src_pixels[(y + y_min) * image->w + (x + x_min)];
            dest_pixels[y * new_image->w + x] = pixel;
        }
    }
    return new_image;
}


void centralize_letter(const char* imagePath) {



    SDL_Surface* image = IMG_Load(imagePath); 


    SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(image); 

    int y_min, y_max, x_min, x_max;
    calculate_coordonates(formattedImage, &x_min, &x_max, &y_min, &y_max);

    if (x_min >= x_max || y_min >= y_max) {
        
    } else {
        x_min = (x_min > 2) ? x_min - 2 : 0;
        x_max = (x_max + 2 < formattedImage->w) ? x_max + 2 : formattedImage->w - 1;
        y_min = (y_min > 2) ? y_min - 2 : 0;
        y_max = (y_max + 2 < formattedImage->h) ? y_max + 2 : formattedImage->h - 1;
        SDL_Surface* new_image = create_new_surface(formattedImage, y_min, x_min, y_max, x_max);
        if (new_image) {
            IMG_SavePNG(new_image, imagePath);
            SDL_FreeSurface(new_image);
        }
    }

    SDL_FreeSurface(formattedImage);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("wwwwwwwww\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        centralize_letter(argv[i]);
    }

    return 0;
}
