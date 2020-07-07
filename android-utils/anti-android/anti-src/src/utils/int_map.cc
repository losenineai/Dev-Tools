#include "int_map.h"
#include <stdlib.h>
#include <stdbool.h>

struct map_t map_allocate()
{
	struct map_t map;
	map.cell_count = 1024;
	map.cells = (cell_t*)malloc(sizeof(struct cell_t) * map.cell_count);
	for (int i = 0; i<map.cell_count; i++)
	{
		map.cells[i].used = 0;
		map.cells[i].size = 1;
		map.cells[i].keys = (int*)malloc(sizeof(int) * map.cells[i].size);
		map.cells[i].values = (int*)malloc(sizeof(int) * map.cells[i].size);
	}
	return map;
}

bool map_set(struct map_t* map , int key , int value) {
    int index = key % map->cell_count;

    if (map->cells[index].used == 0) {
        map->cells[index].keys[0] = key;
        map->cells[index].values[0] = value;
        map->cells[index].used++;
        return true;
    }

    for (int i = 0; i < map->cells[index].used; i++) {
        if (map->cells[index].keys[i] == key) {
            map->cells[index].values[i] = value;
            return true;
        }
    }

	if(map->cells[index].used == map->cells[index].size)
	{
		map->cells[index].size *= 3;
		void *t1 = realloc(map->cells[index].keys , sizeof(int) * map->cells[index].size);
		void *t2 = realloc(map->cells[index].values , sizeof(int) * map->cells[index].size);
		(void )t1; // 去warning
		(void )t2; // 去warning
	}

	map->cells[index].keys[map->cells[index].used] = key;
	map->cells[index].values[map->cells[index].used] = value;
	map->cells[index].used++;

	return true;
}

bool map_contains_key(struct map_t* map , int key)
{
	int index = key % map->cell_count;
	
	if(map->cells[index].used == 0)
		return false;
	
	for (int i=0; i < map->cells[index].used; i++)
		if(map->cells[index].keys[i] == key)
			return true;

	return false;
}

// does not check if key exists
int map_get(struct map_t* map, int key)
{
	int index = key % map->cell_count;
	
	for (int i=0; i < map->cells[index].used; i++)
		if(map->cells[index].keys[i] == key)
			return map->cells[index].values[i];

	return 0;

	/*
	fprintf(stderr , "no key found! detected unsafe usage , check if key exists first");
	exit(EXIT_FAILURE);
	return -1;
	 */
}

int map_delete(struct map_t* map , int key)
{
	bool r = map_contains_key(map, key);
	if(!r)
		return r;
	
	int index = key % map->cell_count;
	
	int found_index = -1;
	for (int i=0; i < map->cells[index].used; i++)
	{
		if(map->cells[index].keys[i] == key)
		{
			found_index = i;
			break;
		}
	}
	
	for (int j=found_index; j < map->cells[index].used-1; j++)
	{
		map->cells[index].keys[j] = map->cells[index].keys[j+1];
		map->cells[index].values[j] = map->cells[index].values[j+1];
	}
	
	map->cells[index].used--;
	return true;
}

void map_trunk(struct map_t* map)
{
    for (int i=0; i<map->cell_count; i++) {
        map->cells[i].used = 0;
    }
}
