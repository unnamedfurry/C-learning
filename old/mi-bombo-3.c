#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int *arr = malloc(1024);
    printf("Размер одного int:      %zu байт\n", sizeof(int));
    printf("Размер указателя arr:   %zu байт\n", sizeof(arr));
    printf("Количество элементов:   %zu\n", 1024 / sizeof(int));

    for (int i=1; i<1000; i++){
        size_t new_size = 1024*(i*2);
        void* tmp = realloc(arr, new_size);
        if (tmp == NULL) {
            fprintf(stderr, "Нехватка памяти.");
            break;
        }
        arr = tmp;
        memset(arr, 0xFF, new_size);
        printf("\nРазмер arr (%d): %zu", i, new_size);
    }
    printf("\n");
    return 0;
}
