#include "compress.h"

char* compress(char* string, int length) {
    int newStringIndex = 0;
    char* newString = malloc(length * sizeof(char));

    char oldChar = string[0];
    int currentRep = 0;
    for (int i = 0; i <= length; i++) {
        if (string[i] == oldChar && i != length) {
            currentRep += 1;
        } else {
            char intString[4];
            sprintf(intString, "%03d", currentRep);
            newString[newStringIndex++] = intString[0];
            newString[newStringIndex++] = intString[1];
            newString[newStringIndex++] = intString[2];
            newString[newStringIndex++] = oldChar;

            if (i != length) {
                oldChar = string[i];
                currentRep = 1;
            }
        }
    }

    newString[newStringIndex] = 0; 

    return newString;
}

void decompress(char* string, int inLen, char* outString, int outLen) {
    int outIndex = 0;
    //char* out = malloc(outLen * sizeof(char));
    

    for (int i = 0; i < (inLen/4); i++) {
        char repChar = string[i*4 + 3];
        string[i*4 + 3] = 0;
        int rep = atoi(&string[i*4]);
        for (int j = 0; j < rep; j++) {
            outString[outIndex++] = repChar;
        }
    }
}


//int main() {
//    char string[] = "ffffffffffff11111222223434343lllllllllllllllllllllllllllllllll";
//    printf("In :%s\n", string);
//    char* comp = compress(string, strlen(string));
//    printf("Comp:%s\n", comp);
//    char* dec = decomp(comp, strlen(comp), strlen(string));
//    printf("Out:%s\n", dec);
//}
