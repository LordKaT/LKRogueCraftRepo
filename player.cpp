#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>

#include <libtcod/libtcod.h>

#include "def.h"
#include "globals.h"
#include "structs.h"
#include "items.h"
#include "itemfunc.h"
#include "inventory.h"

void playerInit() {
	g_ego.m_inventory = NULL;
	g_ego.m_iVisibility = 0;
	g_ego.m_fFOVDistance = 10.0f;
	return;
}

void playerStatsList() {
	for (int x = 76; x < 99; x++) {
		for (int y = 1; y < 29; y++) {
			TCOD_console_print_left(NULL, x, y, TCOD_BKGND_NONE, " ");
		}
	}
	TCOD_console_print_left(NULL, 79, 0, TCOD_BKGND_NONE, "%s", g_ego.m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 2, TCOD_BKGND_NONE, "HP: %i/%i", g_ego.m_stats.m_iHP, g_ego.m_stats.m_iMaxHP);
	TCOD_console_print_left(NULL, 76, 3, TCOD_BKGND_NONE, "SP: %i/%i", g_ego.m_stats.m_iSP, g_ego.m_stats.m_iMaxSP);

	/* visibility meter */

	if (g_ego.m_iVisibility >= 80)
		TCOD_console_put_char(NULL, 97, 2, 219, TCOD_BKGND_NONE);
	if (g_ego.m_iVisibility >= 60)
		TCOD_console_put_char(NULL, 96, 2, 178, TCOD_BKGND_NONE);
	if (g_ego.m_iVisibility >= 40)
		TCOD_console_put_char(NULL, 95, 2, 177, TCOD_BKGND_NONE);
	if (g_ego.m_iVisibility > 0)
		TCOD_console_put_char(NULL, 94, 2, 176, TCOD_BKGND_NONE);
	if (g_ego.m_iVisibility == 0)
		TCOD_console_print_left(NULL, 94, 2, TCOD_BKGND_NONE, "INVIS");

	TCOD_console_print_left(NULL, 76, 5, TCOD_BKGND_NONE, "STR: %i", g_ego.m_stats.m_iStr);
	TCOD_console_print_left(NULL, 76, 6, TCOD_BKGND_NONE, "DEX: %i", g_ego.m_stats.m_iDex);
	TCOD_console_print_left(NULL, 76, 7, TCOD_BKGND_NONE, "AGL: %i", g_ego.m_stats.m_iAgl);
	TCOD_console_print_left(NULL, 76, 8, TCOD_BKGND_NONE, "CHA: %i", g_ego.m_stats.m_iCha);
	TCOD_console_print_left(NULL, 76, 9, TCOD_BKGND_NONE, "WIS: %i", g_ego.m_stats.m_iWis);
	TCOD_console_print_left(NULL, 76, 10, TCOD_BKGND_NONE, "INT: %i", g_ego.m_stats.m_iInt);
	TCOD_console_print_left(NULL, 76, 11, TCOD_BKGND_NONE, "ATK: %i", g_ego.m_stats.m_iAtk);
	TCOD_console_print_left(NULL, 76, 12, TCOD_BKGND_NONE, "DEF: %i", g_ego.m_stats.m_iDef);

	TCOD_console_put_char(NULL, 75, 15, 204, TCOD_BKGND_NONE);
	for (int x = 76; x < 99; x++)
		TCOD_console_put_char(NULL, x, 15, 205, TCOD_BKGND_NONE);
	TCOD_console_put_char(NULL, 99, 15, 185, TCOD_BKGND_NONE);
	TCOD_console_print_left(NULL, 78, 15, TCOD_BKGND_NONE, "Equipment");

	TCOD_console_print_left(NULL, 76, 17, TCOD_BKGND_NONE, "Head:   %s", g_itemData[g_ego.m_equipment.m_iHead].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 18, TCOD_BKGND_NONE, "L Ear:  %s", g_itemData[g_ego.m_equipment.m_iEar_L].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 19, TCOD_BKGND_NONE, "R Ear:  %s", g_itemData[g_ego.m_equipment.m_iEar_R].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 20, TCOD_BKGND_NONE, "Face:   %s", g_itemData[g_ego.m_equipment.m_iFace].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 21, TCOD_BKGND_NONE, "Neck:   %s", g_itemData[g_ego.m_equipment.m_iNeck].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 22, TCOD_BKGND_NONE, "Back:   %s", g_itemData[g_ego.m_equipment.m_iBack].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 23, TCOD_BKGND_NONE, "Chest:  %s", g_itemData[g_ego.m_equipment.m_iChest].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 24, TCOD_BKGND_NONE, "Hands:  %s", g_itemData[g_ego.m_equipment.m_iHands].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 25, TCOD_BKGND_NONE, "Waist:  %s", g_itemData[g_ego.m_equipment.m_iWaist].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 26, TCOD_BKGND_NONE, "Legs:   %s", g_itemData[g_ego.m_equipment.m_iLegs].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 27, TCOD_BKGND_NONE, "Feet:   %s", g_itemData[g_ego.m_equipment.m_iFeet].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 28, TCOD_BKGND_NONE, "L Hand: %s", g_itemData[g_ego.m_equipment.m_iHand_L].m_stats.m_cName);
	TCOD_console_print_left(NULL, 76, 29, TCOD_BKGND_NONE, "R Hand: %s", g_itemData[g_ego.m_equipment.m_iHand_R].m_stats.m_cName);

	return;
}

void playerLook(int iX, int iY) {
	inventoryFree(&g_itemLook);

	if (TCOD_map_is_in_fov(g_tcodMap, iX, iY))
		TCOD_console_print_left(NULL, 78, 31, TCOD_BKGND_NONE, "You see...");
	else if (g_bVisible[iX][iY] == true)
		TCOD_console_print_left(NULL, 78, 31, TCOD_BKGND_NONE, "You recall seeing...");

	if (g_bVisible[iX][iY] == true) {
		if (g_iWorld[iX][iY] != ITEM_NOTHING)
			inventoryAdd(&g_itemLook, g_iWorld[iX][iY], 1);

		if (g_inventoryWorldList[iX][iY] != NULL) {
			sInventory *head = g_inventoryWorldList[iX][iY];
			sItemData tempItem;
			while (head != NULL) {
				if (head->m_iItem < 1000000)
					tempItem = g_itemData[head->m_iItem];
				else {
					sItemList *itemhead = g_itemDynamic;
					while (itemhead != NULL) {
						if (itemhead->m_item.m_iID == head->m_iItem)
							tempItem = g_itemData[head->m_iItem];
						itemhead = itemhead->next;
					}
				}
				inventoryAdd(&g_itemLook, tempItem.m_iID, 1);
				head = head->next;
			}
		}

		if (iX == g_ego.m_stats.m_iWorldX && iY == g_ego.m_stats.m_iWorldY)
			inventoryAdd(&g_itemLook, 28, 1);
	}
	return;
}

void playerCalcVisibility() {
	g_ego.m_iVisibility = 0;

	for (int iX = 0; iX < DEF_WORLD_X; iX++) {
		for (int iY = 0; iY < DEF_WORLD_Y; iY++) {
			if (inventoryCheck(&g_inventoryWorldList[iX][iY], 25) > 0) {
				TCOD_map_compute_fov(g_tcodLightmap, iX, iY, 5, false, FOV_SHADOW);
				if (TCOD_map_is_in_fov(g_tcodLightmap, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY)) {
					int iterX = iX;
					int iterY = iY;
					int iCount = 0;
					TCOD_line_init(iterX, iterY, g_ego.m_stats.m_iWorldX, g_ego.m_stats.m_iWorldY);
					while (!TCOD_line_step(&iterX, &iterY))
						iCount++;
					g_ego.m_iVisibility += 120 - (iCount * 20);
				}
			}
		}
	}

	return;
}
