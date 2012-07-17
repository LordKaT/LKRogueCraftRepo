#include "structs.h"

int itemCraft(int iItem1, int iItem1Count, int iItem2, int iItem2Count);
sItemData itemCombine(sItemData item1, sItemData item2);
sItemList *itemFindDynamic(int iID);
void itemPopulate(int iItemID, int iAmount, int iPercentage, int iIterations);
void itemAdjustStats(int iItemID, bool bEquip);