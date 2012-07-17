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
#include "console.h"
#include "npc.h"

void npcFunc_Imp(int iAction, int iX, int iY, void *vUserData);
void npcFunc_None(int iAction, int iX, int iY, void *vUserData);

sNPCData g_npcData[] = {
	{
		0,
		{"Nothing",	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	false,	true,	0,	{0, 0, 0}, {0, 0, 0}},
		{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},
		NULL, 0, 0, npcFunc_None
	},
	{
		1,
		{"Imp",	1,	0,	0,	0,	1,	1,	10,	10,	0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,	false,	false,	0,	{255, 255, 255}, {255, 255, 255}},
		{0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0},
		NULL, 0, 0, npcFunc_Imp
	},
};

bool npcMove(int iOldX, int iOldY, int iNewX, int iNewY) {
	if (g_iWorld[iNewX][iNewY] <= 0) {
		if (g_npcMap[iNewX][iNewY] == NULL) {
			g_npcMap[iNewX][iNewY] = g_npcMap[iOldX][iOldY];
			g_npcMap[iOldX][iOldY] = NULL;
			return true;
		}
	}
	return false;
}

void npcFunc_None(int iAction, int iX, int iY, void *vUserData) {
	return;
}

void npcFunc_Imp(int iAction, int iX, int iY, void *vUserData) {
	sNPCData *temp = (sNPCData *)(vUserData);
	int iTX;
	int iTY;

	switch (iAction) {
		case ACT_WALK_INTO:
			consoleAdd("Imp: \"y u push me! D:\"");
			break;
		case ACT_NPC_INIT:
			iTX = rand() % DEF_WORLD_X;
			iTY = rand() % DEF_WORLD_Y;

			while (g_iWorld[iTX][iTY] != 0) {
				iTX = rand() % DEF_WORLD_X;
				iTY = rand() % DEF_WORLD_Y;
			}

			temp->m_stats.m_iWorldX = iTX;
			temp->m_stats.m_iWorldY = iTY;
			break;
		case ACT_NPC_THINK:
			break;
		case ACT_NPC_DEATH:
			break;
		case ACT_NPC_DELETE:
			delete g_npcMap[temp->m_stats.m_iWorldX][temp->m_stats.m_iWorldY];
			break;
		default:
			break;
	}
	return;
}

void npcPopulate(int iNPCID, int iAmount, int iPercentage, int iIterations) {
	int iX = rand() % DEF_WORLD_X;
	int iY = rand() % DEF_WORLD_Y;

	for (int i = 0; i < iIterations; i++) {
		if (rand() % 100 <= iPercentage) {
			/* We don't want to populate these in doors/walls/players */
			if (g_iWorld[iX][iY] == ITEM_NOTHING ||
				g_iWorld[iX][iY] > ITEM_DOOR_OPEN ||
				(g_ego.m_stats.m_iWorldX != iX && g_ego.m_stats.m_iWorldY != iY)) {
				// Create NPC structure
				sNPCData temp;
				// Copy data from g_NPCData to the new struct
				temp = g_npcData[iNPCID];
				// Create NPC through their NPC func using ACT_NPC_INIT, use NPC as vUserData
				g_npcData[iNPCID].m_vFunc(ACT_NPC_INIT, 0, 0, &temp);
				// Add NPC into NPC list
				npcListAdd(&g_npcList, temp);
			}
		}
		iX = rand() % DEF_WORLD_X;
		iY = rand() % DEF_WORLD_Y;
	}

	return;
}

/* Go through the list and do NPC stuff */
void npcExecute(sNPCList **npcList) {
	sNPCList *head = *npcList;

	if (head == NULL) {
		return;
	}

	while (head != NULL) {
		(head->m_npcData.m_vFunc)(ACT_NPC_THINK, 0, 0, &head->m_npcData);
		head = head->next;
	}
	return;
}

/*
	Checks to see if the specified NPC is in the list

	returns:
		< 0	- error
		0	- not in possession
		> 0	- amount of item in inventory
*/
int npcListCheck(sNPCList **npcList, int iNPCID) {
	if (*npcList == NULL)
		return 0; // Empty
	sNPCList *head = *npcList;
	while (head->next != NULL && head->m_npcData.m_iID != iNPCID)
		head = head->next;
	if (head->m_npcData.m_iID == iNPCID)
		return head->m_npcData.m_iID;
	else
		return 0;
}

/*
	Add iNPCID to npcList

	npcList - the list
	npcData - the npc to add

	returns:
		none
*/

void npcListAdd(sNPCList **npcList, sNPCData npcData) {
	sNPCList *head;
	int iUIDCount = 1;
	if (*npcList == NULL) {
		*npcList = (sNPCList *)malloc(sizeof(sNPCList));
		head = *npcList;
		head->m_npcData = npcData;
		head->m_npcData.m_iUID = iUIDCount;
		head->next = NULL;
	}
	else {
		head = *npcList;
		while (head->next != NULL) {
			head = head->next;
			iUIDCount++;
		}

		head->next = (sNPCList *)malloc(sizeof(sNPCList));
		head = head->next;
		head->m_npcData = npcData;
		head->m_npcData.m_iUID = iUIDCount;
		head->next = NULL;
	}

	g_npcMap[head->m_npcData.m_stats.m_iWorldX][head->m_npcData.m_stats.m_iWorldY] = &head->m_npcData;

	return;
}

/*
	Remove NPC with iUID from the list

	iItemID - numeric ID of item to be removed
	iAmount - amount to be removed, 0 for all

	returns:
		none
*/

void npcListDel(sNPCList **npcList, int iUID) {
	if (*npcList == NULL) // herp de derp
		return;
	else {
		sNPCList *head = *npcList;

		if (head->m_npcData.m_iUID == iUID) {
			// Actually remove it from the list
			if (head->next == NULL) { // No other items, remove list.
				delete head;
				head = NULL;
				*npcList = NULL;
			}
			else {
				head->m_npcData = head->next->m_npcData;
				sNPCList *temp = NULL;
				if (head->next->next != NULL)
					temp = head->next->next;
				delete head->next;
				head->next = temp;
			}
			return;
		}

		while (head->next != NULL && head->next->m_npcData.m_iUID != iUID)
			head = head->next;

		if (head->next->m_npcData.m_iUID == iUID) {
			// Actually remove it from the list
			if (head->next->next == NULL) {
				delete head->next;
				head->next = NULL;
			}
			else {
				head->next->m_npcData.m_iID = head->next->next->m_npcData.m_iUID;
				sNPCList *temp = NULL;
				if (head->next->next->next != NULL)
					temp = head->next->next->next;
				delete head->next->next;
				head->next->next = temp;
			}
		}
		else 
			return; // NPC not found;
	}
	return;
}

/*
	DEBUG: List the contents of the NPC list to the console

	npcList - NPC list

	returns:
		none
*/

void npcListList(sNPCList **npcList) {
	sNPCList *head = *npcList;

	if (head == NULL) {
		consoleAdd("NPC List is Empty.");
		return;
	}

	while (head != NULL) {
		consoleAdd("%s", head->m_npcData.m_stats.m_cName);
		head = head->next;
	}
	return;
}
