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
#include "npc.h"
#include "inventory.h"
#include "console.h"
#include "scene_intro.h"

int main(int argc, char **argv) {
	int iItemToPlace = ITEM_ORE_STONE;
	int iInventoryOffset = 0;
	int iInventoryPointer = 0;
	int iInventorySelection = -1;
	int iLookX = 0;
	int iLookY = 0;
	int iNewX = 0;
	int iNewY = 0;
	int iSelect = 0;
	TCOD_key_t tKey;
	char cCharCode;

	bool bTurnOver = false;

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

	playerInit();

	TCOD_map_compute_fov(g_tcodMap, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, (int)g_ego.m_fFOVDistance, true, FOV_SHADOW);
	iNewX = g_ego.m_stats.m_iWorldX;
	iNewY = g_ego.m_stats.m_iWorldY;

	/* Debug code for inventory management */
	inventoryAdd(&g_ego.m_inventory, 30, 1);
	inventoryAdd(&g_ego.m_inventory, 31, 1);

	g_iGameMode = MODE_INTRO;

	npcPopulate(1, 1, 55, 100);

	while (!TCOD_console_is_window_closed()) {
		tKey = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED);
		cCharCode = tolower(tKey.c);

		/* Global inputs */
		switch (tKey.vk) {
			case TCODK_ESCAPE:
				if (g_iGameMode != MODE_GAME && g_iGameMode != MODE_INTRO) {
					if (g_iGameMode == MODE_LOOK) {
						iNewX = g_ego.m_stats.m_iWorldX;
						iNewY = g_ego.m_stats.m_iWorldY;
						TCOD_console_put_char(NULL, iLookX, iLookY, 0, TCOD_BKGND_NONE);
					}
					g_iGameMode = MODE_GAME;
					inventoryClear();
				}
				else {
					TCOD_console_delete(NULL);
					SDL_Quit();
					return 1;
				}
				break;
			case TCODK_PRINTSCREEN:
				TCOD_sys_save_screenshot(NULL);
				break;
			case TCODK_KP1:
				iNewX--;
				iNewY++;
				break;
			case TCODK_KP2:
				iNewY++;
				break;
			case TCODK_KP3:
				iNewX++;
				iNewY++;
				break;
			case TCODK_KP4:
				iNewX--;
				break;
			case TCODK_KP6:
				iNewX++;
				break;
			case TCODK_KP7:
				iNewX--;
				iNewY--;
				break;
			case TCODK_KP8:
				iNewY--;
				break;
			case TCODK_KP9:
				iNewX++;
				iNewY--;
				break;
			default:
				break;
		}

		/* Mode inputs */
		switch (g_iGameMode) {
			case MODE_INTRO:
				TCOD_console_clear(NULL);
				scene_intro();
				scene_intro_input(tKey, cCharCode);
				break;
			case MODE_GAME:
				mapDraw();
				inventoryDrawBorder();
				playerStatsList();
				consoleDraw();
				consoleInputDraw();

				TCOD_console_put_char(NULL, 76, 33 + iInventoryPointer, 16, TCOD_BKGND_NONE);
				TCOD_console_put_char(NULL, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, '@', TCOD_BKGND_NONE);
				iInventorySelection = inventoryList(&g_ego.m_inventory, iInventoryPointer, iInventoryOffset);
				TCOD_console_print_left(NULL, 78, 31, TCOD_BKGND_NONE, "Inventory");

				if (tKey.vk >= TCODK_KP1 && tKey.vk <= TCODK_KP9) {
					if (iNewX >= 0 && iNewX < DEF_WORLD_X && iNewY >= 0 && iNewY < DEF_WORLD_Y) {
						if (g_npcMap[iNewX][iNewY] == NULL) {
							if (g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bWalkable) {
								g_ego.m_stats.m_iWorldX = iNewX;
								g_ego.m_stats.m_iWorldY = iNewY;
								TCOD_console_put_char(NULL, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, ' ', TCOD_BKGND_NONE);
								g_itemData[g_iWorld[iNewX][iNewY]].m_vFunc(ACT_WALK_ONTO, iNewX, iNewY, NULL);
							}
							else {
								g_itemData[g_iWorld[iNewX][iNewY]].m_vFunc(ACT_WALK_INTO, iNewX, iNewY, NULL);
								iNewX = g_ego.m_stats.m_iWorldX;
								iNewY = g_ego.m_stats.m_iWorldY;
								mapMakeFOV();
								inventoryClear();
							}
						}
						else {
							/* We've bumped into an NPC. */
							g_npcMap[iNewX][iNewY]->m_vFunc(ACT_WALK_INTO, iNewX, iNewY, g_npcMap[iNewX][iNewY]);
							iNewX = g_ego.m_stats.m_iWorldX;
							iNewY = g_ego.m_stats.m_iWorldY;
						}
						TCOD_map_compute_fov(g_tcodMap, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, (int)g_ego.m_fFOVDistance, true, FOV_SHADOW);
						TCOD_map_set_properties(g_tcodMap, iNewX, iNewY, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bTransparent, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bWalkable);
						TCOD_map_copy(g_tcodMap, g_tcodLightmap);
						mapRemapLight(25);
						playerCalcVisibility();
						bTurnOver = true;
					}
				}

				switch (cCharCode) {
					case 'q': /* Place block (why q?) */
						g_iGameMode = MODE_BLOCK;
						consoleAdd("Place where?");
						break;
					case 'o': /* Open something */
						g_iGameMode = MODE_OPEN;
						consoleAdd("Open where?");
						break;
					case 'l': /* Look around */
						g_iGameMode = MODE_LOOK;
						iLookX = g_ego.m_stats.m_iWorldX;
						iLookY = g_ego.m_stats.m_iWorldY;
						iNewX = iLookX;
						iNewY = iLookY;
						playerLook(iLookX, iLookY);
						inventoryClear();
						iInventoryOffset = 0;
						iInventoryPointer = 0;
						consoleAdd("Looking around...");
						break;
					case 'e': /* Equip selected item in inventory */
						g_itemData[iInventorySelection].m_vFunc(ACT_EQUIP, 0, 0,(void *)(iInventorySelection));
						break;
					case 'd': /* Dequip an item */
						g_iGameMode = MODE_DEQUIP;
						consoleAdd("Remove what?");
						break;
					case 14: /* Up arrow (move inventory pointer) */
						if (iInventoryPointer > 0) {
							TCOD_console_put_char(NULL, 76, 33 + iInventoryPointer, 32, TCOD_BKGND_NONE);
							iInventoryPointer--;
						}
						else
							iInventoryOffset--;
						break;
					case 17: /* Down arrow (move inventory pointer) */
						if (iInventoryPointer < 9) {
							TCOD_console_put_char(NULL, 76, 33 + iInventoryPointer, 32, TCOD_BKGND_NONE);
							iInventoryPointer++;
						}
						else
							iInventoryOffset++;
						break;
					case 64: /* Space */
						g_iGameMode = MODE_CONSOLE_INPUT;
						break;
					default:
						break;
				}
				break;
			case MODE_BLOCK:
				if (tKey.vk >= TCODK_KP1 && tKey.vk <= TCODK_KP9) {
					if (iNewX >= 0 && iNewX < DEF_WORLD_X && iNewY >= 0 && iNewY < DEF_WORLD_Y) {
						g_itemData[iItemToPlace].m_vFunc(ACT_PLACE_ONTO, iNewX, iNewY, (void *)iItemToPlace);
						mapMakeFOV();
						g_iGameMode = MODE_GAME;
						TCOD_map_compute_fov(g_tcodMap, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, (int)g_ego.m_fFOVDistance, true, FOV_SHADOW);
						TCOD_map_set_properties(g_tcodMap, iNewX, iNewY, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bTransparent, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bWalkable);
						TCOD_map_copy(g_tcodMap, g_tcodLightmap);
						mapRemapLight(25);
						playerCalcVisibility();
						bTurnOver = true;
					}
				}
				else {
					if (cCharCode == '1')
						iItemToPlace = ITEM_ORE_STONE;
					if (cCharCode == '2')
						iItemToPlace = ITEM_ORE_IRON;
					if (cCharCode == '3')
						iItemToPlace = ITEM_ORE_COPPER;
					if (cCharCode == '4')
						iItemToPlace = ITEM_ORE_SILVER;
					if (cCharCode == '5')
						iItemToPlace = ITEM_ORE_GOLD;
				}
				break;
			case MODE_OPEN:
				if (tKey.vk >= TCODK_KP1 && tKey.vk <= TCODK_KP9) {
					if (iNewX >= 0 && iNewX < DEF_WORLD_X && iNewY >= 0 && iNewY < DEF_WORLD_Y) {
						g_itemData[g_iWorld[iNewX][iNewY]].m_vFunc(ACT_OPEN, iNewX, iNewY, NULL);
						mapMakeFOV();
						g_iGameMode = MODE_GAME;
						TCOD_map_compute_fov(g_tcodMap, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY, (int)g_ego.m_fFOVDistance, true, FOV_SHADOW);
						TCOD_map_set_properties(g_tcodMap, iNewX, iNewY, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bTransparent, g_itemData[g_iWorld[iNewX][iNewY]].m_stats.m_bWalkable);
						TCOD_map_copy(g_tcodMap, g_tcodLightmap);
						mapRemapLight(25);
						playerCalcVisibility();
						bTurnOver = true;
					}
				}
				break;
			case MODE_DEQUIP:
				switch (cCharCode) {
					case '1':
						iSelect = g_ego.m_equipment.m_iHand_L;
						g_ego.m_equipment.m_iHand_L = ITEM_NOTHING;
						break;
					case '2':
						iSelect = g_ego.m_equipment.m_iHand_R;
						g_ego.m_equipment.m_iHand_R = ITEM_NOTHING;
						break;
					case '3':
						iSelect = g_ego.m_equipment.m_iHead;
						g_ego.m_equipment.m_iHead = ITEM_NOTHING;
						break;
					case '4':
						iSelect = g_ego.m_equipment.m_iEar_L;
						g_ego.m_equipment.m_iEar_L = ITEM_NOTHING;
						break;
					case '5':
						iSelect = g_ego.m_equipment.m_iEar_R;
						g_ego.m_equipment.m_iEar_R = ITEM_NOTHING;
						break;
					case '6':
						iSelect = g_ego.m_equipment.m_iFace;
						g_ego.m_equipment.m_iFace = ITEM_NOTHING;
						break;
					case '7':
						iSelect = g_ego.m_equipment.m_iNeck;
						g_ego.m_equipment.m_iNeck = ITEM_NOTHING;
						break;
					case '8':
						iSelect = g_ego.m_equipment.m_iBack;
						g_ego.m_equipment.m_iBack = ITEM_NOTHING;
						break;
					case '9':
						iSelect = g_ego.m_equipment.m_iChest;
						g_ego.m_equipment.m_iChest = ITEM_NOTHING;
						break;
					case '0':
						iSelect = g_ego.m_equipment.m_iHands;
						g_ego.m_equipment.m_iHands = ITEM_NOTHING;
						break;
					case 'a':
						iSelect = g_ego.m_equipment.m_iWaist;
						g_ego.m_equipment.m_iWaist = ITEM_NOTHING;
						break;
					case 'b':
						iSelect = g_ego.m_equipment.m_iLegs;
						g_ego.m_equipment.m_iLegs = ITEM_NOTHING;
						break;
					case 'c':
						iSelect = g_ego.m_equipment.m_iFeet;
						g_ego.m_equipment.m_iFeet = ITEM_NOTHING;
						break;
					default:
						break;
				}
				if (iSelect != 0) {
					g_itemData[iSelect].m_vFunc(ACT_DEQUIP, 0, 0,(void *)(iSelect));
					g_iGameMode = MODE_GAME;
				}
				break;
			case MODE_LOOK:
				if (tKey.vk >= TCODK_KP1 && tKey.vk <= TCODK_KP9) {
					if (iNewX >= 0 && iNewX < DEF_WORLD_X && iNewY >= 0 && iNewY < DEF_WORLD_Y) {
						TCOD_console_put_char(NULL, iLookX, iLookY, 0, TCOD_BKGND_NONE);
						iLookX = iNewX;
						iLookY = iNewY;
						playerLook(iLookX, iLookY);
						inventoryClear();
						iInventoryOffset = 0;
						iInventoryPointer = 0;
					}
				}
				TCOD_console_put_char(NULL, iLookX, iLookY, 4, TCOD_BKGND_NONE);
				iInventorySelection = inventoryList(&g_itemLook, iInventoryPointer, iInventoryOffset);
				playerLook(iLookX, iLookY);
				break;
			case MODE_CONSOLE_INPUT:
				switch (tKey.vk) {
					case TCODK_ENTER:
						consoleAdd(g_cConsoleInput);
						consoleInputClear();
						g_iGameMode = MODE_GAME;
						break;
					case TCODK_BACKSPACE:
						consoleInputDel();
						break;
					default:
						if (cCharCode >= 32 && cCharCode <= 126)
							consoleInputAdd(cCharCode);
						break;
				}
				break;
			default:
				break;
		}

		if (bTurnOver) {
			npcExecute(&g_npcList);
			bTurnOver = false;
		}

		TCOD_console_flush();
	}
	return 1;
}
