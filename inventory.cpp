#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>

#include <libtcod/libtcod.h>

#include "def.h"
#include "globals.h"
#include "structs.h"

void inventoryClear() {
	for (int x = 76; x < 99; x++) {
		for (int y = 1; y < 74; y++) {
			TCOD_console_print_left(NULL, x, y, TCOD_BKGND_NONE, " ");
		}
	}
	return;
}

void inventoryDrawBorder() {
	TCOD_console_put_char(NULL, 75, 0, 201, TCOD_BKGND_NONE);
	TCOD_console_put_char(NULL, 75, 74, 200, TCOD_BKGND_NONE);
	TCOD_console_put_char(NULL, 99, 0, 187, TCOD_BKGND_NONE);
	TCOD_console_put_char(NULL, 99, 74, 188, TCOD_BKGND_NONE);
	for (int i = 1; i < 74; i++) {
		TCOD_console_put_char(NULL, 75, i, 186, TCOD_BKGND_NONE);
		TCOD_console_put_char(NULL, 99, i, 186, TCOD_BKGND_NONE);
		if (i < 24) {
			TCOD_console_put_char(NULL, i+75, 0, 205, TCOD_BKGND_NONE);
			TCOD_console_put_char(NULL, i+75, 74, 205, TCOD_BKGND_NONE);
		}
	}

	TCOD_console_put_char(NULL, 75, 31, 204, TCOD_BKGND_NONE);
	for (int x = 76; x < 99; x++)
		TCOD_console_put_char(NULL, x, 31, 205, TCOD_BKGND_NONE);
	TCOD_console_put_char(NULL, 99, 31, 185, TCOD_BKGND_NONE);

	return;
}

/*
	Checks to see if the specified item is in the linked list.

	returns:
	< 0	- error
	0	- not in possession
	> 0	- amount of item in inventory
*/
int inventoryCheck(sInventory **inventory, int iItemID) {
	if (*inventory == NULL)
		return 0; // Empty inventory
	sInventory *head = *inventory;
	while (head->next != NULL && head->m_iItem != iItemID)
		head = head->next;
	if (head->m_iItem == iItemID)
		return head->m_iCount;
	else
		return 0;
}

/*
	Add the iAmount of iItemID to inventory

	inventory - the sInventory being affected
	iItemID - the iTemID being added
	iAmount - amount of the iItemId being added

	returns:
	none
*/

void inventoryAdd(sInventory **inventory, int iItemID, int iAmount) {
	sInventory *head;
	if (*inventory == NULL) {
		*inventory = (sInventory *)malloc(sizeof(sInventory));
		head = *inventory;
		head->m_iItem = iItemID;
		head->m_iCount = iAmount;
		head->next = NULL;
	}
	else {
		head = *inventory;
		while (head->next != NULL && head->m_iItem != iItemID)
			head = head->next;

		if (head->m_iItem == iItemID) {
			head->m_iCount += iAmount;
			return;
		}

		head->next = (sInventory *)malloc(sizeof(sInventory));
		head = head->next;
		head->m_iItem = iItemID;
		head->m_iCount = iAmount;
		head->next = NULL;
	}

	return;
}

/*
	Remove iAmount of iITemID from inventory.

	iItemID - numeric ID of item to be removed
	iAmount - amount to be removed, 0 for all

	returns:
	none
*/

void inventoryDel(sInventory **inventory, int iItemID, int iAmount) {
	if (*inventory == NULL) // herp de derp
		return;
	else {
		sInventory *head = *inventory;

		if (head->m_iItem == iItemID) {
			if ((head->m_iCount - iAmount) > 0 && iAmount > 0)
				head->m_iCount -= iAmount;
			else {
				// Actually remove it from the list
				if (head->next == NULL) { // No other items, remove list.
					delete(head);
					head = NULL;
				}
				else {
					head->m_iItem = head->next->m_iItem;
					head->m_iCount = head->next->m_iCount;
					sInventory *temp = NULL;
					if (head->next->next != NULL)
						temp = head->next->next;
					delete(head->next);
					head->next = temp;
				}
			}
			return;
		}

		while (head->next != NULL && head->next->m_iItem != iItemID)
			head = head->next;

		if (head->next->m_iItem == iItemID) {
			if ((head->next->m_iCount - iAmount) > 0 && iAmount > 0)
				head->next->m_iCount -= iAmount;
			else {
				// Actually remove it from the list
				if (head->next->next == NULL) {
					delete(head->next);
					head->next = NULL;
				}
				else {
					head->next->m_iItem = head->next->next->m_iItem;
					head->next->m_iCount = head->next->next->m_iCount;
					sInventory *temp = NULL;
					if (head->next->next->next != NULL)
						temp = head->next->next->next;
					delete(head->next->next);
					head->next->next = temp;
				}
			}
		}
		else 
			return; // Item not found;
	}
	return;
}

/*
	TODO: list the contents of inventory on screen.

	 inventory - inventory list
	 iPointer - pointer offset from main()
	 iOffset - offset counter from main()

	 returns:
		integer value of item pointed to
*/

int inventoryList(sInventory **inventory, int iPointer, int iOffset) {
	sInventory *head = *inventory;
	sItemData tempItem = g_itemData[0];

	int iRet = 0;
	int y = 33;
	int iCount = 0;

	for (int j = 77; j < 99; j++)
		for (int k = 33; k < 43; k++)
			TCOD_console_put_char(NULL, j, k, 32, TCOD_BKGND_NONE);

	if (*inventory == NULL) {
		TCOD_console_print_left(NULL, 78, 33, TCOD_BKGND_NONE, "Nothing.");
		return -1;
	}

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
		if (iCount >= iOffset && y < 43) {
			char tempstr[20];
			memcpy(tempstr, tempItem.m_stats.m_cName, 20);
			TCOD_console_put_char(NULL, 77, y, tempItem.m_stats.m_iASCII, TCOD_BKGND_NONE);
			TCOD_console_set_fore(NULL, 77, y, tempItem.m_stats.m_tcodVisibleColor);
			TCOD_console_print_left(NULL, 79, y, TCOD_BKGND_NONE, "%s", tempstr);
			TCOD_console_print_right(NULL, 98, y, TCOD_BKGND_NONE, "%i", head->m_iCount);
			if (iPointer == (y-33))
				iRet = tempItem.m_iID;
			y++;
		}
		head = head->next;
		iCount++;
	}
	return iRet;
}

int inventoryGetTop(sInventory **inventory) {
	if (*inventory == NULL)
		return -1;
	sInventory *head = *inventory;
	return head->m_iItem;
}

void inventoryFree(sInventory **inventory) {
	if (*inventory == NULL)
		return;
	sInventory *temp = NULL;
	sInventory *head = *inventory;
	while (head != NULL) {
		temp = head->next;
		free(head);
		head = temp;
	}
	*inventory = NULL;
	return;
}
