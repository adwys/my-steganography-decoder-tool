#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

struct bit{
    unsigned char a:1;
    unsigned char b:1;
    unsigned char c:1;
    unsigned char d:1;
    unsigned char e:1;
    unsigned char f:1;
    unsigned char g:1;
    unsigned char h:1;
};

union bit_set{
    unsigned short c;
    unsigned char a;
    struct bit b;
};

int decode(const char * filename, char *txt, int size){
    if(filename==NULL||txt==NULL||size<1)return 1;
    FILE *f;
    union bit_set x,y;
    f = fopen(filename, "r");
    if(f==NULL)return 2;
    int i=0,j=0;
    do{
        if(!fscanf(f, "%hu",&x.c)){
            fclose(f);
            return 3;
        }
        if(i%8==0)y.b.h=x.b.a;
        else if(i%8==1)y.b.g=x.b.a;
        else if(i%8==2)y.b.f=x.b.a;
        else if(i%8==3)y.b.e=x.b.a;
        else if(i%8==4)y.b.d=x.b.a;
        else if(i%8==5)y.b.c=x.b.a;
        else if(i%8==6)y.b.b=x.b.a;
        else if(i%8==7)y.b.a=x.b.a;
        i++;
        if(i%8==0){
            if(y.a>127){fclose(f);return 3;}
            *(txt+j)=y.a;
            j++;
        }
    }while(!feof(f) && j<size);
    if(j != size){fclose(f);return 3;}
    *(txt+j-1) = '\0';
    fclose(f);
    return 0;
}

int encode(const char *input, char *txt, const char *output){

    if(input==NULL || txt==NULL || output==NULL)return 1;
    char e;
    FILE *f,*fp;
    union bit_set x,y;
    f = fopen(input, "r");
    if(f==NULL)return 2;
    fp = fopen(output, "w");
    if(fp==NULL){fclose(f);return 4;}
    int i=0,j=0;
    do{
        x.a=*(txt+j);
        if(!(fscanf(f,"%hhu",&y.a))){
            fclose(f);fclose(fp);return 3;
        }
        if(i%8==0)y.b.a=x.b.h;
        else if(i%8==1)y.b.a=x.b.g;
        else if(i%8==2)y.b.a=x.b.f;
        else if(i%8==3)y.b.a=x.b.e;
        else if(i%8==4)y.b.a=x.b.d;
        else if(i%8==5)y.b.a=x.b.c;
        else if(i%8==6)y.b.a=x.b.b;
        else if(i%8==7)y.b.a=x.b.a;
        fprintf(fp, "%hhu",y.a);

        e=fgetc(f);
        if((e)==' ')fprintf(fp, " ");
        else {fclose(f);fclose(fp);return 3;}
        e=fgetc(f);
        if(e =='\n')fprintf(fp, "\n");
        else{fseek(f, -1, SEEK_CUR);}
        i++;
        if(i%8==0){
            j++;
        }

    }while(!feof(f) && j<(int)strlen(txt));
    int t;

    for (; (t = fscanf(f, "%hhu", &y.a)) != EOF;)
    {
        if (t == 0){fclose(f);fclose(fp);return 3;}
        y.b.a=0;
        fprintf(fp, "%hhu",y.a);
        e=getc(f);
        fprintf(fp, "%c",e);
        e=getc(f);
        if(e=='\n')fprintf(fp, "\n");
        if(e!='\n')fseek(f, -1, SEEK_CUR);
    }

    fclose(f);fclose(fp);
    if(i<(int)(8*strlen(txt)))return 3;
    return 0;
}


int main(){
    char tekst[999];
    char plik[29] = {0};
    char plik1[29] = {0};
    char w;
    int x;

    printf("Do you want to encode (E/e) or decode (D/d) a message? ");
    if(!scanf("%c",&w)){
        printf("Incorrect input");
        return 1;
    }
    while(getchar() != '\n');
    if(w =='D' || w=='d'){
        printf("Enter input file name:");
        if(!scanf("%29[^\n]s",plik)){
            printf("Incorrect input");
            return 1;
        }
        while(getchar() != '\n');
        x=decode(plik, tekst, 999);
        if(x == 2){
            printf("Couldn't open file");
            return 4;
        }
        if(x == 1){
            printf("Incorrect input");
            return 1;
        }
        if(x == 3){
            printf("File corrupted");
            return 6;
        }
        printf("%s",tekst);
    }
    else if(w=='E' || w=='e'){
        printf("Enter a message to be encoded:");
        if(!scanf("%999[^\n]s",tekst)){
            printf("Incorrect input");
            return 1;
        }
        while(getchar() != '\n');
        printf("Enter input file name:");
        if(!scanf("%29[^\n]s",plik)){
            printf("Incorrect input");
            return 1;
        }
        while(getchar() != '\n');
        printf("Enter output file name:");
        if(!scanf("%29[^\n]s",plik1)){
            printf("Incorrect input");
            return 1;
        }
        while(getchar() != '\n');

        x=encode(plik,tekst,plik1);
        if(x == 2){
            printf("Couldn't open file");
            return 4;
        }
        if(x == 1){
            printf("Incorrect input");
            return 1;
        }
        if(x == 3){
            printf("File corrupted");
            return 6;
        }
        if(x == 4){
            printf("Couldn't create file");
            return 5;
        }
        if(x == 0){
            printf("File saved");
        }
    }
    else{
        printf("Incorrect input data");
        return 1;
    }


    return 0;
}