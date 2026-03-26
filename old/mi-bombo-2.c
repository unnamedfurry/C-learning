//
// Created by unnamedfurry on 3/18/26.
//
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    while (true) {
        int* i = malloc(sqrt(sizeof (int)));
        *i = 0;
        printf("\n%d", *i);
    }
    return 0;
}
