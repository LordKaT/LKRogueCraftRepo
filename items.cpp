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

sItemData g_itemData[] = {
	{0,		{"Nothing",			0,		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	TCOD_black,	TCOD_black}, itemFunc_Floor},	// 0

	{1,		{"Stone Wall",		178,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_light_grey,	TCOD_dark_grey}, itemFunc_Ore}, // 1
	{2,		{"Stone Wall",		177,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_light_grey,	TCOD_dark_grey}, itemFunc_Ore},	// 2
	{3,		{"Stone Wall",		176,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	true,	0,	TCOD_light_grey,	TCOD_dark_grey}, itemFunc_Ore},	// 3

	{4,		{"Iron Ore",		178,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_orange,	TCOD_orange},	itemFunc_Ore},	// 4
	{5,		{"Iron Ore",		177,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_orange,	TCOD_orange},	itemFunc_Ore},	// 5
	{6,		{"Iron Ore",		176,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	true,	0,	TCOD_orange,	TCOD_orange},	itemFunc_Ore},	// 6

	{7,		{"Copper Ore",		178,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_dark_orange,	TCOD_dark_orange},	itemFunc_Ore},	// 7
	{8,		{"Copper Ore",		177,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_dark_orange,	TCOD_dark_orange},	itemFunc_Ore},	// 8
	{9,		{"Copper Ore",		176,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	true,	0,	TCOD_dark_orange,	TCOD_dark_orange},	itemFunc_Ore},	// 9

	{10,	{"Silver Ore",		178,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_silver,	TCOD_silver},	itemFunc_Ore},	// 10
	{11,	{"Silver Ore",		177,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_silver,	TCOD_silver},	itemFunc_Ore},	// 11
	{12,	{"Silver Ore",		176,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	true,	0,	TCOD_silver,	TCOD_silver},	itemFunc_Ore},	// 12

	{13,	{"Gold Ore",		178,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_gold,	TCOD_gold},	itemFunc_Ore},	// 13
	{14,	{"Gold Ore",		177,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	TCOD_gold,	TCOD_gold},	itemFunc_Ore},	// 14
	{15,	{"Gold Ore",		176,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	true,	0,	TCOD_gold,	TCOD_gold},	itemFunc_Ore},	// 15

	{16,	{"Stick",			179,	0,	0,	0,	0,	0,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	0,	true,	false,	0,	TCOD_dark_orange,	TCOD_dark_orange},	itemFunc_Stick},					// 16
	{17,	{"Fire Crystal",	7,		0,	0,	0,	0,	0,	-2,	-2, -2,	-2,	-2,	-2,	-2,	-2,	-2,	-2,	-2,	-2,	-2,	-2,	-2,	0,	true,	false,	0,	TCOD_light_red,	TCOD_light_red},	itemFunc_FireCrystal},	// 17

	{18,	{"Closed Door",		219,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	false,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Door},
	{19,	{"Open Door",		224,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Door},

	{20,	{"Test 1",			224,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Door},
	{21,	{"Test 2",			224,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Door},
	{22,	{"Test 3",			224,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Door},
	{23,	{"Test 4",			224,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Door},
	{24,	{"Test 5",			224,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Door},

	{25,	{"Torch (lit)",		173,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	5,	TCOD_dark_orange,	TCOD_dark_orange},	itemFunc_Torch},
	{26,	{"Torch (out)",		173,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	TCOD_dark_orange,	TCOD_dark_orange},	itemFunc_Torch},

	{27,	{"Stone Wall",		178,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	TCOD_dark_orange,	TCOD_dark_orange},	itemFunc_HiddenDoor},

	{28,	{"Test 7",			224,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Door},
	{29,	{"Test 6",			224,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Door},

	{30,	{"Wooden Stick",	0,		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Weapon},
	{31,	{"Rock",			0,		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	true,	true,	0,	{139, 69, 19},	{139, 69, 19},},	itemFunc_Weapon},

	{999998,	{"You",			64,		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	5,	TCOD_black,	TCOD_black},	itemFunc_Empty},

	{999999,	{"(END)",		0,		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	true,	true,	0,	TCOD_black,	TCOD_black},	itemFunc_Error},	// Last item item in the list.
};

// ITEM_NOTHING in either slot is a special case.
sCraftingFormula g_CraftingFormula[] = {
	{ITEM_ORE_STONE_WEAK,	2,	ITEM_NOTHING,	0,	ITEM_ORE_STONE,	1,	100,	NULL},
	{ITEM_ORE_STONE_WEAKER,	4,	ITEM_NOTHING,	0,	ITEM_ORE_STONE,	1,	100,	NULL},
	{-1, -1, -1, -1, -1, -1, -1, NULL},
};

int itemCraft(int iItem1, int iItem1Count, int iItem2, int iItem2Count) {
	for (int i = 0; g_CraftingFormula[i].m_iItem1 != -1; i++) {
		if (g_CraftingFormula[i].m_iItem1 == -1)
			return -1; // end of list
		if ((iItem1 == g_CraftingFormula[i].m_iItem1 || g_CraftingFormula[i].m_iItem1 == ITEM_NOTHING) &&
			(iItem2 == g_CraftingFormula[i].m_iItem2 || g_CraftingFormula[i].m_iItem2 == ITEM_NOTHING)) {
			if (iItem1Count >= g_CraftingFormula[i].m_iItem1Count && iItem2Count >= g_CraftingFormula[i].m_iItem2Count) {
				if (rand() % 100 <= g_CraftingFormula[i].m_iSuccessRate)
					return i;
				else {
					return -2; // Special case;
				}
			}
		}
		else if ((iItem2 == g_CraftingFormula[i].m_iItem1 || g_CraftingFormula[i].m_iItem1 == ITEM_NOTHING)
			&& (iItem1 == g_CraftingFormula[i].m_iItem2 || g_CraftingFormula[i].m_iItem2 == ITEM_NOTHING)) {
			if (iItem2Count == g_CraftingFormula[i].m_iItem1Count && iItem1Count == g_CraftingFormula[i].m_iItem2Count) {
				if (rand() % 100 <= g_CraftingFormula[i].m_iSuccessRate)
					return i;
				else {
					return -2; // Special case;
				}
			}
		}
	}
	return -1;
}

// iX - Dominant number, never changed.
// iY - Manipulated number
int randomMath(int iX, int iY) {
	int iResult = 0;
	int iRand = rand() % 100;

	if (iRand < 5) return iX + iY;
	else if (iRand >= 5 && iRand < 25) return iX + (iY / 2);
	else if (iRand >= 25 && iRand < 30) return iX * iY;
	else if (iRand >= 30 && iRand < 45) return iX + (iY * (iY / 2));
	else if (iRand >= 45 && iRand < 65) return iX + (iY / 3);
	else if (iRand >= 65 && iRand < 100) return iX + (iY / 2);
	else return 0;

	return 0;
}

sItemData itemCombine(sItemData item1, sItemData item2) {
	sItemData tempItem;
	sItemData itemDom;
	sItemData itemSub;
	char *pch;
	char cBuffer[128];
	char cTmpBuf[L_tmpnam];
/*
	char cTempBuffer1[128];
	char cTempBuffer2[128];

	strncpy(cTempBuffer1, item1.m_stats.m_cName, 128);
	strncpy(cTempBuffer2, item2.m_stats.m_cName, 128);

	for (int i = 0; i < 128; i++) {
		if (cTempBuffer1[i] == ' ')
			cTempBuffer1[i] = '_';
		if (cTempBuffer2[i] == ' ')
			cTempBuffer2[i] = '_';
	}
*/
	tmpnam(cTmpBuf);
	FILE *pFile = fopen(cTmpBuf, "w");
	fprintf(pFile, "name \"Combined Item\" {\r\n");

	fprintf(pFile, "\tsyllablesPre = \"The, Heroic, Cowardly, Frightening, Horrific, Shiney, Smelly, Unbalanced, Balanced, Fiendish, Retractable, Carefree, Cavity-filled, Naked, Natural, Unnatural, Flaking, Wobbly, Steady, Short, Sorry, Pitiful, Excellent\"\r\n");

	fprintf(pFile, "\tsyllablesStart = \"");


//	fprintf(pFile, "%s, %s, ", cTempBuffer1, cTempBuffer2);
	strncpy(cBuffer, item1.m_stats.m_cName, 128);
	pch = strtok(cBuffer, " ");
	while (pch != NULL) {
		fprintf(pFile, "%s, ", pch);
		pch = strtok(NULL, " ");
	}
	strncpy(cBuffer, item2.m_stats.m_cName, 128);
	pch = strtok(cBuffer, " ");
	while (pch != NULL) {
		fprintf(pFile, "%s, ", pch);
		pch = strtok(NULL, " ");
	}
	fprintf(pFile, "\"\r\n");

	fprintf(pFile, "\tsyllablesMiddle = \"of, and\"\r\n");

	fprintf(pFile, "\tsyllablesEnd = \"");
	fprintf(pFile, "%s, %s,", item1.m_stats.m_cName, item2.m_stats.m_cName);
	strncpy(cBuffer, item1.m_stats.m_cName, 32);
	pch = strtok(cBuffer, " ");
	while (pch != NULL) {
		fprintf(pFile, "%s, ", pch);
		pch = strtok(NULL, " ");
	}
	strncpy(cBuffer, item2.m_stats.m_cName, 32);
	pch = strtok(cBuffer, " ");
	while (pch != NULL) {
		fprintf(pFile, "%s, ", pch);
		pch = strtok(NULL, " ");
	}
	fprintf(pFile, "\"\r\n");

	fprintf(pFile, "\tsyllablesPost = \"of_Victory, of_Heroism, of_Cowardice, of_Failure, of_Defeat, of_Farting\"\r\n");

	fprintf(pFile, "\trules = \"$25P_$s_$25m_$e_$25p\"\r\n}\r\n");
	fclose(pFile);

	TCOD_namegen_parse(cTmpBuf, NULL);
	strcpy(tempItem.m_stats.m_cName, TCOD_namegen_generate("Combined Item", false));
	TCOD_namegen_destroy();

	int iItemADiff =
		item1.m_stats.m_iHP +
		item1.m_stats.m_iMaxHP +
		item1.m_stats.m_iSP +
		item1.m_stats.m_iMaxSP +
		item1.m_stats.m_iStr +
		item1.m_stats.m_iDex +
		item1.m_stats.m_iAgl +
		item1.m_stats.m_iCha +
		item1.m_stats.m_iWis +
		item1.m_stats.m_iInt +
		item1.m_stats.m_iAtk +
		item1.m_stats.m_iDef;

	int iItemBDiff = 
		item2.m_stats.m_iHP +
		item2.m_stats.m_iMaxHP +
		item2.m_stats.m_iSP +
		item2.m_stats.m_iMaxSP +
		item2.m_stats.m_iStr +
		item2.m_stats.m_iDex +
		item2.m_stats.m_iAgl +
		item2.m_stats.m_iCha +
		item2.m_stats.m_iWis +
		item2.m_stats.m_iInt +
		item2.m_stats.m_iAtk +
		item2.m_stats.m_iDef;

	if (iItemADiff < 0) iItemADiff *= -1;
	if (iItemBDiff < 0) iItemBDiff *= -1;

	/* Dom and Sub - the kinkiest item creation system yet */
	if (iItemADiff > iItemBDiff) {
		itemDom = item1;
		itemSub = item2;
	}
	else if (iItemADiff < iItemBDiff) {
		itemDom = item2;
		itemSub = item1;
	}
	else {
		if (rand() % 100 < 50) {
			itemDom = item1;
			itemSub = item2;
		}
		else {
			itemDom = item2;
			itemSub = item1;
		}
	}

	tempItem.m_stats.m_iASCII = ((rand() % 100 < 65)? itemDom.m_stats.m_iASCII : itemSub.m_stats.m_iASCII);
	tempItem.m_vFunc = ((rand() % 100 < 65)? itemDom.m_vFunc : itemSub.m_vFunc);
	tempItem.m_stats.m_bWalkable = ((rand() % 100 < 65)? itemDom.m_stats.m_bWalkable : itemSub.m_stats.m_bWalkable);
	tempItem.m_stats.m_tcodVisibleColor = ((rand() % 100 < 65)? itemDom.m_stats.m_tcodVisibleColor : itemSub.m_stats.m_tcodVisibleColor);
	tempItem.m_stats.m_tcodHiddenColor = ((rand() % 100 < 65)? itemDom.m_stats.m_tcodHiddenColor : itemSub.m_stats.m_tcodHiddenColor);
	tempItem.m_stats.m_iHP = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iHP, itemSub.m_stats.m_iHP) : randomMath(itemSub.m_stats.m_iHP, itemDom.m_stats.m_iHP));
	tempItem.m_stats.m_iMaxHP = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iMaxHP, itemSub.m_stats.m_iMaxHP) : randomMath(itemSub.m_stats.m_iMaxHP, itemDom.m_stats.m_iMaxHP));
	tempItem.m_stats.m_iSP = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iSP, itemSub.m_stats.m_iSP) : randomMath(itemSub.m_stats.m_iSP, itemDom.m_stats.m_iSP));
	tempItem.m_stats.m_iMaxSP = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iMaxSP, itemSub.m_stats.m_iMaxSP) : randomMath(itemSub.m_stats.m_iMaxSP, itemDom.m_stats.m_iMaxSP));
	tempItem.m_stats.m_iStr = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iStr, itemSub.m_stats.m_iStr) : randomMath(itemSub.m_stats.m_iStr, itemDom.m_stats.m_iStr));
	tempItem.m_stats.m_iDex = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iDex, itemSub.m_stats.m_iDex) : randomMath(itemSub.m_stats.m_iDex, itemDom.m_stats.m_iDex));
	tempItem.m_stats.m_iAgl = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iAgl, itemSub.m_stats.m_iAgl) : randomMath(itemSub.m_stats.m_iAgl, itemDom.m_stats.m_iAgl));
	tempItem.m_stats.m_iCha = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iCha, itemSub.m_stats.m_iCha) : randomMath(itemSub.m_stats.m_iCha, itemDom.m_stats.m_iCha));
	tempItem.m_stats.m_iWis = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iWis, itemSub.m_stats.m_iWis) : randomMath(itemSub.m_stats.m_iWis, itemDom.m_stats.m_iWis));
	tempItem.m_stats.m_iInt = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iInt, itemSub.m_stats.m_iInt) : randomMath(itemSub.m_stats.m_iInt, itemDom.m_stats.m_iInt));
	tempItem.m_stats.m_iAtk = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iAtk, itemSub.m_stats.m_iAtk) : randomMath(itemSub.m_stats.m_iAtk, itemDom.m_stats.m_iAtk));
	tempItem.m_stats.m_iDef = ((rand() % 100 < 65)? randomMath(itemDom.m_stats.m_iDef, itemSub.m_stats.m_iDef) : randomMath(itemSub.m_stats.m_iDef, itemDom.m_stats.m_iDef));

	for (int x = 76; x < 99; x++) {
		for (int y = 10; y < 22; y++) {
			TCOD_console_print_left(NULL, x, y, TCOD_BKGND_NONE, " ");
		}
	}

	if (g_itemDynamic == NULL) {
		g_itemDynamic = (sItemList *)malloc(sizeof(sItemList));
		tempItem.m_iID = 100000;
		g_itemDynamic->m_item = tempItem;
		g_itemDynamic->next = NULL;
	}
	else {
		sItemList *head = g_itemDynamic;
		tempItem.m_iID = head->m_item.m_iID + 1;
		while (head->next != NULL) {
			tempItem.m_iID++;
			head = head->next;
		}

		head->next = (sItemList *)malloc(sizeof(sItemList));
		head = head->next;

		head->m_item = tempItem;
		head->next = NULL;
	}
/*
	TCOD_console_print_left(NULL, 76, 10, TCOD_BKGND_NONE, "%s", tempItem.m_stats.m_cName);
	TCOD_console_put_char(NULL, 76, 11, tempItem.m_stats.m_iASCII, TCOD_BKGND_NONE);
	TCOD_console_set_fore(NULL, 76, 11, tempItem.m_stats.m_tcodVisibleColor);
	TCOD_console_print_left(NULL, 76, 12, TCOD_BKGND_NONE, "HP: %i/%i", tempItem.m_stats.m_iHP, tempItem.m_stats.m_iMaxHP);
	TCOD_console_print_left(NULL, 76, 13, TCOD_BKGND_NONE, "SP: %i/%i", tempItem.m_stats.m_iSP, tempItem.m_stats.m_iMaxSP);
	TCOD_console_print_left(NULL, 76, 14, TCOD_BKGND_NONE, "Str: %i", tempItem.m_stats.m_iStr);
	TCOD_console_print_left(NULL, 76, 15, TCOD_BKGND_NONE, "Dex: %i", tempItem.m_stats.m_iDex);
	TCOD_console_print_left(NULL, 76, 16, TCOD_BKGND_NONE, "Agl: %i", tempItem.m_stats.m_iAgl);
	TCOD_console_print_left(NULL, 76, 17, TCOD_BKGND_NONE, "Cha: %i", tempItem.m_stats.m_iCha);
	TCOD_console_print_left(NULL, 76, 18, TCOD_BKGND_NONE, "Wis: %i", tempItem.m_stats.m_iWis);
	TCOD_console_print_left(NULL, 76, 19, TCOD_BKGND_NONE, "Int: %i", tempItem.m_stats.m_iInt);
	TCOD_console_print_left(NULL, 76, 20, TCOD_BKGND_NONE, "Atk: %i", tempItem.m_stats.m_iAtk);
	TCOD_console_print_left(NULL, 76, 21, TCOD_BKGND_NONE, "Def: %i", tempItem.m_stats.m_iDef);
	TCOD_console_print_left(NULL, 76, 22, TCOD_BKGND_NONE, "ID: %i", tempItem.m_iID);
*/
/*
		head = g_itemDynamic;

		int i = 0;
		while (head->next != NULL) {
			i++;
			TCOD_console_print_left(NULL, 76, 22+i, TCOD_BKGND_NONE, "%s", head->m_item.m_stats.m_cName);
			head = head->next;
		}
*/
	return tempItem;
}

sItemList *itemFindDynamic(int iID) {
	sItemList *head = g_itemDynamic;

	while (head->next != NULL) {
		if (head->m_item.m_iID == iID)
			return head;
		else
			head = head->next;
	}
	return NULL;
}

void itemPopulate(int iItemID, int iAmount, int iPercentage, int iIterations) {
	int iX = rand() % DEF_WORLD_X;
	int iY = rand() % DEF_WORLD_Y;

	for (int i = 0; i < iIterations; i++) {
		if (rand() % 100 <= iPercentage) {
			/* Repalce the stone for ore. */
			if (iItemID < ITEM_DOOR_CLOSED) {
				if (g_iWorld[iX][iY] == ITEM_ORE_STONE)
					g_iWorld[iX][iY] = iItemID;
			}
			else {
				/* We don't want to populate these items in doors/walls. */
				if (g_iWorld[iX][iY] == ITEM_NOTHING || g_iWorld[iX][iY] > ITEM_DOOR_OPEN ) {
					inventoryAdd(&g_inventoryWorldList[iX][iY], iItemID, iAmount);
					g_iWorld[iX][iY] = iItemID;
				}
			}
		}
		iX = rand() % DEF_WORLD_X;
		iY = rand() % DEF_WORLD_Y;
	}

	return;
}

void itemAdjustStats(int iItemID, bool bEquip) {
	int iMult = 1;
	if (!bEquip) iMult = -1;
	g_ego.m_stats.m_iHP += g_itemData[iItemID].m_stats.m_iHP * iMult;
	g_ego.m_stats.m_iMaxHP += g_itemData[iItemID].m_stats.m_iMaxHP * iMult;
	g_ego.m_stats.m_iSP += g_itemData[iItemID].m_stats.m_iSP * iMult;
	g_ego.m_stats.m_iMaxSP += g_itemData[iItemID].m_stats.m_iMaxSP * iMult;
	g_ego.m_stats.m_iStr += g_itemData[iItemID].m_stats.m_iStr * iMult;
	g_ego.m_stats.m_iDex += g_itemData[iItemID].m_stats.m_iDex * iMult;
	g_ego.m_stats.m_iAgl += g_itemData[iItemID].m_stats.m_iAgl * iMult;
	g_ego.m_stats.m_iCha += g_itemData[iItemID].m_stats.m_iCha * iMult;
	g_ego.m_stats.m_iWis += g_itemData[iItemID].m_stats.m_iWis * iMult;
	g_ego.m_stats.m_iInt += g_itemData[iItemID].m_stats.m_iInt * iMult;
	g_ego.m_stats.m_iAtk += g_itemData[iItemID].m_stats.m_iAtk * iMult;
	g_ego.m_stats.m_iDef += g_itemData[iItemID].m_stats.m_iDef * iMult;
	return;
}
