//include things here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define True 1
#define False 0

/*

Fill in the following functions - follow the document for proper declaration and definition
1. encodeNuc
2. decodeBin
3. findProtein
4. proteinReport
5. isolateProtein
6. genMutant

Implement for 1% bonus!
7. checkMutant

*/

char validCodons[2][65][9];
char *codonIndex(char *filename){
    FILE *fp = fopen(filename, "r");
    char b[10];
    char *b2;
    char *n;
    int i = 0;
    const char delim[2] = ",";
    while(fscanf(fp,"%s\n", b) != EOF){
        b2 = strtok(b, delim);
        n = strtok(NULL, delim);
        strcpy(validCodons[0][i], b2);
        *validCodons[1][i] = *n;
        ++i;
    }
    fclose(fp);
}

void encodeNuc(char *filename){
    FILE *fp = fopen(filename,"r");
    char b[32] = "b";
    char *newfile = strcat(b,filename);
    FILE *fo = fopen(newfile, "w");
    for (char ch = getc(fp); ch != EOF; ch = getc(fp)){
        if (ch == 'A')
            fwrite("00", sizeof("00") - 1,1, fo);
        else if(ch == 'C') 
            fwrite("01", sizeof("01") - 1,1, fo);
        else if(ch == 'G')
            fwrite("10", sizeof("10") - 1,1, fo);
        else if(ch == 'T')
            fwrite("11", sizeof("11") - 1,1, fo);
   }
   fclose(fp);
   fclose(fo);
}

void decodeBin(char *filename){
    FILE *fp = fopen(filename,"r");
    char n[32] = "n";
    char* newfile = strncat(n,filename,31);
    FILE *fo = fopen(newfile, "w");
    char *A = "00";
    char *C = "01";
    char *G = "10";
    char *T = "11";
    char ch1;
    char ch2;
    for (ch1 = getc(fp), ch2 = getc(fp); ch2 != EOF; ch1 = getc(fp), ch2 = getc(fp)){
        char ch[2] = {0,0};
        strncat(ch, &ch1, sizeof(char));
        strncat(ch, &ch2, sizeof(char));
        if (strcmp(ch, A) == 0){
            fwrite("A", sizeof("A") - 1,1, fo);
            //printf("prints A\n");
        }else if(strcmp(ch, C) == 0){
            fwrite("C", sizeof("C") - 1,1, fo);
            //printf("prints C\n");
        }else if(strcmp(ch, G) == 0){
            fwrite("G", sizeof("G") - 1,1, fo);
            //printf("prints G\n");
        }else if(strcmp(ch, T) == 0){
            fwrite("T", sizeof("T") - 1,1, fo);
            //printf("prints T\n");
        }
        //printf("%s\n", ch);
   }
   fclose(fp);
   fclose(fo);
}

void findProtein(char *filename, int checkPos, int proteinInfo[]){
    proteinInfo[0] = 0;
    proteinInfo[1] = 0;
    char codonFrame[8] = {0,0,0,0,0,0,0,0};
    int codonFrame_length = 0;
    FILE *fp = fopen(filename,"r");
    codonIndex("codons.txt");
    fseek(fp, 2*(checkPos-3), SEEK_SET);
    char ch1, ch2;
    char *A = "00";
    char *C = "01";
    char *G = "10";
    char *T = "11";
    int busy = False;
    int startPos = checkPos;
    int aminoLength = 0;
    for (ch1 = getc(fp), ch2 = getc(fp); ch2 != EOF; ch1 = getc(fp), ch2 = getc(fp)){
        //printf("%s\n", codonFrame);
        char ch[2] = {0,0};
        strncat(ch, &ch1, sizeof(char));
        strncat(ch, &ch2, sizeof(char));
        if(codonFrame_length == 3){
            memmove(codonFrame, codonFrame + 2, sizeof(codonFrame));
            codonFrame[5] = 0;
            codonFrame[6] = 0;
            --codonFrame_length;
        }
        if(strcmp(ch,A) == 0){
            strcat(codonFrame, A);
            ++codonFrame_length;
        }else if(strcmp(ch,C) == 0){ 
            strcat(codonFrame, C);
            ++codonFrame_length;
        }else if(strcmp(ch,G) == 0){
            strcat(codonFrame, G);
            ++codonFrame_length;
        }else if(strcmp(ch,T) == 0){
            strcat(codonFrame, T);
            ++codonFrame_length;
        }
        const char* startCodon = "001110";
        const char* endCodon1 = "111000";
        const char* endCodon2 = "110000";
        const char* endCodon3 = "110010";
        if((strcmp(codonFrame,startCodon) == 0) && busy == False){
            //printf("NEW PROTEIN\n");
            busy = True;
            proteinInfo[0] = startPos;
            for (ch1 = getc(fp), ch2 = getc(fp); ch2 != EOF; ch1 = getc(fp), ch2 = getc(fp)){
                char ch[2] = {0,0};
                strncat(ch, &ch1, sizeof(char));
                strncat(ch, &ch2, sizeof(char));
                if(codonFrame_length == 3){
                    /*for(int i=0; i < 64; ++i){
                        if(strcmp(codonFrame, validCodons[0][i]) == 0)
                            printf("%s\n",validCodons[1][i]);
                    }*/
                    memset(codonFrame,0,sizeof(codonFrame));
                    codonFrame_length = 0;
                }
                if(strcmp(ch,A) == 0){
                    strcat(codonFrame, A);
                    ++codonFrame_length;
                    ++aminoLength;
                }else if(strcmp(ch,C) == 0){ 
                    strcat(codonFrame, C);
                    ++codonFrame_length;
                    ++aminoLength;
                }else if(strcmp(ch,G) == 0){
                    strcat(codonFrame, G);
                    ++codonFrame_length;
                    ++aminoLength;
                }else if(strcmp(ch,T) == 0){
                    strcat(codonFrame, T);
                    ++codonFrame_length;
                    ++aminoLength;
                }
                if(((strcmp(codonFrame,endCodon1) == 0 || strcmp(codonFrame,endCodon2) == 0 || strcmp(codonFrame,endCodon3) == 0) && busy == True)){
                    busy = False;
                    proteinInfo[1] = (int)((aminoLength)/3);
                    printf("Starting position: %d\nAmino acid length: %d\n",proteinInfo[0]-3, proteinInfo[1]);
                    fclose(fp);
                    return;
                }
            }
        }
        ++startPos;
    }
    printf("Starting position: %d\nAmino acid length: %d\n",proteinInfo[0], proteinInfo[1]);
    fclose(fp);
}

void proteinReport(char *filename){
    FILE *fp = fopen(filename,"r");
    char r[32] = "r";
    char* newfile = strncat(r,filename,31);
    FILE *fo = fopen(newfile, "w");
    char codonFrame[8] = {0,0,0,0,0,0,0,0};
    int codonFrame_length = 0;
    codonIndex("codons.txt");
    char ch1, ch2;
    char *A = "00";
    char *C = "01";
    char *G = "10";
    char *T = "11";
    int busy = False;
    int aminoLength = 0;
    int proteinInfo[2] = {0,0};
    int startPos = 0;
    for (ch1 = getc(fp), ch2 = getc(fp); ch2 != EOF; ch1 = getc(fp), ch2 = getc(fp)){
        //printf("%s\n", codonFrame);
        char ch[2] = {0,0};
        strncat(ch, &ch1, sizeof(char));
        strncat(ch, &ch2, sizeof(char));
        if(codonFrame_length == 3){
            memmove(codonFrame, codonFrame + 2, sizeof(codonFrame));
            codonFrame[5] = 0;
            codonFrame[6] = 0;
            --codonFrame_length;
        }
        if(strcmp(ch,A) == 0){
            strcat(codonFrame, A);
            ++codonFrame_length;
        }else if(strcmp(ch,C) == 0){ 
            strcat(codonFrame, C);
            ++codonFrame_length;
        }else if(strcmp(ch,G) == 0){
            strcat(codonFrame, G);
            ++codonFrame_length;
        }else if(strcmp(ch,T) == 0){
            strcat(codonFrame, T);
            ++codonFrame_length;
        }
        const char* startCodon = "001110";
        const char* endCodon1 = "111000";
        const char* endCodon2 = "110000";
        const char* endCodon3 = "110010";
        if((strcmp(codonFrame,startCodon) == 0) && busy == False){
            //printf("NEW PROTEIN\n");
            busy = True;
            proteinInfo[0] = startPos;
            for (ch1 = getc(fp), ch2 = getc(fp); ch2 != EOF; ch1 = getc(fp), ch2 = getc(fp)){
                char ch[2] = {0,0};
                strncat(ch, &ch1, sizeof(char));
                strncat(ch, &ch2, sizeof(char));
                if(codonFrame_length == 3){
                    /*for(int i=0; i < 64; ++i){
                        if(strcmp(codonFrame, validCodons[0][i]) == 0){
                            printf("%s\n",validCodons[1][i]);
                        }
                    }*/
                    memset(codonFrame,0,sizeof(codonFrame));
                    codonFrame_length = 0;
                }
                if(strcmp(ch,A) == 0){
                    strcat(codonFrame, A);
                    ++codonFrame_length;
                    ++aminoLength;
                }else if(strcmp(ch,C) == 0){ 
                    strcat(codonFrame, C);
                    ++codonFrame_length;
                    ++aminoLength;
                }else if(strcmp(ch,G) == 0){
                    strcat(codonFrame, G);
                    ++codonFrame_length;
                    ++aminoLength;
                }else if(strcmp(ch,T) == 0){
                    strcat(codonFrame, T);
                    ++codonFrame_length;
                    ++aminoLength;
                }
                ++startPos;
                if(((strcmp(codonFrame,endCodon1) == 0 || strcmp(codonFrame,endCodon2) == 0 || strcmp(codonFrame,endCodon3) == 0) && busy == True)){
                    proteinInfo[1] = (int)((aminoLength)/3);
                    fprintf(fo, "%d, %d\n",proteinInfo[0],proteinInfo[1]);
                    proteinInfo[1] = 0;
                    aminoLength = 0;
                    break;
                }
            }
            busy = False;
        }
        ++startPos;
    }
    fclose(fp);
    fclose(fo);
}

void isolateProtein(char *filename, int proteinInfo[]){
    FILE *fp = fopen(filename,"r");
    char p[32] = "p";
    char* newfile = strncat(p,filename,31);
    FILE *fo = fopen(newfile, "w");
    char codonFrame[8] = {0,0,0,0,0,0,0,0};
    int codonFrame_length = 0;
    codonIndex("codons.txt");
    char ch1, ch2;
    char *A = "00";
    char *C = "01";
    char *G = "10";
    char *T = "11";
    int busy = False;
    int aminoLength = 0;
    int startPos = proteinInfo[0];
    fseek(fp, 2*(startPos-3), SEEK_SET);
    for (ch1 = getc(fp), ch2 = getc(fp); ch2 != EOF; ch1 = getc(fp), ch2 = getc(fp)){
        //printf("%s\n", codonFrame);
        char ch[2] = {0,0};
        strncat(ch, &ch1, sizeof(char));
        strncat(ch, &ch2, sizeof(char));
        if(codonFrame_length == 3){
            memmove(codonFrame, codonFrame + 2, sizeof(codonFrame));
            codonFrame[5] = 0;
            codonFrame[6] = 0;
            --codonFrame_length;
        }
        if(strcmp(ch,A) == 0){
            strcat(codonFrame, A);
            ++codonFrame_length;
        }else if(strcmp(ch,C) == 0){ 
            strcat(codonFrame, C);
            ++codonFrame_length;
        }else if(strcmp(ch,G) == 0){
            strcat(codonFrame, G);
            ++codonFrame_length;
        }else if(strcmp(ch,T) == 0){
            strcat(codonFrame, T);
            ++codonFrame_length;
        }
        const char* startCodon = "001110";
        const char* endCodon1 = "111000";
        const char* endCodon2 = "110000";
        const char* endCodon3 = "110010";
        if((strcmp(codonFrame,startCodon) == 0) && busy == False){
            //printf("NEW PROTEIN\n");
            busy = True;
            proteinInfo[0] = startPos;
            for (ch1 = getc(fp), ch2 = getc(fp); ch2 != EOF; ch1 = getc(fp), ch2 = getc(fp)){
                char ch[2] = {0,0};
                strncat(ch, &ch1, sizeof(char));
                strncat(ch, &ch2, sizeof(char));
                int counter = 0;
                if(counter < proteinInfo[1]){
                    if(codonFrame_length == 3){
                        for(int i=0; i < 64; ++i){
                            if(strcmp(codonFrame, validCodons[0][i]) == 0){
                                //printf("%s\n",validCodons[1][i]);
                                fprintf(fo, "%s", validCodons[1][i]);
                            }
                        }
                        memset(codonFrame,0,sizeof(codonFrame));
                        codonFrame_length = 0;
                    }
                    if(strcmp(ch,A) == 0){
                        strcat(codonFrame, A);
                        ++codonFrame_length;
                        ++aminoLength;
                    }else if(strcmp(ch,C) == 0){ 
                        strcat(codonFrame, C);
                        ++codonFrame_length;
                        ++aminoLength;
                    }else if(strcmp(ch,G) == 0){
                        strcat(codonFrame, G);
                        ++codonFrame_length;
                        ++aminoLength;
                    }else if(strcmp(ch,T) == 0){
                        strcat(codonFrame, T);
                        ++codonFrame_length;
                        ++aminoLength;
                    }
                    ++startPos;
                    if(((strcmp(codonFrame,endCodon1) == 0 || strcmp(codonFrame,endCodon2) == 0 || strcmp(codonFrame,endCodon3) == 0) && busy == True)){
                        proteinInfo[1] = (int)((aminoLength)/3);
                        fprintf(fo, "*\n");
                        proteinInfo[1] = 0;
                        aminoLength = 0;
                        fclose(fp);
                        fclose(fo);
                        return;
                    }
                }
            }
            busy = False;
        }
        ++startPos;
    }
    fclose(fp);
    fclose(fo);
}

int genMutant(char *filename, int mutation[]){
    FILE *fp = fopen(filename,"r");
    char m[32] = "m";
    char* newfile = strncat(m,filename,31);
    FILE *fo = fopen(newfile, "w");
    char ch1,ch2;
    char *A = "00";
    char *C = "01";
    char *G = "10";
    char *T = "11";
    int count = 0;
    int delete_flag = False;
    int replace_flag = False;
    for (ch1 = getc(fp), ch2 = getc(fp); ch2 != EOF; ch1 = getc(fp), ch2 = getc(fp)){
        char ch[2] = {0,0};
        strncat(ch, &ch1, sizeof(char));
        strncat(ch, &ch2, sizeof(char));
        if(count == mutation[0]){
            if (mutation[1] == 0){
                delete_flag = True;
            }else if (mutation[1] == 1){
                if(mutation[2] == 0){
                    fprintf(fo, A);
                }else if(mutation[2] == 1){
                    fprintf(fo, C);
                }else if(mutation[2] == 2){
                    fprintf(fo, G);
                }else if(mutation[2] == 3){
                    fprintf(fo, T);
                }
            }else if (mutation[1] == 2){
                delete_flag = True;
                replace_flag = True;
            }
        }
        if (strcmp(ch, A) == 0 && delete_flag == False){
            fprintf(fo, A);
        }else if(strcmp(ch, C) == 0 && delete_flag == False){
            fprintf(fo, C);
        }else if(strcmp(ch, G) == 0 && delete_flag == False){
            fprintf(fo, G);
        }else if(strcmp(ch, T) == 0 && delete_flag == False){
            fprintf(fo, T);
        }
        if (replace_flag == True && mutation[2] == 0){
            fprintf(fo, A);
        }else if(replace_flag == True && mutation[2] == 1){
            fprintf(fo, C);
        }else if(replace_flag == True && mutation[2] == 2){
            fprintf(fo, G);
        }else if(replace_flag == True && mutation[2] == 3){
            fprintf(fo, T);
        }
        delete_flag = False;
        replace_flag = False;
        ++count;
    }
    fclose(fp);
    fclose(fo);
}
/*
int main(){
    encodeNuc("fullSLV.txt");
    decodeBin("bfullSLV.txt");
    int protein[2] = {0,0};
    findProtein("bfullSLV.txt", 4164, protein);
    proteinReport("bfullSLV.txt");
    int protein1[2] = {4161, 38};
    isolateProtein("bfullSLV.txt", protein1);
    int mutation[3] = {0,2,0};
    genMutant("bfullSLV.txt", mutation);
    return 0;
}*/
