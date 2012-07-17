#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>

#include <libtcod/libtcod.h>

#include "def.h"
#include "globals.h"

#include "bsp.h"
#include "inventory.h"

int mapCountNeighbors(int iX, int iY) {
	int iCount = 0;
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			if ((iX + x < 0) || (iX + x > (DEF_WORLD_X - 1)) || (iY + y < 0) || (iY + y > (DEF_WORLD_Y - 1))) continue;
			if (g_iWorld[iX + x][iY + y] == 1) iCount++;
		}
	}
	return iCount;
}

void mapGenerate(int iProbability, int iIterations, bool bOperation, int iClosedNeighbors) {
	int iX;
	int iY;
	int iCounter = 0;
	TCOD_noise_t noisePerlin = TCOD_noise_new(2, 0.5f, 2.0f, NULL);

	for (int x = 0; x < DEF_WORLD_X; x++) {
		for (int y = 0; y < DEF_WORLD_Y; y++) {
			if (rand() % 100 < iProbability)
				g_iWorld[x][y] = 1;
		}
	}

	while (iCounter < iIterations) {
		iY = rand() % DEF_WORLD_X;
		iX = rand() % DEF_WORLD_Y;

		if (g_iWorld[iX][iY] == 0) {
			if (iClosedNeighbors < mapCountNeighbors(iX, iY))
				g_iWorld[iX][iY] = 1;
		}
		else {
			if (iClosedNeighbors > mapCountNeighbors(iX, iY))
				g_iWorld[iX][iY] = 0;
		}
		iCounter++;
	}
	return;
}

void mapClear() {
	sILightmap tempLightmap = {-1, -1};
	for (int x = 0; x < DEF_WORLD_X; x++) {
		for (int y = 0; y < DEF_WORLD_Y; y++) {
			g_iWorld[x][y] = 0;
			g_bVisible[x][y] = false;
			g_iWorldLightmap[x][y] = tempLightmap;
		}
	}
	return;
}

void mapDraw() {
	TCOD_color_t tcodLight = {200, 180, 50};
	TCOD_color_t tcodToColor;
	int iTempX;
	int iTempY;
	for (int x = 0; x < DEF_WORLD_X; x++) {
		for (int y = 0; y < DEF_WORLD_Y; y++) {
			iTempX = g_iWorldLightmap[x][y].iX;
			iTempY = g_iWorldLightmap[x][y].iY;
			if (TCOD_map_is_in_fov(g_tcodMap, x, y)) {
				g_bVisible[x][y] = true;
				TCOD_console_put_char(NULL, x, y, g_itemData[g_iWorld[x][y]].m_stats.m_iASCII, TCOD_BKGND_NONE);
				if (g_npcMap[x][y] != NULL)
					TCOD_console_put_char(NULL, x, y, g_npcMap[x][y]->m_stats.m_iASCII, TCOD_BKGND_NONE);
				if (iTempX > -1 && iTempY > -1) {
					if (g_npcMap[x][y] == NULL) {
//						tcodToColor = TCOD_color_lerp(g_npcMap[x][y]->m_stats.m_tcodVisibleColor, tcodLight, (float)0.4);
						tcodToColor = TCOD_color_lerp(g_itemData[g_iWorld[x][y]].m_stats.m_tcodVisibleColor, tcodLight, (float)0.4);
						TCOD_console_set_back(NULL, x, y, tcodToColor, TCOD_BKGND_SET);
					}
				}
				else {
					TCOD_console_set_back(NULL, x, y, TCOD_black, TCOD_BKGND_SET);
				}
				TCOD_console_set_fore(NULL, x, y, g_itemData[g_iWorld[x][y]].m_stats.m_tcodVisibleColor);
				if (g_npcMap[x][y] != NULL)
					TCOD_console_set_fore(NULL, x, y, g_npcMap[x][y]->m_stats.m_tcodVisibleColor);
			}
			else if (g_bVisible[x][y] == true) {
				TCOD_console_set_back(NULL, x, y, TCOD_black, TCOD_BKGND_SET);
				TCOD_console_put_char(NULL, x, y, g_itemData[g_iWorld[x][y]].m_stats.m_iASCII, TCOD_BKGND_NONE);
				TCOD_console_set_fore(NULL, x, y, g_itemData[g_iWorld[x][y]].m_stats.m_tcodHiddenColor);
/*				if (g_npcMap[x][y] != NULL) {
					TCOD_console_put_char(NULL, x, y, g_npcMap[x][y]->m_stats.m_iASCII, TCOD_BKGND_NONE);
					TCOD_console_set_fore(NULL, x, y, g_npcMap[x][y]->m_stats..m_tcodHiddenColor);
				}
*/
			}
		}
	}
	return;
}

void mapMakeFOV() {
	for (int x = 0; x < DEF_WORLD_X; x++) {
		for (int y = 0; y < DEF_WORLD_Y; y++) {
			TCOD_map_set_properties(g_tcodMap, x, y, g_itemData[g_iWorld[x][y]].m_stats.m_bTransparent, g_itemData[g_iWorld[x][y]].m_stats.m_bWalkable);
		}
	}
	return;
}

void mapClearLightmap() {
	for (int iX = 0; iX < DEF_WORLD_X; iX++) {
		for (int iY = 0; iY < DEF_WORLD_Y; iY++) {
			g_iWorldLightmap[iX][iY].iX = -1;
			g_iWorldLightmap[iX][iY].iY = -1;
		}
	}
}

/*
**	I am writing this function in a way intended to piss you off. 
**	It is a testament to what happens when you don't plan properly,
**		and, instead, cobble functions together as you go.
**
**	In other words: it is my crowning achievment of programming assholery.
**
**							Good luck.
**														-- LordKaT
*/
void mapRemapLight(int iItemID) {
	mapClearLightmap();
	for (int iX = 0; iX < DEF_WORLD_X; iX++) {
		for (int iY = 0; iY < DEF_WORLD_Y; iY++) {
			if (inventoryCheck(&g_inventoryWorldList[iX][iY], 25) > 0) {
				TCOD_map_compute_fov(g_tcodLightmap, iX, iY, g_itemData[iItemID].m_stats.m_iFOV, false, FOV_SHADOW);
				for (int i = 0; i < DEF_WORLD_X; i++) {
					for (int j = 0; j < DEF_WORLD_Y; j++) {
						if (TCOD_map_is_in_fov(g_tcodLightmap, i, j)) {
							if (g_iWorldLightmap[i][j].iX == -1 && g_iWorldLightmap[i][j].iY == -1) {
								g_iWorldLightmap[i][j].iX = iX;
								g_iWorldLightmap[i][j].iY = iY;
							}
						}
					}
				}
			}
		}
	}
	return; /* For good measure. */
}
