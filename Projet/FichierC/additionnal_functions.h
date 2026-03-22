#ifndef ADDITIONNAL_FUNCTIONS_H
#define ADDITIONNAL_FUNCTIONS_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int is_black(Uint32 pixel, SDL_PixelFormat *format);

SDL_Surface* create_new_surface(SDL_Surface* image, int x1, int y1, int x2, int y2);
SDL_Surface* delete_square(SDL_Surface* image, int x1, int y1, int x2, int y2);
int first_black_pixel_from_left(SDL_Surface *image);

int first_black_pixel_from_right(SDL_Surface *image);

int first_black_pixel_from_top(SDL_Surface *image);

int first_black_pixel_from_bottom(SDL_Surface *image);

int calculate_average_black_distance_from_left(SDL_Surface *image);
int calculate_average_black_distance_from_right(SDL_Surface *image);
int calculate_average_black_distance_from_top(SDL_Surface *image);

int calculate_average_black_distance_from_bottom(SDL_Surface *image);

int distance_frequent_and_close_from_left(SDL_Surface *surface,int average_distance);

int distance_frequent_and_close_from_top(SDL_Surface *surface);

int distance_frequent_and_close_from_right(SDL_Surface *surface);
int distance_frequent_and_close_from_bottom(SDL_Surface *surface);
int decremente_x_min(int x, int y_min,int y_max, SDL_Surface *image);

int incremente_x_max(int x, int y_min,int y_max, SDL_Surface *image);

int decremente_y_min(int y,int x_min, int x_max, SDL_Surface *image);

int incremente_y_max(int y, int x_min, int x_max, SDL_Surface *image);

void draw_line(SDL_Surface* image, int x_min,int x_max,int y);

void draw_rows(SDL_Surface* image, int y_min,int y_max,int x);

struct Line* detect_borders(SDL_Surface *image, int *line_count);
     
struct Line;    
 
int count_black_pixels_rows(SDL_Surface* image,int y_min,int y_max,int x,float pourcentage );

int count_black_pixels(SDL_Surface* image ,int x_min,int x_max,int y,float pourcentage);

Uint32 get_pixel(SDL_Surface *surface, int x , int y);

void first_black_pixel_from_each(SDL_Surface* image  , int* x_min, int* y_min, int* x_max, int* y_max );

void drawLineFinal(SDL_Surface* surface, int xd, int yd , int xa , int ya , int thickness,  Uint32 color );




      
#define ADJACENT_PIXEL_COUNT 9

#define MAX_LINES 1000
 
#endif