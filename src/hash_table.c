#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.h"

static ht_item *ht_new_item(const char *k, const char *v);
static void    ht_del_item(ht_item *i);
ht_hash_table  *ht_new();
void           ht_del_hash_table(ht_hash_table *ht);

int
main()
{
        ht_hash_table *ht = ht_new();
        ht_del_hash_table(ht);
}

/* Initialises a new ht_item and returns it */
static ht_item *
ht_new_item(const char *k, const char *v)
{
        ht_item *i = malloc(sizeof(ht_item));
        i->key = strdup(k);
        i->value = strdup(v);
        return i;
}

/* Initialises a new hash table */
ht_hash_table *
ht_new()
{
        ht_hash_table *ht = malloc(sizeof(ht_hash_table));

        ht->size = 53; // size fixed at 53 temporarily
        ht->count = 0;
        ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
        return ht;
}

static void
ht_del_item(ht_item *i)
{
        free(i->key);
        free(i->value);
        free(i);
}

void
ht_del_hash_table(ht_hash_table *ht)
{
        for (int i = 0; i < ht->size; i++) {
                ht_item *item = ht->items[i];
                if (item != NULL)
                        ht_del_item(item);
        }
        free(ht->items);
        free(ht);
}
