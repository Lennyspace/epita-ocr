#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float filtre1[5][5] = {
    { 1,   0,  -1,   0, 1},
    { 2,   0,  -2,   0, 2},
    { 4,   0,  -4,   0, 4},
    { 2,   0,  -2,   0, 2},
    { 1,   0,  -1,   0, 1}
};

float filtre2[5][5] = {
    {  1,   2,   4,   2,   1},
    {  0,   0,   0,   0,   0},
    { -1,  -2,  -4,  -2,  -1},
    {  0,   0,   0,   0,   0},
    {  1,   2,   4,   2,   1}
};

float filtre3[5][5] = {
    { 0,  0,  1,  0,  0},
    { 0,  1,  1,  1,  0},
    { 1,  1,  1,  1,  1},
    { 0,  1,  1,  1,  0},
    { 0,  0,  1,  0,  0}
}; 

float filtre4[5][5] = {
    { 1, 0.5, 0, 0, 0 },
    { 0.5, 1, 0.5, 0, 0 },
    { 0, 0.5, 1, 0.5, 0 },
    { 0, 0, 0.5, 1, 0.5 },
    { 0, 0, 0, 0.5, 1 }
};
float filtre5[5][5] = {
    { 2,  1,  0, -1, -2},
    { 1,  2,  1,  0, -1},
    { 0,  1,  2,  1,  0},
    {-1,  0,  1,  2,  1},
    {-2, -1,  0,  1,  2}
};

float filtre6[5][5] = {
    { 1, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 1 },
    { 0, 0, 0, 0, 0 }

};

float filtreFlou[5][5] = {
    {0, -1, -1, -1, 0},
    {-1, 2, 4, 2, -1},
    {-1, 4, 8, 4, -1},
    {-1, 2, 4, 2, -1},
    {0, -1, -1, -1, 0}
};

float filter_5x5[5][5] = {
        { 1,  0, -1,  0,  1},
        { 0,  1,  0, -1,  0},
        {-1,  0,  2,  0, -1},
        { 0, -1,  0,  1,  0},
        { 1,  0, -1,  0,  1}
    };

float filtreTexture[5][5] = {
    {-2, -1,  0,  1,  2},
    {-1,  0,  1,  2,  1},
    { 0,  1,  2,  1,  0},
    { 1,  2,  1,  0, -1},
    { 2,  1,  0, -1, -2}
};

float filtreE[5][5] = {
    { -1,  0,  1,  0,  0},
    { -2,  0, 2,  0,  0},
    { -1,  0,  1,  0,  0},
    {  0,  0,  0,  0,  0},
    {  0,  0,  0,  0,  0}
};

float kernel[5][5] = {
    {0, 0, -1, 0, 0},
    {0, -1, -2, -1, 0},
    {-1, -2, 16, -2, -1},
    {0, -1, -2, -1, 0},
    {0, 0, -1, 0, 0}
};

float kernel2rond[5][5] = {
    {-1, -1,  0,  1,  1},
    {-1,  0,  1,  1,  0},
    { 0,  1,  1,  0, -1},
    { 1,  1,  0, -1, -1},
    { 1,  0, -1, -1, -1}
};

float kernel1rond[5][5] = {
    { 1,  1,  0, -1, -1},
    { 0,  1,  1,   0, -1},
    {-1,  0,  1,  1,  0},
    {-1, -1,  0,  1,  1},
    {-1, -1, -1,  0,  1}
};

float filtreE2[5][5] = {
    { 0,  0,  0,  0,  0 },
    { 1,  1,  1,  1,  1 }, 
    { 0,  0,  0,  0,   0 },
    {-1, -1, -1, -1, -1 },
    {-2, -2, -2, -2, -2 }
};

float filtre1_3x3_a[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

float filtre1_3x3_c[3][3] = {
    { 2,  0, -2},
    { 4,  0, -4},
    { 2,  0, -2}
};



float filtre1_3x3_b[3][3] = {
    { 1,  0, -1},
    { 2,  0, -2},
    { 1,  0, -1}
};


float filtre1_3x3_d[3][3] = {
    { 0,  1,  0},
    { 1,  2,  1},
    { 0,  1,  0}
};

float filtre2_3x3_a[3][3] = {
    {-1, -2, -1},
    {0,  0,  0},
    {1,  2,  1}
};

float filtre2_3x3_b[3][3] = {
    { 1,  2,  1},
    { 0,  0,  0},
    { -1, -2, -1}
};

float filtre2_3x3_c[3][3] = {
    { 2,  4,  2},
    { 0,  0,  0},
    { -2, -4, -2}
};

float filtre2_3x3_d[3][3] = {
    { 0,  1,  0},
    { 1,  2,  1},
    { 0,  1,  0}
};



//=======================================================================

float filtre3_5[3][3] = {
    { 1,  1, 1 },
    { 1, -8, 1 },
    { 1,  1, 1 }
};

float filtre3_6[3][3] = {
    { 0, -1, 0 },
    {-1,  4, -1 },
    { 0, -1, 0 }
};


float filtre3_7[3][3] = {
    {-1,  2, -1 },
    { 2, -4,  2 },
    {-1,  2, -1}
};

float filtre3_8[3][3] = {
    { 0,  1, 0 },
    { 1, -4, 1 },
    { 0,  1, 0 }
};


//filtre pour les diagonales

float filtre3_9[3][3] = {
    { 2, -1, -1 },
    {-1,  2, -1 },
    {-1, -1,  2 }
};

float filtre3_10[3][3] = {
    {-1, -1,  2 },
    {-1,  2, -1 },
    { 2, -1, -1 }
};


float filtre3_11[3][3] = {
    {-1, -1,  2 },
    {-1,  2, -1},
    {-1, -1,  2 }
};

float filtre3_12[3][3] = {
    { 2, -1, -1 },
    {-1,  2, -1 },
    {-1, -1, -1 }
};

float filtre3_13[3][3] = {
    {1, -1, 1},
    {-1, 4, -1},
    {1, -1, 1}
};


float filtreFlou3[3][3] = {
    {1/16.0, 2/16.0, 1/16.0},
    {2/16.0, 4/16.0, 2/16.0},
    {1/16.0, 2/16.0, 1/16.0}
};


float filtreSobelY[3][3] = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
};

float filtreLaplacien[3][3] = {
    {0, 1, 0},
    {1, -4, 1},
    {0, 1, 0}
};

float filtreDiag1[3][3] = {
    {2, -1, -1},
    {-1, 2, -1},
    {-1, -1, 2}
};


// Détec diagonales desc
float filtreDiag2[3][3] = {
    {-1, -1, 2},
    {-1, 2, -1},
    {2, -1, -1 }
};


float ffilter_3x3_1[3][3] = {
        { 0,  1,  0},
        { 1, -4,  1},
        { 0,  1,  0}
};

float ffilter_3x3_2[3][3] = {
        { 1,  0, -1},
        { 0,  2,  0},
        {-1,  0,  1}
};

float ffilter_3x3_3[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
};



float ffilter_3x3_4[3][3] = {
        { 1,  2,  1},
        { 0,  0,  0},
        {-1, -2, -1}
};

// Biais filtre 5x5
float biais5 = 0.50f;

float biais33=0.820f;

// normaliz les img
float* normaliserImage(SDL_Surface* image, int* outLength) {
    int width = image->w;
    int height = image->h;
    *outLength = width * height;
    float* normalizedPixels = malloc(*outLength * sizeof(float));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint32 pixel = ((Uint32*)image->pixels)[y* (image->pitch / 4) +x];

            Uint8 r = (pixel >> 16) & 0xFF;
            Uint8 g = (pixel >> 8) & 0xFF;
            Uint8 b = pixel & 0xFF;

            
            float grayValue = (0.3f * r + 0.59f * g + 0.11f * b) / 255.0f;
            normalizedPixels[y * width + x] = grayValue;
        }
    }

    return normalizedPixels;

}

float sigmoidf(float x) {
    return 1.f / (1.f + expf(-x));
}


float* filtrerImage5x5(float* pixels, int inputWidth, int inputHeight, float filtre[5][5],  float biais) {
    
    int outputWidth = inputWidth - 4;
    int outputHeight = inputHeight -4;

    float* filteredPixels =  malloc( outputWidth * outputHeight * sizeof(float));

    for (int y = 2; y < inputHeight - 2; ++y) {
        for (int x = 2; x < inputWidth - 2; ++x) {
            float r = biais;
            for (int fy = -2; fy <= 2;  ++fy) {
                for (int fx = -2; fx <= 2; ++fx) {
                    int pixelIndex = (y + fy) * inputWidth + (x  + fx);
                    r += pixels[pixelIndex] *filtre[2  + fy][ 2 + fx];
                }
            }
            
            filteredPixels[(y - 2) * outputWidth + (x - 2)] = fmin(fmax( r, 0), 1);
        }
    }
    return filteredPixels;
}



float* filtrerImage3x3(float* pixels,int inputWidth, int inputHeight, float filtre[3][3], float biais) {
    
    int outputWidth = inputWidth-2;
    int outputHeight = inputHeight- 2;

    float* filteredPixels = malloc(outputWidth * outputHeight * sizeof(float));

    for (int y = 1; y < inputHeight - 1; ++y) {
        for (int x = 1; x < inputWidth - 1; ++x) {
            float r = biais;
            for (int fy = -1; fy <= 1; ++fy) {
                for (int fx = -1; fx <= 1; ++fx) {
                    int pixelIndex = (y+ fy) * inputWidth + (x  + fx);
                    r += pixels[pixelIndex] * filtre[1 + fy][1 + fx];
                }
            }
            filteredPixels[(y - 1) * outputWidth + (x - 1)] = fmin(fmax(r, 0), 1);
        }
    }
    return filteredPixels;
}


float* additionnerImages(float* img1, float* img2, int width, int height) {
    float* result = malloc(width * height * sizeof(float));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = y * width + x;
            result[idx] = fmin((img1[idx] + img2[idx]) / 2.0f, 1.0f);
        }
    }
    return result;
}


float* max_pooling_2x2(float* image, int width, int height) {
    int newWidth = width / 2;
    int newHeight = height /2;
    float* pooledImage = malloc(newWidth * newHeight * sizeof(float));
    
    for (int y = 0; y < height; y += 2) {
        for (int x = 0; x < width; x +=2) {
            float maxPixel = 1;
            for (int fy = 0; fy < 2; ++fy) {
                for (int fx = 0; fx < 2; ++fx) {
                    int pixelIndex = (y + fy) * width + (x + fx);
                    if (image[pixelIndex] <maxPixel) {
                        maxPixel = image[pixelIndex];
                    }
                }
            }
            pooledImage[(y / 2) * newWidth + (x / 2)] =maxPixel;
        }
    }


    return pooledImage;
}

void sauvegarderImage(float* pixels, const char* filename, int width, int height) {
    SDL_Surface* image = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Uint8 grayValue = (Uint8)(pixels[y *width + x] *255);
            Uint32 pixelColor = (grayValue << 16) | (grayValue << 8) | grayValue;



            ((Uint32*)image->pixels)[y * (image->pitch / 4) + x] = pixelColor;
        }
    }

    IMG_SavePNG(image, filename);
        


    SDL_FreeSurface(image);


}




void CNN(SDL_Surface* image, float finalTab[750]) {


    int length;
    float* normalizedPixels = normaliserImage(image,&length);

   
    float* filtered1Image = filtrerImage5x5(normalizedPixels, 28, 28, filtre1, biais5);
    float* filtered2Image = filtrerImage5x5(normalizedPixels, 28, 28, filtre2, biais5);
    float* filtered3Image = filtrerImage5x5(normalizedPixels, 28, 28, filtre3, -1);

    float* filtered4Image = filtrerImage5x5(normalizedPixels, 28, 28, filtre4, biais5);

    float* filtered5Image = filtrerImage5x5(normalizedPixels, 28, 28, filtre5, biais5);

    float* filtered6Image = filtrerImage5x5(normalizedPixels, 28, 28, filtre6, biais5);

    float* filtered7Image = filtrerImage5x5(normalizedPixels, 28, 28, filtreFlou, -1.5);

    float* filtered8Image = filtrerImage5x5(normalizedPixels, 28, 28, filter_5x5, biais5);

    float* filtered9Image = filtrerImage5x5(normalizedPixels, 28, 28, filtreTexture, biais5);

    float* filtered10Image = filtrerImage5x5(normalizedPixels, 28, 28, filtreE, -1.5);

    float* filtered11Image = filtrerImage5x5(normalizedPixels, 28, 28, kernel, biais5);

    float* filtered12Image = filtrerImage5x5(normalizedPixels, 28, 28, kernel2rond, 1.1);
    float* filtered13Image = filtrerImage5x5(normalizedPixels, 28, 28, kernel1rond, 1.1);


    float* filtered14Image = filtrerImage5x5(normalizedPixels, 28, 28, filtreE2, -0.8);

    

    //sauvegarderImage(filtered1Image,"ImageCNN/filtre1.png", 24,24);
    //sauvegarderImage(filtered2Image,"ImageCNN/filtre2.png", 24,24);
    //sauvegarderImage(filtered3Image,"ImageCNN/filtre3.png", 24,24);
    //sauvegarderImage(filtered4Image,"ImageCNN/filtre4.png", 24,24);
    //sauvegarderImage(filtered5Image,"ImageCNN/filtre5.png", 24,24);
    //sauvegarderImage(filtered6Image,"ImageCNN/filtre6.png", 24,24);
    //sauvegarderImage(filtered7Image,"ImageCNN/filtre7.png", 24,24);
    //sauvegarderImage(filtered8Image,"ImageCNN/filtre8.png", 24,24);
    //sauvegarderImage(filtered14Image,"ImageCNN/filtre9.png", 24,24);



    
    float* pooledImage1 = max_pooling_2x2(filtered1Image, 24,24);
    float* pooledImage2 = max_pooling_2x2(filtered2Image, 24,24);
    float* pooledImage3 = max_pooling_2x2(filtered3Image, 24,24);

    float* pooledImage4 = max_pooling_2x2(filtered4Image, 24,24);

    float* pooledImage5 = max_pooling_2x2(filtered3Image, 24,24);

    float* pooledImage6 = max_pooling_2x2(filtered6Image, 24,24);

    float* pooledImage7 = max_pooling_2x2(filtered7Image, 24,24);

    float* pooledImage8 = max_pooling_2x2(filtered8Image, 24,24);

    float* pooledImage9 = max_pooling_2x2(filtered9Image, 24,24);

    float* pooledImage10 = max_pooling_2x2(filtered10Image, 24,24);

    float* pooledImage11 = max_pooling_2x2(filtered11Image, 24,24);

    float* pooledImage12 = max_pooling_2x2(filtered12Image, 24,24);
    float* pooledImage13 = max_pooling_2x2(filtered13Image, 24,24);

    float* pooledImage14 = max_pooling_2x2(filtered14Image, 24,24);


  
    float* finalImages[30]; 


    //filtres 3x3
    float* filtered3x3_1 = filtrerImage3x3(pooledImage1, 12, 12,filtre1_3x3_a , biais33);
    float* filtered3x3_2 = filtrerImage3x3(pooledImage1,12, 12, filtre1_3x3_b,  biais33);
    float* filtered3x3_3 = filtrerImage3x3(pooledImage1, 12, 12, filtre1_3x3_c, biais33);
    float* filtered3x3_4 = filtrerImage3x3(pooledImage1, 12, 12, filtre1_3x3_d,  biais33);


    float* filtered3x3_5 = filtrerImage3x3(pooledImage2, 12, 12, filtre2_3x3_a,  biais33);
    float* filtered3x3_6 = filtrerImage3x3(pooledImage2, 12, 12, filtre2_3x3_b,  biais33);
    float* filtered3x3_7 = filtrerImage3x3(pooledImage2, 12, 12, filtre2_3x3_c,  biais33);
    float* filtered3x3_8 = filtrerImage3x3(pooledImage2, 12, 12, filtre2_3x3_d,  biais33);


    float* filtered3x3_9 = filtrerImage3x3(pooledImage3, 12, 12, filtre3_5,  -1.5);
    float* filtered3x3_10 = filtrerImage3x3(pooledImage3, 12, 12, filtre3_6,  -1.5);
    float* filtered3x3_11 = filtrerImage3x3(pooledImage3, 12, 12, filtre3_7,  -1.5);
    float* filtered3x3_12 = filtrerImage3x3(pooledImage3, 12, 12, filtre3_8,  -1.5);


    float* filtered3x3_13 = filtrerImage3x3(pooledImage4, 12, 12, filtre3_10,  biais33);
    float* filtered3x3_14 = filtrerImage3x3(pooledImage4, 12, 12, filtreDiag1,  biais33);
    float* filtered3x3_15 = filtrerImage3x3(pooledImage4, 12, 12, filtreDiag2,  biais33);
    float* filtered3x3_16 = filtrerImage3x3(pooledImage4, 12, 12, filtre3_12,  biais33);

    float* filtered3x3_17 = filtrerImage3x3(pooledImage5, 12, 12, filtre3_9,  0.6);
    float* filtered3x3_18 = filtrerImage3x3(pooledImage5, 12, 12, filtre3_10, 0.6);
    float* filtered3x3_19 = filtrerImage3x3(pooledImage5, 12, 12, filtre3_11,  0.6);
    float* filtered3x3_20 = filtrerImage3x3(pooledImage5, 12, 12, filtre3_12, 0.6);


    float* filtered3x3_21 = filtrerImage3x3(pooledImage6, 12, 12, filtre3_9,    -1.5);
    float* filtered3x3_22 = filtrerImage3x3(pooledImage6, 12, 12, filtre3_10,   -1.5);
    float* filtered3x3_23 = filtrerImage3x3(pooledImage6, 12, 12, filtre3_11,   -1.5);
    float* filtered3x3_24 = filtrerImage3x3(pooledImage6, 12, 12, filtre3_12,  -1.5);


    float* filtered3x3_25 = filtrerImage3x3(pooledImage7, 12, 12, filtre3_13,      -1);
    float* filtered3x3_26 = filtrerImage3x3(pooledImage7, 12, 12, filtreFlou3,     -1);
    float* filtered3x3_27 = filtrerImage3x3(pooledImage7, 12, 12, filtreSobelY,    -1);
    float* filtered3x3_28 = filtrerImage3x3(pooledImage7, 12, 12, filtreLaplacien, -1);

    float* filtered3x3_29 = filtrerImage3x3(pooledImage8, 12, 12, ffilter_3x3_1,  biais33);
    float* filtered3x3_30 = filtrerImage3x3(pooledImage8, 12, 12, ffilter_3x3_2,  biais33);
    float* filtered3x3_31 = filtrerImage3x3(pooledImage8, 12, 12, ffilter_3x3_3,  biais33);
    float* filtered3x3_32 = filtrerImage3x3(pooledImage8, 12, 12, ffilter_3x3_4,  biais33);


    float* filtered3x3_33 = filtrerImage3x3(pooledImage9, 12, 12, ffilter_3x3_1,  biais33);
    float* filtered3x3_34 = filtrerImage3x3(pooledImage9, 12, 12, ffilter_3x3_2,  biais33);
    float* filtered3x3_35 = filtrerImage3x3(pooledImage9, 12, 12, ffilter_3x3_3,  biais33);
    float* filtered3x3_36 = filtrerImage3x3(pooledImage9, 12, 12, ffilter_3x3_4,  biais33);


    float* filtered3x3_37 = filtrerImage3x3(pooledImage10, 12, 12, ffilter_3x3_1,  -1);
    float* filtered3x3_38 = filtrerImage3x3(pooledImage10, 12, 12, ffilter_3x3_2,  -1);
    float* filtered3x3_39 = filtrerImage3x3(pooledImage10, 12, 12, ffilter_3x3_3,  -1);
    float* filtered3x3_40 = filtrerImage3x3(pooledImage10, 12, 12, ffilter_3x3_4,  -1);

    float* filtered3x3_41 = filtrerImage3x3(pooledImage11, 12, 12,filtre3_13,  biais33);
    float* filtered3x3_42 = filtrerImage3x3(pooledImage11,12, 12, filtreFlou3,  biais33);
    float* filtered3x3_43= filtrerImage3x3(pooledImage11, 12, 12, filtreSobelY, biais33);
    float* filtered3x3_44 = filtrerImage3x3(pooledImage11, 12, 12, filtreLaplacien,  biais33);


    float* filtered3x3_45 = filtrerImage3x3(pooledImage4, 12, 12, ffilter_3x3_1,  biais33);
    float* filtered3x3_46 = filtrerImage3x3(pooledImage4, 12, 12, ffilter_3x3_2,  biais33);
    float* filtered3x3_47 = filtrerImage3x3(pooledImage4, 12, 12, ffilter_3x3_3,  biais33);
    float* filtered3x3_48 = filtrerImage3x3(pooledImage4, 12, 12, ffilter_3x3_4,  biais33);



    float* filtered3x3_49 = filtrerImage3x3(pooledImage12, 12, 12, ffilter_3x3_1,  0);
    float* filtered3x3_50 = filtrerImage3x3(pooledImage12, 12, 12, ffilter_3x3_2,  0);
    float* filtered3x3_51 = filtrerImage3x3(pooledImage12, 12, 12, ffilter_3x3_3,  0);
    float* filtered3x3_52 = filtrerImage3x3(pooledImage12, 12, 12, ffilter_3x3_4,  0);


    float* filtered3x3_53 = filtrerImage3x3(pooledImage13, 12, 12, ffilter_3x3_1,  0);
    float* filtered3x3_54 = filtrerImage3x3(pooledImage13, 12, 12, ffilter_3x3_2,  0);
    float* filtered3x3_55 = filtrerImage3x3(pooledImage13, 12, 12, ffilter_3x3_3,  0);
    float* filtered3x3_56 = filtrerImage3x3(pooledImage13, 12, 12, ffilter_3x3_4,  0);

    float* filtered3x3_57 = filtrerImage3x3(pooledImage14, 12, 12, ffilter_3x3_1,  0);
    float* filtered3x3_58 = filtrerImage3x3(pooledImage14, 12, 12, ffilter_3x3_2,  0);
    float* filtered3x3_59 = filtrerImage3x3(pooledImage14, 12, 12, ffilter_3x3_3,  0);
    float* filtered3x3_60 = filtrerImage3x3(pooledImage14, 12, 12, ffilter_3x3_4,  0);


    //sauvegarderImage(filtered3x3_49,"1.png", 10,10);
    //sauvegarderImage(filtered3x3_50,"2.png", 10,10);
    //(filtered3x3_51,"3.png", 10,10);
    //sauvegarderImage(filtered3x3_52,"4.png", 10,10);





    



    //Addition
    float* addimage_1 = additionnerImages(filtered3x3_1, filtered3x3_2,10, 10);
    float* addimage_2 = additionnerImages(filtered3x3_3, filtered3x3_4,10, 10);
    float* addimage_3 = additionnerImages(filtered3x3_5, filtered3x3_6, 10, 10);
    float* addimage_4 = additionnerImages(filtered3x3_7, filtered3x3_8, 10, 10);
    float* addimage_5 = additionnerImages(filtered3x3_9, filtered3x3_10, 10, 10);
    float* addimage_6 = additionnerImages(filtered3x3_11, filtered3x3_12, 10, 10);

    float* addimage_7 = additionnerImages(filtered3x3_13, filtered3x3_14, 10, 10);
    float* addimage_8 = additionnerImages(filtered3x3_15, filtered3x3_16, 10, 10);

    float* addimage_9 = additionnerImages(filtered3x3_17, filtered3x3_18, 10, 10);
    float* addimage_10 = additionnerImages(filtered3x3_19, filtered3x3_20, 10, 10);

    float* addimage_11 = additionnerImages(filtered3x3_21, filtered3x3_22, 10, 10);
    float* addimage_12 = additionnerImages(filtered3x3_23, filtered3x3_24, 10, 10);

    float* addimage_13 = additionnerImages(filtered3x3_25, filtered3x3_26, 10, 10);
    float* addimage_14 = additionnerImages(filtered3x3_27, filtered3x3_28, 10, 10);

    float* addimage_15 = additionnerImages(filtered3x3_29, filtered3x3_30, 10, 10);
    float* addimage_16 = additionnerImages(filtered3x3_31, filtered3x3_32, 10, 10);


    float* addimage_17 = additionnerImages(filtered3x3_33, filtered3x3_34, 10, 10);
    float* addimage_18 = additionnerImages(filtered3x3_35, filtered3x3_36, 10, 10);


    float* addimage_19 = additionnerImages(filtered3x3_37, filtered3x3_38, 10, 10);
    float* addimage_20 = additionnerImages(filtered3x3_39, filtered3x3_40, 10, 10);


    float* addimage_21 = additionnerImages(filtered3x3_41, filtered3x3_42, 10, 10);
    float* addimage_22 = additionnerImages(filtered3x3_43, filtered3x3_44, 10, 10);

    float* addimage_23 = additionnerImages(filtered3x3_45, filtered3x3_46, 10, 10);
    float* addimage_24 = additionnerImages(filtered3x3_47, filtered3x3_48, 10, 10);

    float* addimage_25 = additionnerImages(filtered3x3_49, filtered3x3_50, 10, 10);
    float* addimage_26 = additionnerImages(filtered3x3_51, filtered3x3_52, 10, 10);

    float* addimage_27 = additionnerImages(filtered3x3_53, filtered3x3_54, 10, 10);
    float* addimage_28 = additionnerImages(filtered3x3_55, filtered3x3_56, 10, 10);

    float* addimage_29 = additionnerImages(filtered3x3_57, filtered3x3_58, 10, 10);
    float* addimage_30 = additionnerImages(filtered3x3_59, filtered3x3_60, 10, 10);







    //maxpooling
    finalImages[0] = max_pooling_2x2(addimage_1,10, 10);

    finalImages[1] = max_pooling_2x2(addimage_2,10, 10);

    finalImages[2] = max_pooling_2x2(addimage_3,10, 10);
  
    finalImages[3] = max_pooling_2x2(addimage_4,10,10);

    finalImages[4] = max_pooling_2x2(addimage_5,10,10);
    
    finalImages[5] = max_pooling_2x2(addimage_6,10,10);

    finalImages[6] = max_pooling_2x2(addimage_7,10,10);

    finalImages[7] = max_pooling_2x2(addimage_8,10,10);

    finalImages[8] = max_pooling_2x2(addimage_9,10,10);

    finalImages[9] = max_pooling_2x2(addimage_10,10,10);

    finalImages[10] = max_pooling_2x2(addimage_11,10,10);

    finalImages[11] = max_pooling_2x2(addimage_12,10,10);

    finalImages[12] = max_pooling_2x2(addimage_13,10,10);

    finalImages[13] = max_pooling_2x2(addimage_14,10,10);

    finalImages[14] = max_pooling_2x2(addimage_15,10,10);

    finalImages[15] = max_pooling_2x2(addimage_16,10,10);

    finalImages[16] = max_pooling_2x2(addimage_17,10,10);

    finalImages[17] = max_pooling_2x2(addimage_18,10,10);

    finalImages[18] = max_pooling_2x2(addimage_19,10,10);

    finalImages[19] = max_pooling_2x2(addimage_20,10,10);


    finalImages[20] = max_pooling_2x2(addimage_21,10,10);

    finalImages[21] = max_pooling_2x2(addimage_22,10,10);

    finalImages[22] = max_pooling_2x2(addimage_23,10,10);

    finalImages[23] = max_pooling_2x2(addimage_24,10,10);

    finalImages[24] = max_pooling_2x2(addimage_25,10,10);

    finalImages[25] = max_pooling_2x2(addimage_26,10,10);

    finalImages[26] = max_pooling_2x2(addimage_27,10,10);

    finalImages[27] = max_pooling_2x2(addimage_28,10,10);

    finalImages[28] = max_pooling_2x2(addimage_29,10,10);

    finalImages[29] = max_pooling_2x2(addimage_30,10,10);



    //sauvegarderImage(finalImages[0],"ImageCNN/fs1.png",5,5);
    //sauvegarderImage(finalImages[1],"ImageCNN/fs2.png",5,5);
    //sauvegarderImage(finalImages[2],"ImageCNN/fs3.png",5,5);
    //sauvegarderImage(finalImages[3],"ImageCNN/fs4.png",5,5);
    //sauvegarderImage(finalImages[4],"ImageCNN/fs5.png",5,5);
    //sauvegarderImage(finalImages[5],"ImageCNN/fs6.png",5,5);

    //sauvegarderImage(finalImages[6],"ImageCNN/fs7.png",5,5);
    //sauvegarderImage(finalImages[7],"ImageCNN/fs8.png",5,5);

    //sauvegarderImage(finalImages[8],"ImageCNN/fs9.png",5,5);
    //sauvegarderImage(finalImages[9],"ImageCNN/fs1!.png",5,5);


    //sauvegarderImage(finalImages[10],"ImageCNN/fs11.png",5,5);
    //sauvegarderImage(finalImages[11],"ImageCNN/fs12.png",5,5);


    //sauvegarderImage(finalImages[12],"ImageCNN/fs13.png",5,5);
    //sauvegarderImage(finalImages[13],"ImageCNN/fs14.png",5,5);



    //sauvegarderImage(finalImages[14],"ImageCNN/fs15.png",5,5);
    //sauvegarderImage(finalImages[15],"ImageCNN/fs16.png",5,5);


    //sauvegarderImage(finalImages[16],"ImageCNN/fs17.png",5,5);
    //sauvegarderImage(finalImages[17],"ImageCNN/fs18.png",5,5);



    



    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 25; j++) {
            finalTab[i * 25 + j] = finalImages[i][j]; 
        }
    }


    free(normalizedPixels);
    for (int i = 0; i < 30; i++) {
        free(finalImages[i]); 
        
    }
    
}