#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "additionnal_functions.h"
#include "net.h"
#include "to_black_and_white.h"

int is_blacktb(Uint32 pixel, SDL_PixelFormat* format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel,format, &r, &g, &b);
    Uint8 gray = (Uint8)(0.299 *r + 0.587 * g + 0.114  * b);

    return gray < (Uint8)(255 / 1.715);
}




float count_black_pixelstb(SDL_Surface* image) {
    int black_pixel_count =0;
    Uint32* pixels = (Uint32*)image->pixels;


    for (int y =0; y < image->h; y++) {
        for (int x = 0 ; x < image->w; x++) {

            Uint32 pixel = pixels[y *image->w + x];


            if (is_blacktb(pixel, image->format)) {
                black_pixel_count++;
            }
        }
    }

    
    return (float)black_pixel_count /(image->w * image->h);
}




SDL_Surface* modifierImage(const char* imagePath) {

    SDL_Surface* image = IMG_Load(imagePath); 

    int isolatepixel =  countIsolatedBlackPixels(image);
    int tint=210;


    if (isolatepixel > 0 && isolatepixel<10)
    {
        tint=255/1.715;
    }

    SDL_Surface* formattedImage =SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(image); 
    image = formattedImage;


    for (int y = 0; y <image->h; y++) {
        for (int x = 0; x< image->w; x++) {


            Uint32* pixels =(Uint32*)image->pixels;
            Uint32 pixel = pixels[y * image->w + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format,&r, &g, &b);
            Uint8 gray = (Uint8)(0.299 * r + 0.587 * g +0.114 * b);

            if (gray>=tint)
            {
                gray= 255;
            }

            else{
                gray=0;
            }
        
            pixels[y * image->w +x] = SDL_MapRGB(image->format, gray, gray, gray);
        }
    }

    return image ;
}





void adjustContrast(SDL_Surface* image, double contrast) {
    double factor = (259 * (contrast + 255)) / (255 * (259 - contrast));


    SDL_LockSurface(image);
    for (int y = 0; y < image->h; y++) {
        Uint32* pixels = (Uint32*)((Uint8*)image->pixels + y * image->pitch);
        for (int x = 0; x < image->w; x++) {
            Uint32 pixel = pixels[x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);



            r = SDL_clamp(factor * (r - 128) + 128, 0, 255);
            g = SDL_clamp(factor * (g - 128) + 128, 0, 255);

            b = SDL_clamp(factor * (b - 128) + 128, 0, 255);

            pixels[x] = SDL_MapRGB(image->format, r, g, b);
        }
    }
    SDL_UnlockSurface(image);

}



void applyMeanFilter(SDL_Surface* image) {
    SDL_Surface* temp = SDL_ConvertSurface(image, image->format, 0);
    SDL_LockSurface(image);
    SDL_LockSurface(temp);



    for (int y = 1; y < image->h - 1; y++) {
        for (int x = 1; x < image->w - 1; x++) {
            int rSum = 0, gSum = 0, bSum = 0;



            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    Uint32 pixel = *((Uint32*)((Uint8*)temp->pixels + (y + ky) * temp->pitch) + (x + kx));
                    Uint8 r, g, b;

                    SDL_GetRGB(pixel, temp->format, &r, &g, &b);

                    rSum += r;
                    gSum += g;
                    bSum += b;
                }
            }


            Uint8 r = rSum / 9;
            Uint8 g = gSum /9;

            Uint8 b = bSum / 9;

            *((Uint32*)((Uint8*)image->pixels +y *image->pitch) + x) =
                SDL_MapRGB(image->format, r, g, b);

        }
    }

    SDL_UnlockSurface(image);
    SDL_UnlockSurface(temp);
    SDL_FreeSurface(temp);

}

void binarizeImage(SDL_Surface* image, int threshold) {
    SDL_LockSurface(image);

    for (int y = 0; y < image->h; y++) {

        Uint32* pixels = (Uint32*)((Uint8*)image->pixels + y * image->pitch);
        for (int x = 0; x < image->w; x++) {
            Uint32 pixel =pixels[x];

            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format,&r, &g, &b);

            Uint8 gray = (Uint8)(0.299 * r + 0.587 *g + 0.114 * b);
            Uint8 bin = (gray > threshold) ? 255 : 0;

            pixels[x] = SDL_MapRGB(image->format, bin, bin, bin);
        }

    }



    SDL_UnlockSurface(image);
}

void enhanceImage(SDL_Surface* image) {
    
    adjustContrast(image, 50.0);

    
    applyMeanFilter(image);

    binarizeImage(image, 170);
}



SDL_Surface* toblackmot(SDL_Surface* image) {
    IMG_SavePNG(image,"couleur.png");


    int blackPixelCount = 0;


    for (int y = 0; y < image->h; y++) {
        Uint32* pixels = (Uint32*)((Uint8*)image->pixels + y *image->pitch);

        for (int x = 0; x < image->w; x++) {

            Uint32 pixel = pixels[x];

            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format,&r, &g, &b);

            

            if (r > 200 && g < 50 && b <50) {

                
                pixels[x] = SDL_MapRGB(image->format, 255, 255, 255);
            } else {

                
                Uint8 gray = (Uint8)(0.299 * r +0.587 * g + 0.114 * b);
                if (gray >= 170) {
                    gray = 255;
                    
                } 

                else{

                    gray = 0;
                    blackPixelCount++;
                }
                pixels[x] =SDL_MapRGB(image->format, gray,gray, gray);

                
            }
        }
    }

    if( blackPixelCount< 1000)
    {



        
        SDL_Surface* imagec= modifierImage("couleur.png");


        removeSparseBlackPixelsmot(imagec);
        enhanceImage(imagec);

        Uint32 black = SDL_MapRGB(imagec->format, 0, 0, 0);
        Uint32 white = SDL_MapRGB(imagec->format, 255, 255, 255);

        int width = imagec->w;
        int height = imagec->h;

        

        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width;x++) {
                
                Uint32 *pixels = (Uint32 *)image->pixels;
                Uint32 pixelColor = pixels[(y * image->w)+ x];

                
                if (pixelColor == black) {

                    pixels[(y * imagec->w) + x] = white;
                }
            }
        }

        removeSparseBlackPixels(imagec);
        netmot(imagec);
        return imagec;

    }


    netmot(image);   
    
    return image;
}



SDL_Surface* toblack(char* adresse) {


    SDL_Surface* sourceImage = modifierImage(adresse);

    
    
    SDL_Surface* res=net(sourceImage);



    return res;
}