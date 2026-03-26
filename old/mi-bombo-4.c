#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>   // для madvise
#include <unistd.h>

int main() {
    int *arr = malloc(1024 * 1024);  // начинаем с 1 МБ
    if (!arr) return 1;

    // Отключаем Transparent Huge Pages для этого блока (очень помогает)
    madvise(arr, 1024*1024, MADV_NOHUGEPAGE);

    printf("Запускаем агрессивный пожиратель памяти...\n");

    size_t total_allocated = 0;

    for (int i = 1; i <= 200; i++) {           // можно ставить 500–1000
        size_t add_size = 50 * 1024 * 1024;    // добавляем по 50 МБ за раз (можно 100 или 200)

        void *tmp = realloc(arr, total_allocated + add_size);
        if (tmp == NULL) {
            fprintf(stderr, "realloc провалился!\n");
            break;
        }
        arr = tmp;

        // Полностью трогаем новую память + старую (чтобы THP не помогал)
        memset(arr, 0xAA, total_allocated + add_size);

        total_allocated += add_size;

        printf("Итерация %3d → выделено %6.2f ГБ реально (RSS должен расти)\n",
               i, total_allocated / (1024.0 * 1024 * 1024));
    }

    printf("\nВсё, ждём 10 секунд перед free...\n");
    sleep(10);   // чтобы ты успел посмотреть в htop

    free(arr);
    return 0;
}