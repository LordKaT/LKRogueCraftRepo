#include "structs.h"

void inventoryClear();
void inventoryDrawBorder();
int inventoryCheck(sInventory **inventory, int iItemID);
void inventoryAdd(sInventory **inventory, int iItemID, int iAmount);
void inventoryDel(sInventory **inventory, int iItemID, int iAmount);
int inventoryList(sInventory **inventory, int iPointer, int iOffset);
int inventoryGetTop(sInventory **inventory);
void inventoryFree(sInventory **inventory);
