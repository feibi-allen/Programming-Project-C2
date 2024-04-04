#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int findSringLength(char *buffer){
    int count = 0, position = 0;

    while (buffer[position] != '\0'){
        count ++;
        position++;
    }
    return count;
}

int main(){
    char string[256];
    scanf("%s",string);
    printf("%ld\n",strlen(string));
    return 0;
}