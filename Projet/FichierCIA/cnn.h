#ifndef cnn_h
#define cnn_h

#include <SDL2/SDL.h>

float* normaliserImage(SDL_Surface* image);
float sigmoidf(float x);
float* filtrerImage5x5(float* pixels, int width, int height, float filtre[5][5], float biais);
float* filtrerImage3x3(float* pixels, int width, int height, float filtre[3][3], float biais);
float* additionnerImages(float* img1, float* img2, int width, int height);
float* max_pooling_2x2(float* image, int width, int height);
void sauvegarderImage(float* pixels, const char* filename);
void CNN(SDL_Surface* image, float* finalTab);


#endif
