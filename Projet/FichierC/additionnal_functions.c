#include <stdio.h>
#include <stdlib.h>
#include "additionnal_functions.h"



void draw_line(SDL_Surface*  image, int x_min, int x_max,int y)
{
    Uint32* pixels = (Uint32*) image->pixels;
    while (x_min<x_max)
    {
        pixels[y *image->w +x_min] = SDL_MapRGB(image->format, 255,0, 0);
        x_min++;
    }
}

void draw_rows (SDL_Surface* image,int y_min,int y_max, int x)
{
    Uint32* pixels =(Uint32*)image->pixels;
    while (y_min<y_max)
    {
        pixels[y_min* image->w + x] = SDL_MapRGB(image->format, 255, 0,0);
        y_min++;
    }
}

SDL_Surface* create_new_surface(SDL_Surface* image, int x1 , int  y1, int x2 ,int y2) {
    int width = x2 -x1;
    int height = y2 -y1;
    SDL_Surface* new_image = SDL_CreateRGBSurface(0, width, height, 32,
                                                  0xFF000000,
                                                  0x00FF0000,
                                                  0x0000FF00,
                                                  0x000000FF);
    SDL_Surface* formattedImage = SDL_ConvertSurfaceFormat(new_image, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(new_image);
    new_image = formattedImage;

    Uint32* pixels1 = (Uint32*)image->pixels;
    Uint32* pixels2 = (Uint32*)new_image->pixels;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint8 r, g, b;
            Uint32 current_pixel = pixels1[(y + y1) * image->w + (x + x1)];
            SDL_GetRGB(current_pixel, image->format, &r, &g, &b);
            pixels2[y * new_image->w + x] = SDL_MapRGB(new_image->format, r, g, b);
        }
    }
    draw_line(new_image, 0, width, height - 1);
    draw_rows(new_image, 0, height, width - 1);
    return new_image;
}

SDL_Surface* delete_square (SDL_Surface* image, int x_min,int y_min,int x_max,int y_max) {
    Uint32* pixels=(Uint32*)image->pixels;

    x_min =x_min < 0 ? 0 :  x_min;
    y_min =y_min < 0 ? 0 :  y_min;
    x_max= x_max >= image->w  ? image->w - 1 : x_max;
    y_max= y_max >= image->h  ? image->h - 1 : y_max;

    for (int y = y_min - 3; y <= y_max + 3 && y < image->h; y++) {
        if (y < 0){
            continue;
        } 
        for (int x = x_min - 3; x <= x_max + 3 && x < image->w; x++) {
            if (x < 0) 
            {
                continue;
            }
            pixels[y *image->w + x] =SDL_MapRGB(image->format,255,255,255);
        }
    }
    return image;
}

int isblack(Uint32 pixel, SDL_PixelFormat *format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    return (r == 0);
}

int calculate_average_black_distance_from_left(SDL_Surface *image) {
    int total_distance=0, lines_with_black = 0;
    for (int y = 0; y<image->h; y++) {
        Uint32 *pixels = (Uint32 *)((Uint8 *)image->pixels + y * image->pitch);
        int found_black=0;
        for (int x=0; x<image->w; x++) {
            if (isblack(pixels[x], image->format)) {
                total_distance+=x; lines_with_black++;
                found_black=1; break;
            }
        }
        if (!found_black) continue;
    }
    if (lines_with_black>0)
        return total_distance/lines_with_black;
    else return -1;
}





int calculate_average_black_distance_from_top(SDL_Surface *image) {
    int total_distance=0, lines_with_black = 0;
    for (int x=0; x<image->w; x++) {
        Uint32 *pixels = (Uint32 *)image->pixels;
        int found_black=0;
        for (int y=0; y<image->h; y++) {
            if (isblack(pixels[y*image->w+x], image->format)) {
                total_distance+=y; lines_with_black++;
                found_black=1; break;
            }
        }
        if (!found_black) continue;
    }
    if (lines_with_black>0)
        return total_distance/lines_with_black;
    else return -1;
}

int calculate_average_black_distance_from_bottom(SDL_Surface *image) {
    int total_distance=0, lines_with_black=0;
    for (int x=0; x<image->w; x++) {
        Uint32 *pixels = (Uint32 *)image->pixels;
        int found_black=0;
        for (int y=image->h-1; y>=0; y--) {
            if (isblack(pixels[y*image->w+x], image->format)) {
                total_distance+=(image->h-1-y); lines_with_black++;
                found_black=1; break;
            }
        }
        if (!found_black) continue;
    }
    if (lines_with_black>0) return total_distance/lines_with_black;
    return 1;
}


int distance_frequent_and_close_from_left(SDL_Surface *surface, int average_distance) {

    int max_distance=surface->w;
    int *distance_count=calloc(max_distance, sizeof(int));


    for (int y=0; y<surface->h; y++) {
        Uint32 *pixels = (Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch);
        int distance=0, found_black=0;

        for (int x=0; x<surface->w; x++) {
            if (isblack(pixels[x], surface->format)) {
                distance=x;
                found_black=1;
                break;
            }
        }

        if (found_black) {
            for (int i=-3; i<=3; i++) {
                int adjusted_distance=distance+i;
                if (adjusted_distance >= 0 && adjusted_distance<max_distance) {
                    distance_count[adjusted_distance]++;
                }
            }
        }
    }

    int best_distance=-1, best_proximity=max_distance;
    for (int i=0; i<max_distance; i++) {
        if (distance_count[i]>0) {
            int proximity=abs(i-average_distance);
            if (proximity<best_proximity) {
                best_proximity=proximity;
                best_distance=i;
            }
        }
    }

    free(distance_count);
    return best_distance;
}


int distance_frequent_and_close_from_top(SDL_Surface *surface) {

    int max_distance = surface->h;
    int *distance_count = calloc(max_distance, sizeof(int));


    for (int x=0; x<surface->w; x++) {
        Uint32 *pixels = (Uint32 *)surface->pixels;
        int distance=0, found_black=0;

        for (int y=0; y<surface->h; y++) {
            if (isblack(pixels[y*surface->w+x], surface->format)) {
                distance=y;
                found_black=1;
                break;
            }
        }

        if (found_black) {
            for (int i=-3; i<=3; i++) {
                int adjusted_distance=distance+i;
                if (adjusted_distance>=0 && adjusted_distance<max_distance) {
                    distance_count[adjusted_distance]++;
                }
            }
        }
    }

    int best_distance=-1, best_frequency=0;
    for (int i=0; i<max_distance; i++) {
        if (distance_count[i]>best_frequency) {
            best_frequency=distance_count[i];
            best_distance=i;
        }
    }

    free(distance_count);
    return best_distance;
}


int distance_frequent_and_close_from_right(SDL_Surface *surface) {

    int max_distance=surface->w;
    int *distance_count=calloc(max_distance, sizeof(int));

    for (int y=0; y<surface->h; y++) {
        Uint32 *pixels = (Uint32 *)((Uint8 *)surface->pixels + (y*surface->pitch));
        int distance=0, found_black=0;

        for (int x=surface->w-1; x>=0; x--) {
            if (isblack(pixels[x], surface->format)) {
                distance=surface->w-1-x;
                found_black=1;
                break;
            }
        }

        if (found_black) {
            for (int i=-3; i<=3; i++) {
                int adjusted_distance=distance+i;
                if (adjusted_distance>=0 && adjusted_distance<max_distance) {
                    distance_count[adjusted_distance]++;
                }
            }
        }
    }

    int best_distance=-1, best_frequency=0;
    for (int i=max_distance; i>0; i--) {
        if (distance_count[i]>best_frequency) {
            best_frequency=distance_count[i];
            best_distance=i;
        }
    }

    free(distance_count);
    return max_distance-best_distance;
}


int distance_frequent_and_close_from_bottom(SDL_Surface *surface) {

    int max_distance=surface->h;
    int *distance_count=calloc(max_distance, sizeof(int));

    for (int x=0; x<surface->w; x++) {
        Uint32 *pixels = (Uint32 *)surface->pixels;
        int distance=0, found_black=0;

        for (int y=surface->h-1; y>=0; y--) {
            if (isblack(pixels[y*surface->w+x], surface->format)) {
                distance=surface->h-1-y;
                found_black=1;
                break;
            }
        }

        if (found_black) {
            for (int i=-3; i<=3; i++) {
                int adjusted_distance=distance+i;
                if (adjusted_distance>=0 && adjusted_distance<max_distance) {
                    distance_count[adjusted_distance]++;
                }
            }
        }
    }

    int best_distance=-1, best_frequency=0;
    for (int i=max_distance; i>0; i--) {
        if (distance_count[i]>best_frequency) {
            best_frequency=distance_count[i];
            best_distance=i;
        }
    }

    free(distance_count);
    return max_distance-best_distance;
}


int decremente_x_min(int x, int y_min, int y_max, SDL_Surface *image) {
    if (x <= 0) return x;

    int count = 0;
    Uint32 *pixels = (Uint32 *)image->pixels;

    if (y_min<0) y_min=0;
    if (y_max>=image->h) y_max=image->h-1;

    for (int y=y_min; y<=y_max; y++) {
        if (isblack(pixels[y*image->w+x], image->format)) 
            count++;
    }

    double percentage = (double)count / (y_max - y_min + 1);
    if (percentage >= 0.01) {
        x--;
        x = decremente_x_min(x, y_min, y_max, image);
    }

    else {
        x -= 2;
        if (x<0) return 0;
    }

    return x;
}


int incremente_x_max(int x, int y_min, int y_max, SDL_Surface *image) {
    if (x >= image->w - 1) return x;

    int count=0;
    Uint32 *pixels = (Uint32 *)image->pixels;

    if (y_min<0) y_min=0;
    if (y_max>=image->h) y_max=image->h-1;

    for (int y=y_min; y<=y_max; y++) {
        if (isblack(pixels[y*image->w+x], image->format)) 
            count++;
    }
    double percentage = (double)count / (y_max - y_min + 1);
    if (percentage >= 0.01) {
        x++;
        x = incremente_x_max(x, y_min, y_max, image);
    }
    else {
        x+=2;
        if (x>=image->w) return image->w-1;
    }
    return x;
}


int decremente_y_min(int y, int x_min, int x_max, SDL_Surface *image) {
    if (y <= 0) return y;

    int count=0;
    Uint32 *pixels = (Uint32 *)image->pixels;

    if (x_min<0) x_min=0;
    if (x_max>=image->w) x_max=image->w-1;

    for (int x=x_min; x<=x_max; x++) {
        if (isblack(pixels[y*image->w+x], image->format)) 
            count++;
    }
    double percentage = (double)count / (x_max - x_min + 1);
    if (percentage >= 0.01) {
        y--;
        y = decremente_y_min(y, x_min, x_max, image);
    }
    else {
        y-=2;
        if (y<0) return 0;
    }

    return y;
}



int incremente_y_max(int y, int x_min, int x_max, SDL_Surface *image) {
    if (y >= image->h - 1) return y;

    int count = 0;
    Uint32 *pixels = (Uint32 *)image->pixels;

    if (x_min < 0) x_min = 0;
    if (x_max >= image->w) x_max = image->w - 1;

    for (int x = x_min; x <= x_max; x++) {
        if (isblack(pixels[y * image->w + x], image->format)) 
            count++;
    }


    double percentage = (double)count / (x_max - x_min + 1);
    if (percentage >= 0.01) {
        y++;
        y = incremente_y_max(y, x_min, x_max, image);
    } else {
        y+=2;

        if (y >= image->h) return image->h - 1;
    }

    return y;
}



int first_black_pixel_from_left(SDL_Surface *image) {
    int x_min = image->w;
    Uint32 *pixels = (Uint32 *)image->pixels;
    
    for (int x=0; x<image->w; x++) {
        for (int y=0; y<image->h; y++) {
            if (isblack(pixels[y*image->w + x], image->format)) {
                if (x_min>x) {
                    x_min = x;
                }
                break;
            }
        }
    }
    return x_min;
}


int first_black_pixel_from_right(SDL_Surface *image) {
    int x_max = -1;
    Uint32 *pixels = (Uint32 *)image->pixels;
    
    for (int x = image->w - 1; x >= 0; x--) {
        for (int y = 0; y < image->h; y++) {
            if (isblack(pixels[y * image->w + x], image->format)) {
                if (x_max < x) {
                    x_max = x;
                }
                break;
            }
        }
    }
    return x_max;
}


int first_black_pixel_from_top(SDL_Surface *image){
    int y_min =image->h;
    Uint32 *pixels =(Uint32 *)image->pixels;

    for (int y=0; y< image->h ; y++) {
        for(int x= 0;x<image->w;x++) {
            if(isblack(pixels[ y*image->w + x], image->format)) {
                if( y <y_min){
                    y_min =y;
                }
                break ;
            }
        }
    }
    return y_min ;
}



int first_black_pixel_from_bottom(SDL_Surface* image){
    int y_max =-1;
    Uint32* pixels=(Uint32 *)image->pixels;

    for(int y=image->h-1;y>=0;y--) {
        for(int x=0; x<image->w;x++) {
            if(isblack(pixels[y* image->w+x],image->format)) {
                if(y_max < y){
                    y_max = y;
                }
                break ;
            }
        }
    }
    return y_max ;
}


struct Line {
    int x_start;
    int y_start;
    int x_end;
    int y_end;
};


struct Line* detect_borders(SDL_Surface* image,int* line_count){
    int width=image->w;
    int height = image->h;

    struct Line *lines=malloc(MAX_LINES*sizeof(struct Line));
    *line_count=0;

    const float threshold_ratio=1.0 /3.0;
    int threshold_length=(int)(width*threshold_ratio);

    for(int y=0; y<height;y++) {
        int start=-1;
        int length = 0;

        for (int x=0;x<width;x++) {
            Uint32 pixel=get_pixel(image,x,y);
            Uint8 r,g,b;
            SDL_GetRGB(pixel,image->format,&r,&g,&b);

            if(r<100&&g<100&&b<100) {
                if(start==-1)start=x;
                length++;
            } else {
                if(length>=threshold_length&&*line_count<MAX_LINES){
                    lines[*line_count].x_start=start;
                    lines[*line_count].y_start=y;
                    lines[*line_count].x_end=start+length-1;
                    lines[*line_count].y_end=y;
                    (*line_count)++;
                }
                start=-1;
                length=0;
            }
        }

        if(length>=threshold_length&&*line_count<MAX_LINES){
            lines[*line_count].x_start=start;
            lines[*line_count].y_start=y;
            lines[*line_count].x_end=start+length-1;
            lines[*line_count].y_end=y;
            (*line_count)++;
        }
    }

    for(int x=0;x<width;x++) {
        int start = -1;
        int length=0;

        for(int y=0;y<height;y++) {
            Uint32 pixel =get_pixel(image,x,y);
            Uint8 r,g,b;
            SDL_GetRGB(pixel,image->format,&r,&g,&b);

            if(r<100 && g<100 && b<100) {
                if(start==-1)start=y;
                length++;
            } else {
                if(length>=threshold_length&&*line_count<MAX_LINES){
                    lines[*line_count].x_start=x;
                    lines[*line_count].y_start=start;
                    lines[*line_count].x_end=x;
                    lines[*line_count].y_end=start+length-1;
                    (*line_count)++;
                }
                start=-1;
                length=0;
            }
        }

        if(length>=threshold_length&&*line_count<MAX_LINES){
            lines[*line_count].x_start=x;
            lines[*line_count].y_start=start;
            lines[*line_count].x_end=x;
            lines[*line_count].y_end=start+length-1;
            (*line_count)++;
        }
    }

    return lines;
}



int count_black_pixels(SDL_Surface*image, int x_min,int x_max, int y, float pourcentage)
{
    Uint32*pixels=(Uint32*)image->pixels;
    int count=0;
    int i = x_min;
    while(i<x_max)
    {
        Uint32 pixel =pixels[y*image->w+i];
        Uint8 r,g,b;
        SDL_GetRGB(pixel,image->format,&r,&g,&b);
        if(r==0)
        {
            count++;
        }
        i++;
    }
    
    if(count <=(int)(pourcentage*(x_max -x_min)))
    {
        return 0;
    }
    return 1;
}


int count_black_pixels_rows(SDL_Surface* image,int y_min, int y_max, int x, float pourcentage)
{
    Uint32*pixels=(Uint32*)image->pixels;
    int count=0;
    int i=y_min;
    while(i<y_max)
    {
        Uint32 pixel = pixels[i*image->w+x];
        Uint8 r,g,b;
        SDL_GetRGB(pixel, image->format,&r, &g, &b);
        if(r==0)
        {
            count++;
        }
        i++;
    }
    if(count<=(int)(pourcentage*(y_max - y_min))||count>=(int)(0.97*(y_max-y_min)))
    {
        return 0;
    }
    return 1;
}


Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                Uint32 color = 0;
                color = (p[0] * 65536) + (p[1] * 256) + p[2];
                return color;
            } else {
                Uint32 color = 0;
                color = (p[2] * 65536) + (p[1] * 256) + p[0];
                return color;
            }
        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void first_black_pixel_from_each(SDL_Surface* image,int* x_min,int* y_min,int* x_max,int* y_max) {
    Uint32* pixels = (Uint32*)image->pixels;
    Uint8 r,g,b;
    int found_top=0,found_bottom=0,found_left=0,found_right=0;

    
    for(int y=0;y<image->h&&!found_top;y++) {
        for(int x=0;x<image->w;x++) {
            SDL_GetRGB(pixels[y*image->w+x],image->format,&r,&g,&b);
            if(r==0 && g==0 && b==0) {
                *y_min=y;
                found_top=1;
                break;
            }
        }
    }

    
    for(int y=image->h-1;y>=0 && !found_bottom;y--) {
        for(int x=0;x<image->w;x++) {
            SDL_GetRGB(pixels[y*image->w+x],image->format,&r,&g,&b);
            if(r==0 && g==0 && b==0) {
                *y_max=y;
                found_bottom=1;
                break;
            }
        }
    }

    
    for(int x=0;x<image->w && !found_left;x++) {
        for(int y=0;y<image->h;y++) {
            SDL_GetRGB(pixels[y*image->w+x],image->format,&r,&g,&b);
            if(r==0 && g==0 && b==0) {
                *x_min=x;
                found_left=1;
                break;
            }
        }
    }

    
    for(int x=image->w-1;x>=0 && !found_right ; x--) {
        for(int y=0;y<image->h;y++) {
            SDL_GetRGB(pixels[y*image->w+x],image->format,&r,&g,&b);
            if(r==0 && g==0 && b==0) {
                *x_max=x;
                found_right=1;
                break;
            }
        }
    }
}


void drawLineFinal(SDL_Surface* surface,int xd,int yd,int xa,int ya,int thickness,Uint32 color) {
    float dx=xa-xd;
    float dy=ya-yd;
    float length=sqrtf(dx*dx+dy*dy);
    if(length==0)return;

    float nx=dx/length;
    float ny=dy/length;

    Uint8 r,g,b,a;
    SDL_GetRGBA(color,surface->format,&r,&g,&b,&a);

    for(int i=0;i<=(int)length;i++) {
        int x=xd+(int)(nx*i);
        int y=yd+(int)(ny*i);
        for(int tx=-thickness/2;tx<=thickness/2;tx++) {
            for(int ty=-thickness/2;ty<=thickness/2;ty++) {
                int drawX=x+tx;
                int drawY=y+ty;
                if(drawX>=0 && drawX<surface->w && drawY>=0 && drawY<surface->h) {
                    Uint32* pixels=(Uint32*)surface->pixels;
                    Uint32* targetPixel=&pixels[drawY*surface->w+drawX];
                    
                    Uint8 tr,tg,tb,ta;
                    SDL_GetRGBA(*targetPixel,surface->format,&tr,&tg,&tb,&ta);

                    Uint8 finalR=(r*a+tr*(255-a))/255;
                    Uint8 finalG=(g*a+tg*(255-a))/255;
                    Uint8 finalB=(b*a+tb*(255-a))/255;

                    *targetPixel=SDL_MapRGBA(surface->format,finalR,finalG,finalB,ta);
                }
            }
        }
    }
}

