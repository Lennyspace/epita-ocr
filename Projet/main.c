#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "FichierC/point.h"
#include "FichierC/additionnal_functions.h"
#include "FichierC/to_black_and_white.h"
#include "FichierC/detecte.c"
#include "FichierC/readletter1.c"
#include "FichierC/findmot.c"
#include "FichierC/extractletter.c"
#include "FichierC/solver.c"
#include "rotation.c"

#include "FichierC/detectermot.c"
#include "FichierCIA/to28.c"
#include "FichierCIA/TestLettre.c"
#include "generateur_grille/generateur_image.h"


void suivant(GtkWidget* window, gpointer data);
void probleme(GtkWidget* window, gpointer data);
void fin(GtkWidget* window, gpointer data);
void detectg(GtkWidget* window, gpointer data);
void load_css(void);
SDL_Surface* rotate_image(SDL_Surface* image, double angle);
void rota_droite(GtkWidget* window, gpointer data);
void rota_gauche(GtkWidget* window, gpointer data);
void image_1_1(GtkWidget* window, gpointer data);
void image_1_2(GtkWidget* window, gpointer data);
void image_2_1(GtkWidget* window, gpointer data);
void image_2_2(GtkWidget* window, gpointer data);
void rota_auto(GtkWidget* window, gpointer data);
void rota_page2(GtkWidget* window, gpointer data);
void debut(GtkWidget* window, gpointer data);
void enregistrer(GtkWidget* window, gpointer data);
void generer_image(GtkWidget* window, gpointer data);
void help(GtkWidget* window, gpointer data);



typedef struct{

    //Pages
    GtkWidget* window;//la fenetre

    //adresse
    char* adresse[4];//nom de l'image

    //adresse image sauvegarder (rotation)
    char* adresse_rota[4];//nom de l'image

    //identifiant image 0 <= id <= 3
    int id;

    //angle
    double angle;//pour calcule de la rotation

    //Fixed
    GtkWidget* fixed;

    //Buttons
    GtkWidget* button[9];

    //image
    GtkWidget* image;

    GtkWidget* grille; //image mais avec juste les lettre

    GtkWidget* mots; //image mais avec juste les mot

    GtkWidget* avertissement;

    //entry
    GtkWidget* entry;//pour indiquer l'angle de la rota

    //labels
    GtkWidget* label;

    GtkWidget* message_pas_bon;//pour la fin

    SDL_Surface* res;

    SDL_Surface** resu;


    SDL_Surface* imagedebut;
    

    int xgrille;
    int ygrille;

    char** liste_mot;//que les mauvais mots

    int* place_mot_pas_bon;

    char** liste_mot_trouver;//tous les mots

    GtkWidget* List_mots_corriger[20];

    int taille_mots;//mot pas trouver

    GtkWidget* Nom_ancien_mots[20];

    GtkWidget* photo_mots_pas_trouver[20];

    char* adresse_image_mots[20];

    const char* nouveau_mots_corriger[20];

    int total_de_mot;

    SDL_Surface* imagedebutv2;

    int nbcol;

    struct Point** newcoord;

    GtkWidget* bouton_retour_au_debut;

    GtkWidget* saved;

    GtkWidget* generer_image;

    int gener;


}g_element;

void help(GtkWidget* window, gpointer data){
    g_element *app;
    app = (gpointer)data;

    //-----GRILLE----//
    int nbcol=0;
    int nbligne=0;
    struct Point** coordonateimage=NULL;

    SDL_Surface** tablettre=readlettermain(app->resu[0],&nbcol,&nbligne,&coordonateimage); //tablettre contienttoutesleslettre de gauche a droite et ensuite on descend t as capte


    SDL_Surface** newres=malloc(sizeof(SDL_Surface*)*nbcol*nbligne);
    for(int i=0;i<nbligne;i++){
        for(int j=0;j<nbcol;j++){
            char filename[50];
            snprintf(filename, sizeof(filename), "Lettre/%d,%d.png",i, j);
            IMG_SavePNG(tablettre[j * nbligne + i],filename);
            newres[i * nbcol + j] = tablettre[j * nbligne + i];

        }
    }

    struct Point** newcoord=malloc(sizeof(SDL_Surface*)*nbcol*nbligne);
    for(int i=0;i<nbligne;i++){
        for(int j=0;j<nbcol;j++){
            newcoord[i * nbcol + j] = coordonateimage[j * nbligne + i];
        }
    }




    free(tablettre);
    char filename[50];

    int nbmot;

    

    SDL_Surface** listemot=decoupelistemot(app->resu[1],&nbmot);
    //printf("nb mot : %d\n",nbmot);

    app->total_de_mot = nbmot;

    for(int i=0;i<nbmot;i++){
        int nbletter=0;
        SDL_Surface** lettermot=extractlettermain(listemot[i],&nbletter);

        SDL_Surface* lettre;
        for(int j=0;j<nbletter;j++){
            
            lettre=lettermot[j];
            char filename[50];
            snprintf(filename, sizeof(filename), "Lettremot/%d,%d.png",i, j);
            IMG_SavePNG(lettre,filename);
            //IMG_SavePNG(lettre,filename);
        }


    }

    app->liste_mot = malloc(sizeof(char*) * 0);
    app->liste_mot_trouver = malloc(sizeof(char *) * 1024);
    app->place_mot_pas_bon = malloc(sizeof(int) * 0);

    int cmpt = 0;

    Uint8 couleurs[20][4] = {
        {255, 0,0, 40},
        {0, 255, 0, 40},
        {0, 0, 255, 40},
        {255, 255, 0, 40},
        {0, 255, 255, 40},
        {255, 0, 255, 40},
        {128, 0, 0, 40},
        {0, 128, 0, 40},
        {0, 0, 128, 40},
        {128, 128, 0, 40},
        {0, 128, 128, 40},
        {128, 0, 128, 40},
        {192, 192, 192, 40},
        {128, 128, 128, 40},
        {64, 64, 64, 40},
        {255, 165, 0, 40},
        {75, 0, 130, 40}, 
        {240,230, 140, 40},
        {123, 104, 238, 40},
        {0, 100, 0, 40}
    };

    app->newcoord = newcoord;
    app->nbcol = nbcol;

    //printf("C la ?\n");


    app->xgrille-=0;
    app->ygrille+=0;
    to28centerized(newres,nbligne*nbcol); //centre et zoom les image en 28x28 pour l ia
    test(newres,nbcol,nbligne);
    char mot[50];
    for(int i=0;i<nbmot;i++){


        int nbletter=0;
        SDL_Surface** lettermot=extractlettermain(listemot[i],&nbletter);
        to28centerized(lettermot,nbletter);
        SDL_Surface* lettre;
        for(int j=0;j<nbletter;j++){
            
            lettre=lettermot[j];
            //sprintf(filename, "LettresMot/(%d %d).png", i, j);
            char let=testlettreunique(lettre);
            mot[j]=let;
        }
        mot[nbletter]=0;
        int xdep=0;
        int ydep=0;
        int xarr=0;
        int yarr=0;

        if(solve("grid.txt",mot,&xdep, &ydep , &xarr, &yarr) == 1){
            app->liste_mot = realloc(app->liste_mot, (cmpt+1) * (sizeof(char*)));
            app->liste_mot[cmpt] = strdup(mot);
            app->place_mot_pas_bon[cmpt] = i;
            IMG_SavePNG(listemot[i],app->adresse_image_mots[i]);
            
            cmpt += 1;

        }

        app->liste_mot_trouver[i] = strdup(mot);


        SDL_SetSurfaceBlendMode(app->imagedebut, SDL_BLENDMODE_BLEND);
        Uint32 color = SDL_MapRGBA(app->imagedebut->format, couleurs[i][0], couleurs[i][1], couleurs[i][2], couleurs[i][3]);
        drawLineFinal(app->imagedebut, app->xgrille+newcoord[ydep*nbcol+xdep]->x - 1 ,app->ygrille+newcoord[ydep*nbcol+xdep]->y - 1,
                        app->xgrille+newcoord[ydep*nbcol+xdep]->x + 1 ,app->ygrille+newcoord[ydep*nbcol+xdep]->y + 1,15,color);       
    }

    

    

    IMG_SavePNG(app->imagedebut, "res/ImageFINALE.png");

    //--------------//

    gtk_container_remove(GTK_CONTAINER(app->window), app->fixed);

    //fin
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/ImageFINALE.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    

    app->label = gtk_label_new(NULL);
    app->fixed = gtk_fixed_new();
    

    gtk_label_set_text(GTK_LABEL(app->label), "\n       You can see the help       \n");
    gtk_widget_set_name(app->label, "black_and_w");

    app->taille_mots = cmpt;

    ///PB avec des mots

    if(cmpt > 0){
        app->message_pas_bon = gtk_label_new(NULL);

        char* mot_pas_found = malloc(sizeof(char*) * 1024);

        mot_pas_found[0] = '\0';

        strcat(mot_pas_found, "\n       Here are the words where there is a problem       \n\n");

        strcat(mot_pas_found, "       ");

        for (int i = 0; i < cmpt; i++){

            strcat(mot_pas_found, "-");
            strcat(mot_pas_found, app->liste_mot[i]);
            
            strcat(mot_pas_found, "\n       ");



        }

        strcat(mot_pas_found, "       \n");

        gtk_label_set_text(GTK_LABEL(app->message_pas_bon), mot_pas_found);
        gtk_widget_set_name(app->message_pas_bon, "black_and_w");

        free(mot_pas_found);

        gtk_fixed_put(GTK_FIXED(app->fixed), app->message_pas_bon, 1100, 400);

        app->button[0] = gtk_button_new();

        gtk_button_set_label(GTK_BUTTON(app->button[0]), "Correct the words");
        gtk_widget_set_size_request(app->button[0], 200, 100);

        gtk_fixed_put(GTK_FIXED(app->fixed), app->button[0], 1200, 700);

        g_signal_connect(G_OBJECT(app->button[0]), "clicked", G_CALLBACK(probleme), (gpointer)app);
    }

    else{

        app->bouton_retour_au_debut = gtk_button_new();

        app->entry = gtk_entry_new();
        app->saved = gtk_button_new();

        gtk_button_set_label(GTK_BUTTON(app->saved), "Save");
        gtk_widget_set_size_request(app->saved, 200, 75);

        gtk_entry_set_placeholder_text(GTK_ENTRY(app->entry), "Enter the name.png of the save");
        gtk_widget_set_size_request(app->entry, 300, 50);

        gtk_button_set_label(GTK_BUTTON(app->bouton_retour_au_debut), "Back to start");
        gtk_widget_set_size_request(app->bouton_retour_au_debut,200, 75);

        gtk_fixed_put(GTK_FIXED(app->fixed), app->bouton_retour_au_debut, 1200, 500);
        gtk_fixed_put(GTK_FIXED(app->fixed), app->entry, 1250, 700);
        gtk_fixed_put(GTK_FIXED(app->fixed), app->saved, 1300, 800);


        g_signal_connect(G_OBJECT(app->bouton_retour_au_debut), "clicked", G_CALLBACK(debut), (gpointer)app);
        g_signal_connect(G_OBJECT(app->saved), "clicked", G_CALLBACK(enregistrer), (gpointer)app);

    }

    

    gtk_fixed_put(GTK_FIXED(app->fixed), app->label, 1200, 200);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100, 100);
    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);

    gtk_widget_show_all(app->window);
}

void generer_image(GtkWidget* window, gpointer data){

    g_element *app;
    app = (gpointer)data;

    app->gener = 1;

    fin_generation();

    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load("adresse_generer.png");

    IMG_SavePNG(image, "res/1Imagetoblack.png");

    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    

    gtk_container_remove(GTK_CONTAINER(app->window), app->fixed);

    //fin
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/1Imagetoblack.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    app->fixed = gtk_fixed_new();

    app->button[0] = gtk_button_new();
    app->label = gtk_label_new(NULL);

    gtk_label_set_text(GTK_LABEL(app->label), "\n       Here you can see the generate grid       \n");
    gtk_widget_set_name(app->label, "black_and_w");

    gtk_button_set_label(GTK_BUTTON(app->button[0]), "Next step");
    gtk_widget_set_size_request(app->button[0], 200, 75);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100, 100);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->label, 1200, 200);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[0], 1200, 500);

    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);

    gtk_widget_show_all(app->window);

    g_signal_connect(G_OBJECT(app->button[0]), "clicked", G_CALLBACK(rota_page2), (gpointer)app);

}

void enregistrer(GtkWidget* window, gpointer data){

    g_element *app;
    app = (gpointer)data;

    gchar* nom = gtk_entry_get_text(GTK_ENTRY(app->entry));

    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load("res/ImageFINALE.png");

    IMG_SavePNG(image, nom);

    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    gtk_container_remove(GTK_CONTAINER(app->window), app->fixed);

    app->label = gtk_label_new(NULL);
    app->fixed = gtk_fixed_new();

    gtk_label_set_text(GTK_LABEL(app->label), "\n       Image save       \n");
    gtk_widget_set_name(app->label, "black_and_w");

    app->bouton_retour_au_debut = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(app->bouton_retour_au_debut), "Back to start");
    gtk_widget_set_size_request(app->bouton_retour_au_debut,200, 75);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->label, 800, 200);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->bouton_retour_au_debut, 800, 500);

    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);

    gtk_widget_show_all(app->window);

    g_signal_connect(G_OBJECT(app->bouton_retour_au_debut), "clicked", G_CALLBACK(debut), (gpointer)app);

}

void suivant(GtkWidget* window, gpointer data){

    g_element *app;
    app = (gpointer)data;

    int nbr = 0;

    

    Uint8 couleurs[20][4] = {
        {255, 0,0, 15},
        {0, 255, 0, 15},
        {0, 0, 255, 15},
        {255, 255, 0, 15},
        {0, 255, 255, 15},
        {255, 0, 255, 15},
        {128, 0, 0, 15},
        {0, 128, 0, 15},
        {0, 0, 128, 15},
        {128, 128, 0, 15},
        {0, 128, 128, 15},
        {128, 0, 128, 15},
        {192, 192, 192, 15},
        {128, 128, 128, 15},
        {64, 64, 64, 15},
        {255, 165, 0, 15},
        {75, 0, 130, 15}, 
        {240,230, 140, 15},
        {123, 104, 238, 15},
        {0, 100, 0, 15}
    };

    app->bouton_retour_au_debut = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(app->bouton_retour_au_debut), "Back to start");
    gtk_widget_set_size_request(app->bouton_retour_au_debut,200, 75);

    

    for(int i = 0; i < app->taille_mots; i++){
        app->nouveau_mots_corriger[i] = gtk_entry_get_text(GTK_ENTRY(app->List_mots_corriger[i]));
    }



    for(int i=0;i<app->taille_mots;i++){

        int xdep=0;
        int ydep=0;
        int xarr=0;
        int yarr=0;


        if (solve("grid.txt",app->nouveau_mots_corriger[i],&xdep, &ydep , &xarr, &yarr) == 1){
            nbr += 1;
        }


        SDL_SetSurfaceBlendMode(app->imagedebut, SDL_BLENDMODE_BLEND);
        Uint32 color = SDL_MapRGBA(app->imagedebut->format, couleurs[i + app->total_de_mot][0], couleurs[i + app->total_de_mot][1], couleurs[i + app->total_de_mot][2], couleurs[i + app->total_de_mot][3]);
        drawLineFinal(app->imagedebut, app->xgrille+app->newcoord[ydep*app->nbcol+xdep]->x ,app->ygrille+app->newcoord[ydep*app->nbcol+xdep]->y ,
                        app->xgrille+app->newcoord[yarr*app->nbcol+xarr]->x ,app->ygrille+app->newcoord[yarr*app->nbcol+xarr]->y,15,color);


    }

    IMG_SavePNG(app->imagedebut, "res/ImageFINALE.png");

    gtk_container_remove(GTK_CONTAINER(app->window), app->fixed);

    

    //fin
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/ImageFINALE.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    

    app->label = gtk_label_new(NULL);
    app->fixed = gtk_fixed_new();

    app->entry = gtk_entry_new();
    app->saved = gtk_button_new();

    
    
    gtk_label_set_text(GTK_LABEL(app->label), "\n       You can see the result       \n");
    gtk_widget_set_name(app->label, "black_and_w");

    gtk_button_set_label(GTK_BUTTON(app->saved), "Save");
    gtk_widget_set_size_request(app->saved, 200, 75);

    gtk_entry_set_placeholder_text(GTK_ENTRY(app->entry), "Enter the name.png of the save");
    gtk_widget_set_size_request(app->entry, 300, 50);

    if (nbr != 0){
        app->message_pas_bon = gtk_label_new(NULL);
        gtk_label_set_text(GTK_LABEL(app->label), "\n       sorry we couldn't find all the words      \n");
        gtk_widget_set_name(app->label, "black_and_w");

        gtk_fixed_put(GTK_FIXED(app->fixed), app->message_pas_bon, 1100, 400);
    }

    gtk_fixed_put(GTK_FIXED(app->fixed), app->label, 1200, 200);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->bouton_retour_au_debut, 1300, 500);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100, 100);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->entry, 1250, 700);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->saved, 1300, 800);

    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);

    gtk_widget_show_all(app->window);

    g_signal_connect(G_OBJECT(app->bouton_retour_au_debut), "clicked", G_CALLBACK(debut), (gpointer)app);
    g_signal_connect(G_OBJECT(app->saved), "clicked", G_CALLBACK(enregistrer), (gpointer)app);

}



void probleme(GtkWidget* window, gpointer data){
    
    g_element *app;
    app = (gpointer)data;

    

    //----------------------------------------------------------------------

    gtk_container_remove(GTK_CONTAINER(app->window), app->fixed);

    char* buffer;
    GdkPixbuf *pixbuf;
    GdkPixbuf *pixbuf_mini;

    for(int i = 0; i < app->taille_mots; i++){
        app->List_mots_corriger[i] = gtk_entry_new();
        app->Nom_ancien_mots[i] = gtk_label_new(NULL);

        asprintf(&buffer, "Change %s by :", app->liste_mot_trouver[app->place_mot_pas_bon[i]]);

        gtk_label_set_text(GTK_LABEL(app->Nom_ancien_mots[i]), buffer);
        gtk_widget_set_name(app->Nom_ancien_mots[i], "black_and_w");


        //Pour l'iamge

        pixbuf = NULL;

        pixbuf = gdk_pixbuf_new_from_file(app->adresse_image_mots[app->place_mot_pas_bon[i]],NULL);

        pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 300, 40, GDK_INTERP_NEAREST);

        app->photo_mots_pas_trouver[i] = gtk_image_new_from_pixbuf(pixbuf_mini);

        //---------------------
        
    }

    app->fixed = gtk_fixed_new();
    app->button[0] = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(app->button[0]), "Submit");
    gtk_widget_set_size_request(app->button[0], 200, 75);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[0], 1200, 500);

    

    for (int i = 0; i < app->taille_mots; i++){
        gtk_fixed_put(GTK_FIXED(app->fixed), app->List_mots_corriger[i], 500, 200 + i*100);
        gtk_fixed_put(GTK_FIXED(app->fixed), app->Nom_ancien_mots[i], 100, 200 + i*100);
        gtk_fixed_put(GTK_FIXED(app->fixed), app->photo_mots_pas_trouver[i], 700, 200 + i*100);
    }

    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);

    gtk_widget_show_all(app->window);

    

    g_signal_connect(G_OBJECT(app->button[0]), "clicked", G_CALLBACK(suivant), (gpointer)app);



}

//fnct qui va resoudre le mot mêlés
//=============================================================================================

void fin(GtkWidget* window, gpointer data){

    g_element *app;
    app = (gpointer)data;

    //-----GRILLE----//
    int nbcol=0;
    int nbligne=0;
    struct Point** coordonateimage=NULL;

    SDL_Surface** tablettre=readlettermain(app->resu[0],&nbcol,&nbligne,&coordonateimage); //tablettre contienttoutesleslettre de gauche a droite et ensuite on descend t as capte


    SDL_Surface** newres=malloc(sizeof(SDL_Surface*)*nbcol*nbligne);
    for(int i=0;i<nbligne;i++){
        for(int j=0;j<nbcol;j++){
            char filename[50];
            snprintf(filename, sizeof(filename), "Lettre/%d,%d.png",i, j);
            IMG_SavePNG(tablettre[j * nbligne + i],filename);
            newres[i * nbcol + j] = tablettre[j * nbligne + i];

        }
    }

    struct Point** newcoord=malloc(sizeof(SDL_Surface*)*nbcol*nbligne);
    for(int i=0;i<nbligne;i++){
        for(int j=0;j<nbcol;j++){
            newcoord[i * nbcol + j] = coordonateimage[j * nbligne + i];
        }
    }




    free(tablettre);
    char filename[50];

    int nbmot;

    

    SDL_Surface** listemot=decoupelistemot(app->resu[1],&nbmot);
    //printf("nb mot : %d\n",nbmot);

    app->total_de_mot = nbmot;

    for(int i=0;i<nbmot;i++){
        int nbletter=0;
        SDL_Surface** lettermot=extractlettermain(listemot[i],&nbletter);

        SDL_Surface* lettre;
        for(int j=0;j<nbletter;j++){
            
            lettre=lettermot[j];
            char filename[50];
            snprintf(filename, sizeof(filename), "Lettremot/%d,%d.png",i, j);
            IMG_SavePNG(lettre,filename);
            //IMG_SavePNG(lettre,filename);
        }


    }

    app->liste_mot = malloc(sizeof(char*) * 0);
    app->liste_mot_trouver = malloc(sizeof(char *) * 1024);
    app->place_mot_pas_bon = malloc(sizeof(int) * 0);

    int cmpt = 0;

    Uint8 couleurs[20][4] = {
        {255, 0,0, 15},
        {0, 255, 0, 15},
        {0, 0, 255, 15},
        {255, 255, 0, 15},
        {0, 255, 255, 15},
        {255, 0, 255, 15},
        {128, 0, 0, 15},
        {0, 128, 0, 15},
        {0, 0, 128, 15},
        {128, 128, 0, 15},
        {0, 128, 128, 15},
        {128, 0, 128, 15},
        {192, 192, 192, 15},
        {128, 128, 128, 15},
        {64, 64, 64, 15},
        {255, 165, 0, 15},
        {75, 0, 130, 15}, 
        {240,230, 140, 15},
        {123, 104, 238, 15},
        {0, 100, 0, 15}
    };

    app->newcoord = newcoord;
    app->nbcol = nbcol;

    //printf("C la ?\n");


    app->xgrille-=0;
    app->ygrille+=0;
    to28centerized(newres,nbligne*nbcol); //centre et zoom les image en 28x28 pour l ia
    test(newres,nbcol,nbligne);
    char mot[50];
    for(int i=0;i<nbmot;i++){


        int nbletter=0;
        SDL_Surface** lettermot=extractlettermain(listemot[i],&nbletter);
        to28centerized(lettermot,nbletter);
        SDL_Surface* lettre;
        for(int j=0;j<nbletter;j++){
            
            lettre=lettermot[j];
            //sprintf(filename, "LettresMot/(%d %d).png", i, j);
            char let=testlettreunique(lettre);
            mot[j]=let;
        }
        mot[nbletter]=0;
        int xdep=0;
        int ydep=0;
        int xarr=0;
        int yarr=0;

        if(solve("grid.txt",mot,&xdep, &ydep , &xarr, &yarr) == 1){
            app->liste_mot = realloc(app->liste_mot, (cmpt+1) * (sizeof(char*)));
            app->liste_mot[cmpt] = strdup(mot);
            app->place_mot_pas_bon[cmpt] = i;
            IMG_SavePNG(listemot[i],app->adresse_image_mots[i]);
            
            cmpt += 1;

        }

        app->liste_mot_trouver[i] = strdup(mot);


        SDL_SetSurfaceBlendMode(app->imagedebut, SDL_BLENDMODE_BLEND);
        Uint32 color = SDL_MapRGBA(app->imagedebut->format, couleurs[i][0], couleurs[i][1], couleurs[i][2], couleurs[i][3]);
        drawLineFinal(app->imagedebut, app->xgrille+newcoord[ydep*nbcol+xdep]->x ,app->ygrille+newcoord[ydep*nbcol+xdep]->y ,
                        app->xgrille+newcoord[yarr*nbcol+xarr]->x ,app->ygrille+newcoord[yarr*nbcol+xarr]->y,15,color);


        


    }

    

    

    IMG_SavePNG(app->imagedebut, "res/ImageFINALE.png");

    //--------------//

    gtk_container_remove(GTK_CONTAINER(app->window), app->fixed);

    //fin
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/ImageFINALE.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    

    app->label = gtk_label_new(NULL);
    app->fixed = gtk_fixed_new();
    

    gtk_label_set_text(GTK_LABEL(app->label), "\n       You can see the result       \n");
    gtk_widget_set_name(app->label, "black_and_w");

    app->taille_mots = cmpt;

    ///PB avec des mots

    if(cmpt > 0){
        app->message_pas_bon = gtk_label_new(NULL);

        char* mot_pas_found = malloc(sizeof(char*) * 1024);

        mot_pas_found[0] = '\0';

        strcat(mot_pas_found, "\n       Here are the words where there is a problem       \n\n");

        strcat(mot_pas_found, "       ");

        for (int i = 0; i < cmpt; i++){

            strcat(mot_pas_found, "-");
            strcat(mot_pas_found, app->liste_mot[i]);
            
            strcat(mot_pas_found, "\n       ");



        }

        strcat(mot_pas_found, "       \n");

        gtk_label_set_text(GTK_LABEL(app->message_pas_bon), mot_pas_found);
        gtk_widget_set_name(app->message_pas_bon, "black_and_w");

        free(mot_pas_found);

        gtk_fixed_put(GTK_FIXED(app->fixed), app->message_pas_bon, 1100, 400);

        app->button[0] = gtk_button_new();

        gtk_button_set_label(GTK_BUTTON(app->button[0]), "Correct the words");
        gtk_widget_set_size_request(app->button[0], 200, 100);

        gtk_fixed_put(GTK_FIXED(app->fixed), app->button[0], 1200, 700);

        g_signal_connect(G_OBJECT(app->button[0]), "clicked", G_CALLBACK(probleme), (gpointer)app);
    }

    else{

        app->bouton_retour_au_debut = gtk_button_new();

        app->entry = gtk_entry_new();
        app->saved = gtk_button_new();

        gtk_button_set_label(GTK_BUTTON(app->saved), "Save");
        gtk_widget_set_size_request(app->saved, 200, 75);

        gtk_entry_set_placeholder_text(GTK_ENTRY(app->entry), "Enter the name.png of the save");
        gtk_widget_set_size_request(app->entry, 300, 50);

        gtk_button_set_label(GTK_BUTTON(app->bouton_retour_au_debut), "Back to start");
        gtk_widget_set_size_request(app->bouton_retour_au_debut,200, 75);

        gtk_fixed_put(GTK_FIXED(app->fixed), app->bouton_retour_au_debut, 1200, 500);
        gtk_fixed_put(GTK_FIXED(app->fixed), app->entry, 1250, 700);
        gtk_fixed_put(GTK_FIXED(app->fixed), app->saved, 1300, 800);


        g_signal_connect(G_OBJECT(app->bouton_retour_au_debut), "clicked", G_CALLBACK(debut), (gpointer)app);
        g_signal_connect(G_OBJECT(app->saved), "clicked", G_CALLBACK(enregistrer), (gpointer)app);

    }

    

    gtk_fixed_put(GTK_FIXED(app->fixed), app->label, 1200, 200);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100, 100);
    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);

    gtk_widget_show_all(app->window);

    

    

}

void detectg(GtkWidget* window, gpointer data){

    g_element *app;
    app = (gpointer)data;

    app->resu=detectgridandword(app->res,&(app->xgrille),&(app->ygrille));
    IMG_SavePNG(app->resu[0], "res/2Imagejustegrille.png"); //Grille
    app->resu[1]=zoomimage(app->resu[1],app->imagedebut);
    IMG_SavePNG(app->resu[1], "res/2ImagemotZOOMavanttoblack.png"); //Mot zoomer
    app->resu[1]=toblackmot(app->resu[1]);

    IMG_SavePNG(app->resu[1], "res/2ImagemotZOOM.png"); //Mot zoomer

    gtk_container_remove(GTK_CONTAINER(app->window), app->fixed);


    //grille
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/2Imagejustegrille.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 700, 700, GDK_INTERP_NEAREST);

    app->grille = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    //mots
    //=====================================================================
    
    pixbuf = NULL;

    error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/2ImagemotZOOM.png", &error);

    pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 200, 500, GDK_INTERP_NEAREST);

    app->mots = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    app->fixed = gtk_fixed_new();
    app->button[0] = gtk_button_new();
    app->label = gtk_label_new(NULL);
    app->avertissement = gtk_label_new(NULL);
    app->button[1] = gtk_button_new();

    gtk_label_set_text(GTK_LABEL(app->label), "\n       Here you can see the images       \n          of the grid and the words       \n");
    gtk_widget_set_name(app->label, "black_and_w");

    gtk_label_set_text(GTK_LABEL(app->avertissement), "\n      The next stage may take some time,       \n                      so don't worry.\n");
    gtk_widget_set_name(app->avertissement, "black_and_w");

    gtk_button_set_label(GTK_BUTTON(app->button[0]), "Next");
    gtk_widget_set_size_request(app->button[0], 200, 100);

    gtk_button_set_label(GTK_BUTTON(app->button[1]), "Help");
    gtk_widget_set_size_request(app->button[1], 200, 75);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->label, 1185, 300);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[0], 1300, 450);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->avertissement, 1150, 575);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->grille, 100, 100);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->mots, 850, 100);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[1], 1400, 800);

    app->bouton_retour_au_debut = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(app->bouton_retour_au_debut), "Back to start");
    gtk_widget_set_size_request(app->bouton_retour_au_debut,200, 75);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->bouton_retour_au_debut, 1150, 800);


    

    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);

    gtk_widget_show_all(app->window);

    //Signaux
    g_signal_connect(G_OBJECT(app->button[0]), "clicked", G_CALLBACK(fin), (gpointer)app);
    g_signal_connect(G_OBJECT(app->bouton_retour_au_debut), "clicked", G_CALLBACK(debut), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[1]), "clicked", G_CALLBACK(help), (gpointer)app);


}



//=============================================================================================


void load_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    g_object_unref(provider);
}


//rotation
//=============================================================================================
//=============================================================================================

SDL_Surface* rotate_image(SDL_Surface* image, double angle) {
    angle = fmod(angle, 360.0);
    if (angle < 0) angle += 360.0;


    double radians = angle * M_PI / 180.0;


    int new_width = (int)ceil(fabs(image->w * cos(radians)) + fabs(image->h * sin(radians)));
    int new_height = (int)ceil(fabs(image->h * cos(radians)) + fabs(image->w * sin(radians)));


    SDL_Surface* rotated_surface = SDL_CreateRGBSurfaceWithFormat(0, new_width, new_height, 32, image->format->format);
    if (!rotated_surface) {
        printf("Erreur de création de la surface : %s\n", SDL_GetError());
        return NULL;
    }


    int cx = image->w / 2;
    int cy = image->h / 2;
    int ncx = new_width / 2;
    int ncy = new_height / 2;

    Uint32* pixels = (Uint32*)image->pixels;
    Uint32* new_pixels = (Uint32*)rotated_surface->pixels;

    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {

            int original_x = (int)((x - ncx) * cos(-radians) - (y - ncy) * sin(-radians) + cx);
            int original_y = (int)((x - ncx) * sin(-radians) + (y - ncy) * cos(-radians) + cy);


            if (original_x >= 0 && original_x < image->w && original_y >= 0 && original_y < image->h) {
                new_pixels[y * new_width + x] = pixels[original_y * image->w + original_x];
            } else {
                new_pixels[y * new_width + x] = SDL_MapRGB(image->format, 255,255, 255);
            }
        }
    }
    return rotated_surface;
}

void rota_droite(GtkWidget* window, gpointer data) {

    
    g_element *app;
    app = (gpointer)data;

    const gchar* angle_donne = gtk_entry_get_text(GTK_ENTRY(app->entry));
    app->angle += atof(angle_donne);

    //fnct de base
    //----------
    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load("res/1Imagetoblack.png");

    SDL_Surface* rotated_image = rotate_image(image, app->angle);

    IMG_SavePNG(rotated_image, "res/2Imagetoblack.png");

    app->res = IMG_Load("res/2Imagetoblack.png");

    // Libération des surfaces et fermeture de SDL
    SDL_FreeSurface(image);
    SDL_FreeSurface(rotated_image);
    IMG_Quit();
    SDL_Quit();

    //----------

    
    gtk_container_remove(GTK_CONTAINER(app->fixed), app->image);
    

    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/2Imagetoblack.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================


    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100,100);

    gtk_widget_show_all(app->window);

}

void rota_gauche(GtkWidget* window, gpointer data) {

    
    g_element *app;
    app = (gpointer)data;

    const gchar* angle_donne = gtk_entry_get_text(GTK_ENTRY(app->entry));
    app->angle -= atof(angle_donne);

    //fnct de base
    //----------
    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load("res/1Imagetoblack.png");

    SDL_Surface* rotated_image = rotate_image(image, app->angle);

    IMG_SavePNG(rotated_image, "res/2Imagetoblack.png");

    app->res = IMG_Load("res/2Imagetoblack.png");

    // Libération des surfaces et fermeture de SDL
    SDL_FreeSurface(image);
    SDL_FreeSurface(rotated_image);
    IMG_Quit();
    SDL_Quit();

    //----------

    
    gtk_container_remove(GTK_CONTAINER(app->fixed), app->image);
    

    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/2Imagetoblack.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100,100);

    gtk_widget_show_all(app->window);

}

//=============================================================================================
//=============================================================================================

void image_1_1(GtkWidget* window, gpointer data){
    g_element *app;
    app = (gpointer)data;

    gtk_container_remove(GTK_CONTAINER(app->fixed), app->image);
    app->id = 0;
    app->angle = 0;

    

    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file (app->adresse[0], &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100,100);

    //===

    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load(app->adresse[0]);

    IMG_SavePNG(image, app->adresse_rota[0]);

    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    //===


    gtk_widget_show_all(app->window);
}

void image_1_2(GtkWidget* window, gpointer data){
    g_element *app;
    app = (gpointer)data;

    gtk_container_remove(GTK_CONTAINER(app->fixed), app->image);
    app->id = 1;
    app->angle = 0;

    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file (app->adresse[1], &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100,100);


    //===

    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load(app->adresse[1]);

    IMG_SavePNG(image, app->adresse_rota[1]);

    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    //===

    gtk_widget_show_all(app->window);
}

void image_2_1(GtkWidget* window, gpointer data){
    g_element *app;
    app = (gpointer)data;

    gtk_container_remove(GTK_CONTAINER(app->fixed), app->image);
    app->id = 2;
    app->angle = 0;

    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file (app->adresse[2], &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100,100);

    //===

    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load(app->adresse[2]);

    IMG_SavePNG(image, app->adresse_rota[2]);

    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    //===

    gtk_widget_show_all(app->window);
}

void image_2_2(GtkWidget* window, gpointer data){
    g_element *app;
    app = (gpointer)data;

    gtk_container_remove(GTK_CONTAINER(app->fixed), app->image);
    app->id = 3;
    app->angle = 0;

    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file (app->adresse[3], &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100,100);

    //===

    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load(app->adresse[3]);

    IMG_SavePNG(image, app->adresse_rota[3]);

    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    //===

    gtk_widget_show_all(app->window);
}

void rota_auto(GtkWidget* window, gpointer data){

    g_element *app;
    app = (gpointer)data;

    gtk_container_remove(GTK_CONTAINER(app->fixed), app->image);

    app->angle -= rotate_auto_stylee("res/1Imagetoblack.png", "res/2Imagetoblack.png",&(app->imagedebut));

    app->res = IMG_Load("res/2Imagetoblack.png");

    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/2Imagetoblack.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100,100);

    gtk_widget_show_all(app->window);

}

void rota_page2(GtkWidget* window, gpointer data){

    g_element *app;
    app = (gpointer)data;

    //pour le black

    if (app->gener == 0){
        app->imagedebut=IMG_Load(app->adresse[app->id]);

        app->res=toblack(app->adresse[app->id]);
    }

    else{
        app->imagedebut=IMG_Load("res/1Imagetoblack.png");

        app->res=toblack("res/1Imagetoblack.png");
    }
    
    IMG_SavePNG(app->res, "res/1Imagetoblack.png"); //Image black

    //-----------

    gtk_container_remove(GTK_CONTAINER(app->window), app->fixed);

    app->adresse_rota[0] = "Images/level_1_image_1_rota.png";
    app->adresse_rota[1] = "Images/level_1_image_2_rota.png";
    app->adresse_rota[2] = "Images/level_2_image_1_rota.png";
    app->adresse_rota[3] = "Images/level_2_image_2_rota.png";

    app->label = gtk_label_new(NULL);
    

    app->button[0] = gtk_button_new();//lancement du programme

    //label

    gtk_label_set_text(GTK_LABEL(app->label), "Click on the image buttons to reset");
    gtk_widget_set_name(app->label, "consigne");

    gtk_button_set_label(GTK_BUTTON(app->button[0]), "Detection of the grid");
    gtk_widget_set_size_request(app->button[0], 200, 75);



    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("res/1Imagetoblack.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================


    app->fixed = gtk_fixed_new();

    app->button[5] = gtk_button_new();//bouton pour la rota gauche
    app->button[6] = gtk_button_new();//bouton pour la rota droite
    app->button[8] = gtk_button_new();//bouton pou la rota automatic en bien

    app->entry = gtk_entry_new();

    app->angle = 0.0f;

    //ppour bouton rota gauche
    gtk_button_set_label(GTK_BUTTON(app->button[5]), "rotate the image to the left");
    gtk_widget_set_size_request(app->button[5], 125, 50);

    //ppour bouton rota droite
    gtk_button_set_label(GTK_BUTTON(app->button[6]), "rotate the image to the right");
    gtk_widget_set_size_request(app->button[6], 125, 50);

    //ppour bouton rota automaitco
    gtk_button_set_label(GTK_BUTTON(app->button[8]), "rotate the image automatically");
    gtk_widget_set_size_request(app->button[8], 125, 50);

    //pour entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(app->entry), "Enter the angle of rotation");
    gtk_widget_set_size_request(app->entry, 200, 50);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[5], 975, 500);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[6], 1325, 500);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[8], 1150, 600);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[0], 1150, 200);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->entry, 1200, 400);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100, 100);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->label, 1175, 460);

    
    app->bouton_retour_au_debut = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(app->bouton_retour_au_debut), "Back to start");
    gtk_widget_set_size_request(app->bouton_retour_au_debut,200, 75);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->bouton_retour_au_debut, 1200, 700);


    //Container
    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);


    //Afficher les elements
    gtk_widget_show_all(app->window);

    g_signal_connect(G_OBJECT(app->button[8]), "clicked", G_CALLBACK(rota_auto), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[5]), "clicked", G_CALLBACK(rota_gauche), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[6]), "clicked", G_CALLBACK(rota_droite), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[0]), "clicked", G_CALLBACK(detectg), (gpointer)app);
    g_signal_connect(G_OBJECT(app->bouton_retour_au_debut), "clicked", G_CALLBACK(debut), (gpointer)app);
    



}

void debut(GtkWidget* window, gpointer data){

    g_element *app;
    app = (gpointer)data;

    gtk_container_remove(GTK_CONTAINER(app->window), app->fixed);

    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("Images/level_1_image_1.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================


    app->fixed = gtk_fixed_new();

    app->gener = 0;

    app->adresse[0] = "Images/level_1_image_1.png";
    app->adresse[1] = "Images/level_1_image_2.png";
    app->adresse[2] = "Images/level_2_image_1.png";
    app->adresse[3] = "Images/level_2_image_2.png";

    app->adresse_rota[0] = "Images/level_1_image_1_rota.png";
    app->adresse_rota[1] = "Images/level_1_image_2_rota.png";
    app->adresse_rota[2] = "Images/level_2_image_1_rota.png";
    app->adresse_rota[3] = "Images/level_2_image_2_rota.png";

    app->adresse_image_mots[0] = "Image/Mot_1";
    app->adresse_image_mots[1] = "Image/Mot_2";
    app->adresse_image_mots[2] = "Image/Mot_3";
    app->adresse_image_mots[3] = "Image/Mot_4";
    app->adresse_image_mots[4] = "Image/Mot_5";
    app->adresse_image_mots[5] = "Image/Mot_6";
    app->adresse_image_mots[6] = "Image/Mot_7";
    app->adresse_image_mots[7] = "Image/Mot_8";
    app->adresse_image_mots[8] = "Image/Mot_9";
    app->adresse_image_mots[9] = "Image/Mot_10";
    app->adresse_image_mots[10] = "Image/Mot_11";
    app->adresse_image_mots[11] = "Image/Mot_12";
    app->adresse_image_mots[12] = "Image/Mot_13";
    app->adresse_image_mots[13] = "Image/Mot_14";
    app->adresse_image_mots[14] = "Image/Mot_15";
    app->adresse_image_mots[15] = "Image/Mot_16";
    app->adresse_image_mots[16] = "Image/Mot_17";
    app->adresse_image_mots[17] = "Image/Mot_18";
    app->adresse_image_mots[18] = "Image/Mot_19";
    app->adresse_image_mots[19] = "Image/Mot_20";

    app->button[0] = gtk_button_new();//lancement du programme
    app->button[1] = gtk_button_new();//image 1 level 1
    app->button[2] = gtk_button_new();//image 2 level 1
    app->button[3] = gtk_button_new();//image 1 level 2
    app->button[4] = gtk_button_new();//image 2 level 2
    

    app->generer_image = gtk_button_new();

    app->avertissement = gtk_label_new(NULL);
    

    
    app->id = 0;




    //mettre les images a 0



    //Caracteristiques des parametres
    //=====================================================================

    

    //Pour "button"
    //Pour lancement du programme
    gtk_button_set_label(GTK_BUTTON(app->button[0]), "Put the image in black and white");
    gtk_widget_set_size_request(app->button[0], 300, 75);


    //Pour Bouton image 1
    gtk_button_set_label(GTK_BUTTON(app->button[1]), "image 1 level 1");
    gtk_widget_set_size_request(app->button[1], 100, 50);

    //Pour Bouton image 2
    gtk_button_set_label(GTK_BUTTON(app->button[2]), "image 2 level 1");
    gtk_widget_set_size_request(app->button[2], 100, 50);

    //Pour Bouton image 3
    gtk_button_set_label(GTK_BUTTON(app->button[3]), "image 2 level 4");
    gtk_widget_set_size_request(app->button[3], 100, 50);

    //Pour Bouton image 4
    gtk_button_set_label(GTK_BUTTON(app->button[4]), "image 2 level 2");
    gtk_widget_set_size_request(app->button[4], 100, 50);

    gtk_label_set_text(GTK_LABEL(app->avertissement), "\n      The next stage may take some time,       \n                      so don't worry.\n");
    gtk_widget_set_name(app->avertissement, "black_and_w");

    gtk_button_set_label(GTK_BUTTON(app->generer_image), "Generate an image");
    gtk_widget_set_size_request(app->generer_image, 100, 50);

    //Pour "fixed"
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[0], 1200, 200);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[1], 75, 880);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[2], 275, 880);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[3], 475, 880);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[4], 675, 880);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->generer_image, 875, 880);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100, 100);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->avertissement, 1150, 575);
    
    

    //=====================================================================
    //iniitialiser a 0 les images de sortie;
    //----------
    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load(app->adresse[0]);

    IMG_SavePNG(image, app->adresse_rota[0]);

    image = IMG_Load(app->adresse[1]);

    IMG_SavePNG(image, app->adresse_rota[1]);

    image = IMG_Load(app->adresse[2]);

    IMG_SavePNG(image, app->adresse_rota[2]);

    image = IMG_Load(app->adresse[3]);

    IMG_SavePNG(image, app->adresse_rota[3]);

    // Libération des surfaces et fermeture de SDL
    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    //----------

    //=====================================================================

    //Container
    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);


    //Afficher les elements
    gtk_widget_show_all(app->window);

    //Signaux
    g_signal_connect(G_OBJECT(app->button[1]), "clicked", G_CALLBACK(image_1_1), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[2]), "clicked", G_CALLBACK(image_1_2), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[3]), "clicked", G_CALLBACK(image_2_1), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[4]), "clicked", G_CALLBACK(image_2_2), (gpointer)app);
    g_signal_connect(G_OBJECT(app->generer_image), "clicked", G_CALLBACK(generer_image), (gpointer)app);

    g_signal_connect(G_OBJECT(app->button[0]), "clicked", G_CALLBACK(rota_page2), (gpointer)app);

}



int main(int argc, char** argv){

    gtk_init(&argc,&argv);

    load_css();

    g_element *app = (g_element*)g_malloc(sizeof(g_element));

    //Image
    //=====================================================================
    
    GdkPixbuf *pixbuf = NULL;

    GError *error = NULL; 
    pixbuf = gdk_pixbuf_new_from_file ("Images/level_1_image_1.png", &error);

    GdkPixbuf *pixbuf_mini = NULL;

    pixbuf_mini = gdk_pixbuf_scale_simple (pixbuf, 800, 700, GDK_INTERP_NEAREST);

    app->image = gtk_image_new_from_pixbuf (pixbuf_mini);

    //=====================================================================


    app->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    app->fixed = gtk_fixed_new();

    app->gener = 0;

    app->adresse[0] = "Images/level_1_image_1.png";
    app->adresse[1] = "Images/level_1_image_2.png";
    app->adresse[2] = "Images/level_2_image_1.png";
    app->adresse[3] = "Images/level_2_image_2.png";

    app->adresse_rota[0] = "Images/level_1_image_1_rota.png";
    app->adresse_rota[1] = "Images/level_1_image_2_rota.png";
    app->adresse_rota[2] = "Images/level_2_image_1_rota.png";
    app->adresse_rota[3] = "Images/level_2_image_2_rota.png";

    app->adresse_image_mots[0] = "Image/Mot_1";
    app->adresse_image_mots[1] = "Image/Mot_2";
    app->adresse_image_mots[2] = "Image/Mot_3";
    app->adresse_image_mots[3] = "Image/Mot_4";
    app->adresse_image_mots[4] = "Image/Mot_5";
    app->adresse_image_mots[5] = "Image/Mot_6";
    app->adresse_image_mots[6] = "Image/Mot_7";
    app->adresse_image_mots[7] = "Image/Mot_8";
    app->adresse_image_mots[8] = "Image/Mot_9";
    app->adresse_image_mots[9] = "Image/Mot_10";
    app->adresse_image_mots[10] = "Image/Mot_11";
    app->adresse_image_mots[11] = "Image/Mot_12";
    app->adresse_image_mots[12] = "Image/Mot_13";
    app->adresse_image_mots[13] = "Image/Mot_14";
    app->adresse_image_mots[14] = "Image/Mot_15";
    app->adresse_image_mots[15] = "Image/Mot_16";
    app->adresse_image_mots[16] = "Image/Mot_17";
    app->adresse_image_mots[17] = "Image/Mot_18";
    app->adresse_image_mots[18] = "Image/Mot_19";
    app->adresse_image_mots[19] = "Image/Mot_20";

    app->button[0] = gtk_button_new();//lancement du programme
    app->button[1] = gtk_button_new();//image 1 level 1
    app->button[2] = gtk_button_new();//image 2 level 1
    app->button[3] = gtk_button_new();//image 1 level 2
    app->button[4] = gtk_button_new();//image 2 level 2

    app->generer_image = gtk_button_new();
    

    app->avertissement = gtk_label_new(NULL);
    

    
    app->id = 0;




    //mettre les images a 0



    //Caracteristiques des parametres
    //=====================================================================

    //Pour la fenetre (window)
    gtk_window_set_decorated (GTK_WINDOW (app->window), TRUE);
    gtk_window_maximize (GTK_WINDOW (app->window));
    gtk_window_set_type_hint (GTK_WINDOW (app->window), GDK_WINDOW_TYPE_HINT_NORMAL);
    gtk_window_set_title(GTK_WINDOW(app->window), "Word search solver program");

    

    //Pour "button"
    //Pour lancement du programme
    gtk_button_set_label(GTK_BUTTON(app->button[0]), "Put the image in black and white");
    gtk_widget_set_size_request(app->button[0], 300, 75);


    //Pour Bouton image 1
    gtk_button_set_label(GTK_BUTTON(app->button[1]), "image 1 level 1");
    gtk_widget_set_size_request(app->button[1], 100, 50);

    //Pour Bouton image 2
    gtk_button_set_label(GTK_BUTTON(app->button[2]), "image 2 level 1");
    gtk_widget_set_size_request(app->button[2], 100, 50);

    //Pour Bouton image 3
    gtk_button_set_label(GTK_BUTTON(app->button[3]), "image 2 level 4");
    gtk_widget_set_size_request(app->button[3], 100, 50);

    //Pour Bouton image 4
    gtk_button_set_label(GTK_BUTTON(app->button[4]), "image 2 level 2");
    gtk_widget_set_size_request(app->button[4], 100, 50);

    //Pour Bouton generer image
    gtk_button_set_label(GTK_BUTTON(app->generer_image), "Generate an image");
    gtk_widget_set_size_request(app->generer_image, 100, 50);

    gtk_label_set_text(GTK_LABEL(app->avertissement), "\n      The next stage may take some time,       \n                      so don't worry.\n");
    gtk_widget_set_name(app->avertissement, "black_and_w");

    //Pour "fixed"
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[0], 1200, 200);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[1], 75, 880);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[2], 275, 880);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[3], 475, 880);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->button[4], 675, 880);
    gtk_fixed_put(GTK_FIXED(app->fixed), app->generer_image, 875, 880);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->image, 100, 100);

    gtk_fixed_put(GTK_FIXED(app->fixed), app->avertissement, 1150, 575);
    
    

    //=====================================================================
    //iniitialiser a 0 les images de sortie;
    //----------
    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* image = IMG_Load(app->adresse[0]);

    IMG_SavePNG(image, app->adresse_rota[0]);

    image = IMG_Load(app->adresse[1]);

    IMG_SavePNG(image, app->adresse_rota[1]);

    image = IMG_Load(app->adresse[2]);

    IMG_SavePNG(image, app->adresse_rota[2]);

    image = IMG_Load(app->adresse[3]);

    IMG_SavePNG(image, app->adresse_rota[3]);

    // Libération des surfaces et fermeture de SDL
    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    //----------

    //=====================================================================

    //Container
    gtk_container_add(GTK_CONTAINER(app->window), app->fixed);


    //Afficher les elements
    gtk_widget_show_all(app->window);

    //Signaux
    g_signal_connect(G_OBJECT(app->window), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(app->button[1]), "clicked", G_CALLBACK(image_1_1), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[2]), "clicked", G_CALLBACK(image_1_2), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[3]), "clicked", G_CALLBACK(image_2_1), (gpointer)app);
    g_signal_connect(G_OBJECT(app->button[4]), "clicked", G_CALLBACK(image_2_2), (gpointer)app);

    g_signal_connect(G_OBJECT(app->button[0]), "clicked", G_CALLBACK(rota_page2), (gpointer)app);

    g_signal_connect(G_OBJECT(app->generer_image), "clicked", G_CALLBACK(generer_image), (gpointer)app);
    

    gtk_main();

    return 0;
}
