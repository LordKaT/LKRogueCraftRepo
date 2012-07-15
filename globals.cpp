#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include <SDL/SDL.h>

#include <libtcod/libtcod.h>

#include "def.h"
#include "structs.h"
#include "globals.h"

sEgoData g_ego;

int g_iWorld[DEF_WORLD_X][DEF_WORLD_Y];
bool g_bVisible[DEF_WORLD_X][DEF_WORLD_Y];
sInventory *g_inventoryWorldList[DEF_WORLD_X][DEF_WORLD_Y];
sILightmap g_iWorldLightmap[DEF_WORLD_X][DEF_WORLD_Y];

TCOD_map_t g_tcodMap;
TCOD_map_t g_tcodLightmap;

sItemList *g_itemDynamic;

sInventory *g_itemLook;	/* Built when using the look command, for the inventory interface. */

int g_iItemTracker;
int g_iGameMode;
char g_cConsole[32][672];
char g_cConsoleInput[32];
int g_iConsoleInputCount;

void globalsInit() {
	g_itemDynamic = NULL; // You just never know ...
	memset(&g_cConsole, 0, sizeof(g_cConsole));

	sILightmap tempLightmap = {-1, -1};

	for (int x = 0; x < DEF_WORLD_X; x++)
		for (int y = 0; y < DEF_WORLD_Y; y++)
			g_iWorldLightmap[x][y] = tempLightmap;
	return;
}
