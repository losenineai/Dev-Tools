#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "anti_config.h"

struct cell_t
{
	int* keys;
	int* values;
	unsigned short int used;
	unsigned short int size;
};

struct map_t
{
	struct cell_t* cells;
	int cell_count;
};

struct map_t map_allocate();
bool map_set(struct map_t* map , int key , int value);
bool map_contains_key(struct map_t* map , int key);
int map_get(struct map_t* map , int key);
int map_delete(struct map_t* map , int key);
void map_trunk(struct map_t* map);

