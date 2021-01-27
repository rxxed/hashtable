typedef struct {
        char *key;
        char *value;
} ht_item;

// stores an array of pointers to ht_items, and some other details
typedef struct {
        int size;
        int count;
        ht_item **items;
} ht_hash_table;
