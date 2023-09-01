#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

//функция создания хеш-таблицы
hash_table* create_ht(int in_size) {
    hash_table* ht = (hash_table*)malloc(sizeof(hash_table));
    if (ht == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }
    ht->table_size = in_size;
    ht->size = 0;
    ht->items = (hash_item*)malloc(in_size * sizeof(hash_item));
    if (ht->items == NULL) {
        printf("Memory allocation error\n");
        free(ht);
        return NULL;
    }
    return ht;
}

//функция чтения слов из файла
int read_file(char *file_name, hash_table* ht) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        printf("Failed to open file\n");
	    return 1;
    }
    char ch;
    char str[STR_SIZE] = "";
    while ((ch = fgetc(file)) != EOF) {
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
            str[strlen(str)] = ch;
            str[strlen(str) + 1] = '\0';
        }
        else {
            if (strlen(str) != 0) {
                int er = insert(ht, str);
                if (er == 1) {
                    return 1;
                }
                ht->am_w_in_txt++;
                memset(str, 0, sizeof(str));
            }
        }
    }
    fclose(file);
    return 0;
}

//функция вставки слова в таблицу
int insert(hash_table* ht, char* key) {
    if (ht->size + 1 > (int)(ht->table_size * EXP_COEF)) {
        int er = resize(ht);
        if (er == 1) {
            return 1;
        }
    }
    int h1 = hash(key, ht->table_size, ht->table_size + 1);
    int index = h1;
    int h2 = hash(key, ht->table_size, ht->table_size - 1);
    if (h2 == 0) { //чтобы не возникали коллизии в связи с нулевым значением h2 и вследствие зацикливание
        h2++;
    }
    int attempt = 1;
    while (ht->items[index].key != NULL) {
        if (strcmp(key, ht->items[index].key) == 0) {
            ht->items[index].value++;
            return 0;
        }
        index = double_hash(h1, h2, attempt, ht->table_size);
        attempt++;
    }
    ht->items[index].key = (char*)malloc(sizeof(char) * STR_SIZE);
    if (ht->items[index].key == NULL) {
        printf("Memory allocation error\n");
        free_ht(ht, index);
        return 1;
    }
    strcpy(ht->items[index].key,key);
    ht->items[index].value = 1;
    ht->size++;
    return 0;
}

//функция хеширования
int hash(char *key, int table_size, int con) {
    int h = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        h = (h * con + key[i]) % table_size;
    }
    h = (h * 2 + 1) % table_size;
    return h;
}

//функция для совмещения двух хешей
int double_hash(int h1, int h2, int attempt, int table_size) {
    return (h1 + attempt * h2) % table_size;
}

//функция изменения размера хеш-таблицы
int resize(hash_table* ht) {
    hash_item* temp = (hash_item*)malloc(sizeof(hash_item) * ht->table_size * 2);
    if (temp == NULL) {
        printf("Memory allocation error\n");
        free_ht(ht, ht->table_size);
        return 1;
    }
    for (int i = 0; i < ht->table_size; i++) {
        if (ht->items[i].key != NULL) {
            int h1 = hash(ht->items[i].key, ht->table_size * 2, ht->table_size * 2 + 1);
            int index = h1;
            if (temp[index].key != NULL) {
                int attempt = 1;
                int h2 = hash(ht->items[i].key, ht->table_size * 2, ht->table_size * 2  - 1);
                if (h2 == 0) {
                    h2++;
                }
                while (temp[index].key != NULL) {
                    index = double_hash(h1, h2, attempt, ht->table_size * 2);
                    attempt++;
                }
            } 
            temp[index].value = ht->items[i].value;
            temp[index].key = (char*)malloc(sizeof(char) * STR_SIZE);
            if (temp[index].key == NULL) {
                printf("Memory allocation error\n");
                free_ht(ht, ht->table_size);
                for (int k = 0; k < index; k++) {
                    if (temp[k].key != NULL) {
                        free(temp[k].key);
                    }
                }
                free(temp);
                return 1;
            }
            strcpy(temp[index].key, ht->items[i].key);
        }
    }
    for (int i = 0; i < ht->table_size; i++) {
        if (ht->items[i].key != NULL) {
            free(ht->items[i].key);
        }
    }
    free(ht->items);
    ht->items = temp;
    ht->table_size = ht->table_size * 2;
    return 0;
}

//функция очистки динамической памяти
void free_ht(hash_table* ht, int end) {
    for (int i = 0; i < end; i++) {
        if (ht->items[i].key != NULL) {
            free(ht->items[i].key);
        }
    }
    free(ht->items);
    free(ht);
}

//функция вывода таблицы на экран
void print(hash_table* ht) {
    printf("word:          |repetitions|frequency \n");
    printf("-------------------------------------\n");
    for (int i = 0; i < ht->table_size; i++) {
        if (ht->items[i].key != NULL) {
            printf("%-14s | %-9d | %f\n", ht->items[i].key, ht->items[i].value, (float)(ht->items[i].value) / (float)(ht->am_w_in_txt));
        }
    }
}
