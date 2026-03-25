// wrote definitely by Anna Senpai and not anyone else :3
// definitely not C learning project ;3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void c0redev() {
    for (int i=0; i<101; i++) {
        char *nyarr[] = {"мя", "няф", "бупь", "мря", "\0"}; // * at start of array's name is required to multiple chars in one slot to work, without * program will fail (expected element length 1, but found 2,3,4,5 etc.)
        srand(time(NULL) + clock()); // initializing randomizer's seed with current time in seconds from 1970 + milliseconds
        int randomizer1488 = rand() % 5; // if we want our range from 0 to 4, we type 5 (bc of arr 0,1,2,3,4)
        if (randomizer1488 == 4) {
            continue;
        }
        printf("%s [%d]\n", nyarr[randomizer1488], randomizer1488);
    }
}

int main(int argc, char **argv) {
    bool toggle1 = false;
    bool toggle2 = false;
    bool toggle3 = false;
    bool toggle4 = false;

    if (argc == 5) {
        toggle1 = (strcmp(argv[1], "1") == 0);
        toggle2 = (strcmp(argv[2], "1") == 0);
        toggle3 = (strcmp(argv[3], "1") == 0);
        toggle4 = (strcmp(argv[4], "1") == 0);
    } else if (argc == 1) {

    } else {
        printf("Usage: ./main 0/1 0/1 0/1\n");
        exit(1);
    }

    if (toggle4 == true) {
        c0redev();
        return 0;
    }

    // basic print
    printf("Hello blyat!\n");
    printf("I like sucking dicks <3\n");
    printf("I am a gay gay faggot mreooooow~~\n");

    // numbers
    int awawa = 170;
    double awawa2 = 0.17;
    printf("\n%d, %.2f\n", awawa, awawa2);
    printf("Int 170 size: %zu, Double 0.17 size: %zu\n", sizeof awawa, sizeof awawa2);

    //reassigning numbers
    awawa2 = awawa + awawa2;
    printf("%.2f\n", awawa2);
    printf("Both damn numbers size: %zu\n", sizeof (awawa + awawa2));

    // char printing
    char sukaBlyat = 'A';
    char nahoi = 65;
    char blablabla[7] = "aaaaaa"; // 6 letters "aaaaaa" plus one closing symbol /0
    char balbalbal[14] = "bbbbbb"; // 7 bytes for one string "bbbbbb/0" and 7 for second "aaaaaa/0"
    strcat(balbalbal, blablabla);
    printf("\n%c, %c, %s, %s, %d\n", sukaBlyat, nahoi, blablabla, balbalbal, sukaBlyat);
    printf("Char 'A' size: %zu\n", sizeof (sukaBlyat));

    //input-output operations (not disk) and if statements
    if (toggle1 == true) {
        printf("\nType one symbol: \n> ");
        char sym;
        scanf("%c", &sym); // scanner analog from java
        int summ = 1 + sym;
        printf("Result of adding 1 more steps: %c (%d), previous input: %c (%d)\n", summ, summ, sym, sym);
    }

    if (toggle2 == true) {
        printf("\nWhich char do you want to see lil nigga? \n> ");
        int userChar;
        scanf("%d", &userChar);
        for (int i=0; i<11; i++) {
            char displayedChar = userChar+i;
            printf("Result %d: >%c< \n", i+1, displayedChar);
        }
    }

    //printf("\nHave a terrible day and do not forget to take out trash that ur mama told you to do half an hour ago ;3");
    char out1[20] = "Have a terrible day"; // 19 chars + closing sym
    char out2[38] = " and do not forget to take out trash"; // 37 chars + closing sym
    char out3[51] = " that your mama told you to do half an hour ago ;3"; // 50 chars + closing sym
    char out4[sizeof out1 + sizeof out2 + sizeof out3 + 1] = ""; // 20+38+51+1 (out3 already contains closing sym so we may don't have to add another one (but one extra byte still isn't critical))
    strcat(out4, out1);
    strcat(out4, out2);
    strcat(out4, out3);
    printf("\n%s\n", out4);

    // calculator
    if (toggle3 == true) {
        printf("\nOoooh you can't make your homework without calculator, poor you?(((\n> ");
        int *num1 = malloc(sizeof (int));
        int *num2 = malloc(sizeof (int));
        char *operator = malloc(sizeof (char));
        double *result = malloc(sizeof (double));
        if (scanf("%d %c %d", num1, operator, num2) == 3){
            switch (*operator) {
                case '+':
                    *result = *num1 + *num2;
                    printf("\nHere's the result specifically for retards: %.2lf", *result);
                    break;
                case '-':
                    *result = *num1 - *num2;
                    printf("\nHere's the result specifically for retards: %.2lf", *result);
                    break;
                case '*':
                    *result = *num1 * *num2;
                    printf("\nHere's the result specifically for retards: %.2lf", *result);
                    break;
                case '/':
                    if (*num2 == 0) {
                        printf("ur iq is ass, session terminated, go kill yourself");
                        break;
                    }
                    *result = (double)*num1 / (double)*num2;
                    printf("\nHere's the result specifically for retards: %.2lf", *result);
                    break;
                case '%':
                    if (*num2 == 0) {
                        printf("ur iq is ass, session terminated, go kill yourself");
                        break;
                    }
                    *result = *num1 % *num2;
                    printf("\nHere's the result specifically for retards: %.2lf", *result);
                    break;
                default:
                    printf("ur iq is ass, session terminated, go kill yourself pls");
                    break;
            }
        } else {
            printf("ur iq is ass, session terminated, go kill yourself pls");
        }
        free(num1);
        free(num2);
        free(operator);
        free(result);
    }

    printf("\n");
    return 0;
}

/**
 * Addition         `+`
 * Substraction     `-`
 * Multiplication   `*`
 * Division         `/`
 * Remainder        `%`
 * Increment        `++`
 * Decrement        `--`
 *
 * sqrt(x)      square root of x
 * cbrt(x)      cube root of x
 * pow(x, y)    value of x to the power of y (x^y)
 * ceil(x)      rounds x upwards to nearest integer
 * floor(x)     rounds x downwards to nearest integer
 * fabs(x)      absolute value of x
 * sin(x)       trigonometric func, x in radians
 * cos(x)       trigonometric func, x in radians
 * tan(x)       trigonometric func, x in radians
 * log(x)       logarithmic func
 * log10(x)     logarithmic func
 *
 * strlen(str)                 length of a string (excluding '\0')
 * strcpy(dest, src)           copies src to dest
 * strcat(dest, src)           appends src to dest
 * strcmp(str1, str2)          compares strings lexicographically, 0 if equal
 * strlcpy(dest, src, n)       strcpy() but safer, copies up to n chars
 * strlcat(dest, src, n)       strcat() but safer, appends up to n chars
 * strncmp(dest, src, n)       strcmp() but safer, compares up to n chars from start, 0 if identical
 * strstr(haystack, needle)    finds first occurrence of needle in haystack   (string in string)
 * strchr(str, ch)             locates first occurrence of character ch       (char in string)
 * strtok(str, splitter)       splits a string into tokens based on splitter char
 *
 * isalnum(c)    checks if c is alphanumeric
 * isalpha(c)    checks if c is alphabetic
 * isdigit(c)    checks if c is a decimal digit
 * islower(c)    checks if c is lowercase letter
 * isupper(c)    checks if c is uppercase letter
 * isspace(c)    checks if c is a space sym
 * isblank(c)    checks if c is a blank sym
 * iscntrl(c)    checks if c is control character
 * ispunct(c)    checks if c is punctuation character
 * isprint(c)    checks if c is printable character
 * isgraph(c)    checks if c is printable character excluding space
 * isxdigit(c)   checks if c is a hexadecimal digit
 * tolower(c)    converts c to lowercase
 * toupper(c)    converts c to uppercase
 *
 *
 *
 * int      (4 bytes) `%d`
 * double   (8 bytes) `%lf`
 * float    (4 bytes) `%f`
 * char     (1 bytes) `%c`
 *
 * boolean ? true : false
 *
 * Formula for a number between min and max (inclusive)
 * int min = 1;
 * int max = 100;
 * int ranged_random_number = (rand() % (max - min + 1)) + min;
 * int ranged_random_number = rand() % 101
 **/