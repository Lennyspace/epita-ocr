#include "generateur_image.h"
#include "generateur.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>



void place_image(SDL_Surface* grid,char* ImagePath,int x,int y)
{
    SDL_Surface* cellule = IMG_Load( ImagePath);
    Uint32* grid_p = grid->pixels;
    Uint32* cell_p = cellule->pixels;
    for (int yi=0;yi<cellule->h;yi++)
    {


        for (int xi = 0;xi<cellule->w;xi++)
        {
            Uint32 pixel = cell_p[yi* cellule->h+xi];
            Uint8 r,g,b;
            SDL_GetRGB(pixel,cellule->format,&r,&g,&b);
            grid_p[(yi+y)*grid->w+xi+x] = SDL_MapRGB(grid->format, r,g,b);
        }
    }


}

void quadrillage(SDL_Surface* image, char** grid)
{
    Uint32* pixels = image->pixels;
    int comp = 0;


    
    for (int y=0;y<image->h;y++) {



        for (int x=0;x<image->w; x++)
        {
            if (x>=5 && y>=5 && x<441 && y<441)
            {
                if ((y-5)%29==0 && (x- 5)%29==0 && y<440 && x< 440)
                {
                    pixels[y*image->w+x] = SDL_MapRGB(image->format,0,0,0);
                    int i = comp/15;int j = comp%15;
                    char imagepath[100];


                    sprintf(imagepath,"generateur_grille/grid/%c.png",grid[i][j]);
                    place_image(image,imagepath,x+1,y+1);
                    comp++;

                }
                else if ((y-5)%29==0)
                {


                    pixels[y*image->w+x] = SDL_MapRGB(image->format,0,0,0);
                    
                }
                else if ((x-5)%29==0)
                {
                    pixels[y *image->w+x] = SDL_MapRGB(image->format,0,0,0);
                }
                
            }
            else
            {
                pixels[y*image->w+x] = SDL_MapRGB(image->format,255,255,255);
            }
            
        }
    }
}

void liste(SDL_Surface* image,char** words,int nb_lines)
{
    Uint32* pixels = image->pixels;
    for (int j=0;j<nb_lines;j++){
        char* word = words[j];
        int lgt = strlen(word);
        for (int i=0;i<lgt;i++){
            char imagepath[100];
            sprintf(imagepath,"generateur_grille/word/%c.png",word[i]);
            SDL_Surface* letter = IMG_Load(imagepath);
            for (int y=0;y<14;y++)
            {
                for (int x=0;x<14; x++)
                {
                    Uint32* pix = letter->pixels;
                    Uint32 pixel = pix[y*letter->w+x];
                    Uint8 r,g,b;
                    SDL_GetRGB(pixel,letter->format,&r,&g,&b);
                    pixels[(y+200+j*20)*image->w+x+450+i*15] = SDL_MapRGB(letter->format,r,g,b);
                }
            }


        }
    }
}

void fin_generation()
{

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, 600, 450,32,0,0,0,0);

    char** mots = NULL;
    char** grid = final_generate(&mots);
    
    quadrillage(surface,grid);

    liste(surface,mots,5);
    IMG_SavePNG(surface,"adresse_generer.png");
}