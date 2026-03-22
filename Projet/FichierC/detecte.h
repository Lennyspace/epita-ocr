#ifndef DETECTE_H
#define DETECTE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int count_black_pixels_rows(SDL_Surface* image,int y_min,int y_max,int x,float pourcentage);

int count_black_pixels(SDL_Surface* image,int x_min,int x_max,int y,float pourcentage);

void draw_line(SDL_Surface* image,int x_min,int x_max,int y);

struct Line* detect_borders(SDL_Surface *image, int *line_count);

int is_black(Uint32 pixel, SDL_PixelFormat *format);

int is_present(struct Line line,struct Line *lines,int size);

SDL_Surface* create(SDL_Surface* image,int* x_minf,int* x_maxf,int* y_minf,int* y_maxf);

void delete_lines(SDL_Surface* image);

int Abs(int x);

Uint32 get_pixel(SDL_Surface *surface, int x, int y);

void draw_rows(SDL_Surface* image,int y_min,int y_max,int x);

SDL_Surface** detectgridandword(SDL_Surface* imageblack,int* x,int* y);
#endif