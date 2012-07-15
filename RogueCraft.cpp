#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include <SDL/SDL.h>

#include <libtcod/libtcod.h>

#include "def.h"
#include "globals.h"
#include "structs.h"
#include "bsp.h"
#include "map.h"
#include "items.h"
#include "player.h"
#include "inventory.h"
#include "console.h"

int main(int argc, char **argv) {
	TCOD_key_t tKey;
	char cCharCode;

	globalsInit();

	TCOD_console_set_custom_font("terminal8x8_gs_ro.png", TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW, 16, 16);
	TCOD_console_init_root(100, 75, "RogueCraft", false);
	TCOD_console_set_background_color(NULL, TCOD_black);
	TCOD_console_set_foreground_color(NULL, TCOD_light_grey);

	g_tcodMap = TCOD_map_new(DEF_WORLD_X, DEF_WORLD_Y);

	mapClear();
	generateBSPtoMap(true);
	itemPopulate(ITEM_ORE_IRON, 0, 45, 100);
	itemPopulate(ITEM_ORE_COPPER, 0, 35, 100);
	itemPopulate(ITEM_ORE_STONE_WEAK, 0, 25, 100);

	/* Populate a torch ... or three. */
	itemPopulate(25, 1, 35, 100);
	mapRemapLight(25);

	int iX = rand() % DEF_WORLD_X;
	int iY = rand() % DEF_WORLD_Y;

	while (g_iWorld[iX][iY] != 0) {
		iX = rand() % DEF_WORLD_X;
		iY = rand() % DEF_WORLD_Y;
	}

	playerInit();

	g_ego.m_stats.m_iWorldX = iX;
	g_ego.m_stats.m_iWorldY = iY;

	playerCalcVisibility();

	TCOD_map_compute_fov(g_tcodMap, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, (int)g_ego.m_fFOVDistance, true, FOV_SHADOW);

	int iItem = ITEM_ORE_STONE;
	int iInventoryOffset = 0;
	int iInventoryPointer = 0;
	int iInventorySelection = -1;
	int iLookX = 0;
	int iLookY = 0;

	g_iGameMode = MODE_GAME;

	strcpy(g_ego.m_stats.m_cName, "Harry the Hero");

	while (!TCOD_console_is_window_closed()) {
		mapDraw();
		inventoryDrawBorder();
		playerStatsList();
		consoleDraw();
		consoleInputDraw();

		TCOD_console_put_char(NULL, 76, 33 + iInventoryPointer, 16, TCOD_BKGND_NONE);

		TCOD_console_put_char(NULL, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, '@', TCOD_BKGND_NONE);

		tKey = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED);
		cCharCode = tolower(tKey.c);

		if (g_iGameMode == MODE_GAME) {
			iInventorySelection = inventoryList(&g_ego.m_inventory, iInventoryPointer, iInventoryOffset);
			TCOD_console_print_left(NULL, 78, 31, TCOD_BKGND_NONE, "Inventory");

			if (cCharCode == 'q') {
				g_iGameMode = MODE_BLOCK;
				consoleAdd("Where?");
			}

			if (cCharCode == 'o') {
				g_iGameMode = MODE_OPEN;
				consoleAdd("Where?");
			}

			if (cCharCode == 'l') {
				g_iGameMode = MODE_LOOK;
				iLookX = g_ego.m_stats.m_iWorldX;
				iLookY = g_ego.m_stats.m_iWorldY;
				playerLook(iLookX, iLookY);
				inventoryClear();
				iInventoryOffset = 0;
				iInventoryPointer = 0;
				consoleAdd("Looking around...");
			}

			if (cCharCode == 'p') {
				consoleAdd("vis: %i", g_ego.m_iVisibility);
				consoleAdd("lm X/Y: %i/%i", g_iWorldLightmap[iX][iY].iX, g_iWorldLightmap[iX][iY].iY);
			}

			if (tKey.vk == TCODK_SPACE)
				g_iGameMode = MODE_CONSOLE_INPUT;
		}

		if (g_iGameMode == MODE_LOOK) {
			TCOD_console_put_char(NULL, iLookX, iLookY, 4, TCOD_BKGND_NONE);
			iInventorySelection = inventoryList(&g_itemLook, iInventoryPointer, iInventoryOffset);
			playerLook(iLookX, iLookY);
		}

		if (g_iGameMode == MODE_CONSOLE_INPUT) {
			if (tKey.vk == TCODK_ENTER) {
				consoleAdd(g_cConsoleInput);
				consoleInputClear();
				g_iGameMode = MODE_GAME;
			}
			else if (tKey.vk == TCODK_BACKSPACE)
				consoleInputDel();
			else {
				if (cCharCode >= 32 && cCharCode <= 126)
					consoleInputAdd(cCharCode);
			}
		}

		if (tKey.vk == TCODK_PRINTSCREEN)
			TCOD_sys_save_screenshot(NULL);

		/* Inventory controls */
		if (tKey.vk == TCODK_DOWN) {
			if (iInventoryPointer < 9) {
				TCOD_console_put_char(NULL, 76, 33 + iInventoryPointer, 32, TCOD_BKGND_NONE);
				iInventoryPointer++;
			}
			else {
				iInventoryOffset++;
			}
		}
		if (tKey.vk == TCODK_UP) {
			if (iInventoryPointer > 0) {
				TCOD_console_put_char(NULL, 76, 33 + iInventoryPointer, 32, TCOD_BKGND_NONE);
				iInventoryPointer--;
			}
			else {
				iInventoryOffset--;
			}
		}

		/* Movement/Look controls */
		if (tKey.vk >= TCODK_KP1 && tKey.vk <= TCODK_KP9) {

			int iNewX = 0;
			int iNewY = 0;

			if (g_iGameMode == MODE_LOOK) {
				iNewX = iLookX;
				iNewY = iLookY;
			}
			else {
				iNewX = g_ego.m_stats.m_iWorldX;
				iNewY = g_ego.m_stats.m_iWorldY;
			}
				

			if (tKey.vk == TCODK_KP1) {
				iNewX--;
				iNewY++;
			}

			if (tKey.vk == TCODK_KP2) {
				iNewY++;
			}

			if (tKey.vk == TCODK_KP3) {
				iNewX++;
				iNewY++;
			}

			if (tKey.vk == TCODK_KP4) {
				iNewX--;
			}

			if (tKey.vk == TCODK_KP6) {
				iNewX++;
			}

			if (tKey.vk == TCODK_KP7) {
				iNewX--;
				iNewY--;
			}

			if (tKey.vk == TCODK_KP8) {
				iNewY--;
			}

			if (tKey.vk == TCODK_KP9) {
				iNewX++;
				iNewY--;
			}

			if (iNewX >= 0 && iNewX < DEF_WORLD_X && iNewY >= 0 && iNewY < DEF_WORLD_Y) {
				switch (g_iGameMode) {
					case MODE_GAME:
						if (g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bWalkable) {
							g_ego.m_stats.m_iWorldX = iNewX;
							g_ego.m_stats.m_iWorldY = iNewY;
							TCOD_console_put_char(NULL, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, ' ', TCOD_BKGND_NONE);
							g_itemData[g_iWorld[iNewX][iNewY]].m_vFunc(ACT_WALK_ONTO, iNewX, iNewY, NULL);
						}
						else {
							g_itemData[g_iWorld[iNewX][iNewY]].m_vFunc(ACT_WALK_INTO, iNewX, iNewY, NULL);
							mapMakeFOV();
							inventoryClear();
						}
						TCOD_map_compute_fov(g_tcodMap, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, (int)g_ego.m_fFOVDistance, true, FOV_SHADOW);
						TCOD_map_set_properties(g_tcodMap, iNewX, iNewY, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bTransparent, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bWalkable);
						TCOD_map_copy(g_tcodMap, g_tcodLightmap);
						mapRemapLight(25);
						playerCalcVisibility();
						break;
					case MODE_BLOCK:
						g_itemData[iItem].m_vFunc(ACT_PLACE_ONTO, iNewX, iNewY, (void *)iItem);
						mapMakeFOV();
						g_iGameMode = MODE_GAME;
						TCOD_map_compute_fov(g_tcodMap, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, (int)g_ego.m_fFOVDistance, true, FOV_SHADOW);
						TCOD_map_set_properties(g_tcodMap, iNewX, iNewY, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bTransparent, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bWalkable);
						TCOD_map_copy(g_tcodMap, g_tcodLightmap);
						mapRemapLight(25);
						playerCalcVisibility();
						break;
					case MODE_OPEN:
						g_itemData[g_iWorld[iNewX][iNewY]].m_vFunc(ACT_OPEN, iNewX, iNewY, NULL);
						mapMakeFOV();
						g_iGameMode = MODE_GAME;
						TCOD_map_compute_fov(g_tcodMap, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, (int)g_ego.m_fFOVDistance, true, FOV_SHADOW);
						TCOD_map_set_properties(g_tcodMap, iNewX, iNewY, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bTransparent, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bWalkable);
						TCOD_map_copy(g_tcodMap, g_tcodLightmap);
						mapRemapLight(25);
						playerCalcVisibility();
						break;
					case MODE_LOOK:
						TCOD_console_put_char(NULL, iLookX, iLookY, 0, TCOD_BKGND_NONE);
						iLookX = iNewX;
						iLookY = iNewY;
						playerLook(iLookX, iLookY);
						inventoryClear();
						iInventoryOffset = 0;
						iInventoryPointer = 0;
						break;
					default:
						break;
				}
			}
		}

		else {
			if (g_iGameMode == MODE_BLOCK) {
				if (tKey.vk < TCODK_KP1 && tKey.vk > TCODK_KP9) {
					if (cCharCode == '1')
						iItem = ITEM_ORE_STONE;
					if (cCharCode == '2')
						iItem = ITEM_ORE_IRON;
					if (cCharCode == '3')
						iItem = ITEM_ORE_COPPER;
					if (cCharCode == '4')
						iItem = ITEM_ORE_SILVER;
					if (cCharCode == '5')
						iItem = ITEM_ORE_GOLD;
				}
			}
		}

		if (tKey.vk == TCODK_ESCAPE) {
			if (g_iGameMode != MODE_GAME) {
				if (g_iGameMode == MODE_LOOK)
					TCOD_console_put_char(NULL, iLookX, iLookY, 0, TCOD_BKGND_NONE);
				g_iGameMode = MODE_GAME;
				inventoryClear();
			}
			else {
				TCOD_console_delete(NULL);
				SDL_Quit();
				break;
			}
		}
		TCOD_console_flush();
	}
	return 1;
}
