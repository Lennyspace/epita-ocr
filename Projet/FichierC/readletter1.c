#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "additionnal_functions.h"
#include "readletter1.h"
#include <unistd.h>
#include "point.h"


int est_pixel_rouge_ligne(SDL_Surface *surface, int x, int y) {
    Uint32 red_color = SDL_MapRGB(surface->format,255, 0, 0);
    
    Uint32 couleur_pixel = ((Uint32 *)surface->pixels)[y *surface->w + x];


    if (couleur_pixel != red_color) return 1;


    for (int i = 1; i<= 3; i++) {

        if (y + i >=surface->h) return 1; 

        Uint32 couleur_voisin = ((Uint32 *)surface->pixels)[(y+ i) * surface->w + x];

        if (couleur_voisin != red_color) return 1;
    }
    return 0;
}




int est_pixel_rouge_colonne(SDL_Surface *surface, int x, int y) {
    Uint32 red_color = SDL_MapRGB(surface->format, 255, 0, 0);

    
    Uint32 couleur_pixel = ((Uint32 *)surface->pixels)[y * surface->w+ x];

    if (couleur_pixel != red_color) return 1;



    
    for (int i = 1; i <= 3; i++) {
        if (x + i >= surface->w) return 1; 
        Uint32 couleur_voisin= ((Uint32 *)surface->pixels)[y * surface->w + (x + i)];

        if (couleur_voisin !=red_color) return 1;
    }
    return 0;
}






void count_lines_and_columns(SDL_Surface* image,int* l,int* c)
{
    
    
    Uint32* pixels = (Uint32*)image->pixels;

    int lines = 0;
    int y =0;

    do{
        int x=0;
        int verif =0;
        while(x<image->w &&verif==0)
        {
            Uint8 r,g,b;
            SDL_GetRGB(pixels[y*image->w+x],image->format,&r,&g,&b);

            if (r!=255|| b!=0 || g!=0)
            {
                verif =1;
            }

            x++;
        }

        x++;

        while(x<image->w)
        {
            Uint8 r,g,b;
            SDL_GetRGB(pixels[y*image->w+ x],image->format,&r,&g,&b);
            Uint8 r1,g1,b1;
            SDL_GetRGB(pixels[y*image->w+ x-1],image->format,&r1,&g1,&b1);

            if (r==255 && b==255 && g==255 &&r1==255 && b1==0 && g1==0)
            {
                lines++;
            }

            x++;
        }

        y++;

    }while(lines==0&& y<image->h);


    int columns = 0;
    int x=0;

    do{
        int y=0;
        int verif =0;

        while(y<image->h &&verif==0)
        {
            Uint8 r,g,b;
            SDL_GetRGB(pixels[y*image->w+x],image->format, &r,&g,&b);


            if (r!=255 || b!=0 || g!=0)
            {
                verif = 1;
            }


            y++;
        }


        y++;


        while(y<image->h)
        {
            Uint8 r,g,b;
            SDL_GetRGB(pixels[y*image->w+x],image->format, &r,&g,&b);

            Uint8 r1,g1,b1;
            SDL_GetRGB(pixels[(y-1)*image->w+x],image->format,&r1,&g1,&b1);

            if (r==255 && b==255 && g==255 && r1==255 && b1==0 && g1==0)
            {
                columns++;
            }

            y++;
        }


        x++;
    }while(columns==0&& x<image->w);

    lines+=1;
    *l = columns;
    *c = lines;
}



void collectredpixels(SDL_Surface *surface, int **final_line_out, int *final_line_weidth_out, 
                              int **colonne_finale_out, int *column_final_taille_out) {
    int red_line[surface->w];
    int red_line_taille = 0;
    int red_column[surface->h];
    int red_column_taille = 0;




    
    for (int x = 0; x < surface->w; x++) {
        if (est_pixel_rouge_ligne(surface, x, 0) == 0) {
            red_line[red_line_taille++] = x;
        }
    }



    
    for (int y = 0; y < surface->h;y++) {
        if (est_pixel_rouge_colonne(surface,0, y) == 0) {
            red_column[red_column_taille++] = y;
        }
    }


    
    int *ligne_finale = (int *)malloc(red_line_taille * sizeof(int));
    *final_line_out = ligne_finale;
    *final_line_weidth_out =0;

    for (int i = 0; i < red_line_taille; i++) {
        if (i == 0 || red_line[i] != red_line[i - 1]) {
            ligne_finale[(*final_line_weidth_out)++] =red_line[i];
        }
    }

    
    int *colonne_finale = (int *)malloc(red_column_taille * sizeof(int));
    *colonne_finale_out= colonne_finale;
    *column_final_taille_out = 0;

    for (int i = 0; i < red_column_taille; i++) {
        if (i == 0 || red_column[i] != red_column[i -1]) {
            colonne_finale[(*column_final_taille_out)++] =red_column[i];
        }
    }
}


int is_blackr(Uint32 pixel, SDL_PixelFormat *format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel, format, &r, &g, &b);

    return (r < 100 && g < 100 && b <100); 
}


SDL_Surface* decoupe(Uint32 *pixels, SDL_Surface *image, int x_min, int y_min, int x_max, int y_max, int i,int j,int* verif,int* nbcol,int* nbligne) {
    
    SDL_Surface *new_image = SDL_CreateRGBSurface(0, x_max - x_min, y_max - y_min, 32,
                                              0x00FF0000,
                                              0x0000FF00,
                                              0x000000FF,
                                              0x00000000);


    Uint32 *pixels2 = (Uint32 *)new_image->pixels;

    int count = 0;

    for (int x =x_min; x < x_max; x++) {

        for (int y = y_min; y <y_max; y++) {
            Uint32 current_pixel = pixels[y * image->w + x];

            
            
            if (is_black(current_pixel, image->format) == 1) { 
                count++; 
            }

            
            pixels2[(y - y_min) * new_image->w + (x - x_min)] = current_pixel;

        }
    }

    char filename[20];

    sprintf(filename, "Lettres/(%i,%i).png", j,i);

    if((new_image->w * 2 > new_image->h && new_image->h * 2 > new_image->w )){
        
        if ((double)count >= 0.03*(x_max - x_min) * (y_max - y_min)){

            Uint32* pixels = new_image->pixels;
            for (int y=0;y<new_image->h; y++)
            {
                for (int x=0;x<new_image->w; x++)
                {
                    Uint8 r,g,b;

                    SDL_GetRGB(pixels[y*new_image->w+x],new_image->format, &r,&g,&b);

                    if (!(r==0 && g==0 && b==0) && !(r==255 &&g==255 && b==255))
                    {
                        pixels[y*new_image->w+x] = SDL_MapRGB(new_image->format,255,255,255);
                    }
                }
            }

            if ( 0) {
                printf("Eror\n");
            }

            else
            {

                
                if(j>*nbligne) *nbligne=j;

                if(i>*nbcol) *nbcol=i;

                *verif= 1;
                return new_image;

            }
        }
    }
    return NULL;
}



SDL_Surface** readlettermain(SDL_Surface* image,int* nbcol,int* nbligne,struct Point*** coordonateimage) {

    Uint32 *pixels = (Uint32 *)image->pixels;

    
    int lines,columns;


    count_lines_and_columns(image,&lines,&columns);
    
    int index = 0;

    int *ligne_finale =NULL;
    int ligne_finale_taille= 0;
    int *colonne_finale = NULL;
    int colonne_finale_taille =0;

    collectredpixels(image, &ligne_finale, &ligne_finale_taille, &colonne_finale, &colonne_finale_taille);


    *nbcol=0;
    *nbligne=0;


    
    SDL_Surface** tabres=malloc(sizeof(SDL_Surface*)*columns*lines*2);

    struct Point** coordonnee=malloc(sizeof(struct Point*)*columns*lines*2);

    int l =0;

    int conteurimage=0;


    int col=0;
    int maxligne=0;
    int ligne=0;
    int isgood=0;




    for (int i = 0; i < ligne_finale_taille-1;i++) {

        int c = 0;

        if(ligne>maxligne) maxligne=ligne;

        ligne=0;

        for (int j = 0; j < colonne_finale_taille-1; j++) {
            int v =0;
            SDL_Surface* reslet=decoupe(pixels,image, ligne_finale[i], colonne_finale[j], ligne_finale[i + 1], colonne_finale[j + 1],index/lines,index%lines,&v,nbcol,nbligne);


            if(reslet){
                tabres[conteurimage]=reslet;
                
                ligne++;
                isgood= 1;


                struct Point* coordcase=malloc(sizeof(struct Point));

                coordcase->x= (ligne_finale[i]+ ligne_finale[i + 1])/2;
                coordcase->y= (colonne_finale[j]+ colonne_finale[j + 1])/2;
                coordonnee[conteurimage]=coordcase;
                conteurimage++;

            }
            
            
            c++;
            if (v==1) index++;

            
        }


        l++;
        if(isgood){
            col++;
        }
        isgood=0;

    }


    *nbcol=col;
    *nbligne=maxligne;
    *coordonateimage=coordonnee;
    //printf("testfin\n");
    for(int i = 0; i < image->w;i++)
    {
        for(int j = 0; j <image->h;j++)
        {


            Uint8 r, g, b;
            SDL_GetRGB(pixels[j *image->w + i], image->format, &r, &g, &b);
        
        }
    }

    
    free(ligne_finale);
    free(colonne_finale);
    
    
    return tabres;
}