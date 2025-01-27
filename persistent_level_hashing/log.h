#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include<sys/mman.h>
#include "pflush.h"
// #include "/home/kparun/Documents/CS730/quartz/src/lib/pmalloc.h"   // path to pmalloc.h in Quartz

#define MAP_PERSISTENT 0x200000
#define MAP_SENSITIVE 0x400000
#define KEY_LEN 16                        // The maximum length of a key
#define VALUE_LEN 15                      // The maximum length of a value

typedef struct log_entry{                
    uint8_t key[KEY_LEN];
    uint8_t value[VALUE_LEN];
    uint8_t flag;
} log_entry;

typedef struct level_log
{ 
    uint64_t log_length;
    log_entry* entry;
    uint64_t current;
}level_log;

level_log* log_create(uint64_t log_length);

void log_write(level_log *log, uint8_t *key, uint8_t *value);
void log_clean(level_log *log);
