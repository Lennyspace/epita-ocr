#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "additionnal_functions.h"
#include "detecte.h"

#define MAX_LINES 1000

struct Line {
    int x_start;
    int y_start;
    int x_end;

    int y_end;

};





int Abs(int x)
{
    if (x<0)
    {
        return -x;
    }
    return x;
}

int is_present(struct Line line,struct Line *lines,int size)
{
    int present = 0;
    int i = 0;
    while (i<size && present==0)
    {
        if (Abs(lines[i].y_start-line.y_start)>1||Abs(lines[i].y_end-line.y_start)>1||Abs(lines[i].y_start-line.y_end)>1||Abs(lines[i].y_end-line.y_end)>1)
        {
            present++;
        }
        else if (Abs(lines[i].x_start-line.x_start)>1||Abs(lines[i].x_end-line.x_start)>1||Abs(lines[i].x_start-line.x_end)>1||Abs(lines[i].x_end-line.x_end)>1)
        {
            present++;
        }
        i++;
    }
    return present;
}

int is_black(Uint32 pixel, SDL_PixelFormat *format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel,format,&r,&g,&b);
    return (r == 0);
}


void delete_lines(SDL_Surface* image)
{
    int line_count;

    struct Line* lines = detect_borders(image,&line_count);
    Uint32* pixels = (Uint32*)image->pixels;
    for (int i=0;i<line_count;i++)
    {
        struct Line line = lines[i];
        if (line.x_start==line.x_end)
        {
            for (int y=line.y_start;y<line.y_end;y++)
            {
                for (int j=-3;j<=3;j++)
                {

                    Uint32 pixel = pixels[y * image->w + line.x_start + j];
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, image->format, &r, &g, &b);

                    if (r==0 && g==0 && b==0)
                    {
                        pixels[y * image->w + line.x_start + j] = SDL_MapRGB(image->format, 255,255, 255);
                    }
                }
            }
        }
        else if (line.y_start==line.y_end)
        {
            for (int x=line.x_start;x<line.x_end;x++)
            {

                for (int j=-3;j<=3;j++)
                {
                    Uint32 pixel =pixels[(line.y_start+j) * image->w+ x];


                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, image->format, &r, &g, &b);
                    if (r==0 && g==0  &&  b==0)
                    {
                        pixels[(line.y_start+j) * image->w + x] = SDL_MapRGB(image->format, 255,255, 255);
                    }
                }
            }
        }
    }
}


SDL_Surface* create(SDL_Surface* image,int* x_minf,int* x_maxf,int* y_minf,int* y_maxf)
{

    SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0);

    SDL_FreeSurface(image); 
    image = formattedImage;

    int line_count = 0;
    struct Line *lines = detect_borders(image, &line_count);
    delete_lines(image);
    if (line_count!=0)    
    {       
        int x_min = lines[0].x_start;    
        int x_max = lines[0].x_end;    
        int y_min = lines[0].y_start;
        int y_max = lines[0].y_end;
        
        
        for (int i= 1;i<line_count;i++)
        {
            
            if (x_min>lines[i].x_start)
                x_min = lines[i].x_start;
            if (x_max<lines[i].x_end)
                x_max = lines[i].x_end;
            if (y_min>lines[i].y_start)
                y_min = lines[i].y_start;
            if (y_max<lines[i].y_end)
                y_max = lines[i].y_end;
        }

        *x_minf = x_min;
        *x_maxf = x_max;
        *y_minf = y_min;
        *y_maxf = y_max;
        for (int y=0;y<image->h;y++)
        {
            for (int x=0; x<image->w;x++)
            {
                Uint32* pixels = (Uint32*)image->pixels;
                if (((y==y_max||y==y_min)&&(x>=x_min&&x<=x_max)))
                {
                    pixels[y * image->w + x] = SDL_MapRGB(image->format, 255, 0, 0);
                    if (y-1>=0)
                        pixels[(y-1) * image->w + x] = SDL_MapRGB(image->format, 255, 0, 0);
                    if (y+1<image->h)
                        pixels[(y+1) * image->w + x] = SDL_MapRGB(image->format, 255, 0, 0);
                }
                else if (((x==x_max||x==x_min)&&(y>=y_min&&y<=y_max)))
                {
                    pixels[y * image->w + x] = SDL_MapRGB(image->format, 255, 0, 0);
                    if (x-1>=0)
                        pixels[y * image->w + x-1] = SDL_MapRGB(image->format, 255, 0, 0);
                    if (x+1<image->w)
                        pixels[y * image->w + x+1] = SDL_MapRGB(image->format, 255, 0, 0);
                }
                
            }
        }
        int count_lines = 0;
        float percentage = 0.17;
        for (int y=y_min;y<y_max;y++)
        {
            if (count_black_pixels(image,x_min,x_max,y,percentage)==0) //no letters
            {
                count_lines++;
                percentage = 0.17;
            } 
            else{
                if (count_lines!=0)
                {
                    count_lines/=2;
                    draw_line(image,x_min,x_max,y-count_lines);
                    count_lines=0;
                    percentage = 0.03;
                }
            }
            
        }
        int count_rows = 0;
        for (int x=x_min;x<x_max;x++)
        {
            if (count_black_pixels_rows(image,y_min,y_max,x,percentage)==0) //no letters
            {
                count_rows++;
                percentage = 0.2;
            } 
            else{
                if (count_rows!=0)
                {
                    count_rows/=2;
                    draw_rows(image,y_min,y_max,x-count_rows);
                    count_rows=0;
                    percentage = 0.04;
                }
            }
        }
        
        
    }
    else
    {
        int x_min = distance_frequent_and_close_from_left(image,calculate_average_black_distance_from_left(image));

        int x_max = distance_frequent_and_close_from_right(image);
        int y_max = distance_frequent_and_close_from_bottom(image);
        int y_min = distance_frequent_and_close_from_top(image);

        x_min =decremente_x_min(x_min,y_min,y_max,image);
        y_min =decremente_y_min(y_min,x_min,x_max,image);
        x_max =incremente_x_max(x_max,y_min,y_max,image);
        y_max =incremente_y_max(y_max,x_min,x_max,image);

        //printf("x_min : %i  \n,xmax : %i  \n,y_min : %i\n,  y_max : %i\n",x_min,x_max,y_min,y_max);
        if (x_max < x_min){
            int temp = x_max;
            x_max = x_min;
            x_min = temp;
        }
        draw_line(image,x_min,x_max,y_min);
        draw_line(image,x_min,x_max,y_max);
        draw_rows(image,y_min,y_max,x_min);
        draw_rows(image,y_min,y_max,x_max);
        int count_lines = 0;
        float percentage = 0.1;
        *x_minf = x_min;
        *x_maxf = x_max;
        *y_minf = y_min;
        *y_maxf = y_max;
        for (int y=0;y<y_max;y++)
        {
            if (count_black_pixels(image,x_min,x_max,y,percentage)==0) 
            {
                count_lines++; 
                percentage = 0.1;
            } 
            else{
                if (count_lines !=0)
                {

                    count_lines/=2;
                    draw_line(image,x_min,x_max,y-count_lines);
                    count_lines=0;
                    percentage = 0.04;
                }
            }
        }
        int count_rows = 0;
        for (int x=x_min;x<x_max;x++)
        {
            if (count_black_pixels_rows(image,y_min,y_max,x,percentage)==0) //pas de lettres
            {
                count_rows++;
                percentage = 0.1;
            } 
            else{
                if (count_rows!=0)
                {
                    count_rows/=2;
                    draw_rows(image,y_min,y_max,x-count_rows);
                    count_rows=0;
                    percentage = 0.04;
                }
            }
        }
        for (int y=0;y<image->h;y++)
        {
            for (int x=0;x<x_min;x++)
            {
                Uint32* pixels = (Uint32*) image->pixels;
                Uint32 pixel = pixels[y*image->w+x];
                Uint8 r,g,b;
                SDL_GetRGB(pixel,image->format,&r,&g,&b);
                if (r==255 && g==0 && b==0)
                {
                    pixels[y*image->w+x] = SDL_MapRGB(image->format,255,255,255);
                }
            }
        }
        
        int found = 0;
        Uint32 *pixels = (Uint32*)image->pixels;
        int x=0;
        while (x<x_max && found==0)
        {
            int good = 1;
            for (int y=0;y<y_max&&good==1;y++)
            {
                Uint8 r, g, b;
                SDL_GetRGB(pixels[y*image->w +x],image->format,&r,&g,&b);
                if (r==255 && g==0 && b==0)
                {
                    good = 0;
                }
            }
            if (good==1)
            {
                found = 1;
            }
            else
            {
                x++;
            }
        }
        if (found==1)
        {
            for (int y=0;y<y_max;y++)
            {
                for (int xi=0;xi<x;xi++)
                {
                    pixels[y*image->w+xi] = SDL_MapRGB(image->format,255,255,255);
                }
            }
        }
    }
    
    

    return image;
}


SDL_Surface** detectgridandword(SDL_Surface* imageblack,int* x,int* y)
{
    
    int x_min,x_max,y_min,y_max;
    SDL_Surface* image=create(imageblack, &x_min, &x_max, &y_min,&y_max);
    *x=x_min;
    *y=y_min;
    SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(image); 
    image = formattedImage;

    SDL_Surface* imagenvgrille=create_new_surface(image,x_min, y_min, x_max,y_max);

    SDL_Surface* imagemots=delete_square(image,x_min,y_min,x_max,y_max);

    SDL_Surface** resf =malloc(sizeof(SDL_Surface*)*2);
    resf[0]=imagenvgrille;
    resf[1]=imagemots;
    
    return resf;
}
