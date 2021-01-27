#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"

static ht_item    *ht_new_item(const char *k, const char *v);
static void        ht_del_item(ht_item *i);
ht_hash_table     *ht_new();
void               ht_del_hash_table(ht_hash_table *ht);
static int         ht_hash(const char *s, const int a, const int m);

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

/*
 * The hash function takes a string as input and returns a number between 0 and m
 * m is our desired bucket array length
 */
static int
ht_hash(const char *s, const int a, const int m)
{
        long hash = 0;
        const int len_s = strlen(s);
        for (int i = 0; i < len_s; i++) {
                hash += (long)pow(a, len_s - (i+1) * s[i]);
                hash = hash % m;
        }
        return (int)hash;
}
