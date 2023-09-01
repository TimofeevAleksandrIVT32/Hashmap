#ifndef HASH_H
#define HASH_H

#define START_SIZE 8 //начальный размер хеш-таблицы
#define STR_SIZE 50
#define EXP_COEF 0.75 //коэффициент расширения хнш-таблицы

typedef struct hash_item {
    int value;
    char* key;
} hash_item;

typedef struct hash_table {
    int table_size; //размер раблицы
    int size; //количество элементов в таблице
    int am_w_in_txt; //для подсчета количества слов в тексте
    hash_item* items;
} hash_table;

hash_table* create_ht(int size);
int read_file(char *file_name, hash_table* ht);
int insert(hash_table* ht, char* key);
int hash(char *key, int table_size, int con);
int double_hash(int h1, int h2, int attempt, int table_size);
int get(hash_table* ht, const char* key);
int resize(hash_table* ht);
void free_ht(hash_table* ht, int end);
void print(hash_table* ht);

#endif /* HASH_H */
