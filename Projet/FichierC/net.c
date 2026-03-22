#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <math.h>

#define BLACK 0x000000FF
#define WHITE 0xFFFFFFFF

typedef struct {
    int top;
    int bottom;
    int left;
    int right;
} Bounds;

int isBlack(Uint32 pixel, SDL_PixelFormat *format) {
    Uint8 r, g, b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    return (r == 0) ? 0 : 1;
}

void drawLine(SDL_Surface* surface, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {

        Uint32* pixels = (Uint32*)surface->pixels;
        Uint32 black = SDL_MapRGB(surface->format, 0, 0, 0);
        pixels[y0 * surface->w + x0] = black;

        if (x0 == x1 && y0 == y1) break;
        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void drawPathsBetweenAllBlackPixels(SDL_Surface* surface) {
    int width =surface->w;
    int height =surface->h;
    Uint32* pixels =(Uint32*)surface->pixels;
    Uint32 black =SDL_MapRGB(surface->format, 0, 0, 0);


    int blackPixels[width*height][2];
    int blackCount= 0;

 
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (pixels[y * width + x] == black) {
                blackPixels[blackCount][0] = x;
                blackPixels[blackCount][1] = y;
                blackCount++;
            }
        }
    }

 
    for (int i = 0;i < blackCount; ++i) {
        int x0 =blackPixels[i][0];
        int y0 =blackPixels[i][1];

        for (int j = 0; j <blackCount; ++j) {
            if (i != j) {
                int x1 =blackPixels[j][0];
                int y1= blackPixels[j][1];
   
                int distance= (int)sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));

                if (distance<= 4) {
                    drawLine(surface,x0,y0,x1,y1);
                }
            }
        }
    }
}

void processImage(const SDL_Surface* source, SDL_Surface* target,int nbnoirmax) {
    int width = source->w;
    int height = source->h;
    Uint32* srcPixels = (Uint32*)source->pixels;
    Uint32* tgtPixels = (Uint32*)target->pixels;
    Uint32 black = SDL_MapRGB(source->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(source->format, 255, 255, 255);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            tgtPixels[y * width + x] = srcPixels[y * width + x];
        }
    }
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (srcPixels[y * width + x] == black) {
                int blackCount = 0;

  
                for (int dy = -4; dy <= 4; ++dy) {
                    for (int dx = -4; dx <= 4; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;

                      
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            if (srcPixels[ny * width + nx] == black) {
                                blackCount++;
                            }
                        }
                    }
                }
                if (blackCount < nbnoirmax) {
                    for (int dy = -4; dy <= 4; ++dy) {
                        for (int dx = -4; dx <= 4; ++dx) {
                            int nx = x + dx;
                            int ny = y + dy;
                           
                            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                                if (srcPixels[ny * width + nx] == black) {
                                    tgtPixels[ny * width + nx] = white; 
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void redrawAdjacentPixels(SDL_Surface* surface) {
    int width = surface->w;
    int height = surface->h;
    Uint32* pixels = (Uint32*) surface->pixels;
    Uint32 black = SDL_MapRGB(surface->format, 0, 0, 0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (pixels[y * width + x] ==black) {

                for (int dy = -2; dy <= 0;++dy) {
                    for (int dx = -2; dx <= 2;++dx) {

                        if ((dx == -2 && dy == -2) || (dx ==-1 && dy ==-1) ||
                            (dx == 0 && dy == -2) || (dx ==1 && dy ==-1)) {
                            int nx = x + dx;
                            int ny = y + dy;


                            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                                pixels[ny * width + nx] = black;
                            }
                        }
                    }
                }
            }
        }
    }
}

int countIsolatedBlackPixels(SDL_Surface* source) {
    IMG_SavePNG(source,"debug.png");
    int width = source->w;
    int height = source->h;
    Uint32* pixels = (Uint32*)source->pixels;
    Uint32 black = SDL_MapRGB(source->format, 0, 0, 0);
    int isolatedCount = 0;


    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            if (pixels[y * width + x] == black) {
                int blackNeighborCount = 0;


                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue; 

                        int nx = x + dx;
                        int ny = y + dy;


                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {

                            if (pixels[ny * width + nx] == black) {
                                blackNeighborCount++;
                            }

                        }
                    }
                }


                if (blackNeighborCount == 0) {
                    isolatedCount++;
                }
            }
        }
    }

    return isolatedCount;
}


void removeSparseBlackPixels(SDL_Surface* source) {
    int width = source->w;
    int height = source->h;
    Uint32* pixels = (Uint32*)source->pixels;
    Uint32 black = SDL_MapRGB(source->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(source->format, 255, 255, 255);
    Uint32* tempPixels = (Uint32*)malloc(width * height * sizeof(Uint32));
    memcpy(tempPixels, pixels, width * height * sizeof(Uint32));


    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            if (tempPixels[y * width + x] == black) {
                int blackCount = 0;


                for (int dy = -2; dy <= 2; ++dy) {
                    for (int dx = -2; dx <= 2; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;


                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            if (tempPixels[ny * width + nx] == black) {
                                blackCount++;
                            }
                        }
                    }
                }


                if (blackCount <= 6) {
                    for (int dy = -2; dy <= 2; ++dy) {
                        for (int dx = -2; dx <= 2; ++dx) {
                            int nx = x + dx;
                            int ny = y + dy;


                            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                                pixels[ny * width + nx] = white;
                            }
                        }
                    }
                }
            }
        }
    }
    free(tempPixels);
}



void removeSparseBlackPixelsmot(SDL_Surface* source) {
    int width = source->w;
    int height = source->h;
    Uint32* pixels = (Uint32*)source->pixels;
    Uint32 black = SDL_MapRGB(source->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(source->format,255,255,255);
    Uint32* tempPixels = (Uint32*)malloc(width * height * sizeof(Uint32));
    memcpy(tempPixels, pixels, width * height * sizeof(Uint32));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tempPixels[y * width + x] == black) {
                int blackCount = 0;
                for ( int dy =-1; dy <=1; ++dy) 
                {
                    for (int dx =-1; dx <=1; ++dx) {
                        int nx =x+ dx;
                        int ny =y + dy;
                        if (nx >=0 && nx< width && ny>= 0 && ny < height) {
                            if (tempPixels[ny *width+nx] == black) {
                                blackCount++;
                            }
                        }
                    }
                }
                if (blackCount <= 3) {
                    pixels[y * width + x] =white;
                }
            }
        }
    }
    free(tempPixels);
}


void removeSparseBlackPixelsmotv2(SDL_Surface* source) {
    int width = source->w;
    int height = source->h;
    Uint32* pixels = (Uint32*)source->pixels;
    Uint32 black = SDL_MapRGB(source->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(source->format, 255, 255, 255);

    Uint32* tempPixels = (Uint32*)malloc(width * height * sizeof(Uint32));
    memcpy(tempPixels, pixels, width * height * sizeof(Uint32));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tempPixels[y * width + x] == black) {
                int blackCount = 0; 
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            if (tempPixels[ny * width + nx] == black) {
                                blackCount++;
                            }
                        }
                    }
                }
                if (blackCount <= 6) {
                    pixels[y * width + x] = white;
                }
            }
        }
    }
    free(tempPixels);
}

void clearRegion(SDL_Surface *surface, Bounds bounds) {
    Uint32 *pixels = (Uint32 *)surface->pixels;
    int width = surface->w;
    SDL_PixelFormat *format = surface->format;

    for (int y = bounds.top; y <= bounds.bottom; y++) {
        for (int x = bounds.left; x <= bounds.right; x++) {
            pixels[y *width +x] = SDL_MapRGB(format, 255, 255, 255);
        }
    }
}

int countPixelsInRegion(SDL_Surface *surface, Bounds bounds) {
    Uint32 *pixels = (Uint32*)surface->pixels;
    int width = surface->w;
    SDL_PixelFormat *format = surface->format;
    int count = 0;
    for (int y = bounds.top; y <= bounds.bottom; y++) {
        for (int x =bounds.left; x <= bounds.right; x++) {
            if (isBlack(pixels[y *width +x], format)==  0) {

                count++;
            }
        }
    }

    return count;
}

int hasFullBlackLine(SDL_Surface *surface, int y, int xStart, int xEnd) {
    Uint32 *pixels = (Uint32 *)surface->pixels;
    SDL_PixelFormat *format = surface->format;

    for (int x =xStart; x<= xEnd;x++) {
        if (isBlack(pixels[y * surface->w +x],format)!= 0) {
            return 1;

        }
    }
    return 0;
}

int isSquareWithFullEdges(SDL_Surface *surface, Bounds bounds) {
    if (hasFullBlackLine(surface, bounds.top, bounds.left, bounds.right) == 0) {
        return 1;
    }

    if (hasFullBlackLine(surface, bounds.bottom,bounds.left,bounds.right)== 0) {
        return 1;
    }
    return 0;

}


void processClusters(SDL_Surface *surface, Bounds *clusters, int clusterCount) {
    int totalPixels = 0;
    for (int i = 0; i < clusterCount; i++) {
        totalPixels += countPixelsInRegion(surface, clusters[i]);
    }
    int averagePixels = totalPixels / clusterCount;

    for (int i = 0; i < clusterCount; i++) {
        int pixelsInCluster = countPixelsInRegion(surface, clusters[i]);
        if (!isSquareWithFullEdges(surface, clusters[i]) && (pixelsInCluster > averagePixels+200 || pixelsInCluster < averagePixels - 500)) {
            clearRegion(surface, clusters[i]); 
        }
    }
}

void findBlackPixelClusters(SDL_Surface *surface, Bounds **clusters, int *clusterCount) {
    int width = surface->w;
    int height = surface->h;
    Uint32 *pixels = (Uint32 *)surface->pixels;
    SDL_PixelFormat *format = surface->format;
    int *visited = (int *)calloc(width * height, sizeof(int));
    *clusters = NULL;
    *clusterCount = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (!visited[y * width + x] && isBlack(pixels[y * width + x], format) == 0) {
                Bounds bounds = {y, y, x, x};
                int *queue =(int *)malloc(width * height * sizeof(int));
                int front = 0, back = 0;
                queue[back++] = y *width + x;
                visited[y * width +x] = 1;

                while (front < back) {
                    int current =queue[front++];
                    int cx =current % width;
                    int cy =current / width;

                    if (cy < bounds.top) bounds.top = cy;
                    if (cy > bounds.bottom) bounds.bottom = cy;
                    if (cx < bounds.left) bounds.left = cx;
                    if (cx > bounds.right) bounds.right = cx;
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            if (dx == 0 && dy == 0) continue; 
                            int nx = cx + dx;
                            int ny = cy + dy;

                            if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                                !visited[ny * width + nx] &&
                                isBlack(pixels[ny * width + nx], format) == 0) 
                            {
                                queue[back++] = ny * width + nx;
                                visited[ny * width + nx] = 1;
                            }
                        }
                    }
                }
                free(queue);
                *clusters = (Bounds *)realloc(*clusters, (*clusterCount + 1) * sizeof(Bounds));
                (*clusters)[*clusterCount] = bounds;
                (*clusterCount)++;
            }
        }
    }
    free(visited);
}



SDL_Surface* net(SDL_Surface* sourceImage) {



    SDL_Surface* targetImage = SDL_CreateRGBSurfaceWithFormat(0, sourceImage->w, sourceImage->h, 32, sourceImage->format->format);
    if (!targetImage) {
        return sourceImage;
    }
    int isolatepixel = countIsolatedBlackPixels(sourceImage);
    if (isolatepixel > (sourceImage->w * sourceImage->h) * 0.004)
    {
        if (isolatepixel > (sourceImage->w * sourceImage->h) * 0.0045)
        {
            removeSparseBlackPixels(sourceImage);
        }
        drawPathsBetweenAllBlackPixels(sourceImage);

        redrawAdjacentPixels(targetImage);

  
        processImage(sourceImage, targetImage,10);

 
        redrawAdjacentPixels(targetImage);


        processImage(sourceImage, targetImage,10);

        redrawAdjacentPixels(targetImage);
        

    }
    else
    {
       Bounds *clusters = NULL;
        int clusterCount = 0;

        findBlackPixelClusters(sourceImage, &clusters, &clusterCount);

        //printf("Nombre d'amas : %d\n", clusterCount);

        processClusters(sourceImage, clusters, clusterCount);

        SDL_SaveBMP(sourceImage, "output.bmp");
        free(clusters);
    }
    return sourceImage;
}

SDL_Surface* netmot(SDL_Surface* sourceImage){
    Bounds *clusters= NULL;
    int clusterCount =0;

    findBlackPixelClusters(sourceImage,&clusters,&clusterCount);

    //printf("Nombre d'amas : %d\n", clusterCount);

    processClusters(sourceImage, clusters, clusterCount);

    SDL_SaveBMP(sourceImage,"output.bmp");

    free(clusters);
}