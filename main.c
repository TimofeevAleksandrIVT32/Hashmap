#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

int main(int argc, char *argv[]) {
    if (START_SIZE < 1) {
        printf("The initial table size cannot be less than 1\n");
        return 1;
    }
    if (argc != 2){
        printf("Invalid number of arguments\n");
        return 1;
    }
    hash_table* ht = create_ht(START_SIZE);
    if (ht == NULL) {
        return 1;
    }
    int er = read_file(argv[1], ht);
    if (er == 1) {
        return 1;
    }
    print(ht);
    free_ht(ht, ht->table_size);
    return 0;
}
