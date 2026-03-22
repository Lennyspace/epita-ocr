#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <fcntl.h>
#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "cnn.h"
#include "TestLettre.h"

#define NB_NEURONS_HIDDEN 26
#define INPUT_SIZE 750


float weights[26][INPUT_SIZE][NB_NEURONS_HIDDEN];
float bias_H[26][NB_NEURONS_HIDDEN];
float weights_H[26][NB_NEURONS_HIDDEN];
float bias[26];


const char* weight_files[] = {
    "Poids/APoid.txt", "Poids/BPoid.txt", "Poids/CPoid.txt", "Poids/DPoid.txt", "Poids/EPoid.txt",
    "Poids/FPoid.txt", "Poids/GPoid.txt", "Poids/HPoid.txt", "Poids/IPoid.txt", "Poids/JPoid.txt",
    "Poids/KPoid.txt", "Poids/LPoid.txt", "Poids/MPoid.txt", "Poids/NPoid.txt", "Poids/OPoid.txt",
    "Poids/PPoid.txt", "Poids/QPoid.txt", "Poids/RPoid.txt", "Poids/SPoid.txt", "Poids/TPoid.txt",
    "Poids/UPoid.txt", "Poids/VPoid.txt", "Poids/WPoid.txt", "Poids/XPoid.txt", "Poids/YPoid.txt",
    "Poids/ZPoid.txt"
};

const int num_weight_files = 26;


static inline float relu(float x) {
    return fmaxf(0.0f, x);

}


void load_all_weights() {
    for (int f = 0; f < num_weight_files; f++) {
        FILE* file = fopen(weight_files[f], "r");


        for (int i = 0; i < INPUT_SIZE; i++) {
            for (int j = 0; j < NB_NEURONS_HIDDEN; j++) {
                if (fscanf(file, "%f", &weights[f][i][j]) != 1) {
                    fclose(file);
                    return;
                }
            }
        }

        for (int j = 0; j < NB_NEURONS_HIDDEN; j++) {
            if (fscanf(file, "%f", &bias_H[f][j]) != 1) {
                fclose(file);
                return;
            }
        }


        for (int j = 0; j < NB_NEURONS_HIDDEN; j++) {
            if (fscanf(file, "%f", &weights_H[f][j]) != 1) {
                fclose(file);
                return;
            }
        }

        if (fscanf(file, "%f", &bias[f]) != 1) {
            fclose(file);
            return;
        }
        fclose(file);
    }
}


float forward(float inputFeatures[INPUT_SIZE], float hiddenLayerActivations[NB_NEURONS_HIDDEN], int weight_index) {
    
    for (int j = 0; j < NB_NEURONS_HIDDEN; j++) {
        hiddenLayerActivations[j] = bias_H[weight_index][j];
        for (int i = 0; i < INPUT_SIZE; i++) {
            hiddenLayerActivations[j] += inputFeatures[i] * weights[weight_index][i][j];
        }
        hiddenLayerActivations[j] = relu(hiddenLayerActivations[j]);
    }

    
    float outputActivation = bias[weight_index];
    for (int j = 0; j < NB_NEURONS_HIDDEN; j++) {
        outputActivation += hiddenLayerActivations[j] *weights_H[weight_index][j];
    }
    return 1.0f / (1.0f + expf(-outputActivation));
}

float test_image(float input[INPUT_SIZE], float hiddenLayerActivations[NB_NEURONS_HIDDEN], int weight_index) {
    return forward(input, hiddenLayerActivations, weight_index);
}


void creerfichiergrid(int tailletab, SDL_Surface** tabsurface,int nbc) {
    

    
    load_all_weights();
    int fichier = open("grid.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int col=1;
    for (int img_index = 0; img_index < tailletab; img_index++) {

        SDL_Surface* image =tabsurface[img_index];

        float input[INPUT_SIZE] = {0};
        float finalImages[INPUT_SIZE] = {0};
        CNN(image, finalImages);

        for (int i = 0; i < INPUT_SIZE; i++) {
            input[i] = finalImages[i];
        }

        SDL_FreeSurface(image);

        float best_output =-1.0f;
        float secondbestoutput =-1.0f;
        int best_weight_index = -1;
        int secondbest_weight_index = -1;
        float hiddenLayerActivations[NB_NEURONS_HIDDEN]= {0};

        for (int w = 0; w < num_weight_files; w++) {
            float output = test_image(input,hiddenLayerActivations,w);
            
            if (output > best_output) {
                secondbestoutput = best_output;
                secondbest_weight_index = best_weight_index;

                best_output =output;
                best_weight_index= w;
            } else if (output > secondbestoutput) {
                secondbestoutput = output;
                secondbest_weight_index = w;
            }
        }
        char weight_letter = 'A' + best_weight_index;
        char secweight_letter = 'A' + secondbest_weight_index;

        //printf("lettre trouve %c avec %.2f et second=  %c avec %.2f\n",weight_letter,best_output,secweight_letter,secondbestoutput);
        write(fichier,&weight_letter, 1);
        col++;
        if(col>nbc){
            write(fichier,"\n", 1);
            col=1;
        }
        
    }
}

int test(SDL_Surface** tablettre,int nbcol,int nbligne) {
    
    creerfichiergrid(nbligne*nbcol, tablettre, nbcol);
    return 0;
}

char testlettreunique(SDL_Surface* lettre){

    float input[INPUT_SIZE] ={0};
    float finalImages[INPUT_SIZE] = {0};
    
    CNN(lettre, finalImages);


    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = finalImages[i];
    }

    float best_output = -1.0f;
    int best_weight_index = -1;

    float hiddenLayerActivations[NB_NEURONS_HIDDEN] = {0};

    for (int w =0; w < num_weight_files; w++) {
        float output = test_image(input, hiddenLayerActivations, w);
            
        if (output> best_output) {
            best_output = output;
            best_weight_index = w;
        }
    }

    char weight_letter = 'A' + best_weight_index;
    return weight_letter;
}
