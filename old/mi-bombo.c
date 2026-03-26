//
// Created by unnamedfurry on 3/12/26.
//

#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("\nHUY\n");
    int *array = malloc(10);
    printf("Start sizeof array: %d\n", (int)sizeof (array));
    for (int i=0; i<100; i++) {
        array[i] = 200;
        array = realloc(array, sizeof (array)+10);
        printf("Sizeof array: %d at position %d\n", (int)sizeof (array), i);
    }
    free(array);
}
