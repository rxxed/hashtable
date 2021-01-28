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
static int         ht_get_hash(const char *s, const int n, const int attempt);

static ht_item HT_DELETED_ITEM = {NULL, NULL};

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

/* Returns an index */
static int
ht_get_hash(const char *s, const int numbuckets, const int attempt)
{
        const int hash_a = ht_hash(s, HT_PRIME_1, numbuckets);
        const int hash_b = ht_hash(s, HT_PRIME_2, numbuckets);
        return (hash_a + (attempt * (hash_b + 1))) % numbuckets;
}

void
ht_insert(ht_hash_table *ht, const char *key, const char *value)
{
        ht_item *item = ht_new_item(key, value);
        int index = ht_get_hash(item->key, ht->size, 0);
        ht_item *cur_item = ht->items[index];
        int i = 1;
        while (cur_item != NULL) {
                if (cur_item != &HT_DELETED_ITEM) {
                        if (strcmp(cur_item->key, key) == 0) {
                                ht_del_item(cur_item);
                                ht->items[index] = item;
                                return;
                        }
                }
                index = ht_get_hash(item->key, ht->size, i);
                cur_item = ht->items[index];
                i++;
        }
        ht->items[index] = item;
        ht->count++;
}

char *
ht_search(ht_hash_table *ht, const char *key)
{
        int index = ht_get_hash(key, ht->size, 0);
        ht_item *item = ht->items[index];
        int i = 1;
        while (item != NULL) {
                if (item != &HT_DELETED_ITEM) {
                        if (strcmp(item->key, key) == 0)
                                return item->value;
                }
                index = ht_get_hash(key, ht->size, i);
                item = ht->items[index];
                i++;
        }
        return NULL;
}

/* Items are marked "deleted" instead of removed as it's possible
 * they're part of a collision chain
 */
void
ht_delete(ht_hash_table *ht, const char *key)
{
        int index = ht_get_hash(key, ht->size, 0);
        ht_item *item = ht->items[index];
        int i = 1;
        while (item != NULL) {
                if (item != &HT_DELETED_ITEM) {
                        if (strcmp(item->key, key) == 0)
                                ht_del_item(item);
                }
                index = ht_get_hash(key, ht->size, i);
                item = ht->items[index];
                i++;
        }
        ht->count--;
}
