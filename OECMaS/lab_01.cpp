#include <stdio.h>
#include <climits>
#define MAXLEN 128

void new_gets(char s[], int lim){
    char c;
    int i;
    i=0;

    while (((c=getchar())!='\n') && (i < lim-1)){ /* Entering symbols to the end of the string or until the limit is reached */
        s[i]=c;
        i++;
    }
    s[i]='\0';
}

void unsigned_char_option(){
    unsigned char value;
}

int main(){
    char input[MAXLEN];
    int number, flag;
    flag=0;
    do{
    printf("Choose the option:\n");
    puts("0 - unsigned char");
    puts("1 - float");
    printf("Etner the option: ");
    new_gets(input, MAXLEN);
    if (sscanf(input, "%d", &number) == 1 && number >= 0 && number <= 1) flag = 1;
    else {
        puts("Entered value is not correct. Please try again!\n");
    }
    } while(flag==0);

    return 0;
}