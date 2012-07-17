#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>

#include <libtcod/libtcod.h>

#include "def.h"
#include "globals.h"
#include "structs.h"
#include "player.h"
#include "items.h"
#include "inventory.h"
#include "console.h"

// itemFunc(int iAction, int iX, int iY, void *vUserData)
// iAction - Action that took place
// iX, iY - X,Y coords where the action took place (may not be character's XY)
// vUserdata - Misc user data that can be passed along

void itemFunc_Error(int iAction, int iX, int iY, void *vUserData) {
	return;
}

void itemFunc_Empty(int iAction, int iX, int iY, void *vUserData) {
	// Empty on purpose.
	return;
}

void itemFunc_Floor(int iAction, int iX, int iY, void *vUserData) {
	if (iAction == ACT_PLACE_ONTO) {
		if (inventoryCheck(&g_ego.m_inventory, ITEM_ORE_STONE) > 0) {
			g_iWorld[iX][iY] = ITEM_ORE_STONE;
			inventoryDel(&g_ego.m_inventory, ITEM_ORE_STONE, 1);
		}
	}
	return;
}

void itemFunc_Ore(int iAction, int iX, int iY, void *vUserData) {
	if (iAction == ACT_WALK_INTO) {
		switch (g_iWorld[iX][iY]) {
			case ITEM_ORE_STONE_WEAKER:		// Stone
				if (inventoryCheck(&g_ego.m_inventory, ITEM_ORE_STONE) < 999)
					inventoryAdd(&g_ego.m_inventory, ITEM_ORE_STONE, 1);
				g_iWorld[iX][iY] = ITEM_NOTHING;
				break;
			case ITEM_ORE_IRON_WEAKER:		// Iron
				if (inventoryCheck(&g_ego.m_inventory, ITEM_ORE_IRON) < 999)
					inventoryAdd(&g_ego.m_inventory, ITEM_ORE_IRON, 1);
				g_iWorld[iX][iY] = ITEM_NOTHING;
				break;
			case ITEM_ORE_COPPER_WEAKER:	// Copper
				if (inventoryCheck(&g_ego.m_inventory, ITEM_ORE_COPPER) < 999)
					inventoryAdd(&g_ego.m_inventory, ITEM_ORE_COPPER, 1);
				g_iWorld[iX][iY] = ITEM_NOTHING;
				break;
			case ITEM_ORE_SILVER_WEAKER:	// Silver
				if (inventoryCheck(&g_ego.m_inventory, ITEM_ORE_SILVER) < 999)
					inventoryAdd(&g_ego.m_inventory, ITEM_ORE_SILVER, 1);
				g_iWorld[iX][iY] = ITEM_NOTHING;
				break;
			case ITEM_ORE_GOLD_WEAKER:		// Gold
				if (inventoryCheck(&g_ego.m_inventory, ITEM_ORE_GOLD) < 999)
					inventoryAdd(&g_ego.m_inventory, ITEM_ORE_GOLD, 1);
				g_iWorld[iX][iY] = ITEM_NOTHING;
				break;
			default:
				g_iWorld[iX][iY]++;
				break;
		}
	}
	if (iAction == ACT_PLACE_ONTO) {
		int iData = (int)(vUserData);
		if (inventoryCheck(&g_ego.m_inventory, iData) > 0 && g_iWorld[iX][iY] == ITEM_NOTHING) {
			g_iWorld[iX][iY] = iData;
			inventoryDel(&g_ego.m_inventory, iData, 1);
		}
	}
	return;
}

void itemFunc_Stick(int iAction, int iX, int iY, void *vUserData) {
	return;
}

void itemFunc_FireCrystal(int iAction, int iX, int iY, void *vUserData) {
	return;
}

void itemFunc_Door(int iAction, int iX, int iY, void *vUserData) {
	if (iAction == ACT_OPEN || iAction == ACT_WALK_INTO) {
		if (g_iWorld[iX][iY] == ITEM_DOOR_CLOSED)
			g_iWorld[iX][iY] = ITEM_DOOR_OPEN;
		else if (g_iWorld[iX][iY] == ITEM_DOOR_OPEN)
			g_iWorld[iX][iY] = ITEM_DOOR_CLOSED;
		TCOD_map_set_properties(g_tcodMap, iX, iY, g_itemData[g_iWorld[iX][iY]].m_stats.m_bTransparent, g_itemData[g_iWorld[iX][iY]].m_stats.m_bWalkable);
	}
	return;
}

void itemFunc_Torch(int iAction, int iX, int iY, void *vUserData) {
	return;
}

void itemFunc_HiddenDoor(int iAction, int iX, int iY, void *vUserData) {
	if (iAction == ACT_SEARCH)
		g_iWorld[iX][iY] = ITEM_DOOR_CLOSED;
	if (iAction == ACT_MINE)
		consoleAdd("*THUD*");
	return;
}

void itemFunc_Weapon(int iAction, int iX, int iY, void *vUserData) {
	int iData = (int)(vUserData);
	if (iAction == ACT_EQUIP) {
		if (g_ego.m_equipment.m_iHand_R == ITEM_NOTHING) {
			g_ego.m_equipment.m_iHand_R = iData;
			inventoryDel(&g_ego.m_inventory, iData, 1);
			itemAdjustStats(iData, true);
			consoleAdd("You wield %s", g_itemData[iData].m_stats.m_cName);
		}
		else if (g_ego.m_equipment.m_iHand_L == ITEM_NOTHING) {
			g_ego.m_equipment.m_iHand_L = iData;
			inventoryDel(&g_ego.m_inventory, iData, 1);
			itemAdjustStats(iData, true);
			consoleAdd("You wield %s", g_itemData[iData].m_stats.m_cName);
		}
		else {
			consoleAdd("Must have an empty hand first!");
		}
	}
	if (iAction== ACT_DEQUIP) {
		inventoryAdd(&g_ego.m_inventory, iData, 1);
		itemAdjustStats(iData, false);
	}
	return;
}
