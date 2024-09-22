#include <stdio.h>
#include <string.h>

const int sizeOfInt = sizeof(int) * 8;

/* io */

void printBinaryInteger(int n) {
    int size = sizeof(int) * 8;
    
    for (int i = size - 1; i >= 0; i--) {
        int bit = (n >> i) & 1; 
        printf("%d", bit);
    }
    printf("\n");
}

void printLine() {
    printf("+");
    for (int i = 1; i <= sizeOfInt; i++) {
        printf("---");
        if (i != sizeOfInt) {
            printf("+");
        }
    }
    printf("+\n");
}

void beautifulPrint(int n) {

    printLine();

    printf("|");
    for (int i = 1; i <= sizeOfInt; i++) {
        printf("%2d |", i);
    }
    printf("\n");

    printLine();

    printf("|");
    for (int i = sizeOfInt - 1; i >= 0; i--) {
        int bit = (n >> i) & 1;
        printf("%2d |", bit);
    }
    printf("\n");
    
    printLine();

    printf("|");
    for (int i = sizeOfInt; i >= 1; i--) {
        printf("%2d |", i);
    }
    printf("\n");
    
    printLine();
}

int inputBinaryAndConvert() {
    char binaryStr[35] = {0};  
    int isValid;
    int result;

    do {
        isValid = 1;
        result = 0;

        printf("Enter binary int (at most %d): ", sizeOfInt);
        
        if (fgets(binaryStr, sizeof(binaryStr), stdin) == NULL) {
            printf("Error reading input, try again.\n");
            isValid = 0;
            continue;
        }

        int len = strlen(binaryStr);
        if (binaryStr[len - 1] == '\n') {
            binaryStr[len - 1] = '\0';
            len--;
        }

        if (len > sizeOfInt) {
            printf("Error: input too long, try again.\n");
            isValid = 0;
            continue;
        }

        for (int i = 0; binaryStr[i] != '\0'; i++) {
            if (binaryStr[i] != '0' && binaryStr[i] != '1') {
                isValid = 0;
                printf("Error: your number is not binary, try again.\n");
                break;
            }
            result = result * 2 + (binaryStr[i] - '0');
        }

    } while (!isValid);

    return result;
}

/* logic */

int getBitGroup(int number, int pos, int len) {
    int result = number >> (sizeOfInt - pos - len + 1);
    int mask = (1 << len) - 1;
    result &= mask;
    return result;
}

int setBitGroup(int number, int pos, int len, int value) {
    int delta = (sizeOfInt - pos - len + 1);
    int mask = ((1 << len) - 1) << delta;
    number &= ~mask;
    number |= (value << delta);
    return number;
}

int swapBitGroups(int number) {
    int pos1, len1, pos2, len2;

    printf("Enter the position of the fisrt bit of the first group (from left to right): ");
    scanf("%d", &pos1);
    printf("Enter the number of bits in the first group: ");
    scanf("%d", &len1);

    if (pos1 < 0 || pos1 >= sizeOfInt || len1 <= 0 || (pos1 + len1) > sizeOfInt) {
        printf("Error: invalid parameters for the first group.\n");
        return number;
    }

    printf("Enter the position of the first bit of the second group (from left to right): ");
    scanf("%d", &pos2);
    printf("Enter the number of bits in the second group: ");
    scanf("%d", &len2);

    if (pos2 < 0 || pos2 >= sizeOfInt || len2 <= 0 || (pos2 + len2) > sizeOfInt) {
        printf("Error: invalid parameters for the second group.\n");
        return number;
    }

    if ((pos2 + len1 > sizeOfInt) || (pos1 + len2 > sizeOfInt)) {
        printf("Error: groups are too long to rearrange.\n");
        return number;
    }

    int group1 = getBitGroup(number, pos1, len1);
    printf("Your group 1:\n");
    printBinaryInteger(group1);
    int group2 = getBitGroup(number, pos2, len2);
    printf("Your group 2:\n");
    printBinaryInteger(group2);

    number = setBitGroup(number, pos1, len2, group2);
    number = setBitGroup(number, pos2, len1, group1);

    return number;
}

int main() {
    int number = inputBinaryAndConvert();
    printf("Your number:\n");
    printBinaryInteger(number);
    printf("And beautiful version:\n");
    beautifulPrint(number);
    number = swapBitGroups(number);
    printf("Your number:\n");
    printBinaryInteger(number);
    printf("And beautiful version:\n");
    beautifulPrint(number);
    printf("In decimal format: %d", number);

    return 0;
}