#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void to_maj(char* mot) {
    int i = 0;
    while (mot[i] != '\0') {
        if (mot[i] >= 'a' && mot[i] <= 'z') {
            mot[i] = mot[i] - 'a' + 'A';  // Convertit en majuscule
        }
        i++;
    }
}
char* read_file_to_string(const char* filename) {
    int fd = open(filename, O_RDONLY); 
    if (fd == -1) {
        return NULL;
    }

    char* tab = malloc(1000);
    if (tab == NULL) {
        return NULL;
    }

    size_t bytes_read = read(fd, tab, 1000);
    
    tab[bytes_read] = '\0'; 

    close(fd); 
    return tab;
}

int* word_find(char* grid,int i,int j,char* word,int lenword,int ligne , int col){

    //Noth
    int c=0;
    int inord=i;
    while(inord>=0 && c<lenword && grid[inord*col + j]==word[c]){
        inord--;
        c++;
    }
    if(c==lenword){

        int* coord=malloc(2*sizeof(int));
        coord[0]=inord+1;
        coord[1]=j;
        return coord;
    }

    //SOUTH
    c=0;
    int isouth=i;
    while(isouth<ligne && c<lenword && grid[isouth*col + j]==word[c]){
        isouth++;
        c++;
    }
    if(c==lenword){

        int* coord=malloc(2*sizeof(int));
        coord[0]=isouth-1;
        coord[1]=j;
        return coord;
    }

    //EAST
    c=0;
    int jeast=j;
    while(jeast<col && c<lenword && grid[i*col + jeast]==word[c]){
        jeast++;
        c++;
    }
    if(c==lenword){
        int* coord=malloc(2*sizeof(int));
        coord[0]=i;
        coord[1]=jeast-1;
        return coord;
    }


    //WEST
    c=0;
    int jw=j;
    while(jw>=0 && c<lenword && grid[i*col + jw]==word[c]){
        jw--;
        c++;
    }
    if(c==lenword){
        int* coord=malloc(2*sizeof(int));
        coord[0]=i;
        coord[1]=jw+1;
        return coord;
    }

    //Noth-WEST
    c=0;
    jw=j;
    int in=i;
    while(jw>=0 && in>=0 && c<lenword && grid[in*col + jw]==word[c]){
        jw--;
        in--;
        c++;
    }
    if(c==lenword){
        int* coord=malloc(2*sizeof(int));
        coord[0]=in+1;
        coord[1]=jw+1;
        return coord;
    }

    //Noth-EST
    c=0;
    int je=j;
    in=i;
    while(je<col && in >=0 && c<lenword && grid[in*col + je]==word[c]){
        je++;
        in--;
        c++;
    }
    if(c==lenword){
        int* coord=malloc(2*sizeof(int));
        coord[0]=in+1;
        coord[1]=je-1;
        return coord;
    }

    //South-WEST
    c=0;
    jw=j;
    int is=i;
    while(jw>=0 && is<ligne && c<lenword && grid[is*col + jw]==word[c]){
        jw--;
        is++;
        c++;
    }
    if(c==lenword){
        int* coord=malloc(2*sizeof(int));
        coord[0]=is-1;
        coord[1]=jw+1;  
        return coord;
    }

    //South-EST
    c=0;
    je=j;
    is=i;
    while(je<col && is<ligne && c<lenword && grid[is*col + je]==word[c]){
        je++;
        is++;
        c++;
    }
    if(c==lenword){
        int* coord=malloc(2*sizeof(int));
        coord[0]=is-1;
        coord[1]=je-1;
        return coord;
    }
    int* resf=malloc(ligne*col*sizeof(char));

    resf[0]=-1;
    resf[1]=-1;

    return resf;


}


int solve(char* file, char* mot,int* xdep,int* ydep , int* xarr, int* yarr) {



    char* tab = read_file_to_string(file);

    if (tab == NULL) {
        return EXIT_FAILURE;
    }


    int ligne = 0;
    int col = 0;
 

    for (int i = 0; tab[i] != '\0'; i++) {
        if (tab[i] == '\n') {
            ligne++;
            
        }
        if(tab[i] == '\r'){
            continue;
        }
        else{
            if (ligne==0) {
                col++; ; 

            }
        }
        
    } 

    char* word=mot;
    to_maj(word);

    int indextab=0;

    char* grid=malloc(ligne*col*sizeof(char));
    for(int i=0;i<ligne;i++){
        for(int j=0;j<col;j++){
            while (tab[indextab]<'A' || tab[indextab]>'Z'){
                indextab++;
            }
            grid[i*col+j]=tab[indextab];
            indextab++;
        }
        
    }
    int lenword=strlen(word);
    for(int i=0;i<ligne;i++){
        for(int j=0;j<col;j++){
            if(grid[i*col+j]==word[0] ){
                int* cord=word_find(grid,i,j,word,lenword,ligne,col);

                if(cord[0]!=-1){
                    *xdep=j;
                    *ydep=i;

                    *xarr=cord[1];
                    *yarr=cord[0];
                    free(grid);
                    return 0;
                }
                
            }
        }
    }
    

    free(grid);
    return 1;

}