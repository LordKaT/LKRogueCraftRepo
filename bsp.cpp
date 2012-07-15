#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>

#include <libtcod/libtcod.h>

#include "globals.h"
#include "structs.h"

typedef	char map_t[75][75];

int g_iBSPDepth = 8;
int g_iMinRoomSize = 3;
int g_iCoorProb = 95;
bool g_bRandomRoom = true; // a room fills a random part of the node or the maximum available space ?
bool g_bRoomWalls = true; // if true, there is always a wall on north & west side of a room

// draw a vertical line
void bspVLine(map_t *map, int x, int y1, int y2) {
	int y = y1;
	int dy = ((y1 > y2) ? -1 : 1);
	(*map)[x][y] = ' ';
	if (y1 == y2) return;
	do {
		y += dy;
		(*map)[x][y] = ' ';
	} while (y != y2);
}

// draw a vertical line up until we reach an empty space
void bspVLine_Up(map_t *map, int x, int y) {
	while (y >= 0 && (*map)[x][y] != ' ') {
		(*map)[x][y] = ' ';
		y--;
	}
}

// draw a vertical line down until we reach an empty space
void bspVLine_Down(map_t *map, int x, int y) {
	while (y < 75 && (*map)[x][y] != ' ') {
		(*map)[x][y] = ' ';
		y++;
	}
}

// draw a horizontal line
void bspHLine(map_t *map, int x1, int y, int x2) {
	int x = x1;
	int dx = ((x1 > x2) ? -1 : 1);
	(*map)[x][y] = ' ';
	if (x1 == x2) return;
	do {
		x += dx;
		(*map)[x][y] = ' ';
	} while (x != x2);
}

// draw a horizontal line left until we reach an empty space
void bspHLine_Left(map_t *map, int x, int y) {
	while (x >= 0 && (*map)[x][y] != ' ') {
		(*map)[x][y] = ' ';
		x--;
	}
}

// draw a horizontal line right until we reach an empty space
void bspHLine_Right(map_t *map,int x, int y) {
	while (x < 75 && (*map)[x][y] != ' ') {
		(*map)[x][y] = ' ';
		x++;
	}
}

static bool bspTraverseCorridor(TCOD_bsp_t *node, void *userData) {
	map_t *map = (map_t *)userData;
	//if (rand() % 100 >= 55) return true;
	if (TCOD_bsp_is_leaf(node)) {
		// calculate the room size
		int minx = node->x + 1;
		int maxx = node->x + node->w - 1;
		int miny = node->y + 1;
		int maxy = node->y + node->h - 1;
		int x, y;
		if (!g_bRoomWalls) {
			if (minx > 1) minx--;
			if (miny > 1) miny--;
		}
		if (maxx == 75 - 1 ) maxx--;
		if (maxy == 75 - 1 ) maxy--;
		if (g_bRandomRoom) {
			minx = TCOD_random_get_int(NULL, minx, maxx - g_iMinRoomSize + 1);
			miny = TCOD_random_get_int(NULL, miny, maxy - g_iMinRoomSize + 1);
			maxx = TCOD_random_get_int(NULL, minx + g_iMinRoomSize - 1, maxx);
			maxy = TCOD_random_get_int(NULL, miny + g_iMinRoomSize - 1, maxy);
		}
		// resize the node to fit the room
		node->x = minx;
		node->y = miny;
		node->w = maxx - minx + 1;
		node->h = maxy - miny + 1;
		// dig the room
		for (x = minx; x <= maxx; x++ )
			for (y = miny; y <= maxy; y++ )
				(*map)[x][y] = ' ';
	} else {
		// resize the node to fit its sons
		TCOD_bsp_t *left = TCOD_bsp_left(node);
		TCOD_bsp_t *right = TCOD_bsp_right(node);
		node->x = MIN(left->x, right->x);
		node->y = MIN(left->y, right->y);
		node->w = MAX(left->x + left->w, right->x + right->w) - node->x;
		node->h = MAX(left->y + left->h, right->y + right->h) - node->y;
		// create a corridor between the two lower nodes
		if (rand() % 100 <= g_iCoorProb) {
			if (node->horizontal) {
				// vertical corridor
				if (left->x + left->w - 1 < right->x || right->x + right->w - 1 < left->x ) {
					// no overlapping zone. we need a Z shaped corridor
					int x1 = TCOD_random_get_int(NULL, left->x, left->x + left->w - 1);
					int x2 = TCOD_random_get_int(NULL, right->x, right->x + right->w - 1);
					int y = TCOD_random_get_int(NULL, left->y + left->h, right->y);
					bspVLine_Up(map, x1, y - 1);
					bspHLine(map, x1, y, x2);
					bspVLine_Down(map, x2, y + 1);
				} else {
					// straight vertical corridor
					int minx = MAX(left->x, right->x);
					int maxx = MIN(left->x + left->w - 1, right->x + right->w - 1);
					int x = TCOD_random_get_int(NULL, minx, maxx);
					bspVLine_Down(map, x, right->y);
					bspVLine_Up(map, x, right->y - 1);
				}
			} else {
				// horizontal corridor
				if (left->y + left->h - 1 < right->y || right->y + right->h - 1 < left->y ) {
					// no overlapping zone. we need a Z shaped corridor
					int y1 = TCOD_random_get_int(NULL, left->y, left->y + left->h - 1);
					int y2 = TCOD_random_get_int(NULL, right->y, right->y + right->h - 1);
					int x = TCOD_random_get_int(NULL, left->x + left->w, right->x);
					bspHLine_Left(map, x - 1, y1);
					bspVLine(map, x, y1, y2);
					bspHLine_Right(map, x + 1, y2);
				} else {
					// straight horizontal corridor
					int miny = MAX(left->y, right->y);
					int maxy = MIN(left->y + left->h - 1, right->y + right->h - 1);
					int y = TCOD_random_get_int(NULL, miny, maxy);
					bspHLine_Left(map, right->x - 1, y);
					bspHLine_Right(map, right->x, y);
				}
			}
		}
	}
	return true;
}

static bool bspTraverseNoCorridor(TCOD_bsp_t *node, void *userData) {
	map_t *map = (map_t *)userData;
	if (TCOD_bsp_is_leaf(node)) {
		// calculate the room size
		int minx = node->x + 1;
		int maxx = node->x + node->w - 1;
		int miny = node->y + 1;
		int maxy = node->y + node->h - 1;
		int x, y;
		if (!g_bRoomWalls ) {
			if (minx > 1) minx--;
			if (miny > 1) miny--;
		}
		if (maxx == 75 - 1 ) maxx--;
		if (maxy == 75 - 1 ) maxy--;
		if (g_bRandomRoom ) {
			minx = TCOD_random_get_int(NULL, minx, maxx - g_iMinRoomSize + 1);
			miny = TCOD_random_get_int(NULL, miny, maxy - g_iMinRoomSize + 1);
			maxx = TCOD_random_get_int(NULL, minx + g_iMinRoomSize - 1, maxx);
			maxy = TCOD_random_get_int(NULL, miny + g_iMinRoomSize - 1, maxy);
		}
		// resize the node to fit the room
		node->x = minx;
		node->y = miny;
		node->w = maxx - minx + 1;
		node->h = maxy - miny + 1;
		// dig the room
		for (x = minx; x <= maxx; x++ ) {
			for (y = miny; y <= maxy; y++ ) {
				(*map)[x][y] = ' ';
			}
		}
	} else {
		// resize the node to fit its sons
		TCOD_bsp_t *left = TCOD_bsp_left(node);
		TCOD_bsp_t *right = TCOD_bsp_right(node);
		node->x = MIN(left->x, right->x);
		node->y = MIN(left->y, right->y);
		node->w = MAX(left->x + left->w, right->x + right->w) - node->x;
		node->h = MAX(left->y + left->h, right->y + right->h) - node->y;
	}
	return true;
}

#define NUM_DOOR_PATTERNS	4

char doorpattern[NUM_DOOR_PATTERNS][3][3] = {
	{
		{' ', ' ', ' '},
		{'#', ' ', '#'},
		{'#', ' ', '#'}
	},
	{
		{'#', ' ', '#'},
		{'#', ' ', '#'},
		{' ', ' ', ' '}
	},
	{
		{'#', '#', ' '},
		{' ', ' ', ' '},
		{'#', '#', ' '}
	},
	{
		{' ', '#', '#'},
		{' ', ' ', ' '},
		{' ', '#', '#'}
	},
/*
	These two patterns make problems. Don't know why.
	{
		{'#', '#', '#'},
		{' ', ' ', ' '},
		{'#', '#', '#'}
	},
	{
		{'#', ' ', '#'},
		{'#', ' ', '#'},
		{'#', ' ', '#'}
	}
*/
};

void makedoors(void *userData) {
	map_t *map = (map_t *)userData;

	for (int x = 0; x < DEF_WORLD_X; x++) {
		for (int y = 0; y < DEF_WORLD_Y; y++) {
			if ((x > 0 && x < DEF_WORLD_X-1) && (y > 0 && y < DEF_WORLD_Y-1)) {
				for (int i = 0; i < NUM_DOOR_PATTERNS; i++) {
					if ((*map)[x-1][y-1] == doorpattern[i][0][0] && (*map)[x][y-1] == doorpattern[i][1][0] && (*map)[x+1][y-1] == doorpattern[i][2][0] &&
						(*map)[x-1][y] == doorpattern[i][0][1] && (*map)[x][y] == doorpattern[i][1][1] && (*map)[x+1][y] == doorpattern[i][2][1] &&
						(*map)[x-1][y+1] == doorpattern[i][0][2] && (*map)[x][y+1] == doorpattern[i][1][2] && (*map)[x+1][y+1] == doorpattern[i][2][2])
						(*map)[x][y] = 'D';
				}
			}
		}
	}

	return;
}

void generateBSPtoMap(bool bCorridor) {
	TCOD_bsp_t *bsp = NULL;
	char map[DEF_WORLD_X][DEF_WORLD_Y];

	bsp = TCOD_bsp_new_with_size(0, 0, DEF_WORLD_X, DEF_WORLD_Y);
	memset(&map, '#', sizeof(map));
	TCOD_bsp_remove_sons(bsp);
	TCOD_bsp_split_recursive(bsp, NULL, g_iBSPDepth, g_iMinRoomSize + ((g_bRoomWalls) ? 1 : 0), g_iMinRoomSize + ((g_bRoomWalls) ? 1 : 0 ), 1.5f, 1.5f);
	if (bCorridor)
		TCOD_bsp_traverse_inverted_level_order(bsp, (TCOD_bsp_callback_t)bspTraverseCorridor, &map);
	else
		TCOD_bsp_traverse_inverted_level_order(bsp, (TCOD_bsp_callback_t)bspTraverseNoCorridor, &map);

	makedoors(&map);

	g_tcodMap = TCOD_map_new(DEF_WORLD_X, DEF_WORLD_Y);
	g_tcodLightmap = TCOD_map_new(DEF_WORLD_X, DEF_WORLD_Y);

	for (int x = 0; x < DEF_WORLD_X; x++) {
		for (int y = 0; y < DEF_WORLD_Y; y++) {
			switch (map[x][y]) {
				case '#':
					g_iWorld[x][y] = ITEM_ORE_STONE;
					break;
				case 'D':
					g_iWorld[x][y] = ITEM_DOOR_CLOSED;
					break;
				case ' ':
					g_iWorld[x][y] = ITEM_NOTHING;
					break;
				default:
					g_iWorld[x][y] = ITEM_NOTHING;
					break;
			}
			TCOD_map_set_properties(g_tcodMap, x, y, g_itemData[g_iWorld[x][y]].m_stats.m_bTransparent, g_itemData[g_iWorld[x][y]].m_stats.m_bWalkable);
			//TCOD_map_set_properties(g_tcodLightmap, x, y, g_itemData[g_iWorld[x][y]].m_stats.m_bTransparent, g_itemData[g_iWorld[x][y]].m_stats.m_bWalkable);
		}
	}

	TCOD_map_copy(g_tcodMap, g_tcodLightmap);

	//g_tcodLightmap = g_tcodMap;

	return;
}
