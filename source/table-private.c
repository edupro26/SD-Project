#include "table-private.h"
#include <stdio.h>
#include <string.h>

int hash_code(char *key, int n) {
    int count = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        count += key[i];
    }
    return count % n;
}
